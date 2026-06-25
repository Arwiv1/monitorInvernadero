// RTC_get_time:
//   1. i2c_start + dirección escritura [8]
//   2. Enviar dirección 0x00 (segundos) [7]
//   3. i2c_start (repeated) + dirección lectura [8]
//   4. Leer segundos, minutos, horas (convertir de BCD a Decimal) [7]
//   5. i2c_stop

// RTC_set_time(HH, MM, SS):
//   1. Convertir Decimal a BCD
//   2. i2c_start + dirección escritura + dirección registro 0x00
//   3. Escribir los 3 bytes y i2c_stop [8]
#include "ds3231.h"
#include "i2c.h"
#define RTC_ADDR 0x68  //dirección fija

uint8_t decimal_to_bcd(uint8_t val){
	return ((val / 10) << 4) | (val % 10);
}

uint8_t bcd_to_decimal(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void rtc_get_time(uint8_t *hora, uint8_t *minutos, uint8_t *segundos){
	uint8_t seg_bcd, min_bcd, hora_bcd;
	i2c_start();
	i2c_write(RTC_ADDR << 1); // Modo escritura (0xD0)
	i2c_write(0x00);          // Forzamos al puntero del RTC a ir a "Segundos"
	i2c_stop();
	//posiciona el rtc en segundos
	
	i2c_start();
	i2c_write(RTC_ADDR << 1| 0x01);
	seg_bcd=i2c_read(0);
	min_bcd=i2c_read(0);
	hora_bcd=i2c_read(1);
	i2c_stop();
	*segundos = bcd_to_decimal(seg_bcd);
	*minutos = bcd_to_decimal(min_bcd);
	*hora = bcd_to_decimal(hora_bcd);
}

void rtc_set_time(uint8_t hora, uint8_t minutos, uint8_t segundos){
	i2c_start();
	i2c_write(RTC_ADDR << 1);
	i2c_write(0x00);
	i2c_write(decimal_to_bcd(segundos));
	i2c_write(decimal_to_bcd(minutos));
	i2c_write(decimal_to_bcd(hora));
	i2c_stop();
}