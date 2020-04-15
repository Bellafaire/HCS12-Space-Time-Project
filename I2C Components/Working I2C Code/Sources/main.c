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

void initI2C(void) 
{
  IBCR_IBEN = 1; // initalize I2C Bus
  IBFD = 0x1F;  // set up baud rate (100 kHz at 24 MHz bus frequency)
  // IBAD = i2c_id;   // sets I2C slave address
  IBCR_IBIE = 1; //disable I2C interrupt
  IBCR_IBSWAI = 1; // disable I2C in wait mode
    
}

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

char checkAck(void) 
{
  if (IBSR_RXAK)
    return -1;
  else
    return 0;
}

void sendByteI2C(char cx) 
{
  IBDR = cx;                  // send byte
  MSDelay(2);
  //while(!(IBSR_IBIF)); // wait for bus to clear
  IBSR_IBIF = 1; // clear IBIF flag 
  
  //return checkAck();
}

void beginTransmit(char id) 
{
  setSlaveID(id); 
}

void endTransmit() 
{
  IBCR_MS_SL = 0; // generates stop condition 
  IBSR_IBIF = 1; 
}

void blinkRate(unsigned char b) 
{
  beginTransmit(DISP_I2C_ADDR);
  if (b > 3) b = 0; // turn off if not sure
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
  endTransmit();  
}

void setBrightness(unsigned char b) 
{
  if (b > 15) b = 15;
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | b);
  endTransmit();  
}

void initDisp(void) 
{
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x21); // turn on oscillator)
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON);
  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | 15);
  endTransmit();
	  
}

void writeDisp(unsigned int* buffer) {
  int i;
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(0x00); // sets initial location
  
  for (i = 0; i < 8; i++) {
    unsigned int out = buffer[i];
    sendByteI2C(out & 0xFF);
    sendByteI2C(out >> 8);
  }
  endTransmit();
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

	//EnableInterrupts;

    while(1) {
      
    buffer[0] = numbertable[0xb];
    buffer[1] = numbertable[0xe];
    buffer[3] = numbertable[0xe];
    buffer[4] = numbertable[0xf];
    writeDisp(buffer);

    MSDelay(1000);
    
    buffer[0] = numbertable[0xf];
    buffer[1] = numbertable[0xe];
    buffer[3] = numbertable[0xe];
    buffer[4] = numbertable[0xf];
    writeDisp(buffer);
    
    MSDelay(1000);  
    }
  for(;;) {
    
   //buffer[2] = 0x02;
    
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
