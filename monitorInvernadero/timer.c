/*
 * timer.c
 *
 * Created: 23/6/2026 15:02:45
 *  Author: labej
 */ 

#include "timer.h"
#include <avr/interrupt.h>

volatile uint8_t flag_timer1 = 0;

static volatile uint8_t cont_ms = 0;
extern volatile uint8_t intervalo_T;

void timer1_init(void) {
	
	TCCR1A = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS11); // configuramos timer1 en CTC con OCR1A
	
	OCR1A = 19999; // Cristal 16MHz / Prescaler 1024 = 15625 ticks por segundo.
	// Como el contador arranca en 0, se resta 1 (15625 - 1 = 15624)
	// para lograr una interrupci�n cada 1 segundo exacto. Entra
	// c�modo en el registro de 16 bits (m�x. 65535).
	// con otro preescaler, no podr�amos dar la interrupci�n cada 1 segundo.	
	TIMSK1 = (1 << OCIE1A); //interrupcion timer1 activada
	
	TCCR1B |= (1 << CS12) | (1 << CS10); // encender el timer
}

ISR(TIMER1_COMPA_vect){
	cont_ms++;
	
	if(cont_ms >= intervalo_T){
		flag=1; //flag para el background
		cont_ms=0;
	}
}