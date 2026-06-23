/*
 * uart.h
 *
 * Created: 23/6/2026 15:32:13
 *  Author: labej
 */ 


#ifndef UART_H_
#define UART_H_



void uart0_init(void); //inicialización
void uart0_sendString(const char* str); //carga en el buffer que periodicamente se enviará

#endif /* UART_H_ */