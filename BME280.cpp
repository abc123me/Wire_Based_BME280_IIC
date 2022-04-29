#include "BME280.h"

#define BME280_ADDR0 0x76
#define BME280_ADDR1 0x76

// Data registers
#define _BME280_HUMI_LSB_RGR 0xFE
#define _BME280_HUMI_MSB_RGR 0xFD0
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

uint16_t BME280::read16(uint8_t rgr) {
	uint16_t tmp = reqByte(addr, rgr);
	tmp |= reqByte(addr, rgr + 1) << 8;
	return tmp;
}
uint8_t BME280::begin(uint8_t addr, uint32_t speed) {
	this->addr = addr;
	
	// Verify chip ID
	Wire.begin(speed);
	chipID = reqByte(addr, _BME280_CHIP_ID_RGR);
	if(chipID != 0x58 && chipID != 0x59)
		return 0;
	
	softReset();
	setMeasurementModes(BME280_SAMPLING_X4, BME280_SAMPLING_X4, BME280_SAMPLING_X4, BME280_MODE_NORMAL);
	setConfiguration(BME280_STANDBY_MS_0_5, BME280_FILTER_X2);
	
	
	// Load compensation values
	this->loadCompensationValues();
	return 1;
}

void BME280::softReset() {
	writeReg(addr, _BME280_SOFT_RESET_RGR, 0xB6);
}
void BME280::setConfiguration(E_BME280_STANDBY standbyTime, E_BME280_FILTERING filtering) {
	uint8_t dat = 0b0000001;
	dat |= (filtering << 1);
	dat |= (standbyTime << 4);
	writeReg(addr, _BME280_CTRL_HUMI_RGR, dat);
}
void BME280::setMeasurementModes(E_BME280_SAMPLING temperatureSampling, E_BME280_SAMPLING pressureSampling, E_BME280_SAMPLING humiditySampling, E_BME280_POWER_MODE powerMode) {
	uint8_t dat = humiditySampling;
	writeReg(addr, _BME280_CTRL_HUMI_RGR, dat);
	dat = powerMode;
	dat |= (pressureSampling << 2);
	dat |= (temperatureSampling << 5);
	writeReg(addr, _BME280_CTRL_MEAS_RGR, dat);
}
void BME280::loadCompensationValues() {
  // Temperature calibration
  t1 = (uint16_t) read16(_BME280_CALIB_T1);
  t2 = read16(_BME280_CALIB_T2);
  t3 = read16(_BME280_CALIB_T3);
  // Pressure calibration
  p1 = (uint16_t) read16(_BME280_CALIB_P1);
  p2 = read16(_BME280_CALIB_P2);
  p3 = read16(_BME280_CALIB_P3);
  p4 = read16(_BME280_CALIB_P4);
  p5 = read16(_BME280_CALIB_P5);
  p6 = read16(_BME280_CALIB_P6);
  p7 = read16(_BME280_CALIB_P7);
  p8 = read16(_BME280_CALIB_P8);
  p9 = read16(_BME280_CALIB_P9);
  // Humidity calibration
  h1 = reqByte(addr, _BME280_CALIB_H1);
  h2 = read16(_BME280_CALIB_H2);
  h3 = reqByte(addr, _BME280_CALIB_H3);
  h4 = ((int8_t) reqByte(addr, _BME280_CALIB_H4) << 4);
  h4 |= (reqByte(addr, _BME280_CALIB_H4 + 1) & 0xF);
  h5 = ((int8_t) reqByte(addr, _BME280_CALIB_H5 + 1) << 4);
  h5 |= (reqByte(addr, _BME280_CALIB_H5) >> 4);
  h6 = (int8_t) reqByte(addr, _BME280_CALIB_H6);
}
float BME280::readTemperatureC(void) {
  int32_t var1, var2;

  int32_t adc_T = 0;
  adc_T |= reqByte(addr, _BME280_TEMP_XLSB_RGR);
  adc_T |= ((uint16_t) reqByte(addr, _BME280_TEMP_LSB_RGR)) << 8;
  adc_T |= ((uint16_t) reqByte(addr, _BME280_TEMP_MSB_RGR)) << 16;
  if (adc_T == 0x800000) // value in case temp measurement was disabled
    return NAN;
  adc_T >>= 4;

  var1 = (int32_t)((adc_T / 8) - ((int32_t)t1 * 2));
  var1 = (var1 * ((int32_t)t2)) / 2048;
  var2 = (int32_t)((adc_T / 16) - ((int32_t)t1));
  var2 = (((var2 * var2) / 4096) * ((int32_t)t3)) / 16384;

  t_fine = var1 + var2 + t_fine_adjust;

  int32_t T = (t_fine * 5 + 128) / 256;

  return (float)T / 100;
}
float BME280::readPressurePa(void) {
  int64_t var1, var2, var3, var4;

  readTemperatureC(); // must be done first to get t_fine

  int32_t adc_P = 0;
  adc_P |= reqByte(addr, _BME280_PRES_XLSB_RGR);
  adc_P |= ((uint16_t) reqByte(addr, _BME280_PRES_LSB_RGR)) << 8;
  adc_P |= ((uint16_t) reqByte(addr, _BME280_PRES_MSB_RGR)) << 16;
  if (adc_P == 0x800000) // value in case pressure measurement was disabled
    return NAN;
  adc_P >>= 4;

  var1 = ((int64_t) t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)p6;
  var2 = var2 + ((var1 * (int64_t)p5) * 131072);
  var2 = var2 + (((int64_t)p4) * 34359738368);
  var1 = ((var1 * var1 * (int64_t)p3) / 256) +
         ((var1 * ((int64_t)p2) * 4096));
  var3 = ((int64_t)1) * 140737488355328;
  var1 = (var3 + var1) * ((int64_t)p1) / 8589934592;

  if (var1 == 0) {
    return 0; // avoid exception caused by division by zero
  }

  var4 = 1048576 - adc_P;
  var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
  var1 = (((int64_t)p9) * (var4 / 8192) * (var4 / 8192)) /
         33554432;
  var2 = (((int64_t)p8) * var4) / 524288;
  var4 = ((var4 + var1 + var2) / 256) + (((int64_t)p7) * 16);

  float P = var4 / 256.0;

  return P;
}
float BME280::readHumidityRH(void) {
  int32_t var1, var2, var3, var4, var5;

  readTemperatureC(); // must be done first to get t_fine

  int32_t adc_H = 0;
  adc_H |= reqByte(addr, _BME280_HUMI_LSB_RGR);
  adc_H |= ((uint16_t) reqByte(addr, _BME280_HUMI_MSB_RGR)) << 8;
  if (adc_H == 0x8000) // value in case humidity measurement was disabled
    return NAN;

  var1 = t_fine - ((int32_t) 76800);
  var2 = (int32_t)(adc_H * 16384);
  var3 = (int32_t)(((int32_t)h4) * 1048576);
  var4 = ((int32_t)h5) * var1;
  var5 = (((var2 - var3) - var4) + (int32_t) 16384) / 32768;
  var2 = (var1 * ((int32_t)h6)) / 1024;
  var3 = (var1 * ((int32_t)h3)) / 2048;
  var4 = ((var2 * (var3 + (int32_t) 32768)) / 1024) + (int32_t) 2097152;
  var2 = ((var4 * ((int32_t)h2)) + 8192) / 16384;
  var3 = var5 * var2;
  var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
  var5 = var3 - ((var4 * ((int32_t)h1)) / 16);
  var5 = (var5 < 0 ? 0 : var5);
  var5 = (var5 > 419430400 ? 419430400 : var5);
  uint32_t H = (uint32_t)(var5 / 4096);

  return (float)H / 1024.0;
}
float BME280::readAltitudeM(float seaLevel) {
  // Equation taken from BMP180 datasheet (page 16):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude. See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

  float atmospheric = readPressurePa() / 100.0F;
  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}
