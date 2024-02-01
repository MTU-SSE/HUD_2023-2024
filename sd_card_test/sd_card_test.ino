#include <string.h>
#include <SD.h>
#include <SPI.h>

//definies the "chip select pin"
//which you can read about here: https://www.arduino.cc/reference/en/libraries/sd/begin/
const int chipSelect = 53;
//a string represeting the path to the log file
String fileName = "";
//tracks the previous time loop() was called
unsigned long prevTime;
//tracks if this is the first write to the SD card since boot
//will create a folder for the data file if true
bool firstWrite = true;

void setup() {
  Serial.begin(9600);

    // Open serial communications and wait for port to open:
  while (!Serial) {
   // wait for serial port to connect. Needed for native USB port only
  } 
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  //set an inital value
  prevTime = millis();
}

/*
 * 
 */
void SD_loop() {
  //first run protocol
  if (firstWrite) {
    SD.mkdir(fileName);
    fileName.concat("data/log.txt"); //saves as a txt file that can be imported into excell       
    firstWrite = false;
    File logFile = SD.open(fileName, FILE_WRITE);
    logFile.println("totalTime,lapTime,Lap,Longitude,Latitude,Speed,Distance,Altitude"); // header of the file
    logFile.close();      
  }
  File logFile = SD.open(fileName, FILE_WRITE); // creates the file
  String dataString = "bees";  
  logFile.println(dataString);
  logFile.close();
  // print to the serial port too:
  Serial.println(dataString);
}

void loop() {

  if (millis()-prevTime>1000) { // only writes to the SD every second
    Serial.println("writing");
    SD_loop();
    prevTime = millis();      
  } 
}
