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
	
	writeLine("SpaceTime              ", 0);
	writeLine("Final Project          ", 1);
	
	MSDelay(1500);
	clearLCD();
	
  for(;;) {
  // sendI2CDisplayCommand(dispAddr, 0x00,   0x3F);	
  // button test code, if a button is pressed this code just lights up that button
  // note how mapping in declarations.h allows us to read the buttons   
  // PORTB = 0xFF ^ PTH;
  homeLCD();
  
  //move of a debugging line, allows us to see the GPS data as it is recieved live
  for(a = 0; a < 16; a++){
       writeChar(getBufferCharacter(a));  
       displaybuffer[a] = 32;     //clearing the display buffer for the second line
    }
     
  //write the current time to the display buffer character-wise 
  for(a = 0; a < 8; a++){
     displaybuffer[a] = getCurrentTimeCharacter(a);
  }
  
  //write the timeString to the display buffere character-wise
  for(a = 0; a < 6; a++){
     displaybuffer[a + 10] = getTimeStringCharacter(a);
  }
  
  //print display buffer to second line of the display 
  writeLine(displaybuffer, 1);
 }
}

