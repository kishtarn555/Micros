/*
 * sine.c
 *
 * Created: 21/02/2023 11:10:27 a. m.
 * Author : Ferna
 */ 

#define K_DEBUG
#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "my_defines.h"


#define notes 78
uint8_t sine[128] = {
23,23,0,23,0,29,23,0,19,0,0,0,39,0,0,0,29,0,0,39,0,0,46,0,0,35,0,31,0,33,35,0,39,23,19,17,0,21,19,0,23,0,29,26,31,0,0,29,0,0,39,0,0,46,0,0,35,0,31,0,33,35,0,39,23,19,17,0,21,19,0,23,0,29,26,31,0,0


};


volatile uint8_t idx;

int main(void)
{
	idx =0;
	OCR0 = 141 ;
	// Activate TIMSK interrupts
	//TIMSK = (1<<TOIE0);
	// Lets do a clock at  8 preescaler => 488 hz (Close-ish to 440) With non-inverted OC0
	TCCR0 = TIMER0_CTC | PSCLR0_8 | (1<<COM00);
	sei(); 
	DDRB |= (1<<3);
	DDRD |= 1;
	PORTD |= 1;
	int8_t cnt =0;
    while (1) {
		if (sine[cnt]==0) {		
			cnt++;	
			cnt%= notes;
			TCCR0 &= ~(1<<COM00);
			_delay_ms(200);
		} else {
			OCR0 = sine[cnt];
			cnt++;
			cnt %= notes;
			_delay_ms(150);
		
			TCCR0 &= ~(1<<COM00);
			_delay_ms(50);
		}
		TCCR0 |= (1<<COM00);
		//_delay_ms(200);
    }
}

