#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "declarations.h"

 static const char numbertable[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* a */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};

unsigned int  displaybuffer[8]; 

#define dispAddr 0x70

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
  initButtons();
  initI2C();
}


void main(void) {
  int a = 0; 

  DDRB = 0xFF;

  init();

	EnableInterrupts;
  
  for(;;) {
   sendI2CDisplayCommand(dispAddr, 0x00,   0x3F);	
  //button test code, if a button is pressed this code just lights up that button
  //note how mapping in declarations.h allows us to read the buttons   
    if(!SW1){
      lightButton(1, 255);   
    } else{
      lightButton(1, 0); 
    }
    if(!SW2){
      lightButton(2, 255);   
    } else{
      lightButton(2, 0); 
    }
    
    if(!SW3){
      lightButton(3, 255);   
    } else{
      lightButton(3, 0); 
    }
    if(!SW4){
      lightButton(4, 255);   
    } else{
      lightButton(4, 0); 
    }

  }
}
