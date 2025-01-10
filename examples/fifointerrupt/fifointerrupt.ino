#include "ADXL372.h"

#define INT1 5

ADXL372class ADXL372(10); // Pin 10 as CS pin of the accelerometer. Use the corresponding pin on your own board
//ADXL372class ADXL372_1(9);




float x = 0.0;
float y = 0.0;
float z = 0.0;
int time = 0;
float counter = 0;
unsigned long prev_time = 0;
int sample_size = 511;


float convertHextoDecimal(uint16_t sample) {
  sample &= 0x0FFF;
  if (sample & 0x0800) {
    sample |= 0xF000;
  }
  return sample;
}

void setup()
{
  pinMode(INT1, INPUT);
  Serial.begin(2000000);
  //Serial.println("Sensor 0");
  ADXL372.begin(2000000);
  //ADXL372.reset();
  delay(1000);
  ADXL372.reset();
  ADXL372.printDevice();
  ADXL372.enableExternalTrigger(false);
  ADXL372.selectInt1Function(FIFO_FULL);
  //ADXL372.setOperatingMode(STANDBY);
  //ADXL372.setFifoMode(FIFO_DISABLED);
  //ADXL372.setOperatingMode(FULL_BANDWIDTH);
  //ADXL372.selfTest();
  ADXL372.setOperatingMode(STANDBY);
  ADXL372.setFifoMode(FIFO_DISABLED);
  ADXL372.setOdr(ODR_6400Hz);
  ADXL372.setBandwidth(BW_3200Hz);
  ADXL372.setFifoSamples(sample_size);
  ADXL372.setFifoFormat(XYZ);
  ADXL372.setOperatingMode(STANDBY);
  ADXL372.setFifoMode(STREAM);
  ADXL372.enableExternalClock(true);
  ADXL372.setOdr(ODR_6400Hz);
  ADXL372.enableExternalTrigger(false);
  ADXL372.setOperatingMode(FULL_BANDWIDTH);
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
  ADXL372.printDevice();
  delay(2000);
}



void loop()
{
  unsigned long timestamp = micros();

  if (counter < 10) { 
  uint16_t fifoData[sample_size];
  ADXL372.enableExternalClock(false);
  ADXL372.setOdr(ODR_6400Hz);
  if (digitalRead(INT1)) {
    
    ADXL372.enableExternalClock(true);
    ADXL372.setOdr(ODR_6400Hz);
    ADXL372.readFifoData2(fifoData);
    ADXL372.setOperatingMode(STANDBY);
    ADXL372.setFifoMode(FIFO_DISABLED);
    Serial.print(timestamp);
    Serial.print(" ");
    for (int i = 0; i < sample_size; i++) {
        /*if ((fifoData[i] & 0x01) == 1) {
          Serial.println();
        }*/

        x = convertHextoDecimal(fifoData[i]>>4) * 100 * 0.001;
          Serial.print(x);
          Serial.print(" ");
        //Serial.print("");
      }
    ADXL372.setFifoMode(STREAM);
    ADXL372.setOperatingMode(FULL_BANDWIDTH);

    Serial.println("");
  }
  /*if (millis()%1000 == 0) {
    counter += 10;
  }*/ 
  }


  //Serial.println(digitalRead(INT1));

  
  
  /*Serial.println(fifoData[0]);
  Serial.print("Shifted 4 bits: ");
  Serial.println((fifoData[0] >> 4) * 100 * 0.001);
  Serial.print(x);*/
}