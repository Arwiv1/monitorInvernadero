/*
    Manejo de USART:
        Enviar STRING: desenmascaramos la interrupcion TX, y en el gestor hasta que no este vacio el string lo cargamos en el registro UDR0 byte a byte
        al finalizar enmascaramos la interrupcion TX

        Recibir STRING: la interrupcion RX debe estar siempre desenmascarada, el gestor de interrupcion RX  debe ir cargando byte a byte recibido 
        en un buffer esperando a que llegue el enter, despues logica de strings para ver que comando mandó e intrrumpir

    UART_init: 9600bps, 8N1, habilita RXCIE y TXCIE

    ISR(USART_RX_vect): (Foreground)

    ISR(USART_TX_vect): (Foreground)

    UART_send_string(msg):

*/
