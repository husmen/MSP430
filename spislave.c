#include <msp430.h> 

#define REDLED BIT5
#define YELLOWLED BIT4
#define GREENLED BIT3
#define BUTTONGREEN BIT2
#define BUTTONYELLOW BIT1
#define BUTTONRED BIT0

int redcounter, yellowcounter, greencounter;
volatile unsigned int i;
/*
 * main.c
 */
void spisend(int valueToSend){
	  for (i = 0xFFF; i > 0; i--);          // Time for slave to ready
	  USISRL = valueToSend;
	  USICNT = 8;                           // init-load counter
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    CCTL0 = CCIE;                            // CCR0 interrupt enabled
    CCR0 = 50000;
    TACTL = TASSEL_2 + MC_2;                 // SMCLK, contmode
    P2OUT = 0x00;
    P1DIR |= BIT0;
	P2DIR = (GREENLED + YELLOWLED + REDLED);
	P2REN |= (BUTTONRED + BUTTONYELLOW + BUTTONGREEN);
	P2IE = (BUTTONRED + BUTTONGREEN + BUTTONYELLOW);
	P2IES &= ~(BUTTONRED + BUTTONGREEN + BUTTONYELLOW);
	P2IFG &= ~(BUTTONRED + BUTTONGREEN + BUTTONYELLOW);
	USICTL0 |= USIPE7 +  USIPE6 + USIPE5 + USIOE; // Port, SPI master
	USICTL1 |= USIIE;                     // Counter interrupt, flag remains set
	USICKCTL = USIDIV_4 + USISSEL_2;      // /16 SMCLK
	USICTL0 &= ~USISWRST;                 // USI released for operation
	USISRL = 0;                        // init-load data
	_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
	return 0;
}

// Timer A0 interrupt service routine

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
  //P1OUT ^= 0x01;                           // Toggle P1.0
  CCR0 += 50000;                           // Add Offset to CCR0
  if(redcounter>0){redcounter--;}
  if(yellowcounter>0){yellowcounter--;}
  if(greencounter>0){greencounter--;}
  if(redcounter == 1){P2IFG &= ~BUTTONRED;P2IE |= BUTTONRED;}
  if(yellowcounter == 1){P2IFG &= ~BUTTONYELLOW;P2IE |= BUTTONYELLOW;}
  if(greencounter == 1){P2IFG &= ~BUTTONGREEN;P2IE |= BUTTONGREEN;}
}

 #pragma vector=PORT2_VECTOR
 __interrupt void Port_2 (void)
 {
	 if(P2IFG & BUTTONRED){
		 P2OUT ^= REDLED;
		 P2IE &=~ BUTTONRED;
		 redcounter = 5;
		 spisend(1);
	 }
	 if(P2IFG & BUTTONYELLOW){
		 P2OUT ^= YELLOWLED;
		 P2IE &=~ BUTTONYELLOW;
		 yellowcounter = 5;
		 spisend(2);
	 }
	 if(P2IFG & BUTTONGREEN){
		 P2OUT ^= GREENLED;
		 P2IE &=~ BUTTONGREEN;
		 greencounter = 5;
		 spisend(3);
	 }
// P1OUT ^= 0x01; // Toggle P5.0
// P2IFG &= ~(BUTTONRED + BUTTONYELLOW + BUTTONGREEN);
 }

 // USI interrupt service routine
 #pragma vector=USI_VECTOR
 __interrupt void universal_serial_interface(void)
 {
   if (USISRL==1){
	   		 P2OUT ^= REDLED;
	   		 redcounter = 5;
	   	 }
	   	 if(USISRL==2){
	   		 P2OUT ^= YELLOWLED;
	   		 yellowcounter = 5;
	   	 }
	   	 if(USISRL==3){
	   		 P2OUT ^= GREENLED;
	   		 greencounter = 5;
	   	 }
	     USISRL = 0;
	     USICNT = 8;                           // re-load counter
 }
