#include "ADXL372.hpp"

class ADXL372class
{
private:
    SPI *spi;
    int m_csPin;

public:
    ADXL372class(SPI *spiDevice, int csPin) : spi(spiDevice), m_csPin(csPin)
    {
        pinMode(m_csPin, OUTPUT);
        digitalWrite(m_csPin, HIGH);
    }

    void ADXL372class::
    {
        byte devidAd = readRegister(DEVID_AD);
        byte devidMst = readRegister(DEVID_MST);
        byte partId = readRegister(PARTID);
        byte revId = readRegister(REVID);
        byte status = readRegister(STATUS);

        std::cout << "DEVID_AD: 0x" << std::hex << static_cast<int>(devidAd) << std::endl;
        std::cout << "DEVID_MST: 0x" << std::hex << static_cast<int>(devidMst) << std::endl;
        std::cout << "PARTID: 0x" << std::hex << static_cast<int>(partId) << std::endl;
        std::cout << "REVID: 0x" << std::hex << static_cast<int>(revId) << std::endl;
        std::cout << "STATUS: 0x" << std::hex << static_cast<int>(status) << std::endl;
    }

    uint8_t ADXL372class::readRegister(uint8_t regAddress)
    {

        digitalWrite(m_csPin, LOW);

        regAddress = (regAddress << 1) | 1;

        uint8_t txBuffer[2] = {regAddress, 0x00};
        uint8_t rxBuffer[2] = {0};

        spi->xfer(txBuffer, 2, rxBuffer, 2);

        digitalWrite(m_csPin, HIGH);

        return rxBuffer[1];
    }

    void writeRegister(uint8_t regAddress, uint8_t value)
    {

        digitalWrite(m_csPin, LOW);

        regAddress = regAddress << 1;

        uint8_t txBuffer[2] = {regAddress, value};
        spi->write(txBuffer, 2);

        digitalWrite(m_csPin, HIGH);
    }

    void ADXL372class::updateRegister(byte regAddress, uint8_t value, byte mask)
    {

        byte registerState = readRegister(regAddress);

        registerState &= mask;

        value |= registerState;

        writeRegister(regAddress, value);
    }

    bool ADXL372class::selfTest()
    {
        // Self test procedure (Page 27 in datasheet)
        setOperatingMode(FULL_BANDWIDTH);
        setFilterSettling(FSP_370ms);
        updateRegister(SELF_TEST, true, ST_MASK);
        delay(300);

        if ((readRegister(SELF_TEST) & ST_DONE_MASK) == false)
        {
            std::cout << "Self Test was not finished" << std::endl;
            return false;
        }

        bool isTestPassed = readRegister(SELF_TEST) & USER_ST_MASK;
        return isTestPassed;
    }

    uint8_t ADXL372class::reset(uint8_t regAddress)
    {
        writeRegister(RESET, 0x52);
    }

    void ADXL372class::setOdr(Odr odr)
    {
        int currentBandwidth = readRegister(MEASURE) & 0b00000111; // Get only the bandwidth bits
        if ((int)odr < currentBandwidth)
        {
            std::cout << "WARNING: ODR must be at least double the bandwidth, "
                      << "to not violate the Nyquist criteria. Otherwise signal"
                      << "integrity will not be maintained" << std::endl;
        }
        byte odrShifted = odr << 5; // odr bits start from bit 5
        updateRegister(TIMING, odrShifted, ODR_MASK);
    }

    void ADXL372class::setBandwidth(Bandwidth bandwidth)
    {
        int currentOdr = (readRegister(TIMING) & 0b11100000) >> 5; // Get only the ODR bits
        if ((int)bandwidth > currentOdr)
        {
            std::cout << "WARNING: Bandwidth must be no greater than half the ODR, "
                      << "to not violate the Nyquist criteria. Otherwise signal integrity will"
                      << "not be maintained" << std::endl;
        }
        updateRegister(MEASURE, bandwidth, BANDWIDTH_MASK);
    }

    void ADXL372class::readAcceleration(float &x, float &y, float &z)
    {
        byte status;
        do
        {
            status = readRegister(0x04);
        } while ((status & 0x01) == 0); // Waiting for status register

        // The register is left justified. *DATA_H has bits 11:4 of the register. *DATA_L has bits 3:0.
        short rawX = readRegister(XDATA_H) << 8 | readRegister(XDATA_L);
        short rawY = readRegister(YDATA_H) << 8 | readRegister(YDATA_L);
        short rawZ = readRegister(ZDATA_H) << 8 | readRegister(ZDATA_L);

        rawX = rawX >> 4;
        rawY = rawY >> 4;
        rawZ = rawZ >> 4;

        // Converting raw axis data to acceleration in g unit
        x = rawX * SCALE_FACTOR * MG_TO_G;
        y = rawY * SCALE_FACTOR * MG_TO_G;
        z = rawZ * SCALE_FACTOR * MG_TO_G;
    }

    void ADXL372class::readAccelerationX(float &x)
    {
        byte status;
        do
        {
            status = readRegister(0x04);
        } while ((status & 0x01) == 0); // Waiting for status register

        // The register is left justified. *DATA_H has bits 11:4 of the register. *DATA_L has bits 3:0.
        short rawX = readRegister(XDATA_H) << 8 | readRegister(XDATA_L);

        rawX = rawX >> 4;

        // Converting raw axis data to acceleration in g unit
        x = rawX * SCALE_FACTOR * MG_TO_G;
    }

    void ADXL372class::readAccelerationY(float &y)
    {
        byte status;
        do
        {
            status = readRegister(0x04);
        } while ((status & 0x01) == 0); // Waiting for status register

        // The register is left justified. *DATA_H has bits 11:4 of the register. *DATA_L has bits 3:0.
        short rawY = readRegister(YDATA_H) << 8 | readRegister(YDATA_L);

        rawY = rawY >> 4;

        // Converting raw axis data to acceleration in g unit
        Y = rawY * SCALE_FACTOR * MG_TO_G;
    }

    void ADXL372class::readAccelerationZ(float &z)
    {
        byte status;
        do
        {
            status = readRegister(0x04);
        } while ((status & 0x01) == 0); // Waiting for status register

        // The register is left justified. *DATA_H has bits 11:4 of the register. *DATA_L has bits 3:0.
        short rawZ = readRegister(ZDATA_H) << 8 | readRegister(ZDATA_L);

        rawZ = rawZ >> 4;

        // Converting raw axis data to acceleration in g unit
        Z = rawZ * SCALE_FACTOR * MG_TO_G;
    }

    void ADXL372class::readFifoData(uint16_t *fifoData)
    {
        byte status;
        do
        {
            status = readRegister(0x04);
        } while ((status & 0x04) == 0); // Waiting for FIFO full

        digitalWrite(m_csPin, LOW);
        SPI.transfer(FIFO_DATA << 1 | 1);
        for (int i = 0; i < m_sampleSize; i++)
        {
            uint8_t msbFifoData = SPI.transfer(0x00);
            uint8_t lsbFifoData = SPI.transfer(0x00);
            fifoData[i] = msbFifoData << 4 | lsbFifoData; // 12 bit data. 8 MSB and 4 LSB.
        }
        digitalWrite(m_csPin, HIGH);
    }
    void ADXL372class::setFifoSamples(int sampleSize)
    {
        checkStandbyMode();
        if (sampleSize > 512)
        {
            std::cout << "WARNING: FIFO samples limit is 512" << std::endl;
            Serial.println("WARNING: FIFO samples limit is 512");
            sampleSize = 512;
        }
        m_sampleSize = sampleSize;
        sampleSize -= 1;
        writeRegister(FIFO_SAMPLES, sampleSize & 0xFF); // Sending the 8 least significant bits in the samples
        updateRegister(FIFO_CTL, (sampleSize > 0xFF), FIFO_SAMPLES_8_MASK);
    }

    void ADXL372class::setFifoMode(FifoMode mode)
    {
        checkStandbyMode();
        byte modeShifted = mode << 1; // starts from bit 1 in register
        updateRegister(FIFO_CTL, modeShifted, FIFO_MODE_MASK);
    }

    void ADXL372class::setFifoFormat(FifoFormat format)
    {
        checkStandbyMode();
        byte formatShifted = format << 3; // starts from bit 3 in register
        updateRegister(FIFO_CTL, formatShifted, FIFO_FORMAT_MASK);
    }

    void ADXL372class::selectInt1Function(InterruptFunction function)
    {
        updateRegister(INT1_MAP, function, INT_MAP_MASK);
    }

    void ADXL372class::selectInt1Functions(uint8_t function)
    {
        writeRegister(INT1_MAP, function);
    }

    void ADXL372class::selectInt2Function(InterruptFunction function)
    {
        updateRegister(INT2_MAP, function, INT_MAP_MASK);
    }

    void ADXL372class::selectInt2Functions(uint8_t function)
    {
        writeRegister(INT2_MAP, function);
    }

    void ADXL372class::enableExternalClock(bool isEnabled)
    {
        byte valueShifted = isEnabled << 1; // bit 1 in register
        updateRegister(TIMING, valueShifted, EXT_CLK_MASK);
    }

    void ADXL372class::enableLowNoiseOperation(bool isEnabled)
    {
        byte valueShifted = isEnabled << 3; // bit 3 in register
        updateRegister(MEASURE, valueShifted, LOW_NOISE_MASK);
    }

    void ADXL372class::setOperatingMode(OperatingMode opMode)
    {
        updateRegister(POWER_CTL, opMode, MODE_MASK);
    }

    void ADXL372class::setFilterSettling(FilterSettlingPeriod filterSettling)
    {
        byte valueShifted = filterSettling << 4; // bit 4 in register
        updateRegister(POWER_CTL, valueShifted, LPF_DISABLE_MASK);
    }
};