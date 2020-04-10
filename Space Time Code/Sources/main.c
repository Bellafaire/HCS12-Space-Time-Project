#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "declarations.h"



char count = 0;

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
  
}



void main(void) {
  int a = 0; 


  init();

	EnableInterrupts;



  for(;;) {

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
