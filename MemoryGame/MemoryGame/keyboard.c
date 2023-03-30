/*
 * keyboard.c
 *
 * Created: 22/02/2023 12:57:03 a. m.
 *  Author: Ferna
 */ 
#include "globals.h"
#include <stdint.h>
#include <avr/io.h>
#include "keyboard.h"
#include <util/delay.h>


void keyboardInit(void) {
	#if KB_READWRITEMODE == 1
		DDRKB = 0b00001111;
	#else
		DDRKB = 0b11110000;
	#endif
	PORTKB = 255;
}

uint8_t keyboardRead() {
	uint8_t response =1;	
	uint8_t test;
	uint8_t i;
	for ( i =0; i < 4; i++) {
		PORTKB = 255 ^ kb_out(i);
		asm("nop");
		test = PINKB & kb_readpins;
		if (test != kb_readpins) {
			_delay_ms(50);
			if (!(test & kb_read0))
				return response;
			if (!(test & kb_read1))
				return response+1;				
			if (!(test & kb_read2))
				return response+2;				
			if (!(test & kb_read3))
				return response+3;
		}
		response+=4;
	}
	return 0;
}

void keyboardHoldUntilOff() {
	while (keyboardRead()) {;}
	_delay_ms(50);
}

uint8_t keyboardReadOnRelease() {
	uint8_t response =0;
	do {
		response = keyboardRead();
	} while (response==0);
	keyboardHoldUntilOff();
	return response;
}

uint8_t keyboardDecode(const uint8_t keycode) {
	return KB_CODE_STR[keycode-1];
}