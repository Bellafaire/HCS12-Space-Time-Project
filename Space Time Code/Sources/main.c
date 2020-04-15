#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Declarations.h"

char displaybuffer[16];
unsigned int segbuffer[8]; 

char count = 0;
char count1 = 0; 
char i = 0;          

//initalization function
void init(){
  initLCD();
  PLL_init();
  initUART2();
  
  // Initalization process for I2C
  initI2C();
  MSDelay(10);
  initDisp();
  MSDelay(10);
  
  // Clear noise on 7-seg
  clearDisp(segbuffer);
  writeDisp(segbuffer);
  
}


void main(void) {
  int a = 0;
  char pips = 0x04; 

  DDRB = 0xFF;

  init();

	EnableInterrupts;
	
	writeLine("SpaceTime              ", 0);
	writeLine("Final Project          ", 1);
	
	MSDelay(1500);
	clearLCD();
	
  for(;;) {
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
  
  // writes the current time to the 7-seg display
  writeNumASCII(segbuffer, displaybuffer); 
  
  /* TODO: Weird noise here. Supposed to light the upper pip if PM, but after some glitches it turns on even at 1am
  // Leaving out for now, may be cool feature though.
  if (displaybuffer[6] == 'P') {
    pips |= 0x02;
  } */ 
  toggleColon(segbuffer, pips);
  
  // Error surpression - makes sure time is valid before sending to 7 seg
  //if (segbuffer[0] <= 1)  //I'm not sure why but this doesn't play well with the output validation of GPS.c, caused an issue where time wouldn't display at all at 10am 
   writeDisp(segbuffer);
  
  MSDelay(250);
  
  
 }
}

