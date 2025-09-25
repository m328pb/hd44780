#include "hd44780.h"

LCD::LCD() : comm(), x(0), y(0) { initialize(); }

void LCD::initialize() {
  comm.init(PCF8574_I2C_ADDRESS, I2C_SPEED);
  comm.send(0x00); // make sure EN is low (disabled)
  // proper power-up can not be guaranteed so
  // initializing by instruction (p.46 HD4780U datasheet)
  _delay_ms(50);
  send1byte(0, 0b11); // 8bit interface, lower bits are zero
  _delay_ms(5);
  send1byte(0, 0b11); // 8bit interface, lower bits are zero
  _delay_us(120);
  send1byte(0, 0b11); // 8bit interface, lower bits are zero
  send1byte(0, 0b10); // we should be in initialized now with 4 bits

  function_set();   // set 4 bits, and no of lines, fonts 5x8
  display(0b100);   // display on, cursor off, blinking off
  clear();          // clear and set pos to 0,0
  entry_mode(0b10); // cursor direction and shift
}
/*
 * @brief display text
 * @details
 * str must end with 0x0
 * shift cursor position, do not skip to next line
 */
void LCD::print(const char *str) {
  uint8_t x = this->x;
  do {
    if (x > NO_COLUMNS)
      break;
    send2byte(1, *str);
    x++;
  } while (*++str);
  _delay_us(50);
  this->x = x;
}

/*
 * @brief display text
 * @details
 * str must end with 0x0
 * DO NOT shift cursor position, do not skip to next line
 */
void LCD::println(const char *str) {
  uint8_t x = this->x;
  do {
    if (x > NO_COLUMNS)
      break;
    send2byte(1, *str);
    x++;
  } while (*++str);
  _delay_us(50);
  setPos(this->x, this->y);
}

/*
 * @brief set cursor position
 * @details
 * no shift or scroll, position possible only within limits
 * line is count from 0
 */
void LCD::setPos(uint8_t x, uint8_t y) {
  uint8_t adr;
  if (x >= NO_COLUMNS)
    x = NO_COLUMNS;
  if (y >= NO_LINES - 1)
    y = NO_LINES - 1;
  this->x = x;
  this->y = y;
  adr = y == 0 ? x : 0x40 + x;
  send2byte(0, 0b10000000 | adr);
  _delay_us(50);
}

void LCD::off() { display(0b000); }

/*
 * @brief set cursor move direction and shift
 * params ds - two bits: (d)direction, (s)hift
 * @details
 * Increments (d = 1) or decrements (d = 0) the DDRAM address
 * by 1 when a character code is written into or read from DDRAM.
 * s: Shifts the entire display either to the right (d = 0) or to
 * the left (d = 1) when S is 1. The display does not shift if S is 0.
 * see p.24
 */
void LCD::entry_mode(uint8_t ds) {
  send2byte(0, 0b0100 | ds);
  _delay_us(50);
}

void LCD::clear() {
  send2byte(0, 0b1);
  _delay_ms(2);
}

/*
 * @brief set display on/of, cursor visibility and blinking
 * params dcb - three bits: (d)isplay, (c)ursor, (b)link
 * @details
 * see p.24
 */
void LCD::display(uint8_t dcb) {
  send2byte(0, 0b00001000 | dcb);
  _delay_us(50);
}

/*
 * @brief function set, see p.24
 * @details
 * D7 D6 D5 D4 D3 D2 D1 D0
 * 0  0  1  DL N  F  -  -
 * DL=1 8bits, DL=0 4 bits
 * N=1 2 lines, N=0 1 line display
 * F=1 5x10, F=0 5x8 font size
 */
void LCD::function_set() {
  send2byte(0, 0b00100000 | (NO_LINES - 1) << 3);
  _delay_us(50);
}

/*
 * @brief compose two bytes from data and send to LCD
 * @params rs - register selection: 1 is data, 0 is cmd
 * @params data - 8 bits of data
 * @details
 * For 4-bit interface data, only four bus lines (DB4 to DB7)
 * are used for transfer. The data transfer between the HD44780U and
 * the MPU is completed after the 4-bit data has been transferred twice.
 * The four high order bits are transferred before the four low order bits.
 * simulate EN pin cycle
 */
void LCD::send2byte(uint8_t rs, uint8_t data) {
  uint8_t packet[6];
  // higher 4 bits
  packet[0] = rs | BL | (data & 0b11110000);      // EN=0 to start hd44780
  packet[1] = rs | EN | BL | (data & 0b11110000); // EN=1 to read hd44780
  packet[2] = rs | BL | (data & 0b11110000);      // EN=0 to stop hd44780

  // lower 4 bits
  packet[3] = rs | BL | data << 4;      // EN=0 to start hd44780
  packet[4] = rs | EN | BL | data << 4; // EN=1 to read hd44780
  packet[5] = rs | BL | data << 4;      // EN=0 to stop hd44780
  comm.send_ln(packet, 6);
}

void LCD::send1byte(uint8_t rs, uint8_t data) {
  uint8_t packet[3];
  // takes first 4bits only and set as higher
  packet[0] = rs | BL | data << 4;      // EN=0 to start hd44780
  packet[1] = rs | EN | BL | data << 4; // EN=1 to read hd44780
  packet[2] = rs | BL | data << 4;      // EN=0 to stop hd44780
  comm.send_ln(packet, 3);
}
