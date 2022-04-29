# Wire based BMP/BME280 library (works with STM32)
A Wire based BMP/BME280 library intended for ARM Cortex STM32 devices, but since it uses the `Wire.h` library it works with ***EVERYTHING*** and yes, you can use it to take over the world!
## Usage
 1. Download the [latest release](https://github.com/abc123me/Wire_Based_BME280/releases)
 2. Extract and install into your arduino libraries folder
 3. Select Examples/BME280/ and experiment with them
## Boards tested
 - STM32F103 (32K) @ 72MHz w/ 100KHz I2C
## Documentation
### MPU6050
```c
BME280();                                  // Default constructor
void begin(uint8_t addr, uint32_t speed);  // Initializes the device with a custom clock speed & address
void begin(uint8_t addr);                  // Initializes the device with a custom address @ 100KHz
void begin();                              // Initializes the device using address 0x76 @ 100KHz
uint8_t readChipID();                      // Gets the chip ID
float readAltitudeM(float seaLevel);       // Reads altitude from sea level
float readHumidityRH();                    // Reads humidity as percent relative humidity
float readTemperatureK();                  // Reads temperature in degrees kelvin
float readTemperatureF();                  // Reads temperature in degrees farenheit
float readTemperatureC();                  // Reads temperature in degrees celcius
float readPressurePa();                    // Reads pressure in pascals
float readPressurePSI();                   // Reads pressure in pounds per square inch
float readPressureBAR();                   // Reads pressure in BAR
uint8_t hasHumidity()                      // Determines if the chip has a humidity sensor 
```
