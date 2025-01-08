#include "ADXL372.h"

ADXL372class ADXL372(7); // Pin 10 as CS pin of the accelerometer. Use the corresponding pin on your own board
//ADXL372class ADXL372_1(9);

float x = 0.0;
float y = 0.0;
float z = 0.0;
int time = 0;
float counter = 0;
unsigned long prev_time = 0;
int sample_size = 170;
void setup()
{
  Serial.begin(2000000);
  //Serial.println("Sensor 0");
  ADXL372.begin(2000000);
  ADXL372.reset();
  //ADXL372.printDevice();
  //ADXL372.setOperatingMode(FULL_BANDWIDTH);
  //ADXL372.selfTest();
  ADXL372.setOperatingMode(STANDBY);
  ADXL372.setOdr(ODR_6400Hz);
  ADXL372.setBandwidth(BW_3200Hz);
  ADXL372.setFifoSamples(sample_size);
  ADXL372.setFifoFormat(XYZ);
  ADXL372.setFifoMode(STREAM);
 
  //ADXL372.enableExternalClock(true);
  //ADXL372.enableExternalTrigger(false);
  ADXL372.setOperatingMode(FULL_BANDWIDTH);
  ADXL372.end();
  /*ADXL372_1.begin(2000000);
  //ADXL372.selfTest();
  //Serial.println("Sensor 1");
  ADXL372_1.reset();
  //ADXL372_1.printDevice();
  //ADXL372_1.selfTest();
  ADXL372_1.setOperatingMode(STANDBY);
  ADXL372_1.setOdr(ODR_6400Hz);
  ADXL372_1.setBandwidth(BW_3200Hz);
  ADXL372_1.setOperatingMode(FULL_BANDWIDTH);
  ADXL372_1.end();*/
  delay(2000);

}

void loop()
{
  if (counter < 1) {
  ADXL372.begin(2000000);
  unsigned long timestamp = micros();
  /*Serial.print(timestamp);
  Serial.print(" ");*/
  uint16_t fifoData[sample_size - 2];
  ADXL372.readFifoData(fifoData);
  //ADXL372.setFifoMode(STREAM);
  for (int i = 0; i < sample_size - 2; i++) {
    if ((fifoData[i] & 0x01) == 1) {
      Serial.println("New Data Point");
    }

    x = (fifoData[i] >> 4) * 100 * 0.001;
      Serial.println(x);

    //Serial.print("");
  }
  counter += 1;
  }
  
  /*Serial.println(fifoData[0]);
  Serial.print("Shifted 4 bits: ");
  Serial.println((fifoData[0] >> 4) * 100 * 0.001);
  Serial.print(x);*/
  
}