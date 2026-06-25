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
#include "dht11.h"



uint16_t t_inicio_pulso;
int8_t bit_actual= -1;
uint8_t datos[5] = {0};
uint16_t t_actual;
uint16_t duracion;
uint8_t checksum;
uint8_t ignorar_flanco;


void DHT11_iniciar(){
	flag_error_sensor = 0;
	espera_30ms = 0;
	
	//PIN C0 como salida, lo pongo en bajo
	DDRC |= (1<<PORTC0);
	PORTC &= ~(1<<PORTC0);
	
	//Mantengo en bajo por mas de 18ms
	while (espera_30ms<3){
		
	}

	//Pongo en alto y lo configuro como entrada, espero respuesta del sensor
	PORTC |= (1<<PORTC0);
	DDRC &= ~(1<<PORTC0);
	bit_actual=-1;
	ignorar_flanco=1;
	PCMSK1 |=  (1 << PCINT8);
}

void DHT11_recibir(uint8_t *humedad, uint8_t *temperatura){
	checksum = datos[0] + datos[1] + datos[2] + datos[3];
	if (checksum == datos[4]){
		*humedad = datos[0];
		*temperatura = datos[2];
	}
	else flag_error_sensor = 1;
	memset(datos, 0, sizeof(datos));
}


ISR(PCINT1_vect) {
    t_actual = TCNT1;
    
    if (PINC & (1 << PINC0)) {
        t_inicio_pulso = t_actual;
    } else {
		if (ignorar_flanco) {
			ignorar_flanco = 0;   // descartamos este, que lo generó el MCU
			return;
		}
		if (t_actual < t_inicio_pulso) t_actual += 20000;
        duracion = t_actual - t_inicio_pulso;
        
        if (bit_actual == -1) {
            // Este es el pulso en ALTO de la respuesta del sensor (80us)
            // 80us con prescaler 8 son 160 cuentas. Verificamos un rango:
            if (duracion > 140 && duracion < 180) { 
                bit_actual = 0; // Respuesta válida, ahora sí esperamos los 40 bits
            } else {
                flag_error_sensor = 1; // El sensor no respondió correctamente
            }
        } else if (bit_actual < 40) {
			if (duracion > 120 && duracion < 160){
				//Es un 1
				datos[bit_actual/8] |= (1 << (7 - (bit_actual % 8)));
			}
			bit_actual++;
            if (bit_actual == 40) {
				PCMSK1 &=  ~(1 << PCINT8);
				flag_dht_listo = 1;
				bit_actual = -1;
			}
        }
    }
}

