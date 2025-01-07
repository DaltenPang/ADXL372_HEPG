#include "ADXL372.h"

ADXL372class ADXL372(10); // Pin 10 as CS pin of the accelerometer. Use the corresponding pin on your own board
ADXL372class ADXL372_1(9);

float x = 0.0;
float y = 0.0;
float z = 0.0;
int time = 0;
float counter = 0;
void setup()
{
  Serial.begin(2000000);
  Serial.println("Sensor 0");
  ADXL372.begin(2000000);
  ADXL372.reset();
  ADXL372.printDevice();
  //ADXL372.setOperatingMode(FULL_BANDWIDTH);
  //ADXL372.selfTest();
  ADXL372.setOperatingMode(FULL_BANDWIDTH);
  ADXL372.setOdr(ODR_6400Hz);
  ADXL372.setBandwidth(BW_3200Hz);
  ADXL372.setFifoFormat(XYZ);
  ADXL372.setFifoMode(STREAM);
  ADXL372.setFifoSamples(170);
  ADXL372.end();
  ADXL372_1.begin(2000000);
  //ADXL372.selfTest();
  Serial.println("Sensor 1");
  ADXL372_1.reset();
  ADXL372_1.printDevice();
  //ADXL372_1.selfTest();
  ADXL372_1.setOperatingMode(FULL_BANDWIDTH);
  ADXL372_1.setOdr(ODR_6400Hz);
  ADXL372_1.setBandwidth(BW_3200Hz);
  ADXL372_1.end();
  delay(1000);
}

void loop()
{
  if (counter <= 1) {
    ADXL372.begin(2000000);
    unsigned long timestamp = micros();
    Serial.print(timestamp);
    Serial.print(" ");
    uint16_t fifoData[170];
    ADXL372.readFifoData(fifoData);
    /*for (int i = 0; i < 170; i++) {
      Serial.print(fifoData[i]);
      Serial.print(" ");
    }*/
    Serial.println(fifoData[0]);
    Serial.print("Shifted 4 bits: ");
    Serial.println(fifoData[0] >> 4);
    x = fifoData[0] >> 12;
    y = (fifoData[0] & 0xF00) >> 8;
    z = (fifoData[0] & 0xF0) >> 4;
    Serial.print("Relevant Bits: ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(z);
    Serial.println();
    ADXL372.end();
  }
  if (millis()%1000 == 0) {
    counter += 1;
  }
}