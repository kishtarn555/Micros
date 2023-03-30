//NOTE MODIFIED

/*
 * keyboard.h
 *
 * Created: 22/02/2023 12:56:31 a. m. 
 *  Author: Hector Fernando Ricardez Lara
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#define KB_PROTEUS_CODE "789/456*123-C0=+"
#define KB_MAT_CODE "123A456B789C*0#D"

#define PORTKB PORTB //Set PORTA, PORTB, PORTC or PORTD
#define DDRKB DDRB
#define PINKB PINB

#define KB_PRESET 0 //0 for manual, otherwise use a PRESET: 1 is proteus, 2 is 4x4 keyboard I had

#if KB_PRESET == 0
//Manual CONFIG
#define KB_READWRITEMODE 1 //Defines which half of port is input. 0: input is low bits
#define KB_READDIR 0 //Defines the direction of input pins; 0 is asc, 1 is desc
#define KB_WRITEDIR 1 //Defines the direction of output pins; 0 is asc, 1 is desc
#define KB_CODE_STR "123456789:0:" //Define the keyboard setup, 

#elif KB_PRESET == 1
#define KB_READWRITEMODE 0
#define KB_READDIR 1
#define KB_WRITEDIR 1
#define KB_CODE_STR KB_PROTEUS_CODE

#elif KB_PRESET == 2
#define KB_READWRITEMODE 0
#define KB_READDIR 1
#define KB_WRITEDIR 1
#define KB_CODE_STR KB_MAT_CODE

#endif
/**
	Inits the Keyboard
**/
void keyboardInit();
/****************************************************************************/
/* Checks keyboard and returns the keycode corresponging to the key pressed	*/
/* Keycodes go from 1 to 16													*/
/* If no key is being pressed, returns 0									*/
/****************************************************************************/
uint8_t keyboardRead();
/************************************************************************/
/* Waits until no key is being pressed									*/
/************************************************************************/
void keyboardHoldUntilOff();
/************************************************************************/
/* Returns keycode when key is realesed                                */
/************************************************************************/
uint8_t keyboardReadOnRelease();

/************************************************************************/
/* Decode keycode to byte                                              */
/************************************************************************/
uint8_t keyboardDecode(uint8_t keycode);

#if KB_READWRITEMODE == 0
	#define kb_readpins 15
	#if KB_WRITEDIR == 0
		#define kb_out(i) (1<<(4+i))
	#else
		#define kb_out(i) (1<<(7-(i)))
	#endif
	#if KB_READDIR == 0
		#define kb_read0 1
		#define kb_read1 2
		#define kb_read2 4
		#define kb_read3 8
	#else	
		#define kb_read0 8
		#define kb_read1 4
		#define kb_read2 2
		#define kb_read3 1
	#endif
#else
	#define kb_readpins 240
	
	#if KB_WRITEDIR == 0
		#define kb_out(i) ((1<<i)*2)
	#else
		#define kb_out(i) ((1<<(3-i))*2)
	#endif
	#if KB_READDIR == 0
		#define kb_read0 16
		#define kb_read1 32
		#define kb_read2 64
		#define kb_read3 128
	#else
		#define kb_read0 128
		#define kb_read1 64
		#define kb_read2 32
		#define kb_read3 16
	#endif
#endif



#endif /* KEYBOARD_H_ */