/*
 * my_defines.h
 *
 * Created: 21/02/2023 01:19:56 p. m.
 *  Author: Ferna
 */ 


// Este archivo me permite establecer los registros con palabras mas sencillas
// Por ejemplo, si quiero el timer0 en modo CTC y un prescaler de 64 escribo:
// TCCR0 = TIMER0_CTC | PSCLR0_64;

#ifndef MY_DEFINES_H_
#define MY_DEFINES_H_


#define PSCLR0_OFF 0
#define PSCLR0_0 1
#define PSCLR0_8 2
#define PSCLR0_64 3
#define PSCLR0_256 4
#define PSCLR_1024 5
#define PSCLR_T0_FALLING 6
#define PSCLR_T0_RISING 7

#define TIMER0_NORMAL 0
#define TIMER0_PHASE_PWM 64
#define TIMER0_CTC 8
#define TIMER0_FAST_PWM 72


#endif /* MY_DEFINES_H_ */