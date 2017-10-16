# LAB 4 - Debouncing
#### Joshua Gould
##### Created - 10-15-2017
##### Updated - 10-16-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development board
* CCS coding software

## Set Requirements
"You need to utilize the TIMER modules within the MSP430 processors to implement a debounced switch to control the state of an LED. You most likely will want to hook up your buttons on the development boards to an oscilloscope to see how much time it takes for the buttons to settle. The idea here is that your processor should be able to run other code, while relying on timers and interrupts to manage the debouncing in the background. You should not be using polling techniques for this assignment. Your code should also be able to detect"


#### Tasks
* [x] Blink one of the on-board LED's at a particular rate (50% duty cycle)
* [x] Use a button to utilize the Timer module
* [x] Uses a debounced switch
* [x] Extra Work

## Compatibility
* MSP430F5529
* MSP430FR2311
* MSP430FR6989
* MSP430FR5594
* MSP430G2553


# Functionality Description
This project consisted of writing a program in C to control an MSP430 microprocessor to toggle an LED. A crucial difference between this button control and the Lab 3 Button Interrupt program is that this program utilizes a timer to prevent repeated triggers of the interrupt vector caused by mechanical bouncing of the button input.

### Detailed Description
* Enable interrupt
* Toggle Button interrupt
* Toggle LED
* Set up timer (ACLK to up mode)
* Count to defineed limit

### Example Code
```C
#include <msp430g2553.h>
//MSP430G2553
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
}

#pragma vector=PORT1_VECTOR			// Define interrupt vector, in this case Port 1
__interrupt void button_press(void) // Define interrupt function that runs whenever interrupt is detected
{

	BTNIE &= !BUTTON;						// Turn off interrupt
	LED1OUT ^= LED1;					    // Toggle LED
	TA0CTL = TASSEL_2 + MC_1 + TACLR;   // ACLK, up mode
	TA0CCR0 = 10000;					// 10ms
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void button_debounce_timer(void)
{
	TA0CTL = MC_0;            // Stop timer
	TA0R = 0;                 // Reset count
	P1IE |= BIT3;             // Enable interrupt
	P1IFG = ~BIT3;            // Disable flag interrupt
}
```
# Extra Work Description

#### Double switch debounce
Uses two switches in debouncing of 5594. Similar to normal 5994 design except including P5.6

#### Example Code
```C
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
```