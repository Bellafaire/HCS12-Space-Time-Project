

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
//void lightButton(int num, char brightness);
//void initButtons();