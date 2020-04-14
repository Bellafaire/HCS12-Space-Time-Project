#include <hidef.h>   
#include "derivative.h"  

//who needs ram? we need strings!
char serialBuffer[255];
long counter = 0; 
int bufferPosition = 0; 

/* ISR triggered when the input register of SCI1 is full. All we do is take the value
from the input buffer and put it into the serialBuffer variable.     */
interrupt ((0x10000-Vsci1)/2-1) void SCI1_ISR(void) { 
    int a = SCI1SR1;
    serialBuffer[bufferPosition] = SCI1DRL;
    if(serialBuffer[bufferPosition] == 13){
      bufferPosition = 0; 
    }else{
    if(bufferPosition < 255 - 1){
      bufferPosition++;    
    } 
    }
  
}


//inits UART on the second UART port, configures the uart output to a baud rate of 9600 and enables the interrupt
//to allow for us to buffer incomming data. 
void initUART2(void){
 SCI1BDH = 0x00;
 SCI1BDL = 156; 
 
 SCI1CR1 = 0x00; //8 bit data frame, no parity bit, no loop; 
 
 SCI1CR2 |= (1 << 2); //Recieve Enable 
 SCI1CR2 |= (1 << 5); //Reciever Full Interrupt Enable 
 __asm CLI; //enable global interrupts
}

char getBufferCharacter(int index){
  return serialBuffer[index]; 
}