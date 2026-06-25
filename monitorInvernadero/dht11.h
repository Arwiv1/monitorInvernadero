#ifndef DHT11_H_
#define DHT11_H_

#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

extern volatile uint8_t flag_error_sensor;
extern volatile uint8_t flag_dht_listo;
extern volatile uint8_t espera_30ms;
extern volatile uint16_t intervalo_T;

void DHT11_iniciar();
void DHT11_recibir(uint8_t *humedad, uint8_t *temperatura);

#endif