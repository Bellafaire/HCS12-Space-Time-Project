#include "Declarations.h" 
#include "derivative.h"      /* derivative-specific definitions */

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
  disableInterrupts();
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x21); // turn on oscillator)
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON);
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | 15);
  endTransmit();
  enableInterrupts();
	  
}

/* Pass in the buffer (8 unsigned integers), it pushes to the LEDs
   Any of the below functions that modify the buffer need this function to be ran to actually see the changes.
*/

void writeDisp(unsigned int* buffer) {
  int i;
  disableInterrupts();
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x00); // sets initial location
  
  for (i = 0; i < 8; i++) {
    unsigned int out = buffer[i];
    sendByteI2C(out & 0xFF);
    sendByteI2C(out >> 8);
  }
  endTransmit();
  enableInterrupts();
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
   If 00:XX is input, 12:XX will be output
   Supresses leading zero too.
   buffer = you need this
   str = input string (only looks at first four characters, so formatting should not matter) */

void writeNumASCII(unsigned int* buffer, char* str) {
 int parts[4];
 int i;
 for (i = 0; i < 4; i++) {
  parts[i] = str[i] & 0x0F;
 }
 
 if (parts[0] == 0 && parts[1] == 0) {    // 12:00am correction
   parts[0] = 1;
   parts[1] = 2;
 } else if (parts[0] == 0) {
   parts[0] = 0x10;
 }
 
 for (i = 0; i < 4; i++) {
  writeDigit(buffer, parts[i], i+1);
 }
}

/* clears an input buffer - useful for initialization and if you need to turn it off for whatever reason */
void clearDisp(unsigned int* buffer) {
  int i;
  for (i = 0; i < 8; i++) buffer[i] = 0;  
}
   