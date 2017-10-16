//  Credit to Thai Nghiem (collaborate with Matthew Rodriguez)
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
// MSP4306989
// Software PWM


#include <msp430.h>
#define LED0 BIT0		// definitions of LED pins
#define LED1 BIT7
#define LED0DIR P1DIR
#define LED1DIR P9DIR
#define LED0OUT P1OUT
#define LED1OUT P9OUT


#define BUTTON BIT1		// definitions of BUTTON pins
#define BTNDIR P1DIR
#define BTNREN P1REN
#define BTNOUT P1OUT
#define BTNIES P1IES
#define BTNIE P1IE
#define BTNIFG P1IFG

volatile unsigned int j;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	LED0DIR |= LED0;                          // Set P1.0 to output direction
	LED0OUT &= ~LED0;                         // Switch LED off


	LED1DIR |= LED1; //set Port 1.6 output ---LED
	LED1OUT &= LED1; //Clear P1.6

	BTNDIR &= ~BUTTON;                        // Set P1.3 as input
	BTNOUT |= BUTTON;                          // Configure P1.3 for Pull-Up
	BTNREN |= BUTTON;                          // Enable Pull Up of P1.3
	BTNIE |= BUTTON; //enable the interrupt on Port 1.3
	BTNIES &= ~BUTTON; //set as falling edge
	BTNIFG &= ~(BUTTON); //clear interrupt flag


	TA0CTL = TASSEL_2 + MC_1;       // SMCLK / Upmode
	TA0CCTL1 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
	TA0CCTL0 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
	TA0CCR0 = 1000 - 1;                        // PWM Frequency 10 kHz
	TA0CCR1 = 500;                           // 50% Duty Cycle

	__bis_SR_register(GIE);
	while (1)
	{
		if ((P1IN & BIT3))
			P1OUT &= ~BIT6; //Clear P1.6

	}
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
	BTNIE &= ~BUTTON; //Port 1 interrupt enable is turned off
	__delay_cycles(1000);  //Debounce
	BTNIE |= BUTTON; //Port 1 interrupt enable is turned back on

	LED1OUT |= LED1; //Sets P1.6
	if (TA0CCR1 >= 1000) // If the brightness is at 100%
	{
		TA0CCR0 = 0; // Reset CCR0
		TA0CCR1 = 0;// Reset CCR1
		TA0CCR0 = 1000; //  Set CCR0 back to 10 kHz
	}
	else if (TA0CCR1 < 1000) { // If the brightness is <= than 90%
		TA0CCR0 = 0; // Reset CCR0
		TA0CCR1 += 100; // Add 10%
		TA0CCR0 = 1000;//  Set CCR0 back to 10 kHz


	}
	BTNIFG &= ~BUTTON; //Clear flag
}

//Timer A interrupt vectors
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
	if (TA0CCR1 != 1000)
	{
		LED0OUT &= ~(LED0); //turns off red led
	}
	TA0CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
	if (TA0CCR1 != 0) {
		LED0OUT |= (LED0); //turns on red led
	}
	TA0CCTL0 &= ~LED0;  //clears flag
}
