/*
 * UART.c
 *
 * Created: 23/03/2023 03:52:28 p. m.
 * Author : Ferna
 */ 

#define F_CPU 1000000
#define BAUD 4800
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>

void USART_init(uint16_t ubrr){ //con este parametro me evito usar las tablitas
	UCSRA = 0b00100000; //modo asincrono a velocidad normal, sobre todo config la velocidad
	//como vamos a transmitir no ocupasmos int de recepcion
	UCSRB|=(1<<TXEN); //0B00001000 -> LE PUSE UN 1 A TX ENABLE, transmision habilitada
	UCSRC=	0b100000110; //ahorita sin paridad, 0 es un bit de parada, tamaño del dato es 8 (11)
	//UBRR = 12 esto no sirve porque UBRR tiene la parte alta y la baja
	UBRRH = (uint8_t)(ubrr>>8);
	UBRRL = (uint8_t) ubrr;
	
}

int main(void)
{
	USART_init(MYUBRR);
    /* Replace with your application code */
    while (1) 
    {
			UDR = 'A'; //Mandamos un A
			_delay_ms(500);
    }
}

