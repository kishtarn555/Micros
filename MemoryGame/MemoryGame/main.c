/*
 * MemoryGame.c
 *
 * Created: 22/02/2023 12:36:12 a. m.
 * Author : Ferna
 */ 
#include "globals.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"
#include "keyboard.h"
void cls() {
	lcd_gotoxy(0, 0);
	lcd_puts("                ");
	lcd_gotoxy(0, 1);
	lcd_puts("                ");
}
void printLines(const char * a, const char * b ) {
	cls();
	lcd_gotoxy(0,0);
	lcd_puts(a);
	lcd_gotoxy(0,1);
	lcd_puts(b);
}

uint8_t gbit(uint16_t pos, uint16_t a, uint16_t b) {
	if (pos < 10) return (a & (1<<pos))?1:0;
	return (b & (1<<(pos-10)))?1:0;
}

void sbit(uint16_t pos, uint16_t *a, uint16_t *b) {
	if (pos < 10) {
		*a |= (1<<pos);
	} else {
		*b |= (1<<(pos-10));
	}
}

int main(void)
{	
	DDRB=255;
	PORTB=0;
    lcd_init(LCD_DISP_ON);
	keyboardInit();
	char firstLine[17];
	char secondLine[17];
	uint8_t state =0;
	uint8_t key =0;
	uint8_t hide=0;
	uint8_t seed =0;
	
	
	uint16_t UP = 0;
	uint16_t LOW = 0;
	uint16_t VH = 0;
	uint16_t VL = 0;
	
    while (1) {
		if (state ==0) {
			UP = LOW = 0;
			VH = VL = 0;
			PORTB = 0;
			cls();
			lcd_gotoxy(0, 0);
			lcd_puts("Cuantos cuadros?");
			do {
				seed++;
				key = keyboardDecode(keyboardReadOnRelease());
			} while (key < '0' || key > '9');
			hide = key-'0';
			state++;
			continue;
		}
		if (state == 1) {
			srand(seed);
			sprintf(firstLine, "Escondere %d", hide);
			sprintf(secondLine, "cuadritos");
			printLines(firstLine, secondLine);		
			_delay_ms(3000);
			
			sprintf(firstLine, "Tu debes buscar");
			sprintf(secondLine, "%d cuadritos.", hide);
			printLines(firstLine, secondLine);
			_delay_ms(3000);
			
			sprintf(firstLine, "2 filas (1, 2)");
			sprintf(secondLine, "10 cols (0-9)");
			printLines(firstLine, secondLine);
			_delay_ms(3000);
			
			sprintf(firstLine, "Presiona +");
			sprintf(secondLine, "para continuar...");
			printLines(firstLine, secondLine);			
			while (keyboardReadOnRelease()!=16);
			state++;
			continue;			
		}
		if (state == 2) {
			sprintf(firstLine, "Trata de");
			sprintf(secondLine, "memorizar...");
			printLines(firstLine, secondLine);
			_delay_ms(3000);
			
			sprintf(firstLine, "Tienes %02d", 2*hide);
			sprintf(secondLine, "Intentos");
			printLines(firstLine, secondLine);
			_delay_ms(3000);
			
			uint8_t coord=0;
			for (uint8_t i =0;i < hide; i++) {
				uint8_t step = 1+rand()%20;
				while (step--) {
					do  {
						coord++;
						coord%=20;
					}while (gbit(coord, LOW, UP));
				}
				sbit(coord, &LOW, &UP);
				sprintf(firstLine, "Cuadrito %d", i+1);
				sprintf(secondLine, "%d, %d", coord/10, coord%10);
				printLines(firstLine, secondLine);
				_delay_ms(750);
			}
			state++;
			continue;
		}
		if (state == 3) {
			
			uint8_t attempts =0, found =0;
			uint8_t f= 0, c = 0;
			while (attempts < hide*2 && found < hide) {
				PORTB = found;
				sprintf(firstLine, "Intento %02d ", attempts+1);
				cls();
				lcd_gotoxy(0,0);
				lcd_puts(firstLine);		
				if (f != 0) {
					sprintf(firstLine, "(%c,", f);
					lcd_puts(firstLine);
				}
				if (c != 0) {
					sprintf(firstLine, "%c)", c);
					lcd_puts(firstLine);
					
					if (f < '0' || f >'1' || c <'0' || c >'9') {
						lcd_gotoxy(0,1);
						lcd_puts("No existe lugar");
						attempts++;
						f=c=0;
						_delay_ms(3000);
						continue;
					}
					f -='0';
					c -='0';
					if (gbit(f*10+c, VL, VH)) {						
						lcd_gotoxy(0,1);
						lcd_puts("Ya estaba!");
						f=c=0;
						_delay_ms(3000);
						continue;
					}
					sbit(f*10+c,&VL, &VH);
					if (gbit(f*10+c, LOW, UP)) {
						lcd_gotoxy(0,1);
						lcd_puts("Acierto");
						attempts++;
						found++;
						f=c=0;
						_delay_ms(3000);
						continue;
					} else {
						lcd_gotoxy(0,1);
						lcd_puts("Error");
						attempts++;
						f=c=0;						
						_delay_ms(3000);
						continue;
					}
				}
				if (f ==0) {
					f = keyboardDecode(keyboardReadOnRelease());
				} else if (c ==0) {
					c = keyboardDecode(keyboardReadOnRelease());
				}
			}
			PORTB = found;
			if (found == hide) {
				sprintf(firstLine,  "Tienes excelente");
				sprintf(secondLine, "memoria !!!");
				printLines(firstLine, secondLine);
				_delay_ms(3000);
				
				sprintf(firstLine,  "Ganaste!!!");
				sprintf(secondLine, "Felicidades!");
				printLines(firstLine, secondLine);
				_delay_ms(3000);
			} else {
				sprintf(firstLine,  "Tu memoria no es");
				sprintf(secondLine, "tan buena =(");
				printLines(firstLine, secondLine);
				_delay_ms(3000);
				
				sprintf(firstLine,  "Perdiste!!!");
				sprintf(secondLine, "Intenta de nuevo");
				printLines(firstLine, secondLine);
				_delay_ms(3000);
			}
			state =0;
			continue;
		}
    }
}

