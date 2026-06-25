/*
 * uart.h
 *
 * Created: 23/6/2026 15:32:13
 *  Author: labej
 */ 


#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#define UART_BAUD_RATE 9600 // la pantalla se maneja a 9600 baudios
#define BUFFER_SIZE 64 // tama?o maximo del string; SET TIME-14:30:00

extern volatile uint8_t string_recibido_flag;
extern volatile uint8_t error_buffer_lleno;
extern volatile uint8_t error_tx_lleno;
extern char buffer_tx[BUFFER_SIZE];
extern char buffer_rx[BUFFER_SIZE];


void uart0_init(void); //inicializaci?n
void uart0_sendString(const char* str); //carga en el buffer que periodicamente se enviar?

#endif /* UART_H_ */