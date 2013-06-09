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
 *
 * This library was inspired to fatihinanc's library (info bit.ly/13P5Lkn).
 * When the display powers up, it is configured as follows:
 *
 * =================================
 * LCD pin              Connect to
 * ---------------------------------
 * 01 - GND             GND, pot
 * 02 - VCC             +5V, pot
 * 03 - Contrast        Pot wiper
 * 04 - RS              (PB_0)
 * 05 - R/W             GND
 * 06 - EN              (PB_1)
 * 07 - DB0             GND
 * 08 - DB1             GND
 * 09 - DB2             GND
 * 10 - DB3             GND
 * 11 - DB4             (PB_4)
 * 12 - DB5             (PB_5)
 * 13 - DB6             (PB_6)
 * 14 - DB7             (PB_7)
 * 15 - BL+             +5V
 * 16 - BL-             GND
 * ================================
 *
 * Note, however, that resetting the Stellarpad doesn't reset the LCD, so we
 * can't assume that its in that state when a sketch starts (and the
 * LCD init is called).
 */

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "Energia.h"
#include <inttypes.h>
#include "LCD.h"

regtype LCDClass::_port = 0;
regsize LCDClass::_bit = 0;
uint8_t LCDClass::_RS_pin = 0;
uint8_t LCDClass::_EN_pin = 0;
uint8_t LCDClass::_data_pins[4] = { 0 };

/******************************************************************************
 * Implementation
 ******************************************************************************/

LCDClass::LCDClass(void) {
}

void LCDClass::fast_w(uint8_t p, boolean state) {
	LCDClass::_port = portOutputRegister(digitalPinToPort(p));
	LCDClass::_bit = digitalPinToBitMask(p);
	state ? sbi(LCDClass::_port, LCDClass::_bit) : cbi(LCDClass::_port, LCDClass::_bit);
}

void LCDClass::rs(int i) {
	i ? LCDClass::fast_w(LCDClass::_RS_pin, HIGH) : LCDClass::fast_w(
				LCDClass::_RS_pin, LOW);
}

void LCDClass::en(int i) {
	i ? LCDClass::fast_w(LCDClass::_EN_pin, HIGH) : LCDClass::fast_w(
				LCDClass::_EN_pin, LOW);
}

void LCDClass::EN_Acknow() {
	LCDClass::en(1); // LCD EN -> 1 ;
	LCDClass::en(0); // LCD EN -> 0 ;
}

void LCDClass::up_w(unsigned char c) {
	for (uint8_t i = 0; i < 4; i++) {
		fast_w(LCDClass::_data_pins[i], c & (B00010000 << i)); // bit mask for DB4:7
	}
}

void LCDClass::down_w(unsigned char c) {
	for (uint8_t i = 0; i < 4; i++) {
		fast_w(LCDClass::_data_pins[i], c & (B00000001 << i)); // bit mask for DB4:7
	}
}

void LCDClass::command(unsigned char c) {
	LCDClass::rs(0); // LCD RS -> 0;
	delay(1);
	LCDClass::up_w(c);
	LCDClass::EN_Acknow();
	LCDClass::down_w(c);
	LCDClass::EN_Acknow();
}

void LCDClass::push(char c) {
	LCDClass::rs(1); // LCD RS 1;
	delay(1);
	LCDClass::up_w(c);
	LCDClass::EN_Acknow();
	LCDClass::down_w(c);
	LCDClass::EN_Acknow();
}

char* LCDClass::itoa(int val, int base) {
	static char buf[32] = { 0 };
	int i = 30;
	for (; val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i + 1];
}

void LCDClass::goTo(uint8_t x, uint8_t y) {
	if (x == 1)
		LCDClass::command(0x80 + ((y - 1) % 16));
	else
		LCDClass::command(0xC0 + ((y - 1) % 16));
}

void LCDClass::clear(void) {
	LCDClass::command(0x01);
	delay(5);
}

void LCDClass::print(int n) {
	if (n < (2.0e+15 - 1)) {
		LCDClass::print(LCDClass::itoa(n, 10));
	}
}

void LCDClass::print(char c) {
	LCDClass::push(c);
}

void LCDClass::print(const char* s) {
	int count = 0;
	while (*s && (count < 16)) {
		LCDClass::push(*s++);
		count++;
	}
}

void LCDClass::print(int n, uint8_t x, uint8_t y) {
	LCDClass::goTo(x, y);
	if (n < (2.0e+15 - 1)) {
		LCDClass::print(LCDClass::itoa(n, 10));
	}
}

void LCDClass::print(char c, uint8_t x, uint8_t y) {
	LCDClass::goTo(x, y);
	LCDClass::push(c);
}

void LCDClass::print(const char* s, uint8_t x, uint8_t y) {
	LCDClass::goTo(x, y);
	int count = 0;
	while (*s && (count < 16)) {
		LCDClass::push(*s++);
		count++;
	}
}

void LCDClass::scrollLeft(String text, uint8_t row, int ms) {
	int dim = (text.length() - 16);
	String crop = "";
	for (int i = 0; i <= dim; i++) {
		LCDClass::goTo(row, 1);
		crop = text.substring(i, 16 + i);
		LCDClass::print(&crop[0]);
		delay(ms);
	}
}

void LCDClass::scrollRight(String text, uint8_t row, int ms) {
	int dim = (text.length() - 16);
	String crop = "";
	for (int i = dim; i >= 0; i--) {
		LCDClass::goTo(row, 1);
		crop = text.substring(i, 16 + i);
		LCDClass::print(&crop[0]);
		delay(ms);
	}
}

void LCDClass::init(int inRS, int inEN, int inDB4, int inDB5, int inDB6,
		int inDB7) {
	LCDClass::_RS_pin = inRS; // RS
	LCDClass::_EN_pin = inEN; // EN
	LCDClass::_data_pins[0] = inDB4; // DB4
	LCDClass::_data_pins[1] = inDB5; // DB5
	LCDClass::_data_pins[2] = inDB6; // DB6
	LCDClass::_data_pins[3] = inDB7; // DB7

	pinMode(LCDClass::_RS_pin, OUTPUT);
	pinMode(LCDClass::_EN_pin, OUTPUT);

	for (uint8_t i = 0; i < 4; i++) {
		pinMode(LCDClass::_data_pins[i], OUTPUT); // DB4:7
	}

	fast_w(LCDClass::_RS_pin, LOW); // LCD RS -> 0;
	fast_w(LCDClass::_EN_pin, LOW); // LCD EN -> 0;

	LCDClass::command(0x28); // Set 4 Bit, double line LCD
	LCDClass::command(0x0C); // cursor hiding
	LCDClass::command(0x06); // activation of clockwise writing
	LCDClass::command(0x80); // LCD first row position
	LCDClass::command(0x28); // Set 4 Bit, double line LCD
	LCDClass::clear(); // LCD clear
}

LCDClass LCD;

