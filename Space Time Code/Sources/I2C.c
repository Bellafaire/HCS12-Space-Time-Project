/*
   I2C interfacing functions for the LED displays.
*/ 

#include "derivative.h" 

//inits the I2C interface on pins PJ6 (SDA) and PJ7 (SCL)
//external pullups on I2C lines are absolutely required for the interface to function
void initI2C(){

  //all registers will be configured here and their function noted
  //this is mostly just in case we need to change something later, should save 
  //some trouble
  
  //IIC Bus Address Register, this sets the HCS12's slave address when operating
  //as a slave device. Since all our displays work exclusively as slaves we don't have
  //to worry about this at all. But if you want to use the HCS12 as a slave device, this would be its address
  IBAD = 1; 
  
  //IIC Frequency Divider Register, determines the frequency divider of the IIC interface from the bus clock
  IBFD = 0x3F; 
  
  
  //IIC Control Register, since this one's more important its broken out to individual bits
  IBCR_IBEN = 1;             //IIC Bus Enable Bit (1 = IIC Enabled)
  IBCR_IBIE = 0;             //IIC Interrupt enable (1 = interrupt enabled) 
  IBCR_MS_SL = 0;            //IIC Master/Slave select (1 = master 0 = slave) NOTE: on transistion from 0 to 1 this generates a start signal
  IBCR_TX_RX = 1;            //IIC Transmit/recieve mode select bit (1 = transmit 0 = recieve)
  IBCR_TXAK = 0;             //IIC Acknowledge enable (0 = acknowledge sent 1= no acknowledge) (i know it seems backwards)
  IBCR_RSTA = 0;             //IIC repeat start (takes control of the i2c bus essentially)
  IBCR_IBSWAI = 0;           //IIC stop in wait mode (0 = no stop in wait mode) 
  
  //I2C Status Register
  //IBSR = 0;                //We don't write to this one at all but i wanted it listed in this file
  
  //I2C DataIO Register
  //IBDR = 0;                //data register.(also don't write to) 
  
}

/*Since the display we use for the project is the adafruit 7seg I2C display. 
In order to write to the display we generally give a command and the command's data
this function is intended to simplify interfacing with the 7 segment display of the 
project. Interface driver for the display is the HT16K33V110 datasheet found https://cdn-shop.adafruit.com/datasheets/ht16K33v110.pdf */
void sendI2CDisplayCommand(unsigned char addr, unsigned char command,unsigned char data){
  while(IBSR_IBB){
  }
  
  IBCR_IBEN = 1; //enable IIC 
  IBCR_TX_RX = 1; //set to transmit mode
  IBCR_MS_SL = 1; //set to master mode, the transition from 0 to 1 will begin the IIC Transmission
 
  //load data register with slave address IIC Bit format is (A7 A6....A1 A0 R/W) we only write in this case so LSB is always zero
  IBDR = (addr<<1);   
  while(!IBSR_TCF){
    // wait until data transfer is complete by polling TCF (Data Transffering bit) in the status register
  }
  

  //load the data register with the command  
  IBDR = command;
  while(!IBSR_TCF){
    // wait until data transfer is complete by polling TCF (Data Transffering bit) in the status register
  }
  
  //load the data register with the data  
  IBDR = data;
  while(!IBSR_TCF){
     // wait until data transfer is complete by polling TCF (Data Transffering bit) in the status register
   }
  
  //generate stop condition by setting IBCR_MS_SL to 0 
  IBCR_MS_SL = 0;
  IBCR_IBEN = 0;    //disable I2C
}

