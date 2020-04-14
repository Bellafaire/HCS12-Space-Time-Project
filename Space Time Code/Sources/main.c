#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Declarations.h"

char displaybuffer[16]; 

char count = 0;
char count1 = 0; 
char i = 0;          

//initalization function (probably going to spin off other functions for UART and IIC)
//for now just handles buttons
void init(){
  initLCD();
  PLL_init();
  initUART2();
}


void main(void) {
  int a = 0; 

  DDRB = 0xFF;

  init();

	EnableInterrupts;
	
	writeLine("Hello world!", 0);
	//MSDelay(1000);
	writeLine("Testing attempt.", 1);
	
	MSDelay(1000);
	clearLCD();
	
	setCursor(1, 5);
  writeChar('n');
  writeChar('o');
  writeChar('t');
  
  homeLCD();
  writeChar('Y');
  writeChar('e');
  writeChar('s');
  
  for(;;) {
  // sendI2CDisplayCommand(dispAddr, 0x00,   0x3F);	
  // button test code, if a button is pressed this code just lights up that button
  // note how mapping in declarations.h allows us to read the buttons   
  // PORTB = 0xFF ^ PTH;
  homeLCD();
  for(a = 0; a < 16; a++){
       writeChar(getBufferCharacter(a));  
       displaybuffer[a] = 32;
    }
  for(a = 0; a < 6; a++){
      displaybuffer[a] = getTimeStringCharacter(a);
  }
  writeLine(displaybuffer, 1);
 }
}

