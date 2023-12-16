#include "Arduino.h"
#include "SPI.h"

class ADXL372class
{
public:
    ADXL372class(int csPinInput); 
    virtual ~ADXL372class();

    int begin();
    void end();
    void readAcceleration(float& x, float& y, float& z);
    int accelerationAvailable(); 
    byte SPIreadByte(uint8_t subAddress);

private:
    int m_csPin; //Choose any digital pin

    void SPIwriteByte(uint8_t subAddress, uint8_t data);
    void readAccelerometerRegister(uint16_t* data, size_t length);

};