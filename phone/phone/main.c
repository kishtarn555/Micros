/*
 * phone.c
 *
 * Created: 16/02/2023 01:54:56 p. m.
 * Author : Ferna
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "lcd.h"

volatile uint8_t press; //Press es un contador que determina cuantas veces el TIMER tiene que pulsar para que pase un segundo

int main(void)
{
    /* Replace with your application code */
	//Establece el A1 como salida, los demas son entrada
	DDRA = 2;
	//Pull UP en el A0
	PORTA = 1;
	//Activa interrupciones por comparacion OCIE0. Consulta el libro de Micros en moodle.
	TIMSK = (1<<OCIE0);
	// Establece el punto de comparación en 250.
	OCR0 = 249;		
	//Modo CTC usando un prescaler de 256. WGM01 es para el modo CTC y CS02 es para el prescaler.
	// Consulta el libro de Micros o: https://maxembedded.com/2011/06/avr-timers-timer0/
	TCCR0 = (1 << WGM01)|(1 << CS02);
	//LCD	
	lcd_init(LCD_DISP_ON);
	
	//Inicia en press como 0
	press = 0;
	// pos es la posicion donde debemos escribir nuestra letra, pero offsetada por uno. Es decir si pos == -1, entonces escribe el caracter 0. Si pos es 5, escribe el caracter 6.
	int8_t pos =-1;
	//Determina el caracter a escribir 0 es 'A', 1 es 'B', 2 es 'C', etc.
	uint8_t c = 0;
	
	sei(); // Activa interrupciones
    while (1)  {
		if ((PINA & 1)) //Espera a que se presione el boton
			continue;
	
		_delay_ms(50); //Retraso
		while(!(PINA & 1)); //Traba
		_delay_ms(50); //Retardo
		
		if (press ==0) { // Si press es 0, es porque ha pasado un segundo o mas.	
			c=0; //El caracter a escribir es 0, es decir 'A'.
			
			if (pos < 15) { //Revisa que aun haya espacio para escribir
				pos++;//Aumenta uno la posicion
				
			} else { //Ya habia demasiados caracteres, por lo que debemos reiniciar la pantalla
				pos=-1; //Reinicia el valor 
				//Codigo para borrar el LCD
				lcd_gotoxy(0,0); 
				lcd_puts("                ");				
				continue; //Regresa al inicio del while
			}
		} else { //press aun no es 0, por lo que no ha pasado un segundo	
			c++; //Sumale uno a c
			c%=26;//Sacale modulo 26 para que 'Z' pase a 'A'
		}
		
		lcd_gotoxy(pos,0); //Pon el cursor en la posicion a escribir el caracter.
		lcd_putc('A'+c); //Imprime el caracter 'A'+c.
		press=125; //Establece press como 125. Esto es porque un segundo toma 125 interrupciones del Timer0.
	}
}

ISR(TIMER0_COMP_vect) {
	// Note: for UX purposes, we only measure the time between presses, not between edges.
	// Nota: Por motivos de experiencia de usuario, 
	//medimos el tiempo en el que el boton no esta siendo presionado, no el tiempo entre altos.
	//Por eso (PINA & 1).
	if (press >0 && (PINA & 1)) {
		press--; //Decrementa press en uno si aun no es 0.
	}
}

