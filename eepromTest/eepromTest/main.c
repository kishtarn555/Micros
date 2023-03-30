/*
 * eepromTest.c
 *
 * Created: 09/03/2023 04:03:24 p. m.
 * Author : Ferna
 */ 

#include "globals.h"
#include "lcd.h"
#include "keyboard.h"
#include "myeeprom.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

uint16_t val;

void display() {
	lcd_gotoxy(0,0);
	lcd_putc(val%10+'0');
	lcd_putc(val/10%10+'0');
	lcd_putc(val/100%10+'0');
	lcd_putc(val/1000%10+'0');
}

uint8_t readADC(int8_t pin) {
	ADMUX = (0b01100000) | pin; // Read from pin ADCpin, eg. ADC0, ADC1, ADC2...
	ADCSRA |= (1<< ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // ADC is async, so this is Await for conversion to end.
	_delay_us(600); //Just in case delay
	return ADCH; //Return ADCH, looses two bits from ADCL, but meh.
}


int main(void)
{	
	lcd_init(LCD_DISP_ON);
	keyboardInit();
	DDRA = 255;
	TCCR0 = 
	val = eeRead16(1);
    while (1) 
    {
		display();		
		_delay_ms(10);
		uint8_t nexo = keyboardDecode(keyboardReadOnRelease())-'0';
		_delay_ms(10);
		PORTA = nexo;	
		if (nexo > 9) continue;
		val = val%1000;
		val*=10;
		val+=nexo;
		eeWrite16(1, val);
    }
}

