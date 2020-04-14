#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Declarations.h"



unsigned int  displaybuffer[8]; 

char count = 0;
char count1 = 0; 
char i = 0; 

 void MSDelay(unsigned int itime)
  {
    unsigned int i; unsigned int j;
    for(i=0;i<itime;i++)
      for(j=0;j<4000;j++);
  }


//initalization function (probably going to spin off other functions for UART and IIC)
//for now just handles buttons
void init(){

}


void main(void) {
  int a = 0; 

  DDRB = 0xFF;

  init();

	EnableInterrupts;
	
	
  
  for(;;) {
  // sendI2CDisplayCommand(dispAddr, 0x00,   0x3F);	
  //button test code, if a button is pressed this code just lights up that button
  //note how mapping in declarations.h allows us to read the buttons   
  PORTB = 0xFF ^ PTH;

  }
}
