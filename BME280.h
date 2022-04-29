#ifndef _BME280_H
#define _BME280_H

#include "wire_util.h"

#define BME280_ADDR0 0x76
#define BME280_ADDR1 0x77
#define BME280_DEFAULT_SPEED 100000

// Data registers
#define _BME280_HUMI_LSB_RGR 0xFE
#define _BME280_HUMI_MSB_RGR 0xFD
#define _BME280_TEMP_LSB_RGR 0xFB
#define _BME280_TEMP_MSB_RGR 0xFA
#define _BME280_PRES_LSB_RGR 0xF8
#define _BME280_PRES_MSB_RGR 0xF7
#define _BME280_PRES_XLSB_RGR 0xF9
#define _BME280_TEMP_XLSB_RGR 0xFC
// Control registers
#define _BME280_CTRL_HUMI_RGR 0xF2
#define _BME280_CTRL_MEAS_RGR 0xF4
#define _BME280_SOFT_RESET_RGR 0xE0
#define _BME280_CHIP_ID_RGR 0xD0
#define _BME280_CONFIG_RGR 0xF5
#define _BME280_STATUS_RGR 0xF3
// Calibration registers
#define _BME280_CALIB_T1 0x88
#define _BME280_CALIB_T2 0x8A
#define _BME280_CALIB_T3 0x8C
#define _BME280_CALIB_P1 0x8E
#define _BME280_CALIB_P2 0x90
#define _BME280_CALIB_P3 0x92
#define _BME280_CALIB_P4 0x94
#define _BME280_CALIB_P5 0x96
#define _BME280_CALIB_P6 0x98
#define _BME280_CALIB_P7 0x9A
#define _BME280_CALIB_P8 0x9C
#define _BME280_CALIB_P9 0x9E
#define _BME280_CALIB_H1 0xA1
#define _BME280_CALIB_H2 0xE1
#define _BME280_CALIB_H3 0xE3
#define _BME280_CALIB_H4 0xE4
#define _BME280_CALIB_H5 0xE5
#define _BME280_CALIB_H6 0xE7

class BME280 {
private:
	// Temperature compensation values
	uint16_t t1;
	int16_t t2, t3;
	// Fine temperature compensation
	int32_t t_fine, t_fine_adjust = 0;
	// Pressure compensation values
	uint16_t p1;
	int16_t p2, p3, p4, p5;
	int16_t p6, p7, p8, p9; 
	// Humidity compensation values
	uint8_t h1, h3;
	int16_t h2, h4, h5; 
	int8_t h6; 
	// Device constants
	uint8_t addr, chipID = 0xFF;
	
	uint16_t read16(uint8_t rgr);
	void loadCompensationValues();
public:
	uint8_t begin(uint8_t addr, uint32_t speed);
	inline uint8_t begin(uint8_t addr) { return begin(addr, BME280_DEFAULT_SPEED); }
	inline uint8_t begin() { return begin(BME280_ADDR0, BME280_DEFAULT_SPEED); }
	uint8_t readChipID();
	float readAltitudeM(float seaLevel);
	float readHumidityRH();
	float readTemperatureC();
	float readPressurePa();
	
	inline float readTemperatureF() { return readTemperatureC() * 1.8 + 32.0; };
	inline float readTemperatureK() { return readTemperatureC() - 273.15; };
	inline float readPressurePSI() { return readPressurePa() * 1.450377e-4; };
	inline float readPressureBAR() { return readPressurePa() * 1e-5; };
	inline uint8_t hasHumidity() { return chipID == 0x59; }
};
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

#endif
