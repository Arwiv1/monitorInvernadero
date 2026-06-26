/*
 * rtc.h
 *
 * Created: 25/6/2026
 * Author: labej
 */ 

#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include "i2c.h"

void RTC_get_time(uint8_t *hora, uint8_t *minutos, uint8_t *segundos);

void RTC_set_time(uint8_t hora, uint8_t minutos, uint8_t segundos);

#endif /* RTC_H_ */