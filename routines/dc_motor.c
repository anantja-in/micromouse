/*	dc_motor.c
*
*	This code is used drives the dc motors at M1,M2,M3,M4.
*	The motors are driven by the H-bridge IC L293d.
*	M1=PD4,PD5   M2=PD6,PD7
*   	M3=PB0,PB1   M4=PB2,PB3
* 
*	First call the init_motor() to intialize the ports, then
*	call the corresponding macros to control the motor.
*	Say if you want to control the motor connected at M1 then
*	call M1_Fwd(), M1_Rev(), M1_Stop to make the motor turn, 
*	reverse, and stop
*
*/

/*	
	A motor will turn when its inputs differ. The direction depends on which
	input is positive and which is ground. (If both are the same, the motor
	will stop moving!). Now suppose you want turn a motor connected at M1,now
	or that we have to produce a high(1) at PD4 and low(0) at PD5. The macro
	M1_fwd() just does this thing. If you want to reverse the direction of the
	motor then you have to make PD4 low(0) and PD5 high(1). This is implemented
	in the M1_Rev() macro. Now to stop the motor we need to make both the PD4 
	PD5 pin either both low or both high. See the M1_Stop() macro definition,
	it is making both the PD4 and PD5 pins low.

	Now one thing you may notice is that we are using a macro definition instead
	defining it as a function. Defining it as a function will also work but it
	has a drawback. Calling a function has a overhead of pushing the current 
	PC(program counter) value to the stack and then jumping to the address of
	the subroutine. After the subroutine is executed, the PC address is poped.
	But if we use a macro definition, there is no overhead like this. The 
	compiler just replaces the defined token with the code as if has been
	directly written there.
  
*/

#include<avr/io.h>

/*Macros definition*/
#define BIT(x)	(1 << (x))		//Set a particular bit mask
#define CHECKBIT(x,b) x&b		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

/* Motor control definitions */
#define M1_Fwd()	( SETBIT(PORTD,BIT(4)) , CLEARBIT(PORTD,BIT(5)) )	//Set M2 in Fwd direction(PD4=1 and PD5=0)
#define M1_Rev() 	( CLEARBIT(PORTD,BIT(4)) , SETBIT(PORTD,BIT(5)) )	//Set M2 in Rev direction(PD4=0 and PD5=1)
#define M1_Stop()	( CLEARBIT(PORTD,BIT(4)) , CLEARBIT(PORTD,BIT(5)) )	//Stop M2(PD4=0,PD5=0)
#define M2_Fwd()	( SETBIT(PORTD,BIT(6)) , CLEARBIT(PORTD,BIT(7)) )	//Set M2 in Fwd direction(PD6=1 and PD7=0)
#define M2_Rev() 	( CLEARBIT(PORTD,BIT(6)) , SETBIT(PORTD,BIT(7)) )	//Set M2 in Rev direction(PD6=0 and PD7=1)
#define M2_Stop()	( CLEARBIT(PORTD,BIT(6)) , CLEARBIT(PORTD,BIT(7)) )	//Stop M2(PD6=0,PD7=0)
#define M3_Fwd()	( SETBIT(PORTB,BIT(0)) , CLEARBIT(PORTB,BIT(1)) )	//Set M3 in Fwd direction(PB0=1 and PB1=0)
#define M3_Rev() 	( CLEARBIT(PORTB,BIT(0)) , SETBIT(PORTB,BIT(1)) )	//Set M3 in Rev direction(PB0=0 and PB1=1)
#define M3_Stop()	( CLEARBIT(PORTB,BIT(0)) , CLEARBIT(PORTB,BIT(1)) )	//Stop M3(PB0=0,PB1=0)
#define M4_Fwd()	( SETBIT(PORTB,BIT(2)) , CLEARBIT(PORTB,BIT(3)) )	//Set M4 in Fwd direction(PB2=1 and PB3=0)
#define M4_Rev() 	( CLEARBIT(PORTB,BIT(2)) , SETBIT(PORTB,BIT(3)) )	//Set M4 in Rev direction(PB2=0 and PB3=1)
#define M4_Stop()	( CLEARBIT(PORTB,BIT(2)) , CLEARBIT(PORTB,BIT(3)) )	//Stop M4(PB2=0,PB3=0)


void init_motor(void);

int main(void)
{
	init_motor();	//This should be called first before using any motor command macro
	M1_Rev();	//Set the M1 motor in Rev direction	
	while(1)
	{}
	return 0;
}

/*This functions sets motor control pins as output pins*/
void init_motor(void)
{
	DDRB |=0x0F;	//set PB0-3 as output
	DDRD |=0xF0;	//set PD4-7 as output
}

