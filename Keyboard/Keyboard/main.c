/*
 * Keyboard.c
 *
 * Created: 07/02/2023 03:22:19 p. m.
 * Author : Ferna
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define MAT_KEY_PIN PINA
#define MAT_KEY_DDR DDRA
#define MAT_KEY_PORT PORTA

#include "lcd.h"

void initMatrixKeyboard() {
	// Establce Los puertos del teclado unos de salida y otros de entrada 
	// WARNING: Esto esta feo
	MAT_KEY_DDR = 0b11110000;
	//Activa todos los puertos, en la salida saca 1s, en la entradas activa los pull ups
	MAT_KEY_PORT = 255;
}

/*
	Esto lee el teclado matricial, regresa un valor entre 1 y 16, dependiendo de cual tecla se este pulsando
	Si ninguna tecla esta siendo pulsada, regresa 0.
*/
uint8_t readFirstMatrixKeyboard() {	
	uint8_t returnCode=0;
	//Revisa cada ¿fila?
	for (uint8_t i =0; i < 4; i++) {		
		//ACtiva esa fila, prendiendo todos los pines excepto el i+4.
		MAT_KEY_PORT = 255 ^ (1<<(i+4));
		//Delay muy corto (un ciclo de reloj)
		asm("nop");		
		//Revisa las ¿columnas? y lee ese valor
		uint8_t res = MAT_KEY_PIN & 15;
		// No será 15 si algun boton en se esta presionando
		if (res != 15) {
			_delay_ms(50);
			//Retardo pq es un botón
			
			//Fue el primer boton en la ¿fila?
			if (!(res &1)) {
				return returnCode+1;
			}
			
			//Fue el segundo boton en la ¿fila?
			if (!(res &2)) {
				return returnCode+2;
			}			
			//Fue el tercero boton en la ¿fila?
			if (!(res &4)) {
				return returnCode+3;
			}			
			//Fue el cuarto boton en la ¿fila?
			if (!(res &8)) {
				return returnCode+4;
			}
		}
		//Sumale 4 a la salida candidata, porque cada fila tiene 4 botones
		returnCode+=4;
	}
	return 0;
}


int main(void)
{
	//Inicia los puertos del teclado matricial
	initMatrixKeyboard();
	lcd_init(LCD_DISP_ON); //ACtiva el lcd
    /* Replace with your application code */
    uint8_t response=0; //Esto lo usamos para guardar el boton presionado del teclado
    uint8_t length=0; // La longitud del mensaje en el LCD
    while (1) {
		//Mientras no hayas presionado algun teclado, vuelve a checar
		while (!response) {
			response = readFirstMatrixKeyboard();
		}
		//Revisa la respuesta, si fue el boton 4, es el de limpiar la pantalla
		if (response == 4) {
			//Empieza a escribir desde el inicio del LCD
			lcd_gotoxy(0,0);
			//Escribe espacios para borrar
			lcd_puts("          ");			
			//Regresa al inicio
			lcd_gotoxy(0,0);
			//Establece la longitud como 0
			length=0;
		} else if (response == 12) { // 12 es el boton que solo borra un caracter
			if (length >= 1) { // Revisa que si haya caracter a borrar
				lcd_gotoxy(--length,0);//Pon el cursos sobre el caracter a borrar y restale uno a la longitud
				lcd_putc(' '); // borrarlo con un espacio
			
				lcd_gotoxy(length,0); //Pon el cursor en el lugar correcto
			}
		
		} else if (length < 10) {
			
			lcd_putc(" 147*25803694A987"[response]); //Escribe la tecla, la string decide cual es el simbolo para el boton 1, 2, 3, 4, 5, 6, ..., 16
			length++; //Sumale uno a longitud, escribiste un caracter
		}
		//Traba, espera a que el usuario suelte el boton
		while (response) {
			response = readFirstMatrixKeyboard();
		}
		//Retado
		_delay_ms(50);
    }
}

