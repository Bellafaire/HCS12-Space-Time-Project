/*
   
   buttons and button lighting is encapsulated here
   initButtons() needs to be called before anything works properly out of 
   this .c file.

*/ 
//it pains me but code warrior demands that i import these files AGAIN, so whatever
#include "Declarations.h"     
#include "derivative.h"       
     
void initButtons(){
  //init buttons as input
  //declarations.h maps to pinout listed in schematic
  SW1_PINMODE = 0; 
  SW2_PINMODE  = 0; 
  SW3_PINMODE  = 0;
  SW4_PINMODE  = 0;
  
  //set pwm driving pins as outputs (temporary code)
//  SW1_PWM_PINMODE = 1; 
//  SW2_PWM_PINMODE = 1; 
//  SW3_PWM_PINMODE = 1; 
//  SW4_PWM_PINMODE = 1; 

  PWME |= 0b00001111; //enable pwm on channels 0-3
  PWMPOL |= 0b00001111; //set PWM on channels 0-3 to be high on counter start
  
  PWMDTY0 = 255;
   PWMDTY1 = 255;
    PWMDTY2 = 255;
     PWMDTY3 = 255;
  //we're not using any prescalers here, there's no real point since we're just lighting up some LEDS
}


//simple function with an almost identical function to analogWrite() with the exception
//that it only supports input numbers 1 to 4 and is mostly a cheap and easy way to PWM
//the basic idea here is to encapsulate everything so there's no confusion between group members 
//about what's doing what
void lightButton(int num, char brightness){
    switch(num){
      case 1: 
        PWMDTY0 = brightness;  
      break;
      case 2: 
          PWMDTY1 = brightness; 
      break;
      case 3: 
           PWMDTY2 = brightness; 
      break;
      case 4: 
      PWMDTY3 = brightness; 
      break;
      //oh look no default
    }
}



