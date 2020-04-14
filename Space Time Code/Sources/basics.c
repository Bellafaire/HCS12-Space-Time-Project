#include "Declarations.h"     
#include "derivative.h"     


void MSDelay(unsigned int itime)
{
  unsigned int i; unsigned int j;
  for(i=0;i<itime;i++)
    for(j=0;j<4000;j++);
} 

// Sets PLL to output a 48 MHz clock = 24 MHz bus frequency
void PLL_init(void) 
{
    // PLLCLK= 2 * OSCCLK * {[SYNR + 1] / [REFDV + 1]}
    // For 48 MHz, 2 * 8 MHz * (6 / 2)
    SYNR = 5;
    REFDV = 1;
    PLLCTL = 0x60;
    while(!(CRGFLG & 0x08));
    CLKSEL = 0x80; 
} 

// Initalizes the LEDs for output
void led_init(void) 
{
    DDRJ |= 0x02;
    PTJ = 0x00;
    DDRB |= 0xFF;
    PORTB |= 0x00;
    DDRP |= 0x0F;
    PTP |= 0x0F;
}

// Quick function to output values to the LEDs
void led_write(unsigned char state) 
{
    PORTB = state;  
}

// Function that inputs a number less than 1000 and outputs that number in the first 3 entries of an array
// Ascii representation = ready for output to LCD
void bin2ascii(unsigned int data, char out[]) 
{
    int x = data / 10;
    int preOut[3];
    int i = 0;
    
    preOut[2] = (data % 10);
    preOut[1] = x % 10;
    preOut[0] = x / 10;
    
    for (i = 0; i < 3; i++) {
        out[i] = (char) (preOut[i]) | 0x30;
    }
    
    out[3] = 0;
    
    
}

