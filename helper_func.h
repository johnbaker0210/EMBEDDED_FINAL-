#include <msp430.h>
#include "LcdDriver/lcd_driver.h"
#include "LcdDriver/lower_driver.h"

// USE OCTAVE 4 : C D E F G A B 
#define n_C 262
#define n_D 294
#define n_E 330
#define n_F 349
#define n_G 392
#define n_A 440
#define n_B 494

int note_ar = {n_C,n_D,n_E,n_F,n_G,n_A,n_B};
// !!BUZZER SETUP 
void setupBuzzer() {
    // J4.40 = P2.7 = TB0.6   (P2SEL1.7 = 0, P2SEL0.7 = 1)
    P2DIR  |=  BIT7;       // output
    P2SEL1 &= ~BIT7;       // SEL1 = 0
    P2SEL0 |=  BIT7;       // SEL0 = 1  -> TB0.6

    PM5CTL0 &= ~LOCKLPM5; 
}

void play_note(uint32_t noteFrequency) {
    if (noteFrequency == 0) {          // silence
        TB0CTL  = MC_0;                
        TB0CCTL6 = OUTMOD_0;           // OUTMOD = 0 : output bit, held low
        P2OUT &= ~BIT7;
        return;
    }
    
    // Default SMCLK is 1 MHz with no division (/1) -> 1 MHz timer clock
    uint16_t period = (uint16_t)(1000000UL / noteFrequency) - 1;

    TB0CTL   = TBCLR;                    // reset the counter and divider
    TB0CCR0  = period;                   // sets the note frequency
    TB0CCTL6 = OUTMOD_7;                 // OUTMOD = 7 : reset/set
    TB0CCR6  = period / 2;               // 50% duty cycle
    TB0CTL   = TBSSEL_2 | ID_0 | MC_1;   // TBSSEL=2 (SMCLK), ID=0 (/1), MC=1 (up mode)
}
// !!JOYSTICK SETUP 
void Initialize_ADC(void)
{
    // Configure joystick pins for analog functionality

    // Horizontal: A10/P9.2
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;

    // Vertical: A4/P8.7
    P8SEL1 |= BIT7;
    P8SEL0 |= BIT7;

    // Enable ADC, multiple sample/conversion,
    // and select 16 sample-and-hold clock cycles
    ADC12CTL0 = ADC12SHT0_2 | ADC12MSC | ADC12ON;

    // Software trigger (default), sampling timer,
    // MODOSC clock (default), divider 1 (default),
    // sequence-of-channels mode
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;

    // 12-bit resolution, unsigned binary (default)
    ADC12CTL2 = ADC12RES_2;

    // ADC12CTL3 remains at its default value.
    // Starting conversion address = ADC12MEM0.

    // MEM0: Horizontal joystick, channel A10
    ADC12MCTL0 = ADC12VRSEL_0 | ADC12INCH_10;

    // fINAL CONVERSION, CHANNEL A4, VERTICAL JOYSTICK 
    ADC12MCTL1 = ADC12VRSEL_0 |
                 ADC12INCH_4 |
                 ADC12EOS;

    // Enable conversions after configuration
    ADC12CTL0 |= ADC12ENC;
}
// ----PIXEL SCREEN INITIALIZATION
void Initialize_Clock_System() {
  // DCO frequency = 16 MHz
  // MCLK = fDCO/1 = 16 MHz
  // SMCLK = fDCO/1 = 16 MHz

  // Activate memory wait state
  FRCTL0 = FRCTLPW | NWAITS_1;    // Wait state=1
  CSCTL0 = CSKEY;
  // Set DCOFSEL to 4 (3-bit field)
  CSCTL1 &= ~DCOFSEL_7;
  CSCTL1 |= DCOFSEL_4;
  // Set DCORSEL to 1 (1-bit field)
  CSCTL1 |= DCORSEL;
  // Change the dividers to 0 (div by 1)
  CSCTL3 &= ~(DIVS2|DIVS1|DIVS0);    // DIVS=0 (3-bit)
  CSCTL3 &= ~(DIVM2|DIVM1|DIVM0);    // DIVM=0 (3-bit)
  CSCTL0_H = 0;

  return;
}
//STARTUP TUNE 
void startup(){
    // TWINKLE TWINKLE LITTLE STAR: C C G G A A G 
    play_note(n_C);
    __delay_cycles(10000);
    play_note(n_C);
    __delay_cycles(10000);
    play_note(n_G);
    __delay_cycles(10000);
    play_note(n_G);
    __delay_cycles(10000);
    play_note(n_A);
    __delay_cycles(10000);
    play_note(n_A);
    __delay_cycles(10000);
    play_note(n_G);
    __delay_cycles(10000);
}



