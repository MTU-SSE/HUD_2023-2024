#include <SoftwareSerial.h>

int lapPin = 2;
int lapCount = 0;
long lapPoint = 0;
bool lapFlag = false;

unsigned long startTime = 0;
unsigned long lapTime;
long prevTime;

long currentLapButtonTime = 0;
long previousLapButtonTime = 0;

SoftwareSerial Display (12,13);

void setup() {
  Serial.begin(9600);
  Display.begin(9600);
  prevTime = millis();
  pinMode(lapPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lapPin),lapEvent, RISING);
}
void lapEvent() {
  currentLapButtonTime = millis();
  if (currentLapButtonTime - previousLapButtonTime > 2000){
  lapFlag = true;
  previousLapButtonTime = currentLapButtonTime;
  }
}

void loop() {
  Timers();

}

String displayFormatted(double input) {
  // the display interprets doubles weird they need to be multiplied by 10
  return (String) (int) (input * 10);
}

void setDisplayVar(String variable, String type, String newValue) {
  //send the updated value to the display
  Display.print(variable + "." + type + "=" + newValue);
  // tells the display to update info (not completely sure how...)
  Display.write(0xff);
  Display.write(0xff);
  Display.write(0xff);
}

String normalizeTen(int time) { // used for clock values just for looks
  return time < 10 ? ("0" + String(time)) : (String(time));
}

void Timers() {
  if (lapFlag) {
    if (startTime == 0) {
      startTime = millis();
    }
    lapCount++; 
    lapPoint = millis();
    lapFlag = false;
    //setDisplayVar("LAPCOUNT", "
  } 
  long totalTime = millis() - startTime;
  lapTime = totalTime - lapPoint;

  int totalMinutes = ((totalTime / 1000) / 60) % 60;
  int totalSeconds = ((totalTime / 1000) % 60);

  setDisplayVar("TOTMIN", "txt", "\"" + normalizeTen(totalMinutes) + "\"");
  setDisplayVar("TOTSEC", "txt", "\"" + normalizeTen(totalSeconds) + "\"");

  int lapMinutes = ((lapTime / 1000) / 60) % 60;
  int lapSeconds = ((lapTime / 1000) % 60);

  setDisplayVar("LAPMIN", "txt", "\"" + normalizeTen(lapMinutes) + "\"");
  setDisplayVar("LAPSET", "txt", "\"" + normalizeTen(lapSeconds) + "\"");
}
