/*
 * eepromToAdc.c
 *
 * Created: 14/03/2023 04:32:04 p. m.
 * Author : Ferna
 */ 
#include "globals.h"
#include "lcd.h"
#include "my_defines.h"
#include "myeeprom.h"
#include "keyboard.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t readADC(int8_t pin) {
	ADMUX = (0b01100000) | pin; // Read from pin ADCpin, eg. ADC0, ADC1, ADC2...
	ADCSRA |= (1<< ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // ADC is async, so this is Await for conversion to end.
	_delay_us(600); //Just in case delay
	return ADCH; //Return ADCH, looses two bits from ADCL, but meh.
}

volatile uint16_t pos;
uint8_t crashed;

void program() {
	sei();
	
	lcd_gotoxy(0,0);
	
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	
	lcd_puts("                ");
	lcd_gotoxy(0,0);
	lcd_puts("Sensando...");
	pos=0;
	crashed  =0;
	while (keyboardReadOnRelease()!= 14 && !crashed);	
	cli();
	if (crashed) {
		lcd_gotoxy(0,0);
		lcd_puts("EEPROM llena");
		_delay_ms(3000);
		return;
	}
	uint16_t cursor = pos-1;
	if (cursor >= 512) {
		lcd_gotoxy(0,0);
		lcd_puts("EEPROM vacia");
		_delay_ms(3000);
		return;
		
	}
	while (1) {
		lcd_gotoxy(0, 0);
		lcd_puts("Ultimos vals");
		lcd_gotoxy(0, 1);
		uint8_t val = eeRead(cursor);
		lcd_putc(val/100%10+'0');
		lcd_putc(val/10%10+'0');
		lcd_putc(val%10+'0');
		lcd_gotoxy(13, 1);
		
		lcd_putc(cursor/100%10+'0');
		lcd_putc(cursor/10%10+'0');
		lcd_putc(cursor%10+'0');
		do {
			val = keyboardReadOnRelease();
			if (val < 13 || val > 15) val=0;
			if (val == 13 && cursor ==0) val=0;
			if (val == 15 && cursor ==pos-1) val=0;
		} while (val==0);
		if (val == 13) cursor--;
		if (val == 15) cursor++;
		if (val == 14) break;		;
	}
}
int main(void)
{
	ADMUX = (1<<REFS0) | (1<< ADLAR);	
	ADCSRA = (1 << ADPS1) | (1 << ADPS0) | (1<< ADEN) | (1<< ADIE) | (1<< ADATE);
	SFIOR = (1 << ADTS1) | (1 << ADTS0);
	TCCR0 = TIMER0_CTC | PSCLR0_1024;		
	OCR0 = 244;
	TIMSK = (1 << OCIE0);

	DDRB = 1;	
	sei();
		
	lcd_init(LCD_DISP_ON);
	
	keyboardInit();
	while (1) {
		program();
	}
	
}




ISR(ADC_vect) {
	if (pos == 512) {
		ADCSRA &= ~(1 << ADATE);//Disable
		crashed = 1;//Inform of the crash
	}
	eeWrite(pos, ADCH);
	pos++;	
}

ISR(TIMER0_COMP_vect) {
	
}