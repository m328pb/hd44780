# hd44780

Library for LCD on hd44780 with I2C interface.
Assumption is to minimaze memory footprint with only basic
text display.

Inspiration come from fact that Adafruit_SSD1306 with
Arduino library takes ~2708 bytes

```bash
AVR Memory Usage
----------------
Device: Unknown

Program:     1254 bytes
(.text + .data + .bootloader)

Data:          2 bytes
(.data + .bss + .noinit)
```

## usage

Just set isplay size (lines and columns). Library implemented with class LCD():

- I2C::&get() - return communication protocol object
- I2C::clear(); - clear display
- I2C::setPos(uint8_t x, uint8_t y); - set cursor position
- I2C::println(const char *str); - print txt without moving the
cursor, so next print will overwrite
- I2C::print(const char *str); - print and move cursor
- I2C::off(); - display off
