/*
 * i2c.h
 *
 * Created: 25/6/2026 00:58:01
 *  Author: labej
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

void i2c_init(void);
void i2c_start(void);
unsigned char i2c_read(unsigned char isLast);
void i2c_write(unsigned char data);
void i2c_stop(void);

#endif /* I2C_H_ */