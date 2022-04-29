#ifndef _WIRE_UTIL_H
#define _WIRE_UTIL_H

#include "Wire.h"
#include "stdint.h"

uint8_t reqByte(uint8_t addr, uint8_t reg);
int16_t reqSingle(uint8_t addr, uint8_t reg);
void reqCoord(uint8_t addr, int16_t* buf, uint8_t reg);
void writeBytes(uint8_t addr, uint8_t* dat, uint8_t len);
void writeByte(uint8_t addr, uint8_t dat);
void writeReg(uint8_t addr, uint8_t reg, uint8_t dat);

#endif
