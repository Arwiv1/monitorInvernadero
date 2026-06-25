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
#include "ds3231.h"
#include "i2c.h"
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
uint8_t hh;
uint8_t mm;
uint8_t ss;
uint8_t hhin;
uint8_t mmin;
uint8_t ssin;
uint8_t enviar_alerta = 0;
uint8_t cant_alertas = 0;
char estado[10];
char comando[80];
char mensaje[120];
volatile uint16_t intervalo_T = 200; //100 por cada segundo de interrupcion
uint8_t seg = 2;
int libre =1;

uint8_t validar_set_time(const char *str, uint8_t *h, uint8_t *m, uint8_t *s) {
	int h_, m_, s_;
	
	if (sscanf(str, "SET_TIME=%d:%d:%d", &h_, &m_, &s_) != 3)
	return 0;  // no matchea el formato
	
	// validar rangos
	if (h_ > 23 || m_ > 59 || s_ > 59)
	return 0;
	
	*h = h_;
	*m = m_;
	*s = s_;
	return 1;
}

uint8_t validar_set_tm(const char *str, uint8_t *seg) {
	int s_;

	if (sscanf(str, "SET_TM=%d", &s_) != 1) return 0;

	if (s_ < 2 || s_ > 60) return 0;

	*seg = s_;
	return 1;
}

uint8_t evaluar_condiciones(uint8_t h, uint8_t m, uint8_t s, uint8_t temp, uint8_t hum) {
	if ((h>6) && (h<19)){
		if ((temp<20) | (temp>30) | (hum<50) | (hum>70)) return 0;
		else return 1;
	}
	else {
		if ((temp<15) | (temp>22) | (hum<60) | (hum>80)) return 0;
		else return 1;
	}
}

int main(void)
{
	PCICR |= (1 << PCIE1);
	PCMSK1 &=  ~(1 << PCINT8);
	timer1_init();
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_enable();
	uart0_init();
	i2c_init();
	sei();
	uart0_sendString("ARRANCA EL TEST\r\n");
    while(1)
    {
		if(flag_timer1){	
			flag_timer1=0;
			DHT11_iniciar();
		}
		
		if (flag_dht_listo){
			flag_dht_listo=0;
			DHT11_recibir(&humedad,&temperatura);
			
			if (!flag_error_sensor){
				if(evaluar_condiciones(hh,mm,ss,temperatura,humedad)) {
					strcpy(estado, "NORMAL");
					cant_alertas=0;
				}
				else {
					strcpy(estado, "ALERTA");
					if (cant_alertas==0) cant_alertas= 2;
					else cant_alertas--;
					
				}
				if(cant_alertas!=2){
					sprintf(
					mensaje,
					"[%02u:%02u:%02u] T: %u C | H: %u%% | Estado: %s\r\n",
					hh, mm, ss,
					temperatura,
					humedad,
					estado
					);
				}
				else{
					enviar_alerta = 0;
					
					sprintf(
					mensaje,
					"[ALERTA] [%02u:%02u:%02u] Parametros fuera de rango optimo! T: %u C | H: %u%%\r\n",
					hh, mm, ss,
					temperatura,
					humedad
					);
					
				}
				while(error_tx_lleno);
				uart0_sendString(mensaje);
				
			}
			else {
				flag_error_sensor=0;
				while(error_tx_lleno);
				uart0_sendString("ERROR SENSOR\r\n");
			}
			
		}
		
		if (flag_error_sensor){
			flag_error_sensor=0;
			while(error_tx_lleno);
			uart0_sendString("ERROR SENSOR\r\n");
		}
		
		if (string_recibido_flag){
			string_recibido_flag=0;
			strcpy(comando, buffer_rx);
			while(error_tx_lleno);
			uart0_sendString(strcat(comando, "\r\n"));
			if (validar_set_time(comando,&hhin,&mmin,&ssin)){
				rtc_set_time(hhin,mmin,ssin);
			}
			else if (validar_set_tm(comando,&seg)){
				intervalo_T = 100*seg;
			}
			else {
				while(error_tx_lleno);
				uart0_sendString("COMANDO INVALIDO\r\n");
			}
		}
			
	}
		
}
