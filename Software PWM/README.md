# LAB 4 - Software PWM
#### Joshua Gould
##### Created - 10-16-2017
##### Updated - 10-16-2017

## Library Used
msp430.h - default library

## Dependencies
* MSP430 Development board
* CCS coding software

## Set Requirements
"You need to generate a 1kHz PWM signal with a duty cycle between 0% and 100%. Upon the processor starting up, you should PWM one of the on-board LEDs at a 50% duty cycle. Upon pressing one of the on-board buttons, the duty cycle of the LED should increase by 10%. Once you have reached 100%, your duty cycle should go back to 0% on the next button press. You also need to implement the other LED to light up when the Duty Cycle button is depressed and turns back off when it is let go. This is to help you figure out if the button has triggered multiple interrupts."

#### Tasks
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
Generate a 1kHz PWM signal with a duty cycle between 0% and 100%. Each processor can hold multiple PWM designs and the code below shows the manipulation of PWMs.
### Detailed Description

* Set GPIO Pins
* Set Clock (SMCLK in up mode)
* Set interrupt
* Describe increment and brightness reset with CCR0 (10%)
* Toggle LED with timer

### Example Code
```C
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

	BTNDIR &= ~BUTTON;              // Set button as input
	BTNOUT |= BUTTON;               // Configure Pull-resistor
	BTNREN |= BUTTON;               // Pull-up
	BTNIE |= BUTTON; 		//enable the interrupt
	BTNIES &= ~BUTTON; 		//set as falling edge
	BTNIFG &= ~(BUTTON); 		//clear interrupt flag


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

	LED1OUT |= LED1; 
	if (TA0CCR1 >= 1000) 		// 100% Brightness
	{
		TA0CCR0 = 0; 		// Reset CCR0
		TA0CCR1 = 0;		// Reset CCR1
		TA0CCR0 = 1000; 	//  Set CCR0 back to 10 kHz
	}
	else if (TA0CCR1 < 1000) { 	// 90% brightness
 		TA0CCR0 = 0; 		// Reset CCR0
		TA0CCR1 += 100; 	// Add 10%
		TA0CCR0 = 1000;		//  Set CCR0 back to 10 kHz


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

```
