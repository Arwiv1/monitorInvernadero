#include "timer.h"
#include "dht11.h"
#include "uart.h"
#include "ds3231.h"
#include "i2c.h"
#include <stdio.h>

volatile uint8_t flag_dht11_error = 0;
volatile uint8_t flag_dht11_esperando=0;
volatile uint8_t flag_dht11_listo = 0;
volatile uint8_t flag_timer1_periodo = 0;
volatile uint8_t flag_uart_comando = 0;
volatile uint8_t flag_uart_rxlleno = 0;
volatile uint8_t flag_uart_txlleno = 0;
char buffer_tx[BUFFER_SIZE];
char buffer_rx[BUFFER_SIZE];
uint8_t hh;
uint8_t mm;
uint8_t ss;
uint8_t seg = 2;

uint8_t validar_set_time(const char *str, uint8_t *hhin, uint8_t *mmin, uint8_t *ssin) {
	int h, m, s;
	
	//Valido formato y valores del comando
	if (sscanf(str, "SET_TIME=%d:%d:%d", &h, &m, &s) != 3)
	return 0;
	
	if (h > 23 || m > 59 || s > 59)
	return 0;
	
	*hhin = h;
	*mmin = m;
	*ssin = s;
	return 1;
}

uint8_t validar_set_tm(const char *str) {
	int s;

	//Valido formato y valores del comando
	if (sscanf(str, "SET_TM=%d", &s) != 1) return 0;

	if (s < 2 || s > 60) return 0;

	seg = s;
	return 1;
}

uint8_t evaluar_condiciones(uint8_t temperatura, uint8_t humedad) {
	if ((hh>6) && (hh<19)){
		//Evalúo valores optimos de día
		if ((temperatura<20) | (temperatura>30) | (humedad<50) | (humedad>70)) return 0;
		else return 1;
	}
	else {
		//Evalúo valores optimos de noche
		if ((temperatura<15) | (temperatura>22) | (humedad<60) | (humedad>80)) return 0;
		else return 1;
	}
}

void MAIN_init(){
	DHT11_init();
	TIMER1_init();
	UART0_init();
	I2C_init();
	sei();
	UART0_sendString("ARRANCA EL TEST\r\n");
} 

int main(void)
{
	uint8_t humedad;
	uint8_t temperatura;
	uint8_t hhin;
	uint8_t mmin;
	uint8_t ssin;
	uint8_t cant_alertas = 0;
	char estado[10];
	char comando[80];
	char mensaje[120];
	
	MAIN_init();
	
    while(1)
    {
		//Espero que el timer me avise que es momento de reportar, y me aseguro no estar espernando respuestas del sensor;
		if(flag_timer1_periodo && !(flag_dht11_esperando)){	
			flag_timer1_periodo=0;
			DHT11_iniciar();
		}
		
		//El sensor respondió correctamente
		if (flag_dht11_listo){
			flag_dht11_listo=0;
			
			//Verifico integridad de los datos de respuesta
			DHT11_recibir(&humedad,&temperatura);
			
			//Si no hubo problemas de integridad
			if (!flag_dht11_error){
				//Segun la hora del día calculo estado NORMAL o de ALERTA
				if(evaluar_condiciones(temperatura, humedad)) {
					strcpy(estado, "NORMAL");
					cant_alertas=0;
				}
				else {
					strcpy(estado, "ALERTA");
					if (cant_alertas==0) cant_alertas= 2;
					else cant_alertas--;
					
				}
				//En caso de estado ALERTA, cada dos mensajes mando un mensaje de ALERTA especial
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
					sprintf(
					mensaje,
					"[ALERTA] [%02u:%02u:%02u] Parametros fuera de rango optimo! T: %u C | H: %u%%\r\n",
					hh, mm, ss,
					temperatura,
					humedad
					);
					
				}
				while(flag_uart_txlleno);
				UART0_sendString(mensaje);	
			}
		}
		
		//El sensor falló, no respodió o no lo hizo correctamente
		if (flag_dht11_error){
			flag_dht11_error=0;
			flag_dht11_esperando=0;
			while(flag_uart_txlleno);
			UART0_sendString("ERROR SENSOR\r\n");
		}
		
		//Por la terminal llegan comandos
		if (flag_uart_comando){
			flag_uart_comando=0;
			strcpy(comando, buffer_rx);
			while(flag_uart_txlleno);
			UART0_sendString(strcat(comando, "\r\n"));
			//Chequeo formato del comando
			if (validar_set_time(comando, &hhin, &mmin, &ssin)){
				//Es SET_TIME=HH:MM:SS
				sprintf(
				mensaje,
				"HORA CAMBIADA: [%02u:%02u:%02u]\r\n",
				hhin, mmin, ssin
				);
				while(flag_uart_txlleno);
				UART0_sendString(mensaje);
				RTC_set_time(hhin,mmin,ssin);
			}
			else if (validar_set_tm(comando)){
				//Es SET_TM=SS
				TIMER1_set_T(seg);
				sprintf(
				mensaje,
				"PERIODO CAMBIADO: T = %02u\r\n",
				seg
				);
				while(flag_uart_txlleno);
				UART0_sendString(mensaje);
			}
			else {
				while(flag_uart_txlleno);
				UART0_sendString("COMANDO INVALIDO\r\n");
			}
		}
			
	}
		
}
