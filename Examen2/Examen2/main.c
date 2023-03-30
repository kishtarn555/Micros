/*
 * Examen2.c
 *
 * Created: 28/03/2023 11:18:48 a. m.
 * Author : Ferna
 */ 

#include "globals.h"
#include "keyboard.h"
#include "lcd.h"
#include "easy_registers.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define string char * 

#include <avr/eeprom.h>

#define PRICE_PTR 6
#define OPEN_PTR 5
#define LIGHT_PTR 4
#define DOOR_PIN 64
#define LIGHT_PIN 128


volatile uint8_t doorRemaingTime;
volatile uint8_t lightRemaingTime;

volatile uint16_t accum;

uint8_t price;
uint8_t open_time;
uint8_t light_time;

uint8_t accumulated;

void writeConfig() {
	cli();
	eeprom_write_byte((uint8_t*)PRICE_PTR,price);
	eeprom_write_byte((uint8_t*)OPEN_PTR,open_time);
	eeprom_write_byte((uint8_t*)LIGHT_PTR,light_time);	
	sei();
}

void readConfig() {
	cli();
	price = eeprom_read_byte((uint8_t*)PRICE_PTR);
	open_time = eeprom_read_byte((uint8_t*)OPEN_PTR);
	light_time = eeprom_read_byte((uint8_t*)LIGHT_PTR);
	sei();
}

void debugInit() {
		open_time = 10;
		light_time = 24;
		price = 52;
}


void title(const char * msg) {
	uint8_t len=0;
	while (msg[len++]);
	len = 8-len/2;
	while (len--) lcd_putc(' ');
	lcd_puts(msg);
}

void lcd_cls() {
	lcd_gotoxy(0,0);
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	lcd_puts("                ");	
	lcd_gotoxy(0,0);
}

uint8_t readValue(uint8_t* value) {
	uint8_t temporal = 0;	
	uint8_t key;
	uint8_t i = 2;
	while (i--) {
		do {
			key = keyboardReadOnRelease();
			if (key == 16)return 1;
			key = keyboardDecode(key);
			key -= '0';
		} while(key > 9);
		lcd_putc(key+'0');
		temporal *=10;
		temporal += key;
	}
	while (1) {
		key = keyboardReadOnRelease();
		if (key == 16) return 1;
		if (key == 13) break;
	}
	if (temporal ==0) {
		return 2;
	}
	key = *value;	
	*value = temporal;	
	if (open_time > light_time) {
		*value = key;
		return 2;
	}
	return 0;
}	

void inputValue(string name, string prefix, string suffix, uint8_t * value) {
	lcd_cls();
	title(name);
	lcd_gotoxy(3,1);
	lcd_puts(prefix);
	
	lcd_gotoxy(7,1);
	lcd_puts(suffix);	
	lcd_gotoxy(5,1);
	uint8_t returnCode =readValue(value);	
	lcd_cls();
	if (returnCode == 0) {
		writeConfig();
		lcd_puts("GUARDANDO...");
	} else if (returnCode == 1){		
		lcd_puts("CANCELANDO...");
	} else {
		lcd_puts("ERROR...");
	}
	_delay_ms(1000);
	
}

void printCount() {
	lcd_gotoxy(7,1);
	lcd_putc(accumulated/10+'0');
	lcd_putc(accumulated%10+'0');
}

void reset() {
	accumulated =0;
	lcd_cls();
	title("Bienvenido");
	lcd_gotoxy(0,1);
	lcd_puts("Lleva: ");
	printCount();
	
}

uint8_t decode(uint8_t x) {
	if (x < 51) {
		return 2;
	} else
	if (x < 102) {
		return 0;
	} else
	if (x< 153) {
		return 5;
	} else
	if (x < 203) {
		return 10;
	} else {
		return 1;
	}
}

uint8_t readADC() {
	ADCSRA |= (1<< ADSC);
	while (ADCSRA & (1 << ADSC));
	_delay_ms(50);
	return ADCH;
}

void activate() {
	lcd_cls();
	lcd_puts("OCUPADO...");
	accum=1;
	doorRemaingTime=open_time;
	lightRemaingTime=light_time;
	while (doorRemaingTime || lightRemaingTime) {
		if (lightRemaingTime) {
			PORTD |= LIGHT_PIN;
			} else {
			PORTD &= ~LIGHT_PIN;
		}
		
		if (doorRemaingTime) {
			PORTD |= DOOR_PIN;
			} else {
			PORTD &= ~DOOR_PIN;
		}
	}
	PORTD=0;
}
int main(void) {    
	PORTB=128;
	DDRD = 255;
	PORTD = 0;
	
	
	TCCR0 = TIMER0_CTC | PSCLR0_64;
	OCR0 = 124;
	TIMSK = (1 << OCIE0);
	
	readConfig();
	accum =0;
	sei();
	//If memory was corrupted (or it's first time opening the device), reset factory defaults
	if (price == 0 || price > 99) price = 10;
	if (light_time == 0 || light_time > 99) light_time = 10;
	if (open_time == 0 || open_time > light_time) open_time = light_time;
	writeConfig();
	
	keyboardInit();
	lcd_init(LCD_DISP_ON);
	reset();
	uint8_t last_button_read =128;
	ADMUX = (1<<REFS0) | (1<< ADLAR) | 7;
	SFIOR = 0;
	//ADEN enables ADC, ADPSX is the prescaler. F_CPU/prescaler must be in [50K, 200K]
	ADCSRA = (1<< ADEN)|(1 << ADPS2) | (1 << ADPS1);
	
    while (1) {
		uint8_t keycode = keyboardRead();
		if (keycode != 0) {
			if (keycode == 4) {
				inputValue("COSTO","$","", &price);
				reset();
			} else if (keycode == 8) {
				inputValue("APERTURA"," "," seg", &open_time);
				reset();
			} else if (keycode == 12) {				
				inputValue("ILUMNICACION"," "," seg", &light_time);
				reset();
			}
		}
		uint8_t button_read = (PINB & 128);
		if (button_read != last_button_read) {
			_delay_ms(100);
			last_button_read =button_read;
			if (button_read == 128) {
				accumulated += decode(readADC());
				if (accumulated >= price) {
					activate();
					reset();
				} else {					
					printCount();
				}
				
			}
			
		}
		
		
    }
	return 0;
}


ISR(TIMER0_COMP_vect) {
	accum++;
	accum%=1000;
	if (accum == 0) {
		if (doorRemaingTime) doorRemaingTime--;
		if (lightRemaingTime) lightRemaingTime--;
	}
	
	
}