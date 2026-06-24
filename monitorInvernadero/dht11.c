/*
    Comunicacion con el sensor dht11:
        El MCU pone el pin como salida y lo pone en bajo por mas de 18 ms para despertar al sensor, despues se pone en alto y se configura como entrada
        Si el sensor esta conectado correctamente, respondera con un 0 80us y despues un 1 80us
        Seguido de esto el sensor manda 5 bytes: HHTTC (humedad, temperatura, checksum)
        Cada bit comienza con 50us de bajo, seguido de 28us/70us de alto segun si es un 0 o un 1 
*/

/*
    DHT11_start_reading
        Poner pin como salida, ponerlo en 0 por mas de 18 ms
        poner pin como entrada 
*/

/* DHT11_process_data:
        A esta altura el timer1 ya almaceno los bits que mando el sensor
        Hay que organizar la info, hacer el checksum y devoler humedad y temperatura
*/

ISR(PCINT1_vect) {
    uint16_t t_actual = TCNT1;
    
    if (PINC & (1 << PC0)) { 
        t_inicio_pulso = t_actual;
    } else {
        uint16_t duracion = t_actual - t_inicio_pulso;
        
        if (bit_actual == -1) {
            // Este es el pulso en ALTO de la respuesta del sensor (80us)
            // 80us con prescaler 8 son 160 cuentas. Verificamos un rango:
            if (duracion > 140 && duracion < 180) { 
                bit_actual = 0; // Respuesta válida, ahora sí esperamos los 40 bits
            } else {
                flag_error_sensor = 1; // El sensor no respondió correctamente
            }
        } else if (bit_actual < 40) {
            tiempos_pulsos[bit_actual++] = duracion;
            if (bit_actual == 40) flag_dht_listo = 1;
        }
    }
}