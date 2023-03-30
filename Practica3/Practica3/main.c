/*
 * Practica3.c
 *
 * Created: 02/02/2023 03:21:21 p. m.
 * Author : Ferna
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "lcd.h"


int main(void)
{
	lcd_init(LCD_DISP_ON);
	
	DDRB = 0;
	PORTA = 255;
	
    while (1) {

    }
}

