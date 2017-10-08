#include <msp430.h>
/*
MSP430G2553
Software PWM
credit to nprasan on GitHub for reference
*/
#define LED0 BIT0		// definitions of LED pins
#define LED1 BIT6		//focus on green LED
#define LED1DIR P1DIR
#define LED1OUT P1OUT

void delay(unsigned int t)		    //delay functions
{
	unsigned int d;
	for (d = t; d > 0; d--)		    //set equal to t and decrement
		_delay_cycles(1);
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stop watchdog
	LED1DIR |= (LED1);              // configure LED as output

	while (1)
	{
		unsigned int i;
		for (i = 1; i < 500; i++)	// Increase
		{
			LED1OUT |= LED1;		//turn on led
			delay(i);
			LED1OUT &= ~LED1;		//turn off led
			delay(500 - i);			// off Time = Period - on Time
		}
		for (i = 500; i > 1; i--)	// Decrease
		{
			LED1OUT |= LED1;
			delay(i);
			LED1OUT &= ~LED1;
			delay(500 - i);
		}
	}
}