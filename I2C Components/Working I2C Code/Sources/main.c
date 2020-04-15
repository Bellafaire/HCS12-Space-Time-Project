#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define DISP_I2C_ADDR 0b11100000
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0

#define SEVENSEG_DIGITS 5

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

void MSDelay(unsigned int itime)
{
  unsigned int i; unsigned int j;
  for(i=0;i<itime;i++)
    for(j=0;j<4000;j++);
}

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

/* Starts I2C module as master */
void initI2C(void) 
{
  IBCR_IBEN = 1; // initalize I2C Bus
  IBFD = 0x1F;  // set up baud rate (100 kHz at 24 MHz bus frequency)
  // IBAD = i2c_id;   // sets I2C slave address
  IBCR_IBIE = 1; //disable I2C interrupt
  IBCR_IBSWAI = 1; // disable I2C in wait mode
    
}

/* Generates start condition for communication with requested slave */
void setSlaveID(char id)
{
  while (IBSR_IBB);  // wait until I2C bus is idle
  IBAD = id; // Set up slave address for I2C
  IBCR |= 0b00110000; // generate a start condiiton 
  IBDR = id; // Send slave address with R/W bit
  MSDelay(2);
  //while(!(IBSR_IBIF)); // wait for address transmition to complete
  IBSR_IBIF = 1; // clear IBIF flag
  
}

/* Checks to see if a byte transmission was successfully received by the slave
   Returns 0 if successful, -1 if slave did not adknowledge the data. */
char checkAck(void) 
{
  if (IBSR_RXAK)
    return -1;
  else
    return 0;
}

/* Sends a byte via I2C
   cx = byte to send */
void sendByteI2C(char cx) 
{
  IBDR = cx;                  // send byte
  MSDelay(2);
  //while(!(IBSR_IBIF)); // wait for bus to clear
  IBSR_IBIF = 1; // clear IBIF flag 
  
  //return checkAck();
}

/* Called prior to transmitting data
   id = I2C_ID of device to communicate with
   Generates start condition and sends slave ID */
void beginTransmit(char id) 
{
  setSlaveID(id); 
}

/* Called after the end of each communication string
   Generates the stop condition */
void endTransmit() 
{
  IBCR_MS_SL = 0; // generates stop condition 
  IBSR_IBIF = 1; 
}

/* Sets rate for whole device blinking
   Use one of the following defines
   HT16K33_BLINK_OFF 
   HT16K33_BLINK_2HZ
   HT16K33_BLINK_1HZ  
   HT16K33_BLINK_HALFHZ */
     
void blinkRate(unsigned char b) 
{
  beginTransmit(DISP_I2C_ADDR);
  if (b > 3) b = 0; // turn off if not sure
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
  endTransmit();  
}

/* Sets brightness of the display
   b = Value between 0 and 15 */ 
void setBrightness(unsigned char b) 
{
  if (b > 15) b = 15;
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | b);
  endTransmit();  
}

/* Initalize the display
   Call after initalizing I2C */
void initDisp(void) 
{
  DisableInterrupts;
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x21); // turn on oscillator)
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON);
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | 15);
  endTransmit();
  EnableInterrupts;
	  
}

/* Pass in the buffer (8 unsigned integers), it pushes to the LEDs
   Any of the below functions that modify the buffer need this function to be ran to actually see the changes.
*/

void writeDisp(unsigned int* buffer) {
  int i;
  DisableInterrupts;
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x00); // sets initial location
  
  for (i = 0; i < 8; i++) {
    unsigned int out = buffer[i];
    sendByteI2C(out & 0xFF);
    sendByteI2C(out >> 8);
  }
  endTransmit();
  EnableInterrupts;
}

/* toggles decimal points (1 turns on, 0 turns off)
   buffer = buffer to pass
   bit 0 - lower dot
   bit 1 - upper dot
   bit 2 - colon
   Ex: 0b101 = 0x5 = colon and lower dot on */
   
void toggleColon(unsigned int* buffer, char state) {
  buffer[2] = (0x02 * (state >> 2)) + (0x04 * (0x01 & state >> 1) + (0x08 * (0x01 & state)));
}

/* writes a single digit to the buffer at a given position
   buffer = self-explanatory
   pos = requested position (between 1 to 4, from left to right)
   d = data value from 0 to f (to turn the digit off, set 0x10 */

void writeDigit(unsigned int* buffer, char d, char pos) {
  unsigned int idx, val;
  if (pos > 4) return;
  idx = pos;
  if (pos <= 2) idx -= 1;
  
  if (d == 0x10) val = 0x00;
  else val = numbertable[d];
  
  buffer[idx] = val;
}

/* writes an entire 4 digit number to the buffer at once in decimal
   buffer = you need this
   d = data value from 0 to 9999 */
   
void writeNumDec(unsigned int* buffer, unsigned int d) {
  int parts[4];
  int i;
  parts[3] = d % 10;
  parts[2] = d % 100 / 10;
  parts[1] = d % 1000 / 100;
  parts[0] = d / 1000;
  
  for (i = 0; i < 4; i++) {
    writeDigit(buffer, parts[i], i+1);
  }
}

/* writes an entire 4 digit number to the buffer at once in hex
   buffer = you need this
   d = data value from 0 to FFFF */
   
void writeNumHex(unsigned int* buffer, unsigned int d) {
  int parts[4];
  int i;
  parts[3] = d % 0x10;
  parts[2] = d % 0x100 / 0x10;
  parts[1] = d % 0x1000 / 0x100;
  parts[0] = d / 0x1000;
  
  for (i = 0; i < 4; i++) {
    writeDigit(buffer, parts[i], i+1);
  }
}

/* writes an entire 4 digit number to the buffer at once from a string (no hex)
   buffer = you need this
   str = input string (only looks at first four characters, so formatting should not matter) */

void writeNumASCII(unsigned int* buffer, char* str) {
 int parts[4];
 int i;
 for (i = 0; i < 4; i++) {
  parts[i] = str[i] & 0x0F;
  writeDigit(buffer, parts[i], i+1);
 }
}
   


void main(void) {
  /* put your own code here */
  unsigned int buffer[8] = {
      0, 0, 0, 0, 0, 0, 0, 0
    };
    
  PLL_init();
  initI2C();
  MSDelay(10);
  initDisp();
  MSDelay(10);
  
  writeDisp(buffer);
  MSDelay(10);

	EnableInterrupts;
	
  for(;;) {
    
   //buffer[2] = 0x02;
    
    writeNumHex(buffer, 0xbeef);
    writeDisp(buffer);

    MSDelay(1000);
    
    writeNumASCII(buffer, "1234");
    toggleColon(buffer, 0x02);
    writeDisp(buffer);
    
    MSDelay(1000);
    
    
    
    /*buffer[2] = 0x2;
    writeDisp(buffer);
    MSDelay(1000);
    buffer[2] = 0x0;
    writeDisp(buffer);
    MSDelay(1000);  */
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
