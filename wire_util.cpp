#include "wire_util.h"

void writeByte(uint8_t addr, uint8_t dat) {
  Wire.beginTransmission(addr);
  Wire.write(dat);
  Wire.endTransmission();
}
void writeBytes(uint8_t addr, uint8_t* dat, uint8_t len) {
  Wire.beginTransmission(addr);
  for(uint8_t i = 0; i < len; i++)
	Wire.write(dat[i]);
  Wire.endTransmission();
}
void writeReg(uint8_t addr, uint8_t reg, uint8_t dat){
	Wire.beginTransmission(addr);
	Wire.write(reg); Wire.write(dat);
	Wire.endTransmission();
}
uint8_t reqByte(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);
  return Wire.read();
}
int16_t reqSingle(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 2);
  int16_t n = 0;
  n = Wire.read() << 8 | Wire.read();
  return n;
}
void reqCoord(uint8_t addr, int16_t* buf, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 6);
  buf[0] = Wire.read() << 8 | Wire.read();
  buf[1] = Wire.read() << 8 | Wire.read();
  buf[2] = Wire.read() << 8 | Wire.read();
}
