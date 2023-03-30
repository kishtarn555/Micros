/*
 * GccApplication1.c
 *
 * Created: 31/01/2023 03:29:40 p. m.
 * Author : Ferna
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int main(void)
{
    // Establece los puertos de A como entrada
    DDRA = 0;
	// Pull ups
    PORTA = 0b11111111;
	//Puerto C salida
    DDRC = 0b11111111;
	//Valor a mostrar en los displays
	uint8_t counter = 0;
	//Valor actual en el puerto A
	uint8_t current = 0;
	//Entrada del puerto A en el ciclo (del while) anterior
	uint8_t previous = 0;
	while (1) {
		// Leer entrada en A
		current = PINA;
		//Si es la misma que antes, el usuario no ha hecho nada (ya sea soltar el boton, o presionarlo)
		if (current == previous)
			continue; // Entonces termina el ciclo y regresa arriba 
			
		_delay_ms(50); // Retardo
		//((current^previous) & (1 << 0)): Usando XOR revisamos si vario en el pin 0 y  !(current & (1 << 0)) actualmente esta siendo pulsado
		// La primera parte revisa que el usuario interactuo con el boton 0, y la segunda parte que esa interaccion sea pulsarlo
			if (((current^previous) & (1 << 0)) && !(current & (1 << 0))) {
				counter++; // Sumale 1
				if (counter > 99) { // Si se paso de 99, regresalo a 99
					counter = 99;
				}
		}
		// Misma logica de arriba, pero con el boton conectado a 7
		if (((current^previous) & (1 << 7)) && !(current & (1 << 7))) {
			// Solo haz el decremento si el contador es mayor que 0
			if (counter >0 ) {
			counter--;
			}
		}
		//Muestra el contador (counter/10) nos da los decenas. Nos da las unidades ((counter%10)<<4), pero en la parte superior del puerto
		PORTC = (counter/10)|((counter%10)<<4);		
		// Cambia previous a current, para el siguiente ciclo.
		previous = current;
	}
	return 0;
}

