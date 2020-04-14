#include "Declarations.h"     
#include "derivative.h"     

/* Port K
   RS         0
   EN         1
   DB4        2
   DB5        3
   DB6        4
   DB7        5
   RW         7
*/


void uSDelay(unsigned long constant) {
  volatile unsigned long counter;

  for(counter = constant * 50; counter > 0; counter--);
 }


/* sendLCD = sends data to the LCD
   data = 8 bit data to send
   type = 0 for command
          1 for data
*/

void sendLCD(unsigned char data, char rs) {
  unsigned char hi, lo;

  //split byte into 2 nibbles and shift to line up
  hi = ((data & 0xf0) >> 2) | (rs & 0x01) ;
  lo = ((data & 0x0f) << 2) | (rs & 0x01) ;

  /* do write pulses for upper, then lower nibbles */
  PORTK = hi; 
  PORTK = hi | 0x02; //  EN=1
  PORTK = hi; 
  PORTK = lo; 
  PORTK = lo | 0x02; // EN=1
  PORTK = lo; 

  uSDelay(40);
}

void clearLCD(void) {
  sendLCD(0x01, 0); 
}

void homeLCD(void) {
  sendLCD(0x02, 0); 
}

// Initializes on-board LCD
void initLCD(void) {
   /* initialise port */
   DDRK = 0xff;

   sendLCD(0x30, 0); // initalize
   uSDelay(4400);
   sendLCD(0x30, 0); 
   uSDelay(100);
   sendLCD(0x30, 0);  


  sendLCD(0x20, 0); // forces 4-bit mode

  sendLCD(0x28, 0); // last function set: 4-bit mode, 2 lines, 5x7 matrix
  sendLCD(0x0c, 0); // display on, cursor off, blink off
  clearLCD(); // display clear
  sendLCD(0x06, 0); // cursor auto-increment, disable display shift
}


/* Writes an entire line of an LCD
   str = input string
   l = requested line
     0 = top line
     1 = bottom line
*/

void writeLine(char *str, int l) {
	int i;
	
	// which line to select
	// C0 for bottom, 80 for upper
	sendLCD(((l == 1) ? 0xC0 : 0x80), 0);
	
	for( i = 0; i < 16; i++) {
		sendLCD( str[i], 1); // rs=1 means data
	}
}  

/* Moves the cursor to a specific location to write
   pos = cell to write in (1 to 16)
   l = line (0 or 1)
   
   0x80  0x81  0x82  0x83  0x84  0x85  ...  0x8F
   0xC0  0xC1  0xC2  0xC3  0xC4  0xC5  ...  0xCF  */
   

// TODO - cursor can only be set on first line.  Whenever you try to set it to a position on the second line, it was as if you were setting it on the first line
void setCursor(unsigned char l, unsigned char pos){
  unsigned char addr[] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF
  };
  unsigned char i = 0;
  if (l == 1) i = 0xC0 | pos;
  else i = 0x80 | pos;
  sendLCD(i, 0);
}

// Writes an ASCII character in the current location
void writeChar(char c) {
  sendLCD(c, 1);
}


