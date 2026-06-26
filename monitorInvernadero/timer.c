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
static volatile uint16_t cont_ms2 = 0;
static volatile uint16_t intervalo_T = 200;
volatile uint8_t cont_esperando=0;

void TIMER1_init(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	//Modo CTC con preescaler 8, 20000 cuentas equivalen a 10 ms
	//El prescaler 8 es necesario para poder medir la respuesta del sensor en función de cuentas del TIMER1
	OCR1A = 19999;
	TIMSK1 = (1 << OCIE1A);
}

void TIMER1_set_T(uint8_t seg){
	//100 overflows equivalen a 1 segundos
	intervalo_T = 100*seg;
}

ISR(TIMER1_COMPA_vect) {
	cont_ms++;
	cont_ms2++;
	espera_30ms++;
	if (cont_ms >= intervalo_T) {
		flag_timer1_periodo=1;
		cont_ms = 0;
		RTC_get_time(&hh,&mm,&ss);
	}
	//Este contador es para esperar la respuesta del sensor, si pasa mucho tiempo activa flag de error
	if (cont_ms2 >= 100) {
		cont_ms2 = 0;
		if (flag_dht11_esperando){
			cont_esperando++;
			if (cont_esperando>1){
				cont_esperando=0;
				flag_dht11_error=1;
			}
		}
	}
}