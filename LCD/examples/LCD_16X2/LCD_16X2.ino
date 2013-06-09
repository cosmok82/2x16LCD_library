// Core library - MCU-based
#if defined(__LM4F120H5QR__) // StellarPad specific
#include "Energia.h"
#else
#error Platform not supported
#endif

/*
 Demonstrates the use a 16x2 LCD display. The LCD library
 works with all LCD displays that are compatible with
 the Hitachi HD44780 driver (like JDH162A model).
 This library are configurated to work in 4 bit mode.
 Following the wire connections to do this configuration.
 
 The circuit:
 =================================
 LCD pin              Connect to
 ---------------------------------
 01 - GND             GND, pot
 02 - VCC             +5V, pot
 03 - Contrast        Pot wiper
 04 - RS              (PB_0)
 05 - R/W             GND
 06 - EN              (PB_1)
 07 - DB0             GND
 08 - DB1             GND
 09 - DB2             GND
 10 - DB3             GND
 11 - DB4             (PB_4)
 12 - DB5             (PB_5)
 13 - DB6             (PB_6)
 14 - DB7             (PB_7)
 15 - BL+             +5V
 16 - BL-             GND
 =================================
 
 For more info:
 http://creativityslashdesign.tk/
 */

#include <LCD.h>

void setup() {
  LCD.init(PB_0, PB_1, PB_4, PB_5, PB_6, PB_7);
  LCD.print("- Demo JDH162A -");
  LCD.scrollLeft("CreativitySlashDesign .TK", 2, 200);
  delay(1000);
  LCD.clear();
}

void loop() {
  LCD.clear();
  LCD.print("Example 1", 1, 3);
  delay(500);
  LCD.print("floating...", 2, 3);
  
  delay(1000);
  
  LCD.clear();
  LCD.scrollLeft("Nel mezzo del cammin di nostra vita...", 1, 200);
  delay(1000);
  LCD.scrollRight("Nel mezzo del cammin di nostra vita...", 1, 200);
  delay(1000);
  
  LCD.clear();
  LCD.print("Example 2", 1, 3);
  delay(500);
  LCD.print("print number", 2, 3);
  
  delay(1000);
  
  LCD.clear();
  LCD.print('*', 1, 3);
  LCD.print(1023);
  LCD.print('*');
  delay(1000);
  
  LCD.clear();
  LCD.print("Example 3", 1, 3);
  delay(500);
  LCD.print("num. increasing", 2, 1);
  
  delay(1000);
  
  LCD.clear();
  for(int i=0; i<100; i++){
    LCD.print(i, 1, 1);
    delay(200);
  }
  for(int i=99; i>0; i--){
    LCD.print(i, 2, 15);
    delay(200);
  }
}

