const int lapPin =2;

int lapCount = 0;
long lapPoint = 0;

bool nextLap = false;
long previousTime = 0;
long currentTime =0;

void setup() {
  Serial.begin(9600);
  pinMode(lapPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(lapPin),lapEvent, RISING);
}

void lapEvent() {
  currentTime = millis();
  if (currentTime - previousTime > 2000){
  nextLap = true;
  previousTime = currentTime;
  }
}

void loop() {
  //Serial.print("l");
   if (nextLap) {
    lapCount++; 
    lapPoint = millis();
    nextLap = false;  

    Serial.print("lapPoint: ");
    Serial.println(lapPoint);  
    Serial.print("You are on lap: ");
    Serial.println(lapCount); 
  } 

}