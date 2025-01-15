#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include "ADXL372.hpp"

spi_config_t spi_config;
uint8_t tx_buffer[32];
uint8_t rx_buffer[32];



int cs_pin = 27;


int sample_size = 510;

int main(void)
{

  SPI *ardunioSPI = NULL;

  spi_config.mode = 0;
  spi_config.speed = 1000000; // Do not set higher than 10 MHz
  spi_config.delay = 0;
  spi_config.bits_per_word = 8;

  wiringPiSetupGpio();
  pinMode(cs_pin, OUTPUT);

  ardunioSPI = new SPI("/dev/spidev0.1", &spi_config);

  ADXL372 = new ADXL372class(&arduinoSPI, cs_pin);

  if (ardunioSPI->begin())
  {
    // ADXL372 -> reset();
    ADXL372->printDevice();
    ADXL372->setOperatingMode(STANDBY);
    ADXL372->setOdr(ODR_6400Hz);
    ADXL372->setBandwidth(BW_3200Hz);
    ADXL372->setFifoSamples(sample_size);
    ADXL372->setFifoFormat(XYZ);
    ADXL372->setFifoMode(STREAM);
    ADXL372->setOperatingMode(FULL_BANDWIDTH);

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    auto start = std::chrono::high_resolution_clock::now();

    uint16_t fifoData[sample_size - 2];
    ADXL372.readFifoData(fifoData);
    while (true)
    {
      ADXL372class->readAcceleration(x, y, z);
      auto finish = std::chrono::high_resolution_clock::now();
      std::cout << "Time: " << finish << 
      for (int i = 0; i < sample_size - 2; i++)
      {
        if ((fifoData[i] & 0x01) == 1)
        {
          std::cout << "" << std::endl;
        }
        x = (fifoData[i] >> 4) * 100 * 0.001;
        std::cout << x;
      }
      std::cout << finish << " " << x << " " << y << " " << z << std::endl;
    }
    ardunioSPI->end();
  }
  else
  {
    STD::cout << "Failed to initiate communication via SPI" << std::endl;
  }
  return 1;
}