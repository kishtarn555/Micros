/*
 * adc.c
 *
 * Created: 07/03/2023 03:24:32 p. m.
 * Author : Ferna
 */ 

#define F_CPU 4000000
#include <util/delay.h>
#include <avr/io.h>

uint8_t readADC(int8_t pin) {
	ADMUX = (0b01100000) | pin; // Read from pin ADCpin, eg. ADC0, ADC1, ADC2...
	ADCSRA |= (1<< ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // ADC is async, so this is Await for conversion to end.
	_delay_us(600); //Just in case delay
	return ADCH; //Return ADCH, looses two bits from ADCL, but meh.
}

uint8_t decode(uint8_t x) {	
	if (x < 51) {
		return 0;
	} else
	if (x < 102) {
		return 1;
	} else
	if (x< 153) {
		return 3;
	} else
	if (x < 203) {
		return 7;
	} else {
		return 15;
	}
}

int main(void)
{
	// ADMUX  REFS0 makes it to decode [0V-5V]  (If set in a base, if alone, read docs).
	// ADLAR Makes it so ADCH has the msb and ADCL the lsb.
	ADMUX = (1<<REFS0) | (1<< ADLAR);
	//Free running
	SFIOR = 0;
	//ADEN enables ADC, ADPSX is the prescaler. F_CPU/prescaler must be in [50K, 200K]
	ADCSRA = (1<< ADEN)|(1 << ADPS2) | (1 << ADPS0);
	DDRA = 0;
	DDRB = 255;
	DDRC = 255;
	uint8_t sen1, sen2, sen3;
    while (1) {
		sen1 = readADC(0);
		sen2 = readADC(1);
		sen3 = readADC(2);
		PORTB = decode(sen1) | (decode(sen2)<<4);
		PORTC = decode(sen3);
    }
}

