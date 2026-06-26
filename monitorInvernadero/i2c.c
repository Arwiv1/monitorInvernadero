/*
 * i2c.c
 *
 * Created: 24/6/2026 22:45:09
 *  Author: labej
 */ 
#include "i2c.h"

void I2C_init(void){
	TWSR=0x00; //preescalar en 0
	TWBR=152; //16Mhz
	TWCR=0x04; //Activar TWI
}

void i2c_start(void){
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while((TWCR & (1<< TWINT)) == 0);
}

unsigned char i2c_read(unsigned char isLast){
	if(isLast == 0)
		TWCR = (1<< TWINT)|(1<< TWEN)|(1<< TWEA);
	else
		TWCR =	(1 << TWINT)|(1<< TWEN);
	while((TWCR & (1 << TWINT))==0);
	return TWDR;
}

void i2c_write(unsigned char data){
	TWDR = data;
	TWCR = (1<< TWINT)|(1<< TWEN);
	while((TWCR & (1<< TWINT))==0);
}

void i2c_stop(){
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}