/*
 * sine.c
 *
 * Created: 21/02/2023 11:10:27 a. m.
 * Author : Ferna
 */ 

//#define K_DEBUG //Esto Activa el LED de depuracion
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "my_defines.h" // Este es un archivo que cree que me permite configurar el timer de forma mas sencilla

//Estos son los valores de la onda sinuidal, si los graficas veras la funcion seno
uint8_t sine[] = {
	127, 130, 133, 136, 139, 143, 146, 149, 152, 155, 158, 161, 164, 167, 170, 173, 176, 179, 182, 184, 187, 190, 193, 195, 198, 200, 203, 205, 208, 210, 213, 215, 217, 219, 221, 224, 226, 228, 229, 231, 233, 235, 236, 238, 239, 241, 242, 244, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 254, 254, 254, 255, 254, 254, 254, 254, 254, 253, 253, 252, 251, 251, 250, 249, 248, 247, 246, 245, 244, 242, 241, 239, 238, 236, 235, 233, 231, 229, 228, 226, 224, 221, 219, 217, 215, 213, 210, 208, 205, 203, 200, 198, 195, 193, 190, 187, 184, 182, 179, 176, 173, 170, 167, 164, 161, 158, 155, 152, 149, 146, 143, 139, 136, 133, 130, 127, 124, 121, 118, 115, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 72, 70, 67, 64, 61, 59, 56, 54, 51, 49, 46, 44, 41, 39, 37, 35, 33, 30, 28, 26, 25, 23, 21, 19, 18, 16, 15, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 26, 28, 30, 33, 35, 37, 39, 41, 44, 46, 49, 51, 54, 56, 59, 61, 64, 67, 70, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 115, 118, 121, 124
	};

//Contador para mostrar donde en la onda estamos. Va de 0 a 
volatile uint8_t idx;

int main(void)
{
	idx =0;
	OCR0 = 128;
	// Activate TIMSK interrupts
	TIMSK = (1<<TOIE0);
	// Lets do a clock at  8 preescaler => 488 hz (Close-ish to 440) With non-inverted OC0
	//TIMER0_FAST_PWM y PSCLR0_0 estan en el archivo my_defines.h
	// 1 << COM01 dice que el PWM controle el puerto B3
	TCCR0 = TIMER0_FAST_PWM | PSCLR0_0 | (1<<COM01);
	//Activa interrupciones
	sei(); 
	// Establece PORTB 3 como salida 
	DDRB |= (1<<3);
	//Led de depuracion de salida
	DDRD |= 1;
	PORTD |= 1;
    while (1) {

    }
}


//Interrupcion del timer 0 por overflow 
//ISR = Interrupt Service Routine
//Timer0_OVF = Timer 0, overflow.
ISR(TIMER0_OVF_vect) {
	idx++; //Incrementa idx en uno, si idx era 255, pasa a 0 por overflow
	OCR0 = sine[idx];//cambia el duty time del PWM a ser el valor de sine[idx]
	
	//Este codigo controla el LED de depuracion, pero no hace nada
	#ifdef K_DEBUG
		if (idx==0) {
			PORTD ^= 1;
		}
	#endif
}

