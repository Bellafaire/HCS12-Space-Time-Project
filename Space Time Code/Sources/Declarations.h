

/*     Declarations of pinouts for the spacetime project following the schematic included in the project repo     */

#define enableInterrupts() _asm(" cli")
#define disableInterrupts() _asm(" sei")


//button declarations
#define SW1 PTH_PTH3
#define SW2 PTH_PTH2
#define SW3 PTH_PTH1
#define SW4 PTH_PTH0

#define SW1_PINMODE DDRH_DDRH3
#define SW2_PINMODE DDRH_DDRH2
#define SW3_PINMODE DDRH_DDRH1
#define SW4_PINMODE DDRH_DDRH0

// I2C declarations
#define DISP_I2C_ADDR 0b11100000
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0xE0


