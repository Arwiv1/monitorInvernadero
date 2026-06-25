/*
 * main.c
 *
 * Created: 6/23/2026 11:39:27 AM
 *  Author: labej
 */ 

#define F_CPU 16000000UL
#include <xc.h>
#include <avr/interrupt.h>//creo q no es necesaria PROBAR DESPUES
#include <util/delay.h>
//#include "timer.h"
#include "uart.h"

volatile uint8_t intervalo_T = 100;//por defecto lo pongo en 1 segundo

int main(void)
{ /*
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
*/
	uart0_init();
	sei();
	
	uart0_sendString("ARRANCA EL TEST\r\n");

	while(1) {
		if(string_recibido_flag == 1){
			uart0_sendString("He recibido:");
			_delay_ms(50);
			uart0_sendString(buffer_rx);
			uart0_sendString("\r\n");
			string_recibido_flag = 0;
		}
		
		if (error_buffer_lleno == 1) {
			uart0_sendString("ERROR: Buffer lleno. Espera a que termine el reenvio.\r\n");
			error_buffer_lleno = 0;
		}
	}
}