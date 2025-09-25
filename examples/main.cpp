/*
 simply routine to check and demonstarte the library
  - plug in network analyzer
  - check recived text with network analyzer
 */
#include "hd44780.h"

int main(){
	LCD display;
	display.print("hello");
	_delay_ms(1000);
	display.print(" world");
	display.setPos(2, 2);
	display.println("2nd line");
	_delay_ms(1000);
	display.print("very long line which exceed counter");

}
