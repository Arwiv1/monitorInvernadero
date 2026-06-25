/*
 * timer.c
 *
 * Created: 23/6/2026 15:02:45
 *  Author: labej
 */ 

#include "timer.h"
#include "ds3231.h"
#include <avr/interrupt.h>

static volatile uint16_t cont_ms = 0;

void timer1_init(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	// Modo CTC, prescaler 8
	TCCR1B |= (1 << WGM12) | (1 << CS11);

	// 16MHz / 8 / 20000 = 100Hz → 1ms por interrupción
	OCR1A = 19999;

	TIMSK1 = (1 << OCIE1A);  // interrupción por comparación A
}

ISR(TIMER1_COMPA_vect) {
	cont_ms++;
	espera_30ms++;
	if (cont_ms >= intervalo_T) {
		flag_timer1=1;
		cont_ms = 0;
		rtc_get_time(&hh,&mm,&ss);
	}
}