#include <LcdDriver/lower_driver.h>
#include "msp430fr6989.h"
#include "Grlib/grlib/grlib.h"
#include <stdint.h>
void HAL_LCD_PortInit(void) {
    // 1. Configure control GPIO pins: CS (P2.5), DC (P2.3), RST (P2.4)
    P2DIR |= (BIT3 | BIT4 | BIT5);
    
    // Per Lab 9: Set CS low permanently to activate display interface
    P2OUT &= ~BIT5;

    // 2. Configure SPI peripheral pins on Port 1: P1.4 (SCLK) and P1.6 (SIMO)
    P1DIR |= (BIT4 | BIT6);
    P1SEL0 |= (BIT4 | BIT6);  // Select Primary Peripheral Module Function
    P1SEL1 &= ~(BIT4 | BIT6);

    // Disable high-impedance mode after reset for FRAM ports
    PM5CTL0 &= ~LOCKLPM5;
}

void HAL_LCD_SpiInit(void) {
    // Put eUSCI_B0 in software reset for configuration
    UCB0CTL1 |= UCSWRST;

    // Configure 3-pin SPI Master Mode:
    // Synchronous (UCSYNC), Master (UCMST), MSB First (UCMSB), Phase 1 / Polarity 0 (SPI Mode 0)
    UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;

    // Select SMCLK (16 MHz) as source clock
    UCB0CTL1 = UCSSEL__SMCLK | UCSWRST;

    // Set prescaler to divide 16 MHz SMCLK by 2 -> 8 MHz SPI clock (Max supported display clock = 10 MHz)
    UCB0BR0 = 2;
    UCB0BR1 = 0;

    // Release eUSCI_B0 from software reset
    UCB0CTL1 &= ~UCSWRST;
}
void HAL_LCD_writeData(uint8_t data)
{
    // Wait as long as the module is busy
    while (UCB0STATW & UCBUSY);

    // Set DC' bit back to high
    P2OUT |= BIT3;

    // Transmit data
    UCB0TXBUF = data;

    return;
}