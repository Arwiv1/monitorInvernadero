/*
 * timer.h
 *
 * Created: 23/6/2026 15:02:30
 *  Author: labej
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

extern volatile uint8_t flag_timer1_periodo;
extern volatile uint8_t flag_dht11_esperando;
extern volatile uint8_t flag_dht11_error;
extern volatile uint8_t espera_30ms;
extern uint8_t hh;
extern uint8_t mm;
extern uint8_t ss;

void TIMER1_init(void);
void TIMER1_set_T(uint8_t);

#endif /* TIMER_H_ */