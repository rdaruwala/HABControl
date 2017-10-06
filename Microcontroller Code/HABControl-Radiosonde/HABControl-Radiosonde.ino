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
File dataFile;

//Initialize BME Sensor (Temperature, Pressure, Humidity)
Adafruit_BME280 bme; 
#define SEALEVELPRESSURE_HPA (1013.25) //CALIBRATION
float CALI_PRESSURE = 1013.25; 


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
  if(!accel.begin()){
    Serial.println(F("Unable to initialize the accelerometer portion of the LSM303!"));
    while(1);
  }

  mag.enableAutoRange(true);
  //If magnetometer can't be started, stall the program
  if(!mag.begin())
  {
    Serial.println(F("Unable to initialize the magnetometer portion of the LSM303!"));
    while(1);
  }

  //If SD Card can't be started we'll stall
  if (!SD.begin(10)) {
    Serial.println(F("Unable to initialize the SD Card!"));
    while(1);
  }
  else{
    //SD Card is here, time to load Config
    //First, check to see if Config File exists
    if(SD.exists("config.txt")){
      if(loadSDValues()){
        Serial.println(F("Loaded config values successfully!"));

        //If we're datalogging, let's load up the data file
        dataFile = SD.open(dataDumpFile, FILE_WRITE);
        //TODO PUT INITIAL CSV VALUES
        dataFile.close();
      }
      else{
        Serial.println(F("Error loading SD card values! Please fill out the settings and try again."));
        SD.remove("config.txt");
        setupSDCard();
        while(1);
      }
    }
    else{
      setupSDCard();
      while(1);
    }
    

    
  } 

  //If BME Sensor can't be started, stall the program
  if (!bme.begin()) {
        Serial.println(F("Unable to initialize the BME280!"));
        while (1);
  }
  

}

void loop() {
  //TODO

  //BME Variables
  float BMEtemp = bme.readTemperature();
  float BMEpressure = bme.readPressure() / 100.0F;
  float BMEaltitude = bme.readAltitude(CALI_PRESSURE);
  float BMEhumidity = bme.readHumidity();

  //Accelerometer + Magnetometer Variables
  sensors_event_t event;
  accel.getEvent(&event);

  float magX = event.magnetic.x; //Raw (Non-unified) is mag.raw.x 
  float magY = event.magnetic.y; //Raw (Non-unified) is mag.raw.y
  float magZ = event.magnetic.z; //Raw (Non-unified) is mag.raw.z

  float accelX = event.acceleration.x; //Raw (Non-unified) is accel.raw.x 
  float accelY = event.acceleration.y; //Raw (Non-unified) is accel.raw.y
  float accelZ = event.acceleration.z; //Raw (Non-unified) is accel.raw.z

  Serial.println("BME Temperature: " + (String)BMEtemp);
  Serial.println("BME Pressure: " + (String)BMEpressure);
  Serial.println("BME Altitude: " + (String)BMEaltitude);
  Serial.println("BME Humidity: " + (String)BMEhumidity);

  Serial.println("MagX: " + (String)magX);
  Serial.println("MagY: " + (String)magY);
  Serial.println("MagZ: " + (String)magZ);

  Serial.println("AccelX: " + (String)accelX);
  Serial.println("AccelY: " + (String)accelY);
  Serial.println("AccelZ: " + (String)accelZ);

  delay(3000);
}

void setupSDCard(){
  File configFile = SD.open("config.txt", FILE_WRITE);
  Serial.println(F("Writing variables..."));

  if(configFile){
  configFile.println(F("//Callsign, this will essentially be your payload ID"));
  configFile.println(F("CallSign="));
  configFile.println();
  configFile.println(F("//Radio frequency, this is the approximate frequency the MTX transmitter will transmit at"));
  configFile.println(F("//Default is 434.650"));
  configFile.println(F("Frequency=434.650"));
  configFile.println();
  configFile.println(F("//Sea-Level Pressure for the Barometer, in units hPa"));
  configFile.println(F("//Default is 1013.25"));
  configFile.println(F("Sea Pressure=1013.25"));
  configFile.println();
  configFile.println(F("//Do we want to log data?"));
  configFile.println(F("//Default is true"));
  configFile.println(F("Log Data=true"));
  configFile.println();
  configFile.println(F("//Name of the data dump file"));
  configFile.println(F("Default is data.csv"));
  configFile.println(F("Data File=data.csv"));
  configFile.close();
  Serial.println(F("Please fill out the config files on the SD card and restart the program"));
  }
  else{
    Serial.println(F("There was an error writing to the SD Card."));
  }
}

boolean loadSDValues(){
  File configFile = SD.open("config.txt", FILE_READ);
  boolean setFrequency = false;
  boolean setCallSign = false;
  boolean setDataFile = false;
  boolean setLogData = false;
  boolean setCalibrationPressure = false;

  //Read lines until we get our config values, then set them to the global variables
  while(configFile.available()){
    String nextLine = "";
    boolean doneWithLine = false;
    while(!doneWithLine){
      char nextChar = configFile.read();
      if(nextChar == '\r' || nextChar == '\n'){
        doneWithLine = true;
      }
      else{
        nextLine = nextLine + nextChar;
      }
      
    }

    //Check if it's a line with one of our vars
    if(nextLine.startsWith("CallSign=")){
      CallSign = nextLine.substring(9); //Everything after the "="
      CallSign.trim();
      Serial.println("CallSign set to: " + CallSign);
      setCallSign = CallSign.length() > 0;
    }
    else if(nextLine.startsWith("Frequency=")){
      frequency = nextLine.substring(10); //Everything after the "="
      frequency.trim();
      Serial.println("Frequency set to: " + frequency + " MHz");
      setFrequency = frequency.length() > 0;
    }
    else if(nextLine.startsWith("Log Data=")){
      String temp = nextLine.substring(9); //Take string to convert to boolean
      temp = nextLine.substring(9); //Everything after the "="
      temp.trim();
      temp.toLowerCase();
      logData = (bool) temp;
      Serial.println("Logging data set to: " + (String)logData);
      setLogData = true;
    }
    else if(nextLine.startsWith("Data File=")){
      dataDumpFile = nextLine.substring(10); //Everything after the "="
      dataDumpFile.trim();
      Serial.println("Data File to: " + dataDumpFile);
      setDataFile = dataDumpFile.length() > 0;
    }
    else if(nextLine.startsWith("Sea Pressure=")){
      String temp = nextLine.substring(13); //Everything after the "="
      temp.trim();
      CALI_PRESSURE = temp.toFloat();
      Serial.println("Calibration Pressure set to: " + (String)CALI_PRESSURE);
      setCalibrationPressure = temp.length() > 0;
    }
  }

  configFile.close();
  //Return whether settings have been set or not
  if(logData){
    return setFrequency && setCallSign && setDataFile && setLogData && setCalibrationPressure;
  }
  else{
    return setFrequency && setCallSign && setLogData && setCalibrationPressure;
  }
  
}
