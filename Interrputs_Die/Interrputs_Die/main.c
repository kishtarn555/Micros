/*
 * Interrputs_Die.c
 *
 * Created: 14/02/2023 12:00:57 p. m.
 * Author : Hector Fernando Ricardez Lara
 */ 
#define F_CPU 8000000
//#define UP_DEBUG

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
//IMPORTANTE: son volatiles porque interrupt
volatile uint16_t seed; //Semilla para el valor aleatorio, es el valor entre que arranca el micro y la primera vez que el usuario pide un random
volatile uint8_t number; // El numero aleatorio generado

int main(void) {
	DDRD = 0; // Establece el puerto D como entrada
	PORTD = 0; // Establece el puerto D en high-impedance. Esto es porque estaremos usando una resistencia pulldown afuera
	DDRA = 255;	// Establece el puerto A como salida
	DDRB = 255; // Establece el puerto B como salida
	PORTA = PORTB = 0; //Al inicilo los LEDs estaran apagados
	sei(); // Activa las interrupciones
	GIFR |= (1<<6);	// Limpia las banderas de interrupcion externa, evita llamar una interrupcion accidental al inicio	
	GICR |= (1<< 6); // Activa la interrucpion 
	MCUCR = MCUCR & 0b11110000; // Activa interrupciones en nivel bajo
	number=0; //Establece el numero como 0, esto es util para detectar cuando presionamos el boton al inicio
	while (1) {
		seed++; // Aumentale uno a la semilla, para contar cuanto tiempo paso desde el arranque a la primera pulsasion
    }
}

// quick defines for die LEDS
// Esto es util para prender los focos del dado
// Los usamos de forma que cada PIN controla un LED del dado de la siguiente manera:
// p00 p01 p02
// p10 p11 p12
// p20 p21 B
//Nota: El foco que sería p22 es controlado por el puerto B
#define p00 1
#define p01 2
#define p02 4
#define p10 8
#define p11 16
#define p12 32
#define p20 64
#define p21 128
//INT0_vect es Interrupcion 0
ISR(INT0_vect) {
	_delay_ms(100); //Se activo una interrupcion externa, haz retardo
	uint8_t x=0;
	while ((PIND & (1<<2))==0) { // Traba, x no hace nada
		x++;
	}
	_delay_ms(100); // Retardo
	if (number ==0 ) { //Si number es la primera vez que se presiona
		srand(seed); // usa la variable seed para establecer la semilla aleatorioa del pseudo generador de numeros aleatorios
	}
	number = rand()%6+1; // Genera el siguiente numero aleatorio, mod 6 + 1 => un numero entre 1 y 6.
	// number to die decoder
	// Revisa cual numero se genero y en base a eso prende los pines
	switch(number) {
		case 1:
			PORTA = p11; //Prende el foco p11, el de enmedio
			PORTB = 0; // Apaga el foco derecho inferior (p22)
			break;
		case 2:
			PORTA = p02 | p20; //Prende el foco p02 y p20
			PORTB = 0; //Apaga el foco p22
			break;
		case 3:
			PORTA = p00 | p11; // Prende el foco p00 y p11
			PORTB = 1; // Prende el foco p22
			break;
		case 4:
			PORTA = p00 | p02 | p20; // Prende el foco p0, p02 y p20	
			PORTB = 1; //Prende el foco "p22"
			break;
		case 5:
			PORTA = p00 | p02 | p20 | p11; //Prende el foco p00, p02 y p11
			PORTB = 1; // Prende el foco "p22"
			break;
		case 6:
			PORTA = p00 | p02 | p10 | p12 | p20;
			PORTB = 1;
			break;
	
	}
	//Este codigo de aqui abajo es para debuguear/depurar. No es necesario entenderlo, de hecho. 
	// Ni siquiera es compilado
	// Podemos ignorarlo, pero si quieres saber mas puedes leer:
	//https://es.stackoverflow.com/questions/120518/para-que-funcionan-las-sentencias-endif-if-ifdef#:~:text=%23ifdef%20permite%20compilar%20una%20secci%C3%B3n,importar%20cu%C3%A1l%20sea%20su%20valor.&text=El%20c%C3%B3digo%20dentro%20de%20la,compilado%20si%20PRUEBA%20esta%20definida.	
	// put number to debug
	#ifdef UP_DEBUG
		PORTB |= number << 1;
	#endif
}