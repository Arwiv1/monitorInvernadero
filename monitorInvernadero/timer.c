/*
 * timer.c
 *
 * Created: 23/6/2026 15:02:45
 *  Author: labej
 */ 

#include "timer.h"
#include <avr/interrupt.h>

volatile uint8_t flag_timer1 = 0;

static volatile uint8_t contador_segundos = 0;
extern volatile uint8_t intervalo_T;

void timer1_init(void) {
	
	TCCR1A = 0;
	TCCR1B = (1 << WGM12); // configuramos timer1 en CTC con OCR1A
	
	OCR1A = 15624; // Cristal 16MHz / Prescaler 1024 = 15625 ticks por segundo.
	// Como el contador arranca en 0, se resta 1 (15625 - 1 = 15624)
	// para lograr una interrupción cada 1 segundo exacto. Entra
	// cómodo en el registro de 16 bits (máx. 65535).
	// con otro preescaler, no podríamos dar la interrupción cada 1 segundo.	
	TIMSK1 = (1 << OCIE1A); //interrupcion timer1 activada
	
	TCCR1B |= (1 << CS12) | (1 << CS10); // encender el timer
}

ISR(TIMER1_COMPA_vect){
	contador_segundos++;
	
	if(contador_segundos >= intervalo_T){
		flag=1; //flag para el background
		contador_segundos=0;
	}
}