#include <SoftwareSerial.h>
#include <string.h>

//Display Vars:
//MPH_PREV
//MPH_NEED
//TOT_MIN TOT_SEC
//LAP_MIN LAP_SEC
//REM_MIN LAP_SEC
//LAP_NUM

//digital pin on the arduino that the lap button is connected to
const int lapPin = 2;

SoftwareSerial Display (12,13); // sets up the software Serial to digital pins 12 and 13

//used for debounce for the lap button
long previousTime = 0;
long currentTime = 0;
bool nextLap = false;

//the distance of one lap in milli miles
//milli miles are used to avoid slow floating point math
const long distancePerLap = 2381;

//the total time of the race in seconds
const long raceTime = 35 * 60;

//counts how many laps have been completed
int lapCount = -1;

//tracks the time of the most recent lap relative to the start of the race
long lapPoint = 0;

//tracks when the race started in milliseconds
long startTime = 0;

//returns the current time in seconds
long seconds() {
  return millis() / 1000;
}

//returns the number of laps remining in the race
int lapsRemaining() {
  return (4-lapCount)*(lapCount<4);
}

//returns the time remaining in the race in seconds
long timeRemaining() {
  return raceTime - (lapPoint / 1000);
}

//returns the target MPH for the next lap
float wantedMPH(int lapsRemaining, long timeRemaining) {
  //Serial.println("vals: ");
  //Serial.println(distancePerLap);
  //Serial.println(lapsRemaining);
  //Serial.println(timeRemaining);
  return ((float) (distancePerLap * lapsRemaining) / ((float) timeRemaining * 16.666)) * 60;
}

void setup() {
  Serial.begin(9600);
  Display.begin(9600); // Display software serial
  pinMode(lapPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lapPin),lapEvent, RISING);
  Serial.println(wantedMPH(4, raceTime));
}

void lapEvent() {
  currentTime = millis();
  if (currentTime - previousTime > 2000){
    nextLap = true;
    previousTime = currentTime;
  }
}

void loop() {
   if (nextLap) {
    if(lapCount == -1){
      startTime = millis();
    }

    lapPoint = millis() - startTime;

    Serial.print("lapPoint: ");
    Serial.println(lapPoint);
    Serial.print("raceTime: ");
    Serial.println(raceTime);
    Serial.print("TimeRem: ");
    Serial.println(timeRemaining());
    
    
    lapCount++; 
    
    nextLap = false;

    if(lapsRemaining() != 4){
      Serial.print("lapPoint: ");
      Serial.println(lapPoint);  
      Serial.print("You are on lap: ");
      Serial.println(lapCount);
      Serial.print("MPH needed: ");
      Serial.println(wantedMPH(lapsRemaining(), timeRemaining()));
    }
  
  }
  
  if (lapCount != -1) {
    long totalTime = millis() - startTime;
    long lapTime = millis() - (lapPoint + startTime);

    long totalMinutes = ((totalTime / 1000) / 60) % 60;
    long totalSeconds = (totalTime / 1000) % 60;

    long lapMinutes = ((lapTime / 1000) / 60) % 60;
    long lapSeconds = (lapTime / 1000) % 60;

    long timeRemainingInRace = (startTime / 1000 + raceTime) - seconds();
    long remainingMinutes = ((timeRemainingInRace / (long) lapsRemaining()) / 60) % 60;
    long remainingSeconds = (timeRemainingInRace / (long) lapsRemaining()) % 60;

    //Show the driver what lap they are on, instead of how many laps they have completed
    setDisplayVar("LAP_NUM", "val", (String) (lapCount + 1));
    setDisplayVar("t0", "txt", "\"" + displayFormatted(wantedMPH(lapsRemaining(), timeRemaining())) + "\"");
    Serial.print(timeRemainingInRace);
    
    setDisplayVar("TOT_MIN", "txt", "\"" + normalizeTen(totalMinutes) + "\"");
    setDisplayVar("TOT_SEC", "txt", "\"" + normalizeTen(totalSeconds) + "\"");

    setDisplayVar("LAP_MIN", "txt", "\"" + normalizeTen(lapMinutes) + "\"");
    setDisplayVar("LAP_SEC", "txt", "\"" + normalizeTen(lapSeconds) + "\"");

    setDisplayVar("REM_MIN", "txt", "\"" + normalizeTen(remainingMinutes) + "\"");
    setDisplayVar("REM_SEC", "txt", "\"" + normalizeTen(remainingSeconds) + "\"");
  }

}

String displayFormatted(float input) {
  return (String) (input);
}

void setDisplayVar(String variable, String type, String newValue) {
  //send the updated value to the display
  Display.print(variable + "." + type + "=" + newValue);
  // tells the display to update info (not completely sure how...)
  Display.write(0xff);
  Display.write(0xff);
  Display.write(0xff);
}

String normalizeTen(long time) { // used for clock values just for looks
  return time < 10 && time >= 0 ? ("0" + String(time)) : (String(time));
}
