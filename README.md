# hd44780

Library for LCD on hd44780 with I2C interface.
Assumption is to minimaze memory footprint with only basic
text display.

Inspiration come from fact that Arduino library with 
typical LCD lib takes ~2708 bytes

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

- LCD::&get() - return communication protocol object
- LCD::clear(); - clear display
- LCD::setPos(uint8_t x, uint8_t y); - set cursor position
- LCD::println(const char *str); - print txt without moving the
cursor, so next print will overwrite
- LCD::print(const char *str); - print and move cursor
- LCD::off(); - display off
