/*
 * main.c
 *
 * Created: 6/23/2026 11:39:27 AM
 *  Author: labej
 */ 

#include <xc.h>
#include <avr/interrupt.h>//creo q no es necesaria PROBAR DESPUES
#include <avr/sleep.h>
#include "timer.h"
#include "dht11.h"
#include "uart.h"
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>

volatile uint8_t flag_error_sensor = 0;
volatile uint8_t flag_dht_listo = 0;
volatile uint8_t flag_timer1 = 0;
volatile uint8_t espera_30ms = 0;
volatile uint8_t string_recibido_flag=0;
volatile uint8_t error_buffer_lleno=0;
volatile uint8_t error_tx_lleno=0;

char buffer_tx[BUFFER_SIZE];
char buffer_rx[BUFFER_SIZE];
uint8_t humedad;
uint8_t temperatura;
volatile uint16_t intervalo_T = 100; //100 por cada segundo de interrupcion
int libre =1;


int main(void)
{
	PCICR |= (1 << PCIE1);
	PCMSK1 &=  ~(1 << PCINT8);
	timer1_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	uart0_init();
	sei();
	uart0_sendString("ARRANCA EL TEST\r\n");
    while(1)
    {
		/*
		if(flag_timer1){	
			flag_timer1=0;
		}
		
		if (flag_dht_listo){
			flag_dht_listo=0;
			DHT11_recibir(&humedad,&temperatura);
			
			if (!flag_error_sensor){
				
			}
			
		}
		
		if (flag_error_sensor){
			flag_error_sensor=0;
			DHT11_iniciar();
		}
		
		sleep_cpu();
		*/
		if (libre==1){
			libre=0;
			DHT11_iniciar();
			
			while (!flag_dht_listo);
			
			flag_dht_listo=0;
			DHT11_recibir(&humedad,&temperatura);
				
			if (!flag_error_sensor){
				uart0_sendString("Anduvo el sensor\r\n");
				
				char str1[4];   // "255" + '\0' -> máximo 4 chars

				sprintf(str1, "%u", temperatura);
				
				while(error_tx_lleno);
				
				uart0_sendString(str1);
			}
				
		}
			
	}
		
}
