/*
 * UART.c
 *
 * Created: 23/03/2023 03:52:28 p. m.
 * Author : Ferna
 */ 

#define F_CPU 1000000
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void USART_init(uint16_t ubrr){ //con este parametro me evito usar las tablitas
	UCSRA = 0b00100000; //modo asincrono a velocidad normal, sobre todo config la velocidad
	//como vamos a transmitir no ocupasmos int de recepcion
	UCSRB|=(1<<TXEN) | (1 << RXEN) |(1<<RXCIE); 
	UCSRC=	0b100000110; //ahorita sin paridad, 0 es un bit de parada, tamaño del dato es 8 (11)
	//UBRR = 12 esto no sirve porque UBRR tiene la parte alta y la baja
	UBRRH = (uint8_t)(ubrr>>8);
	UBRRL = (uint8_t) ubrr;	
}

uint8_t readADC(int8_t pin) {
	ADMUX = (0b01100000) | pin; // Read from pin ADCpin, eg. ADC0, ADC1, ADC2...
	ADCSRA |= (1<< ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // ADC is async, so this is Await for conversion to end.
	_delay_us(600); //Just in case delay
	return ADCH; //Return ADCH, looses two bits from ADCL, but meh.
}

void adcInit() {
	ADMUX = (1<<REFS0) | (1<< ADLAR);
	//Free running
	SFIOR = 0;
	//ADEN enables ADC, ADPSX is the prescaler. F_CPU/prescaler must be in [50K, 200K]
	ADCSRA = (1<< ADEN)|(1 << ADPS2) | (1 << ADPS0);
}

int main(void)
{
	USART_init(MYUBRR);
	adcInit();	
    /* Replace with your application code */
    while (1) {
		
    }
}


ISR(USART_RX_vect) {
	uint8_t data = UDR;
	if 
}