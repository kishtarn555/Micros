/*
 * Examen.c
 *
 * Created: 23/02/2023 12:34:21 p. m.
 * Author : Ferna
 */ 

#include "globals.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "keyboard.h"
#include "my_defines.h"
void showMessage(char * up, char * down);
void part1();
void part2();
void part3();
void part4();



void setup() {
	lcd_init(LCD_DISP_ON);	
	DDRA = 255;
	PORTA = 0;
	keyboardInit();
	DDRD =0;
	PORTD = 255;
	TCCR0 = TIMER0_CTC | PSCLR0_8;
	OCR0 = 199;
	TIMSK = (1 << OCIE0);
	cli();
}



#define INICIO 4
#define DETENER 64

#define IN PIND

volatile int16_t ticks;
volatile int16_t centiseconds;

int main(void)
{
	setup();
	
    while (1) {
		part1();
		while((IN &INICIO)) ticks++;
		_delay_ms(50);
		part2();
		part3();
		while((IN &DETENER));		
		_delay_ms(50);
		while(!(IN &DETENER));				
		cli(); 
		_delay_ms(50);
		
		part4();		
		_delay_ms(1000);
    }
}

void cls() {
	lcd_gotoxy(0,0);
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	lcd_puts("                ");
	//For good measure
	lcd_gotoxy(0,0);
}

void showMessage(char * up, char *down) {
	cls();
	lcd_puts(up);
	lcd_gotoxy(0,1);	
	lcd_puts(down);	
	_delay_ms(1000);
}

void part1() {
	PORTA = 15;
	showMessage("Mediremos tu", "reaccion.");
	showMessage("Debes presionar", "INICIO");	
	showMessage("Presiona los", "numeros en orden");	
	showMessage("Luego presiona", "DETENER");	
	showMessage("Listo?", "Te espero...");
}

int8_t randoms[5];
uint8_t guess[5];
void part2() {
	cls();
	srand(ticks);
	lcd_gotoxy(0,0);
	lcd_puts("Los numeros son:");	
	lcd_gotoxy(0,1);
	for (int8_t i =0; i < 5; i++) {
		randoms[i] = rand()%10;
		if (i !=0) {
			lcd_puts(", ");
		}
		lcd_putc(randoms[i]+'0');
	}
	
}

void part3() {
	centiseconds=0;
	ticks=0;
	sei();
	int8_t ptr=0;
	while (ptr < 5) {
		guess[ptr] = keyboardDecode(keyboardReadOnRelease())-'0';
		PORTA = guess[ptr];
		ptr++;
	}		
}	

void part4() {
	for (uint8_t i =0; i < 5; i++) {
		if (guess[i]!=randoms[i]) {
			showMessage("Los numeros", "estan mal :C");
			return;
		}
	}
	cls();
	lcd_gotoxy(0,0);
	lcd_puts("Muy bien!");
	lcd_gotoxy(0,1);
	lcd_puts("Tardaste: ");	
	lcd_putc(centiseconds/1000+'0');
	lcd_putc(centiseconds/100%10+'0');
	lcd_putc(':');
	lcd_putc(centiseconds/10%10+'0');
	lcd_putc(centiseconds%10+'0');
	lcd_puts("s");
	_delay_ms(1000);
	
}

ISR(TIMER0_COMP_vect) {
	ticks++;
	if (ticks == 50) {
		ticks=0;
		centiseconds++;
	}
}