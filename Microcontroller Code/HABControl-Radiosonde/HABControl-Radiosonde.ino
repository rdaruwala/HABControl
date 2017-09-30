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
*/



#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;
boolean SDPresent;

Adafruit_BME280 bme; 
#define SEALEVELPRESSURE_HPA (1013.25) //CALIBRATION


Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);




void setup() {
  Serial.begin(9600);
  SDPresent = true;

  if(!accel.begin())
  {
    Serial.println("Unable to initialize the accelerometer portion of the LSM303!");
    while(1);
  }

  if(!mag.begin())
  {
    Serial.println("Unable to initialize the magnetometer portion of the LSM303!");
    while(1);
  }

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("Unable to initialize the SD Card!");
    SDPresent = false;
  } 

  if (!bme.begin()) {
        Serial.println("Unable to initialize the BME280!");
        while (1);
  }
  

}

void loop() {
  //TODO

}
