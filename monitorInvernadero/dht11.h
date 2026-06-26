#ifndef DHT11_H_
#define DHT11_H_

#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>

extern volatile uint8_t flag_dht11_error;
extern volatile uint8_t flag_dht11_esperando;
extern volatile uint8_t flag_dht11_listo;
extern volatile uint8_t cont_esperando;

void DHT11_init();
void DHT11_iniciar();
void DHT11_recibir(uint8_t *humedad, uint8_t *temperatura);

#endif