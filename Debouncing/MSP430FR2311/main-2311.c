#include <msp430g2553.h>
//MSP430FR5994
//Debounce
#define LED0 BIT0		// definitions of LED pins
#define LED1 BIT6
#define LED0DIR P1DIR
#define LED1DIR P1DIR
#define LED0OUT P1OUT
#define LED1OUT P1OUT
#define BUTTON BIT3		// definitions of BUTTON pins
#define BTNDIR P1DIR
#define BTNREN P1REN
#define BTNOUT P1OUT
#define BTNIES P1IES
#define BTNIE P1IE
#define BTNIFG P1IFG
#define BTNOUT P1OUT

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	LED1DIR |= LED1;            // Set to output
	BTNDIR &= ~BUTTON;           // Set to input
	BTNREN |= BUTTON;            // Set resistor
	BTNOUT |= BUTTON;            // pullup resistor
	BTNIE |= BUTTON;             // Set interrupt pin

	TA0CCTL0 = CCIE;          // Enable capture/compare interrupt
	__enable_interrupt();     // Enable interrupt code
	while (1)				  //do nothing continuously by default to keep main running
	{
	}             
}

#pragma vector=PORT1_VECTOR			// Define interrupt vector, in this case Port 1
__interrupt void button_press(void) // Define interrupt function that runs whenever interrupt is detected
{

	BTNIE &= !BUTTON;						// Turn off P1.3 interrupt
	LED1OUT ^= LED1;					    // Toggle P1.6 (LED)
	TA0CTL = TASSEL_2 + MC_1 + TACLR;   // Set Timer A0 to ACLK, up mode
	TA0CCR0 = 10000;					// Count to ~10 ms
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void button_debounce_timer(void)
{
	TA0CTL = MC_0;            // Stop timer
	TA0R = 0;                 // Reset count TA0R
	P1IE |= BIT3;             // Enable interrupt code
	P1IFG = ~BIT3;            // Disable flag interrupt
}