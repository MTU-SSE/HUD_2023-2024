#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <SD.h>
#include <SPI.h>



// pin assignments
const int chipSelect = 53;
const int lapPin = 2;

//Adafruit_MPU6050 mpu;
TinyGPSPlus GPS; // creates the gps object
SoftwareSerial Display (12,13); // sets up the software Serial to digital pins 12 and 13

// global variables used to store data

// for GPS data
double latitude;
double longitude;
double previousLatitude;
double previousLongitude;
int satellites;
double speed;
double altitude;
double heading;
double distance;
double currentDistanceBetween;

// for logging
String dataString;
String fileName = "";
String startHour;

//for timers
unsigned long startTime;
unsigned long totalTime;
unsigned long lapTime;
int totalMinutes;
int totalSeconds;
int lapMinutes;
int lapSeconds;
int lapCount = 0;
unsigned long lapPoint;
unsigned long prevTime;
bool nextLap = false;
bool first_cords = true; // used to set the prevlng and prevlat to the right starting value
bool flag = true; // used for the creation of the folder
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265;
int maxVal=402;
double x;
double y;
double z;

/**
 * Default arduino function that runs once when the arduino is first powered/reset.
 * We use this function to start serial connections to the GPS and the Display
 */
void setup() {
  Serial2.begin(9600); // GPS port
  
  Serial.begin(9600);

}

void lapEvent(){
   nextLap = true; 
}

void Timers() {
  if (nextLap) {
    lapCount++; 
    lapPoint = millis();
    nextLap = false;      
  } 
  totalTime = millis() - startTime;
  lapTime = totalTime - lapPoint;

  totalMinutes = ((totalTime / 1000) / 60) % 60;
  totalSeconds = ((totalTime / 1000) % 60);

  setDisplayVar("TOTMIN", "txt", "\"" + normalizeTen(totalMinutes) + "\"");
  setDisplayVar("TOTSEC", "txt", "\"" + normalizeTen(totalSeconds) + "\"");

  lapMinutes = ((lapTime / 1000) / 60) % 60;
  lapSeconds = ((lapTime / 1000) % 60);

  setDisplayVar("LAPMIN", "txt", "\"" + normalizeTen(lapMinutes) + "\"");
  setDisplayVar("LAPSET", "txt", "\"" + normalizeTen(lapSeconds) + "\"");
}

void makeFolder() { // makes the folder using the month and date
  
}

String dateTime() {
  return "hi";
}

void setLog() {
  
}

void SD_loop() {
  if (flag) { // first run protocal
    makeFolder();
    Serial.println(fileName);
    if (SD.exists(fileName)==1) { // checks if a run has happened in that hour
      fileName.concat("_");
                 
    } 
    fileName.concat(".txt"); //saves as a txt file that can be imported into excell       
    flag = false;
    File logFile = SD.open(fileName, FILE_WRITE);
    logFile.println("totalTime,lapTime,Lap,Longitude,Latitude,Speed,Distance,Altitude"); // header of the file
    logFile.close();      
  }
  File logFile = SD.open(fileName, FILE_WRITE); // creates the file 

  // if the file is available, write to it:
  if (logFile) { 
    setLog();    
    logFile.println(dataString);
    logFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening the file");
  }
}

void GPS_loop() {
  //check everything is good with the GPS before trying to run the main loop code
  //if any of these checks fail the loop returns before the rest of the code can be run
  if (Serial2.available() < 0) { // check if the GPS is available by checking if there is data in the buffer
    Serial.print("GPS unavailable\n");
    return;
  }

  GPS.encode(Serial2.read()); // sends the NHEMA sentence to be parsed

  if (not GPS.location.isValid()) { // checks if the GPS location is valid
    Serial.print("GPS Bad Location\n");
    return;
  }

  if (not GPS.location.isUpdated()) { // if the GPS data has been updated
    Serial.print("GPS Location not updated\n");
    return;
  }

  if (flag) { // first time protocol
    flag = false; // coverts it to false
    previousLatitude = GPS.location.lat(); // sets prevLatitude and prevLongitude to correct values
    previousLongitude = GPS.location.lng();
  }

  // set the GPS variables
  latitude = GPS.location.lat();
  longitude = GPS.location.lng();
  altitude = GPS.altitude.feet(); //the GPS altitude in feet
  speed = GPS.speed.mph();
  satellites = GPS.satellites.value();
  heading = GPS.course.deg();

  Serial.print(satellites);
  
}

void angle_Loop(){ // loop for the angle and acceleration
  
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

/**
 * Default arduino function that runs repeatedly while the arduino is on.
 */
void loop() {
  GPS_loop();
}
