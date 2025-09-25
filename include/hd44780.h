#pragma once
#include "i2c.h"
#include <util/delay.h>

#define PCF8574_I2C_ADDRESS 0x27 // I2C driver address

#define NO_LINES 2    // number of lines on LCD
#define NO_COLUMNS 16 // number of columns
		      // always x8 font size

// PCF8574 shift bit as follow
// PCF8574: p7 p6 p5 p4 p3 p2 p1 p0
// HD44780: d7 d6 d5 d4 BL EN RW RS
// RS - is register select; command when 0 or data when 1
// RW=0(only write), BL=1 (backlight on)
// EN=1 hd44780 read byte, EN=0 pause between bytes
#define EN 1 << 2
#define BL 1 << 3

class LCD {
private:
  I2C comm;
  uint8_t x;
  uint8_t y;
  I2C::error err;
  void initialize();
  void send2byte(uint8_t rs, uint8_t data);
  void send1byte(uint8_t rs, uint8_t data);
  void function_set();
  void display(uint8_t dcb);
  void entry_mode(uint8_t ds);

public:
  I2C &get() { return comm; };
  void clear();
  void setPos(uint8_t x, uint8_t y);
  void println(const char *str);
  void print(const char *str);
  void off();
  LCD();
};
