/*The HABControl kit is supplied as is with no guarantees of performance or operation. Although the HABControl suite 
 is designed for short-term Near Space operation, the conditions may exceed the specification of individual 
 components which may fail to operate correctly because of low temperatures or pressures of increased 
 radiation levels. We have no control over the effectiveness of your payload design especially the 
 temperature and electromagnetic conditions  within, thus we make no warrnaty express or implied as to
 the ability of our products to operate to any degree of effectiveness when used in your application.
 
 If you decide to use this product under a balloon it’s your responsibility to ensure you comply with the 
 local legislation and laws regarding meteorological balloon launching and radio transmission in the air.

  The HABControl software and hardware is designed for hobbyist use and is not intended for mission-critical implementations.
  We assume no responsibility or liability for any damages implicitly or explicitly caused by this product.
 
 The Radiometrix MTX2 434Mhz is NOT license exempt in the United States of America and requires a radio
 amateur license.
 

 
 The latest code is available here : https://github.com/rdaruwala/HABControl

 Maintained by Rohan Daruwala

 Contact @ rdaruwala@gmail.com
*/



//Import Libraries Needed For Sensors
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"

//Pins needed for the BME (Temperature, Pressure, Humidity)
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

//Variables used for the SD Card (Data Logging & Storing Variable Data (Transmit Frequency, Callsign, etc.))
Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;
boolean SDPresent = true;
File configFile;
File dataFile;

//Initialize BME Sensor (Temperature, Pressure, Humidity)
Adafruit_BME280 bme; 
#define SEALEVELPRESSURE_HPA (1013.25) //CALIBRATION


//Initialize subparts for the LSM303 (Accelerometer & Magnetometer) 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


//Data to get
String frequency = "";
String CallSign = "";
String dataDumpFile = "data.csv";
boolean logData = true;


void setup() {
  Serial.begin(9600);

  //If accelerometer can't be started, stall the program
  {
    Serial.println("Unable to initialize the accelerometer portion of the LSM303!");
    while(1);
  }

  //If magnetometer can't be started, stall the program
  if(!mag.begin())
  {
    Serial.println("Unable to initialize the magnetometer portion of the LSM303!");
    while(1);
  }

  //If SD Card can't be started we'll continue the program, but set SDPresent to false
  //so that we know later not to try and log to or read from the SD Card
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("Unable to initialize the SD Card!");
    SDPresent = false;
  }
  else{
    //SD Card is here, time to load Config
    //First, check to see if Config File exists
    if(SD.exists("config.txt")){
      //If so, open it
      configFile = SD.open("config.txt", FILE_READ);
      //TODO PULL VARIABLES
      configFile.close();
    }
    else{
      //If not, create it and input variables
    }
    

    
  } 

  //If BME Sensor can't be started, stall the program
  if (!bme.begin()) {
        Serial.println("Unable to initialize the BME280!");
        while (1);
  }
  

}

void loop() {
  //TODO

}

void setupSDCard(){
  configFile = SD.open("config.txt", FILE_WRITE);
  Serial.println("Empty SD Card detected, writing variables...");
  
  configFile.println("//Callsign, this will essentialle be your payload ID");
  configFile.println("CallSign=");
  configFile.println();
  configFile.println("//Radio frequency, this is the approximate frequency the MTX transmitter will transmit at");
  configFile.println("//Default is 434.650");
  configFile.println("Frequency=434.650");
  configFile.println();
  configFile.println("//Do we want to log data?");
  configFile.println("//Default is true");
  configFile.println("Log Data=true");
  configFile.println();
  configFile.println("//Name of the data dump file");
  configFile.println("Default is data.csv");
  configFile.println("Data File=data.csv");

  configFile.close();
}

boolean loadSDValues(){
  configFile = SD.open("config.txt", FILE_READ);
  boolean setFrequency = false;
  boolean setCallSign = false;
  boolean setDataFile = false;
  boolean setLogData = false;

  //Read lines until we get our config values, then set them to the global variables
  while(configFile.available()){
    String nextLine = configFile.read();

    //Check if it's a line with one of our vars
    if(nextLine.startsWith("CallSign=")){
      CallSign = nextLine.substring(9).trim(); //Everything after the "="
      Serial.println("CallSign set to: " + CallSign);
      setCallSign = CallSign.length > 0;
    }
    else if(nextLine.startsWith("Frequency=")){
      frequency = nextLine.substring(10).trim(); //Everything after the "="
      Serial.println("Frequency set to: " + frequency + " MHz");
      setFrequency = frequency.length() > 0;
    }
    else if(nextLine.startsWith("Log Data=")){
      logData = (bool) nextLine.substring(9).trim().toLowerCase(); //Everything after the "="
      Serial.println("Logging data set to: " + logData);
      setLogData = true;
    }
    else if(nextLine.startsWith("Data File=")){
      dataDumpFile = nextLine.substring(10).trim(); //Everything after the "="
      Serial.println("Data File to: " + dataDumpFile);
      setDataFile = dataDumpFile.length() > 0;
    }
  }

  //Return whether settings have been set or not
  if(logData){
    return setFrequency && setCallSign && setDataFile && setLogData;
  }
  else{
    return setFrequency && setCallSign && setLogData;
  }
  
}
