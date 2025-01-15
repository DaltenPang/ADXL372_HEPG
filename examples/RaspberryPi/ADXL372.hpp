#include <stdint.h>  
#include <wiringPi.h> 
#include <spidev_lib++.h>
#include <iostream>

typedef uint8_t byte;

enum FifoMode
{
    FIFO_DISABLED = 0b00,
    STREAM = 0b01,
    TRIGGER = 0b10,
    OLDEST_SAVED = 0b11
};

enum FifoFormat
{
    XYZ = 0b000,
    X = 0b001,
    Y = 0b010,
    XY = 0b011,
    Z = 0b100,
    XZ = 0b101,
    YZ = 0b110,
    XYZ_PEAK = 0b111
};

enum Odr
{
    ODR_400Hz = 0b000,
    ODR_800Hz = 0b001,
    ODR_1600Hz = 0b010,
    ODR_3200Hz = 0b011,
    ODR_6400Hz = 0b100
};

enum WakeUpRate
{
    WUR_52ms = 0b000,
    WUR_104ms = 0b001,
    WUR_208ms = 0b010,
    WUR_512ms = 0b011,
    WUR_2048ms = 0b100,
    WUR_4096ms = 0b101,
    WUR_8192ms = 0b110,
    WUR_24576ms = 0b111,
};

enum Bandwidth
{
    BW_200Hz = 0b000,
    BW_400Hz = 0b001,
    BW_800Hz = 0b010,
    BW_1600Hz = 0b011,
    BW_3200Hz = 0b100
};

enum LinkLoop
{
    DEFAULT = 0b00,
    LINKED = 0b01,
    LOOPED = 0b10
};

enum OperatingMode
{
    STANDBY = 0b00,
    WAKE_UP = 0b01,
    INSTANT_ON = 0b10,
    FULL_BANDWIDTH = 0b11
};

enum FilterSettlingPeriod
{
    FSP_370ms = 0,
    FSP_16ms = 1
};

enum InstantOnThreshold
{
    IOT_LOW_THRESH = 0,
    IOT_HIGH_THRESH = 1
};

enum InterruptFunction {
    DATA_RDY = 0b00000000,
    FIFO_RDY = 0b00000010,
    FIFO_FULL = 0b00000100,
    FIFO_OVR = 0b00001000,
    INACT = 0b00010000,
    ACT = 0b00100000,
    ACT2 = 0b00100000,
    AWAKE = 0b01000000,
    INT_LOW = 0b10000000
};



#define DEVID_PRODUCT 0xFA // 372 in octal :)

// Data registers. Each axis data has a 12 bit value. Data is left justified, MSBFIRST.
// Register *_H contains the eight most significant bits (MSBs), and Register *_L contains the four least significant bits (LSBs) of the 12-bit value
#define XDATA_H 0x08
#define XDATA_L 0x09
#define YDATA_H 0x0A
#define YDATA_L 0x0B
#define ZDATA_H 0x0C
#define ZDATA_L 0x0D

// Peak Data registers.
#define MAXPEAK_X_H 0x15
#define MAXPEAK_X_L 0x16
#define MAXPEAK_Y_H 0x17
#define MAXPEAK_Y_L 0x18
#define MAXPEAK_Z_H 0x19
#define MAXPEAK_Z_L 0x1A

// ID registers
#define DEVID_AD 0x00
#define DEVID_MST 0x01
#define PARTID 0x02
#define REVID 0x03

// System registers
#define STATUS 0x04 // Status register

#define OFFSET_X 0x20
#define OFFSET_Y 0x21
#define OFFSET_Z 0x22

#define THRESH_ACT_X_H 0x23 // Activity threshold register
#define THRESH_ACT_X_L 0x24
#define THRESH_ACT_Y_H 0x25
#define THRESH_ACT_Y_L 0x26
#define THRESH_ACT_Z_H 0x27
#define THRESH_ACT_Z_L 0x28

#define TIME_ACT 0x29 // Activity time register

#define THRESH_INACT_X_H 0x2A // Inactivity threshold register
#define THRESH_INACT_X_L 0x2B
#define THRESH_INACT_Y_H 0x2C
#define THRESH_INACT_Y_L 0x2D
#define THRESH_INACT_Z_H 0x2E
#define THRESH_INACT_Z_L 0x2F

#define TIME_INACT_H 0x30 // Inactivity time register
#define TIME_INACT_L 0x31

#define THRESH_ACT2_X_H 0x32 // Motion Warning Threshold register
#define THRESH_ACT2_X_L 0x33
#define THRESH_ACT2_Y_H 0x34
#define THRESH_ACT2_Y_L 0x35
#define THRESH_ACT2_Z_H 0x36
#define THRESH_ACT2_Z_L 0x37

#define FIFO_SAMPLES 0x39 // FIFO samples register
#define FIFO_CTL 0x3A     // FIFO control register

#define INT1_MAP 0x3B // Interrupt 1 & 2 map register
#define INT2_MAP 0x3C

#define TIMING 0x3D    // Timing control register
#define MEASURE 0x3E   // Measurement control register
#define POWER_CTL 0x3F // Power control register

#define SELF_TEST 0x40 // Self test register
#define RESET 0x41     // Reset register
#define FIFO_DATA 0x42 // FIFO data register

// System bitmasks
#define THRESH_ACT_L_MASK 0x1F // Activity detection
#define ACT_EN_MASK 0xFE
#define ACT_REF_MASK 0xFD

#define THRESH_INACT_L_MASK 0x1F // Inactivity detection
#define INACT_EN_MASK 0xFE
#define INACT_REF_MASK 0xFD

#define THRESH_ACT2_L_MASK 0x1F // Motion Warning
#define ACT2_EN_MASK 0xFE
#define ACT2_REF_MASK 0xFD

#define FIFO_SAMPLES_8_MASK 0xFE // FIFO control
#define FIFO_MODE_MASK 0xF9
#define FIFO_FORMAT_MASK 0xC7

#define INT_MAP_MASK 0xFF // Interrupt 1 and 2

#define EXT_SYNC_MASK 0xFE // Timing
#define EXT_CLK_MASK 0xFD
#define WAKEUP_RATE_MASK 0xE3
#define ODR_MASK 0x1F

#define BANDWIDTH_MASK 0xF8 // Measure
#define LOW_NOISE_MASK 0xF7
#define LINKLOOP_MASK 0xCF
#define AUTOSLEEP_MASK 0xBF

#define MODE_MASK 0xFC // Power Control
#define HPF_DISABLE_MASK 0xFB
#define LPF_DISABLE_MASK 0xF7
#define FILTER_SETTLE_MASK 0xEF
#define INSTANT_ON_THRESH_MASK 0xDF

#define ST_MASK 0xFE // Self test
#define ST_DONE_MASK 0xFD
#define USER_ST_MASK 0xFB

// Accelerometer Constants
#define SPI_SPEED 10000000 // ADXL372 supports up to 10MHz in SCLK frequency
#define SCALE_FACTOR 100   // mg per LSB
#define MG_TO_G 0.001      // g per mg

class ADXL372class
{
private:
    SPI *spi;
    int m_csPin;
    int m_sampleSize;


public:
    // Constructor
    ADXL372class(SPI *spiDevice, int csPin);

    // Methods
    void initialize();
    uint8_t readRegister(uint8_t regAddress);
    void writeRegister(uint8_t regAddress, uint8_t value);
    void updateRegister(uint8_t regAddress, uint8_t value, uint8_t mask);
    bool selfTest();
    uint8_t reset();
    void setOdr(Odr odr);
    void setBandwidth(Bandwidth bandwidth);
    void readAcceleration(float &x, float &y, float &z);
    void readAccelerationX(float &x);
    void readAccelerationY(float &y);
    void readAccelerationZ(float &z);
    void readFifoData(uint16_t *fifoData);
    void setFifoSamples(int sampleSize);
    void setFifoMode(FifoMode mode);
    void setFifoFormat(FifoFormat format);
    void selectInt1Function(uint8_t function);
    void selectInt2Function(uint8_t function);
    void enableExternalClock(bool isEnabled);
    void enableLowNoiseOperation(bool isEnabled);
    void setOperatingMode(OperatingMode opMode);
    void checkStandbyMode();
    void setFilterSettling(FilterSettlingPeriod filterSettling);
};

#endif // ADXL372_HPP