

#ifndef _M24C64_H_//this file is ribrary for M24C64(FRAM)
#define _M24C64_H_

#include "mbed.h"

#define EEPROM_ADDR		0b1010000 //adress for i2c
#define EEPROM_PAGESIZE	32			//32byte
#define EEPROM_ROM		0x2000		//8kbyte=256page

class M24C64{
private:
	I2C i2c;//M24C64 use i2c to write and read
public:
	M24C64();//make object
	M24C64(PinName sda,PinName scl);//make object
	uint8_t Read(uint16_t addr,uint8_t* buf,uint16_t length);//read
	uint8_t Write(uint16_t addr,uint8_t* buf,uint16_t length);//write
};


#endif //_M24C64_H_
