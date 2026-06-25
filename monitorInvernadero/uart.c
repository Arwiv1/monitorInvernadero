/*
    Manejo de USART:
        Enviar STRING: desenmascaramos la interrupcion TX, y en el gestor hasta que no este vacio el string lo cargamos en el registro UDR0 byte a byte
        al finalizar enmascaramos la interrupcion TX

        Recibir STRING: la interrupcion RX debe estar siempre desenmascarada, el gestor de interrupcion RX  debe ir cargando byte a byte recibido 
        en un buffer esperando a que llegue el enter, despues logica de strings para ver que comando mandó e intrrumpir

    UART_init: 9600bps, 8N1, habilita RXCIE y TXCIE

    ISR(USART_RX_vect): (Foreground)

    ISR(USART_UDRE_vect): (Foreground)

    UART_send_string(msg):

*/

#include "uart.h"
#include <avr/interrupt.h>
#include <string.h>



void uart0_init(void){
	//función de inicialización de uart
	
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);	 //configura formato 8N1
	UBRR0L = 103;						 //configura BAUDRATE = 9600bps@16MHz
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	// Activa el Transmisor del Puerto Serie
}

void uart0_sendString(const char* str){
	error_tx_lleno=1;
	if (UCSR0B & (1 << UDRIE0)) {
		return 2; // No se pudo mandar porque el buffer TX estaba lleno
	}
	//función para guardar en un buffer el string y enviarlo de a poco
	strncpy(buffer_tx, str, BUFFER_SIZE - 1);
	// Nos aseguramos de que termine con el caracter nulo
	buffer_rx[BUFFER_SIZE - 1] = '\0';
	
	UCSR0B |= (1<<UDRIE0);
	
}

volatile uint8_t rx_index=0;

ISR(USART_RX_vect){
	char datoRecibido= UDR0;
	if (string_recibido_flag == 1) {
		error_buffer_lleno = 1; // avisamos de buffer lleno, aun esta enviando
		return;
	}
	
	if(datoRecibido != '\n' && datoRecibido != '\r'){
		if(rx_index< (BUFFER_SIZE - 1)){
			buffer_rx[rx_index] = datoRecibido;
			rx_index++;
		}
	}else{
		if (rx_index > 0) {
			buffer_rx[rx_index] = '\0';
			string_recibido_flag = 1;
			rx_index = 0;
		}
	}
}

volatile uint8_t tx_index=0;

ISR(USART_UDRE_vect){
	//interrupción de transmisión (el registro esta vacio)
	if(buffer_tx[tx_index]!='\0'){
		UDR0 = buffer_tx[tx_index];
		tx_index++;
	}else{
		tx_index=0;
		UCSR0B &= ~(1 << UDRIE0); // cuando termina de transmitir, enmascara la interrupción y así evita el bucle infinito
		error_tx_lleno=0;
	}
}