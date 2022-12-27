
#include <miniOled.h>

void setup(){

	delay(700);

	Oled.init();  // initialze OLED display
	Oled.wideFont = true;
	Oled.chrSpace=3;
	Oled.drawLine(3, 0xe0); // 0xe0 is the pattern repeated for line, try also 0xa0 0xb7 0xac 0xaf 0xba 0xa4 0xa6 0xa7 0xd7 0x95 0x97 0x9c 0x5e 0x7e
	Oled.printString("Hello",0,0); // Print Hello starting from row 0 column 0
	Oled.printInt(F_CPU/1000000,0,5); // Print CPU frequency starting from row 0 column 5
	Oled.printString("MHz",0,6);
	Oled.wideFont = false;
	Oled.chrSpace=1;
	Oled.printString("Hello Brian",0,2); 
	delay(1000);
	// Oled.clearDisplay(); // cleat all the screen content
	Oled.printBigNumber("-31",6,4);
	Oled.printBigNumber("9",12,0);
	// delay(1000);
	// Oled.printBigNumber("8",9,4); 
	// Oled.printBigNumber("5",12,4);
	delay(1000);
	Oled.printBigNumber("   ", 6,4);  // clear those positions
	Oled.printBigNumber(62, 6,4); 
	// Oled.setPowerOff(); // power off display
    // Oled.setPowerOn(); // power on display



}

void loop(){
  
}

