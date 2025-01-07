#include "ADXL372.h"

ADXL372class ADXL372_1(10); // Pin 10 as CS pin of the accelerometer. Use the corresponding pin on your own board
ADXL372class ADXL372_2(9); // Pin 9

float x_1 = 0;
float y_1 = 0;
float z_1 = 0;
bool isEnabled = true;

//Setup for 2nd accelerometer
float x_2 = 0;
float y_2 = 0;
float z_2 = 0;

void setup()
{
  Serial.begin(2000000);
  ADXL372_1.begin();
  ADXL372_2.begin();

  ADXL372_1.printDevice();
  ADXL372_2.printDevice();

  ADXL372_1.setOperatingMode(STANDBY); // Set accelerometer in standby to update ceratin registers
  ADXL372_2.setOperatingMode(STANDBY); // Set accelerometer in standby to update ceratin registers

  //ADXL372.enableLowNoiseOperation(isEnabled);
  
  ADXL372_1.setBandwidth(BW_3200Hz);
  ADXL372_2.setBandwidth(BW_3200Hz);
  
  ADXL372_1.setOdr(ODR_6400Hz);
  ADXL372_2.setOdr(ODR_6400Hz);

  ADXL372_1.setOperatingMode(FULL_BANDWIDTH);
  ADXL372_2.setOperatingMode(FULL_BANDWIDTH); 
}
void loop()
{
  ADXL372_1.readAcceleration(x_1, y_1, z_1);
  //ADXL372_1.readAccelerationZ(z_1);
  ADXL372_2.readAcceleration(x_2, y_2, z_2);
  //ADXL372_2.readAccelerationZ(z_2);
  //Serial.print((millis()/1000.0), 5);
  Serial.print(micros()/1000000.0, 6); //for micro seconds to seconds with 5 decimal places
  //Serial.print(' '); Serial.print(x); Serial.print(' '); Serial.print(y); 
  Serial.print(' '); Serial.print(z_1); Serial.print(' '); Serial.println(z_2); // Print Z acceleration of the first then second accelerometer. 
  //Serial.print(' '); Serial.println(x_2);
  //delay(1); // Change this for faster/slower measurements
  //delayMicroseconds(1); // 100 = 1/10th of a microsecond or 0.0001 s
}