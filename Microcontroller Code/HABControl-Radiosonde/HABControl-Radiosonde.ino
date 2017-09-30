#include <Wire.h>
#include <SPI.h>
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10


Adafruit_BME280 bme; // I2C
#define SEALEVELPRESSURE_HPA (1013.25) //CALIBRATION


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
