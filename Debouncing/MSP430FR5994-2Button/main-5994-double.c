#include <msp430.h> 
/**
* main.c
* credit to Sean Hastings for reference
* MSP430FR5994
*/
#define LED0 BIT0
#define LED1 BIT2
#define BUTTON BIT5
#define BUTTON2 BIT6

#define LEDDIR P1DIR
#define LEDOUT P1OUT

#define BTNDIR P5DIR
#define BTNREN P5REN
#define BTNOUT P5OUT
#define BTNIE P5IE
#define BTNIES P5IES
#define BTNIFG P5IFG
#define BTNIN P5IN


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
	LEDDIR |= LED0;		//setup LEDs
	LEDDIR |= LED1; 

	BTNREN |= BUTTON;	//setup button 5.5
	BTNOUT |= BUTTON;
	BTNDIR |= BUTTON;
	BTNIE |= BUTTON;
	BTNIES |= BUTTON;
	BTNIFG &= ~BUTTON;

	BTNREN |= BUTTON2;	//setup button 5.6
	BTNOUT |= BUTTON2;
	BTNDIR |= BUTTON2;
	BTNIE |= BUTTON2;
	BTNIES |= BUTTON2;
	BTNIFG &= ~BUTTON2;

	PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode
	__bis_SR_register(LPM0_bits + GIE); //enable low power
}


#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void) {		//button 5 interrupt service routine
	if (BTNIN & ~BUTTON) {				//if button1 pressed
		BTNIE &= ~BUTTON;				//disable interrupt
		WDTCTL = WDT_MDLY_32;		//Watchdog 32ms delay
		SFRIE1 |= WDTIE;			//enable WDT interrupt
		BTNIFG &= ~BUTTON;          //clear flag
	}
	if (BTNIN & ~BUTTON2) {			//if button2 is pressed
		BTNIE &= ~BUTTON2;          //disable interrupt
		WDTCTL = WDT_MDLY_32;		//Watchdog 32ms delay
		SFRIE1 |= WDTIE;			//enable WDT interrupt
		BTNIFG &= ~BUTTON2;         //clear flag
	}
}


#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	SFRIE1 &= ~WDTIE;			//disable WDT interrupt
	SFRIFG1 &= ~WDTIFG;			//clear flag
	WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

	if (!(BTNIN & BUTTON))		//if button is pressed, switch LED
	{
		LEDOUT ^= LED1;			//toggle led 1.1
	}
		BTNIE |= BUTTON;		//enable button interrupt

	if (!(BTNIN & BUTTON2))		//if button is pressed, switch LED
	{
		BTNOUT ^= LED0;			//toggle led 1.0
	}
		BTNIE |= BUTTON2;		//reset enable
}