# Wire based MPUxxxx library (works with STM32)
A Wire based MPUxxxx library intended for ARM Cortex STM32 devices, but since it uses the `Wire.h` library it works with ***EVERYTHING*** and yes, you can use it to take over the world!
## Usage
 1. Download the [latest release](https://github.com/abc123me/Wire_Based_MPU6050/releases)
 2. Extract and install into your arduino libraries folder
 3. Select Examples/MPU6050/ and experiment with them
## Boards tested
 - Arduino Uno (ATmega328P) @ 16MHz w/ 10KHz I2C
 - STM32F103 (32K) @ 72MHz w/ 400KHz I2C
## Documentation
### MPU6050
```c
MPU6050();                                 //Default constructor, address is MPU_DEFAULT_ADDRESS
MPU6050(uint8_t addr);                     //Allows you to set the address
void begin(uint32_t speed);                //Initializes the device with a custom clock speed
void begin();                              //Initializes the device with a clock speed of 10KHz
bool test();                               //Checks if the library can communicate to the device
int16_t whoAmI();                          //Returns the WHO_AM_I id from the chip, or -1 if error
void setInterruptsEnabled(bool en);        //Enables/disables interrupts
void setAccelerationRange(uint8_t range);  //Sets the range of the accelerometer
void setGyroscopeRange(uint8_t range);     //Sets the range of the gyrometer
void readAccelRaw(int16_t* buf);           //Reads the raw value of the accelerometer into buf
void readGyroRaw(int16_t* buf);            //Reads the raw value of the gyrometer into buf
void readAccelG(float* buf);               //Reads the value of the accelerometer (in Gs) into buf
void readAccelMS(float* buf);              //Reads the value of the accelerometer (in meters per second) into buf
void readGyroDPS(float* buf);              //Reads the value of the gyrometer (in degrees per second) into buf
float readTempC();                         //Reads the temperature in degrees celcius
float readTempF();                         //Reads the temperature in degrees farenheit
float getAccelerationMagnitude();          //Gets the magnitude of the acceleration vector
uint8_t getFeatureMask();                  //Returns a mask containing information about (the devices) features
void setBypass(bool bypass);               //Enables/Disables the I2C bypass feature
bool bypassOn();                           //Indicates the state of the I2C bypass feature
```
### MPU9250
```c
/* Derives functions from MPU6050 */ 
MPU6050();                                 //Default constructor, addresses are MPU_DEFAULT_ADDRESS and AK8963_DEFAULT_ADDRESS
MPU6050(uint8_t addr, uint8_t mag_addr);   //Allows you to set both the MPU address and AK8963 address
uint8_t readMagnetometer(int16_t* buf);    //Reads the values (in uT) from the magnetometer into a buffer, returns 1 on success
void setMagnetometerMode(AK8963Mode mode); //Sets the magnetometer's mode
AK8963Mode getMagnetometerMode();          //Gets the magnetometer's mode
bool isMagnetometerReady();                //Returns true if the magnetometer has data available
```
***PLEASE NOTE*** that in order to use the AK8963 inside of the MPU9250 you ***MUST*** 
enable the bypass feature, otherwise the I2C commands sent to the AK8963 will go 
to the external I2C bus instead of the AK8963's I2C bus!

Also, If a MPU9520 is connected and you attempt to use it as an MPU6050 the `test` function will fail
### Valid sensor ranges / compile constants
```c
#define ONE_G                      //One G (on Earth)
//Default addresses
#define MPU_DEFAULT_ADDRESS     0x68
#define AK8963_DEFAULT_ADDRESS  0x0C
//Accelerometer range options
#define MPU_ACCEL_RANGE_2G      0b00
#define MPU_ACCEL_RANGE_4G      0b01
#define MPU_ACCEL_RANGE_8G      0b10
#define MPU_ACCEL_RANGE_16G     0b11
//Gyrometer range options
#define MPU_GYRO_RANGE_250DPS   0b00
#define MPU_GYRO_RANGE_500DPS   0b01
#define MPU_GYRO_RANGE_1000DPS  0b10
#define MPU_GYRO_RANGE_2000DPS  0b11
// From the getFeatureMask() function
#define MPU_MAGNETOMETER        1
#define MPU_GYROMETER           2
#define MPU_ACCELEROMETER       4
#define MPU_THERMOMETER         8
```
### Valid AK8963 modes
```c
enum AK8963Mode { 
	Shutdown = 0,       // The sensor is offline, values got in this state are garbage
	Single = 0b0001,    // The sensor polls when asked (via readMagnetometer), then enters shutdown
	Cont100Hz = 0b0110, // The sensor polls 100 times per second
	Cont8Hz = 0b0010,   // The sensor polls 8 times per second
	SelfTest = 0b1000,  // Self-Testing mode (NOT YET IMPLEMENTED)
	ExtTrig = 0b0100,   // The sensor polls upon external trigger
	FuseROM = 0b1111    // The sensor is in ROM mode allowing you to view the ROM (NOT YET IMPLEMENTED)
};
```
### Porting from older versions
The main header file is no longer `MPU6050.h`, it is now `MPU.h`. This means that any
instances of `#include "MPU6050.H"` will have to be replaced with `#include "MPU.h"`
