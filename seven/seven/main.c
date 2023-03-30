/*
 * seven.c
 *
 * Created: 02/02/2023 03:04:01 p. m.
 * Author : Ferna
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	PORTA = 0b11111111;
	DDRA = 0;
	DDRC = 0b11111111;
	uint8_t value;
	uint8_t decode[10]= {
		0b11111100,
		0b01100000,
		0b11011010,
		0b11110010,
		0b10110110,
		0b10111110,
		0b11100000,
		0b11111110,
		0b11101110
	};
    while (1) 
    {
		value = PINA & ((1<<4)-1);
		
    }
}

