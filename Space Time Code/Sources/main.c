#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "declarations.h"



//initalization function (probably going to spin off other functions for UART and IIC)
//for now just handles buttons
void init(){

  //init buttons as input
  //declarations.h maps to pinout listed in schematic
  SW1 = 0; 
  SW2 = 0; 
  SW3 = 0;
  SW4 = 0;
  
  //set pwm driving pins as outputs (temporary code)
  SW1_PWM_PINMODE = 1; 
  SW2_PWM_PINMODE = 1; 
  SW3_PWM_PINMODE = 1; 
  SW4_PWM_PINMODE = 1; 
  
  SW1_PWM = 1;  
}




void main(void) {

  init();

	EnableInterrupts;


  for(;;) {
    _FEED_COP();
  }
}
