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
extern volatile uint8_t espera_30ms;
extern volatile uint16_t intervalo_T;
extern uint8_t hh;
extern uint8_t mm;
extern uint8_t ss;

void timer1_init(void);

#endif /* TIMER_H_ */