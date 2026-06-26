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

volatile uint8_t espera_30ms = 0;
volatile uint16_t t_inicio_pulso;
volatile int8_t bit_actual= -1;
volatile uint8_t datos[5] = {0};
volatile uint16_t t_actual;
volatile uint16_t duracion;
volatile uint8_t ignorar_flanco;
uint8_t checksum;

void DHT11_init(){
	PCICR |= (1 << PCIE1);
	PCMSK1 &=  ~(1 << PCINT8);
}

void DHT11_iniciar(){
	flag_dht11_error = 0;
	espera_30ms = 0;
	
	//PIN C0 como salida, lo pongo en bajo
	DDRC |= (1<<PORTC0);
	PORTC &= ~(1<<PORTC0);
	
	//Mantengo en bajo por mas de 18ms, "despierto al sensor"
	while (espera_30ms<3){
		
	}

	//Pongo en alto y lo configuro como entrada, espero respuesta del sensor
	PORTC |= (1<<PORTC0);
	DDRC &= ~(1<<PORTC0);
	cont_esperando=0;
	flag_dht11_esperando=1;
	bit_actual=-1;
	ignorar_flanco=1;
	PCMSK1 |=  (1 << PCINT8);	
}

void DHT11_recibir(uint8_t *humedad, uint8_t *temperatura){
	//Verifico integridad de los datos recibidos y devuelvo informacion
	checksum = datos[0] + datos[1] + datos[2] + datos[3];
	if (checksum == datos[4]){
		*humedad = datos[0];
		*temperatura = datos[2];
	}
	else flag_dht11_error = 1;
	memset(datos, 0, sizeof(datos));
}


ISR(PCINT1_vect) {
    t_actual = TCNT1;
    
    if (PINC & (1 << PINC0)) {
		//Flanco de subida
        t_inicio_pulso = t_actual;
    } else {
		//Flanco de bajada, el primero lo genera el MCU al iniciar la comunicación, lo ignoro
		if (ignorar_flanco) {
			ignorar_flanco = 0;
			return;
		}
		if (t_actual < t_inicio_pulso) t_actual += 20000;
        duracion = t_actual - t_inicio_pulso;
        
        if (bit_actual == -1) {
			//Espero confirmación del sensor
            if (duracion > 140 && duracion < 180) {
				//160 cuentas equivalen a 80 us 
                bit_actual = 0;
				cont_esperando=0;
				flag_dht11_esperando=0;
            } else {
				// El sensor no respondió correctamente
                flag_dht11_error = 1;
            }
        } else if (bit_actual < 40) {
			if (duracion > 120 && duracion < 160){
				//140 cuentas equivalen a 70 us, es un 1
				datos[bit_actual/8] |= (1 << (7 - (bit_actual % 8)));
			}
			bit_actual++;
            if (bit_actual == 40) {
				//Fin comunicación
				PCMSK1 &=  ~(1 << PCINT8);
				flag_dht11_listo = 1;
				bit_actual = -1;
			}
        }
    }
}

