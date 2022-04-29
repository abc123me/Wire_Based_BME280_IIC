# Wire based BMP/BME280 library (works with STM32)
A Wire based BMP/BME280 library intended for ARM Cortex STM32 devices, but since it uses the `Wire.h` library it works with ***EVERYTHING*** and yes, you can use it to take over the world!
## Usage
 1. Download the [latest release](https://github.com/abc123me/Wire_Based_BME280_IIC/releases)
 2. Extract and install into your arduino libraries folder
 3. Select Examples/BME280/ and experiment with them
## Boards tested
 - STM32F103 (32K) @ 72MHz w/ 100KHz I2C
## Documentation
### Main class
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
void softReset();                          // Soft resets BMP/BME280
// Sets config register
void setConfiguration(E_BME280_STANDBY standbyTime, E_BME280_FILTERING filtering);
// Sets measurement mode registers
void setMeasurementModes(E_BME280_SAMPLING temperatureSampling, E_BME280_SAMPLING preessureSampling, E_BME280_SAMPLING humiditySampling, E_BME280_POWER_MODE powerMode);
```
### Constants
```c
#define BME280_ADDR0 0x76
#define BME280_ADDR1 0x77
#define BME280_DEFAULT_SPEED 100000

enum E_BME280_SAMPLING {
	BME280_SAMPLING_NONE = 0b000,
	BME280_SAMPLING_X1 = 0b001,
	BME280_SAMPLING_X2 = 0b010,
	BME280_SAMPLING_X4 = 0b011,
	BME280_SAMPLING_X8 = 0b100,
	BME280_SAMPLING_X16 = 0b101
};
enum E_BME280_POWER_MODE {
	BME280_MODE_SLEEP = 0b00,
	BME280_MODE_FORCED = 0b01,
	BME280_MODE_NORMAL = 0b11
};
enum E_BME280_FILTERING {
	BME280_FILTER_OFF = 0b000,
	BME280_FILTER_X2 = 0b001,
	BME280_FILTER_X4 = 0b010,
	BME280_FILTER_X8 = 0b011,
	BME280_FILTER_X16 = 0b100
};
enum E_BME280_STANDBY {
	BME280_STANDBY_MS_0_5 = 0b000,
	BME280_STANDBY_MS_10 = 0b110,
	BME280_STANDBY_MS_20 = 0b111,
	BME280_STANDBY_MS_62_5 = 0b001,
	BME280_STANDBY_MS_125 = 0b010,
	BME280_STANDBY_MS_250 = 0b011,
	BME280_STANDBY_MS_500 = 0b100,
	BME280_STANDBY_MS_1000 = 0b101
};
```
