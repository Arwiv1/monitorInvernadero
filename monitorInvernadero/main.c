/*
 * main.c
 *
 * Created: 6/23/2026 11:39:27 AM
 *  Author: labej
 */ 

#include <xc.h>
#include <avr/interrupt.h>//creo q no es necesaria PROBAR DESPUES
#include "timer.h"

volatile uint8_t intervalo_T = 100;//por defecto lo pongo en 1 segundo

int main(void)
{
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT8);
	timer1_init();
	sei();
    while(1)
    {
		if(flag_timer1=1){
			//bla bla bla	
			flag_timer1=0;
		}
        //TODO:: Please write your application code
    }
}