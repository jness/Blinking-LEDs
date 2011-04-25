/*
 Code to switch LED colors
  The example found at Hack-a-Day / had_launchpad-blink
  was used to achieve my end goal:

    https://github.com/Hack-a-Day/had_launchpad-blink

  msp430-gcc was used to compile, CFLAG for hardware were passed:
   
    msp430-gcc -mmcu=msp430x2231
*/

#include <io.h>
#include <signal.h>

#define     LED0                  BIT0
#define     LED1                  BIT6
#define     LED_DIR               P1DIR
#define     LED_OUT               P1OUT

void initLEDs(void) {
  LED_DIR |= LED0 + LED1;	//Set LED pins as outputs
  LED_OUT |= LED0;		//Turn on LED0
  LED_OUT &= LED1;		//Turn off LED1
}


int main(void) {
  WDTCTL = WDTPW + WDTHOLD;		// Stop WDT
  initLEDs();				//Setup LEDs

  BCSCTL3 |= LFXT1S_2;			//Set ACLK to use internal VLO (12 kHz clock)

  TACTL = TASSEL_1 | MC_1; 		//Set TimerA to use auxiliary clock in UP mode
  TACCTL0 = CCIE;			//Enable the interrupt for TACCR0 match

  TACCR0 = 11999;			/*Set TACCR0 which also starts the timer. At
					12 kHz, counting to 12000 should output
					an LED change every 1 second. Try this
					out and see how inaccurate the VLO can be */
			
  WRITE_SR(GIE);			//Enable global interrupts

  while(1) {}				//Loop forever, interrupts take care of the rest

}

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) {
  LED_OUT ^= (LED0 + LED1);	//Toggle both LEDs
}


