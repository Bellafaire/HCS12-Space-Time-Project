

/*     Declarations of pinouts for the spacetime project following the schematic included in the project repo     */


//button declarations
#define SW1 PTH_PTH3
#define SW2 PTH_PTH2
#define SW3 PTH_PTH1
#define SW4 PTH_PTH0

#define SW1_PINMODE DDRH_DDRH3
#define SW2_PINMODE DDRH_DDRH2
#define SW3_PINMODE DDRH_DDRH1
#define SW4_PINMODE DDRH_DDRH0

//button backlight declarations
/*#define SW1_PWM PTP_PTP0
#define SW2_PWM PTP_PTP1
#define SW3_PWM PTP_PTP2
#define SW4_PWM PTP_PTP3

#define SW1_PWM_PINMODE DDRP_DDRP0
#define SW2_PWM_PINMODE DDRP_DDRP1
#define SW3_PWM_PINMODE DDRP_DDRP2
#define SW4_PWM_PINMODE DDRP_DDRP3
*/




//buttons.c
void lightButton(int num, char brightness);
void initButtons(void);

/*//I2C declarations
#define DISP_I2C_ADDR 0x70
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0

#define SEVENSEG_DIGITS 5

//I2C.c
void initI2C(void);
void setSlaveID(char id);
char checkAck(void);
void sendByteI2C(char cx);
void beginTransmit(char id);
void endTransmit(void);
void blinkRate(unsigned char b); 
void setBrightness(unsigned char b);
void initDisp(void);
*/