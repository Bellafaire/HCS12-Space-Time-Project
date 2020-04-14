#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

//I2C declarations
#define DISP_I2C_ADDR 0x70
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

unsigned int buffer[5] = {
  numbertable[0],
  numbertable[1],
  0x02,
  numbertable[2],
  numbertable[3]
};


void initI2C(void) {
 IBFD = 0x3F;
 IBAD = 0x01;
 IBCR_IBEN = 1;
 
 IBCR_IBIE = 0;
 IBCR_IBSWAI = 0;
 IBCR_TX_RX = 1;  
}

void sendSlaveID(char cx) 
{
  while (IBSR_IBB);
  IBCR_MS_SL = 1;
  IBDR = cx;
  while (!(IBSR_IBIF));
  IBSR_IBIF = 1;
  
    
}

char openDisp(char ctrl) {
  sendSlaveID(0b01110000);
  if (IBSR_RXAK) return -1;
  IBDR = 0x21;
  while (!(IBSR_IBIF));
  IBSR_IBIF = 1;
  if (IBSR_RXAK) return -1;
  //IBDR = ctrl;
  //while (!(IBSR_IBIF));
  //IBSR_IBIF = 1;
  //if (IBSR_RXAK) return -1;
  IBCR_MS_SL = 0;
  return 0;
   
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

void led_init(void) 
{
    DDRJ |= 0x02;
    PTJ = 0x00;
    DDRB |= 0xFF;
    PORTB |= 0x00;
    DDRP |= 0x0F;
    PTP |= 0x0F;
}

void led_write(unsigned char state) 
{
    PORTB = state;  
}

void main(void) {

  

  unsigned char i = 0;
  
  EnableInterrupts;
  PLL_init();
  led_init();
  initI2C();
  
  i = (unsigned char) openDisp(0x90);
  led_write(i);
  
  /*
  IBCR_IBEN = 1; // initalize I2C Bus
  IBFD = 0x1F;  // set up baud rate (100 kHz at 24 MHz bus frequency)
  // IBAD = i2c_id;   // sets I2C slave address
  IBCR_IBIE = 1; //disable I2C interrupt
  IBCR_IBSWAI = 1; // disable I2C in wait mode
  
  while (IBSR_IBB);
  IBCR |= (0b00100000 + 0b00010000);
  IBDR = 0x70;
  while (!(IBSR_IBIF));
  IBSR_IBIF = 1;
  
  IBDR = 0x21;
  while(!(IBSR_IBIF));
  IBSR_IBIF = 1;
  IBCR_MS_SL = 0; // generates stop condition
  
  if (IBSR_RXAK) i = -1;   */
  
  //i = IBSR_RXAK;
  /*while (IBSR_IBB);  // wait until I2C bus is idle
  IBCR |= 0b00110000; // generate a start condiiton 
  IBDR = DISP_I2C_ADDR; // Send slave address with R/W bit
  while(!(IBSR_IBIF)); // wait for address transmition to complete
  IBSR_IBIF = 1; // clear IBIF flag 
  
  if (IBSR_RXAK) i = -1; 
  
  IBDR = 0x21;
  while(!(IBSR_IBIF));
  IBSR_IBIF = 1;
  IBCR_MS_SL = 0; // generates stop condition
  
  if (IBSR_RXAK) i = -1; 
  
  while (IBSR_IBB);  // wait until I2C bus is idle
  IBCR |= 0b00110000; // generate a start condiiton 
  IBDR = DISP_I2C_ADDR; // Send slave address with R/W bit
  while(!(IBSR_IBIF)); // wait for address transmition to complete
  IBSR_IBIF = 1; // clear IBIF flag 
  
  if (IBSR_RXAK) i = -1; 
  
  IBDR = HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (HT16K33_BLINK_OFF << 1);
  while(!(IBSR_IBIF));
  IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1; 
  
  while (IBSR_IBB);  // wait until I2C bus is idle
  IBCR |= 0b00110000; // generate a start condiiton 
  IBDR = DISP_I2C_ADDR; // Send slave address with R/W bit
  while(!(IBSR_IBIF)); // wait for address transmition to complete
  IBSR_IBIF = 1; // clear IBIF flag 
  
  if (IBSR_RXAK) i = -1; 
  
  IBDR = HT16K33_CMD_BRIGHTNESS | 15;
  while(!(IBSR_IBIF));
  IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1; 
  
  
   */
  
  



  for(;;) {
    _FEED_COP(); /* feeds the dog */
    /*while (IBSR_IBB);  // wait until I2C bus is idle
  IBCR |= 0b00110000; // generate a start condiiton 
  IBDR = DISP_I2C_ADDR; // Send slave address with R/W bit
  while(!(IBSR_IBIF)); // wait for address transmition to complete
  IBSR_IBIF = 1; // clear IBIF flag 
  
  if (IBSR_RXAK) i = -1;
  
  // send address to start
  IBDR = 0x00;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  // send buffer
  IBDR = buffer[0] & 0xFF;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[0] >> 8;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[1] & 0xFF;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[1] >> 8;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[2] & 0xFF;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[2] >> 8;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[3] & 0xFF;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[3] >> 8;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[4] & 0xFF;
  while(!(IBSR_IBIF));
  //IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;
  
  IBDR = buffer[4] >> 8;
  while(!(IBSR_IBIF));
  IBCR_MS_SL = 0; // generates stop condition
  IBSR_IBIF = 1;
  
  if (IBSR_RXAK) i = -1;     */
  }               
}/* loop forever */
  /* please make sure that you never leave main */

