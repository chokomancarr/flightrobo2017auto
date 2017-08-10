
#include "eeprom.h"

M24C64::M24C64() : i2c(I2C_SDA,I2C_SCL){
	i2c.frequency(400000);
}

M24C64::M24C64(PinName sda, PinName scl) :i2c(sda,scl) {
	i2c.frequency(400000);
}

uint8_t M24C64::Read(uint16_t addr, uint8_t* buf, uint16_t length) {
	uint8_t b[2],res;
	b[0] = addr >> 8;
	b[1] = addr & 0xFF;

	res=i2c.write(EEPROM_ADDR<<1,(const char*)b,2,false);
	res<<=1;
	wait_ms(1);
	res|=i2c.read(EEPROM_ADDR<<1,(char*)buf,length,false);
	return res;
}

// up to 1 kbyte
uint8_t M24C64::Write(uint16_t addr, uint8_t* buf, uint16_t length) {
	uint8_t send[34];
	uint16_t page_num;
	const char *p;
	int i,j,m;
	if(addr & EEPROM_PAGESIZE-1)
		return 1;
	if(length>= 1024)
		return 1;

	p=(const char*)send;
	page_num = length / EEPROM_PAGESIZE;

	if (length < 32) {
		send[0] = addr >> 8;
		send[1] = addr & 0xFF;
		for(i=0;i<length;i++){
			send[i+2]=*buf++;
		}
		i2c.write(EEPROM_ADDR << 1, p, length + 2, false);
	} else {
		for (i = 0; i < page_num; i++) {
			send[0] = addr >> 8;
			send[1] = addr & 0xFF;
			for (j = 0; j < 32; j++) {
				send[j + 2] = *buf++;
			}
			i2c.write(EEPROM_ADDR << 1, p, 34, false);
			wait_ms(7);
			addr += EEPROM_PAGESIZE;
		}
		m = length % EEPROM_PAGESIZE;
		if (m) {
			send[0] = addr >> 8;
			send[1] = addr & 0xFF;
			for(i=0;i<m;i++){
				send[i+2]=*buf++;
			}
			i2c.write(EEPROM_ADDR << 1, p, m+2, false);
		}
	}
}
