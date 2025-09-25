//minimal arduino sketch for memory usage comparision

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Common I2C backpack addresses are 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();          // initialize LCD
  lcd.backlight();     // turn on backlight
  lcd.print("Hello, world!");
}

void loop() {
  // nothing needed here
}
