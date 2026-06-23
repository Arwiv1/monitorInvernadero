/*
 * timer.h
 *
 * Created: 23/6/2026 15:02:30
 *  Author: labej
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

extern volatile uint8_t flag_timer1;

void timer1_init(void);

#endif /* TIMER_H_ */