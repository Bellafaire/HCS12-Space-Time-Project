#include "derivative.h"
#include "Declarations.h"


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
  while(!(IBSR_TCF)); // wait for address transmition to complete
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
  while(!(IBSR_IBIF)); // wait for bus to clear
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
  sendByteI2C(0x21); // turn on oscillator)  endTransmit();
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON);
  endTransmit;
  
  beginTransmit(DISP_I2C_ADDR);
  sendByteI2C(HT16K33_CMD_BRIGHTNESS | 15);
  endTransmit();
  //blinkRate(HT16K33_BLINK_OFF);
  //setBrightness(15);	  
}





