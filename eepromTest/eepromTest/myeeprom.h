
#ifndef MYEEPROM_H_
#define MYEEPROM_H_

#include <stdint.h>


void eeWrite(uint16_t pos, uint8_t val);
void eeWrite16(uint16_t pos, uint16_t val);
uint8_t eeRead(uint8_t pos);
uint16_t eeRead16(uint8_t pos);



#endif /* MYEEPROM_H_ */