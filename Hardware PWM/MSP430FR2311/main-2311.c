//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>
//CODE ADAPTED FOR FR2311
//Hardware PWM
//MSP430FR2311
#define LED0 BIT0
#define LED1 BIT1
#define BUTTON BIT1
#define LEDSEL P1SEL
#define LEDDIR P4DIR
#define LEDOUT P4OUT

#define BTNDIR P1DIR
#define BTNREN P1REN
#define BTNOUT P1OUT
#define BTNIE P1IE
#define BTNIES P1IES
#define BTNIFG P1IFG
#define BTNIN P1IN



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog
	LEDDIR |= LED0 + LED1;                   // P1.0 and P1.6 output
	LEDSEL |= LED1;                         // P1.0 options select

	BTNREN |= BUTTON;						   //Enable and pull-up resistor
	BTNOUT |= BUTTON;
	BTNIE |= BUTTON;
	BTNIES |= BUTTON;

	TA0CCR0 = 1000 - 1;                    // PWM Period
	TA0CCTL1 = OUTMOD_7;                   // CCR1 reset/set
	TA0CCR1 = 500;                         // CCR1 PWM duty cycle

	TA0CTL = TASSEL_2 + MC_1;			   // SMCLK, up mode, clear TBR

	__bis_SR_register(LPM0_bits + GIE);    // Enter LPM0

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	//P1IE &= ~BIT1;
	if (TA0CCR1 >= 1000) {		//1 khz check
		TA0CCR1 = 0;			//reset to 0
	}
	else {
		TA0CCR1 = TA0CCR1 + 100; //increment timer 100Hz
	}
	BTNIFG &= ~BUTTON;				//reset flag
}