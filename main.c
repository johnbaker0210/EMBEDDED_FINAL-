#include <helper_func.h>
#include <stdlib.h>
// ****************************************************************************
void display(){
    char mystring[20];
    // -- DISPLAY MESSGAE 
    // Configure WDT & GPIO
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LEDs
    P1DIR |= redLED;                P9DIR |= greenLED;
    P1OUT &= ~redLED;               P9OUT &= ~greenLED;

    // Configure buttons
    P1DIR &= ~(S1|S2);
    P1REN |= (S1|S2);
    P1OUT |= (S1|S2);
    P1IFG &= ~(S1|S2);          // Flags are used for latched polling

    // Set the LCD backlight to highest level
    //P2DIR |= BIT6;
    //P2OUT |= BIT6;

    // Configure clock system
    Initialize_Clock_System();

    ////////////////////////////////////////////////////////////////////////////////////////////
    // Graphics functions
    Graphics_Context g_sContext;        // Declare a graphic library context
    Crystalfontz128x128_Init();         // Initialize the display

    // Set the screen orientation
    Crystalfontz128x128_SetOrientation(0);

    // Initialize the context
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

    // Set background and foreground colors
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

    // Set the default font for strings
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    //Clear the screen
    Graphics_clearDisplay(&g_sContext);
    ////////////////////////////////////////////////////////////////////////////////////////////

    // Print message
    Graphics_drawStringCentered(&g_sContext, "Welcome to", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);
    sprintf(mystring, "Music Trainer!");
    Graphics_drawStringCentered(&g_sContext, mystring, AUTO_STRING_LENGTH, 64, 55, OPAQUE_TEXT);
    // 
    Graphics_drawString(&g_sContext, "How many questions do you want? ", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);
}
void main(void) {
    int x = rand() % (6 - 0 + 1); // ]0 TO 6 
    // PLAY STARTUP TUNE 
    for (int i = 0; i < 30; i++){
        startup();
    }
    // SEND STARTING DISPLAY MESSAGE 
    display();
    // PLAY THREE RANDOM NOTES 
    for (int i )
    for (int i = 0; i < 3; i++){
      play_note(note_ar[x]);
      __delay_cycles(10000);
    }
    // READ JOYSTICK 
}




