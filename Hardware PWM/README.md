# LAB 4 - Hardware PWM
#### Joshua Gould
##### Created - 10-16-2017
##### Updated - 10-16-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development board
* CCS coding software

## Set Requirements
"You need to replicate the same behavior as in the software PWM, only using the Timer Modules ability to directly output to a GPIO Pin instead of managing them in software. "

#### Tasks
* [x] Use PWM in Hardware Design
* [x] PWM one of the on-board LEDs at a 50% duty cycle.
* [x] The duty cycle of the LED should increase by 10%. 
* [x] Once the cycle has reached 100%, the duty cycle should go back to 0% on the next button press.
* [ ] Extra Work

## Compatibility
* MSP430F5529
* MSP430FR2311
* MSP430FR6989
* MSP430FR5594
* MSP430G2553

# Functionality Description
Increment with PWM in hardware

### Detailed Description

* Initialize GPIO
* Set clock (SMCLK in up mode)
* Increment timer and synconize with LED

### Example Code
```C
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>
//CODE ADAPTED FOR G2553
//Hardware PWM
//MSP430G2553
#define LED0 BIT0		//Define bits for the LEDs and BUTTON.
#define LED1 BIT6
#define BUTTON BIT3

#define LEDDIR P1DIR	//Define DIR and OUT for the correct LED1 pin.
#define LEDOUT P1OUT
#define LEDSEL P1SEL

#define BTNDIR P1DIR		//Define button pin outs.
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

	if (!((P1IN & BIT3) == BIT3)) {
		LEDOUT ^= LED1; //toggle green led

		if (TA0CCR1 <= 1000) {
			TA0CCR1 = TA0CCR1 + 100; //increments red led duty cycle

		}
		else {
			TA0CCR1 = 0; //resets duty cycle back to 0%
		}
	}

	//clears interrupt flag to exit interrupt
	BTNIFG &= ~BUTTON;
}
```