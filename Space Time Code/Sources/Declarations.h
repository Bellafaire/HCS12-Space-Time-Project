

/*     Declarations of pinouts for the spacetime project following the schematic included in the project repo     */


//button declarations
#define SW1 PORTE_BIT3
#define SW2 PORTE_BIT2
#define SW3 PORTE_BIT1
#define SW4 PORTE_BIT0

#define SW1_PINMODE DDRE_BIT3
#define SW2_PINMODE DDRE_BIT2
#define SW3_PINMODE DDRE_BIT1
#define SW4_PINMODE DDRE_BIT0

//button backlight declarations
#define SW1_PWM PTP_PTP0
#define SW2_PWM PTP_PTP1
#define SW3_PWM PTP_PTP2
#define SW4_PWM PTP_PTP3

#define SW1_PWM_PINMODE DDRP_DDRP0
#define SW2_PWM_PINMODE DDRP_DDRP1
#define SW3_PWM_PINMODE DDRP_DDRP2
#define SW4_PWM_PINMODE DDRP_DDRP3


