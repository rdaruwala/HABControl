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
#include <SoftwareSerial.h>
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"


//Variables used for the SD Card (Data Logging & Storing Variable Data (Transmit Frequency, Callsign, etc.))
//File dataFile;

//Initialize BME Sensor (Temperature, Pressure, Humidity)
Adafruit_BME280 bme; 
float CALI_PRESSURE = 1013.25; 


//Initialize subparts for the LSM303 (Accelerometer & Magnetometer) 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


//Data to get
String frequency = "";
String CallSign = "";
String dataDumpFile = "data.csv";
boolean logData = true;
boolean radio = true;

//GPS Stuff
SoftwareSerial GPS(4, 3);
byte gps_set_sucess = 0 ;


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
  if (!SD.begin(8)) {
    Serial.println(F("Unable to initialize the SD Card!"));
    while(1);
  }


  //Start GPS @ 9600 bps
  GPS.begin(9600); 
  //
  // THE FOLLOWING COMMAND SWITCHES MODULE TO 4800 BAUD
  // THEN SWITCHES THE SOFTWARE SERIAL TO 4,800 BAUD
  //
  GPS.print("$PUBX,41,1,0007,0003,4800,0*13\r\n"); 
  GPS.begin(4800);
  GPS.flush();

  //FLIGHT MODE
  uint8_t setNav[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 
    0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC };
  while(!gps_set_sucess)
  {
    sendUBX(setNav, sizeof(setNav)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(setNav);
  }
  gps_set_sucess=0;

  //Disable auto-GPS polling
  GPS.println("$PUBX,40,GLL,0,0,0,0*5C"); 
  GPS.println("$PUBX,40,GGA,0,0,0,0*5A");
  GPS.println("$PUBX,40,GSA,0,0,0,0*4E");
  GPS.println("$PUBX,40,RMC,0,0,0,0*47");
  GPS.println("$PUBX,40,GSV,0,0,0,0*59");
  GPS.println("$PUBX,40,VTG,0,0,0,0*5E");
 

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


  //Request GPS String
  GPS.println("$PUBX,00*33");
  // Wait for response
  delay(1200); 

  //Loop through and print out what's available (To be changed later)
  while(GPS.available()){
    Serial.write(GPS.read());
  }

  delay(3000);
}



// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i=0; i<len; i++) {
    GPS.write(MSG[i]);
    Serial.print(MSG[i], HEX);
  }
  GPS.println();
}
 
 
// Calculate expected UBX ACK packet and parse UBX response from GPS
boolean getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  Serial.print(" * Reading ACK response: ");
 
  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;  // header
  ackPacket[1] = 0x62;  // header
  ackPacket[2] = 0x05;  // class
  ackPacket[3] = 0x01;  // id
  ackPacket[4] = 0x02;  // length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];  // ACK class
  ackPacket[7] = MSG[3];  // ACK id
  ackPacket[8] = 0;   // CK_A
  ackPacket[9] = 0;   // CK_B
 
  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }
 
  while (1) {
 
    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      Serial.println(" (SUCCESS!)");
      return true;
    }
 
    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
      Serial.println(" (FAILED!)");
      return false;
    }
 
    // Make sure data is available to read
    if (GPS.available()) {
      b = GPS.read();
 
      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) { 
        ackByteID++;
        Serial.print(b, HEX);
      } 
      else {
        ackByteID = 0;  // Reset and look again, invalid order
      }
 
    }
  }
}
