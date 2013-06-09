/**
 * Orlando Cosimo
 * CreativitySlashDesign .TK
 *
 * 08.06.2013
 *
 * Platform -> Stellaris LaunchPad (LM4F)
 * Dev tool -> Energia
 *
 * Library for 2x16 LCD
 */

#ifndef LCD_h
#define LCD_h

#include "Energia.h"
#include <inttypes.h>

typedef volatile uint32_t regtype;
typedef uint8_t regsize;

#define portOutputRegister(x) (regtype)portBASERegister(x)
#define cbi(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi(reg, mask) GPIOPinWrite(reg, mask, mask)

class LCDClass
{

private:

	static uint8_t _RS_pin, _EN_pin;
	static uint8_t _data_pins[4]; // 4 bit mod
	static regtype _port;
	static regsize _bit;

	void fast_w(uint8_t p, boolean state);
	void rs(int i);
	void en(int i);
	void EN_Acknow();
	void up_w(unsigned char c);  // write the upper half of character
	void down_w(unsigned char c);// write the lower half of character
	void command(unsigned char c);
	void push(char c);
	char* itoa(int val, int base);
	void goTo(uint8_t x, uint8_t y);

public:

	LCDClass(void);
	void clear(void);
	void print(int n);
	void print(char c);
	void print(const char* s);
	void print(int n, uint8_t x, uint8_t y);
	void print(char c, uint8_t x, uint8_t y);
	void print(const char* s, uint8_t x, uint8_t y);
	void scrollLeft(String text, uint8_t row, int ms);
	void scrollRight(String text, uint8_t row, int ms);
	void init(int inRS, int inEN, int inDB4, int inDB5, int inDB6, int inDB7);

};

extern LCDClass LCD;

#endif
