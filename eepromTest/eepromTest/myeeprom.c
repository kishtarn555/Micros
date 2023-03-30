#include "myeeprom.h"
#include <avr/interrupt.h>

void eeWrite(uint16_t pos, uint8_t val) {
	while (EECR &(1<<EEWE));
	
	EEAR = pos;
	EEDR = val;
	#ifdef cli
	cli();
	#endif
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	#ifdef sei
	sei();
	#endif
}

void eeWrite16(uint16_t pos, uint16_t val) {
	eeWrite(pos, val >> 8);
	eeWrite(pos+1, (uint8_t)val);
}
uint8_t eeRead(uint8_t pos) {
	while (EECR &(1<<EEWE));
	EEAR = pos;
	EECR |= (1<<EERE);
	return EEDR;
}

uint16_t eeRead16(uint8_t pos) {
	uint16_t ans = eeRead(pos);
	return (ans << 8) | eeRead(pos+1);
}