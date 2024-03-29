const int lapPin =2;

int lapCount = 0;
long lapPoint = 0;

int lapsRemaining() {return (4-lapCount)*(lapCount<4);}
float seconds() { return millis() / 1000;}

float distancePerLap = 9.523 / 4;

bool nextLap = false;
float startTime = 0;
float previousTime = 0;
float currentTime = 0;
float TotalTime = 2100;

long TimeLeft() {return TotalTime-lapPoint;}

float WantedMPH() {return ( distancePerLap * lapsRemaining() ) / TimeLeft() * 60;}

void setup() {
  Serial.begin(9600);
  pinMode(lapPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lapPin),lapEvent, RISING);
}

void lapEvent() {
  currentTime = seconds();
  if (currentTime - previousTime > 2){
  nextLap = true;
  previousTime = currentTime;
  }
}

void loop() {
  //Serial.print("l");
   if (nextLap) {
    if(lapCount == 0){
      startTime = seconds();
    }

    lapPoint = seconds() - startTime;

    if(lapsRemaining() != 4){
      Serial.print("lapPoint: ");
      Serial.println(lapPoint);  
      Serial.print("You are on lap: ");
      Serial.println(lapCount);
    }
    Serial.print("Optimal MPH: ");
    Serial.println(WantedMPH());

    lapCount++; 
    nextLap = false;   
  } 

}