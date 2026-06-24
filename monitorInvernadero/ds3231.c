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