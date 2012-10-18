/*	Stepper.c
*
*	This code drives a bi-polar stepper motor connected to M2-M3 of
*	the AVR board. Both half stepping and full stepping modes have been implemented.
*	The arrays half[] and ful[] contains the half steppimg and full stepping patterns.
*
*/


#include <avr/io.h>
#include<avr/interrupt.h>


#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

unsigned char value,hvalue,t=0,s=0;
unsigned char half[8] = {8,9,1,5,4,6,2,10};
unsigned char full[4] ={5,6,10,9};
unsigned char h=0,f=0;
unsigned int speed;

void WaitMs(unsigned int ms);

void cwhalf(unsigned int steps);
void ccwhalf(unsigned int steps);
void cw(unsigned int steps);
void ccw(unsigned int steps);

int main(void)
{

	
	//stepper
	DDRD=0xFF;
	DDRB=0xFF;
	DDRC=0xFF;
	
	speed=15;
	//ccwhalf(300);
	while(1)
	{
		cw(300);
	}
	
	return 0;
}


//Rotate the stepper counter-clockwise in half Stepping mode
void ccwhalf(unsigned int steps)
{
	unsigned int j=0;
	for(j=0;j<steps;j++)
	{
	h++;
	if(h>7)
		h=0;
	PORTB=half[h];
	WaitMs(speed);
	}
}

//Rotate the stepper clockwise in half Stepping mode
void cwhalf(unsigned int steps)
{
	unsigned int j=0;
	for(j=0;j<steps;j++)
	{
	h--;
	if(h==255)
		h=7;
	PORTB=half[h];
	WaitMs(speed);

	}
}

//Rotate the stepper counter-clockwise in full stepping mode
void ccw(unsigned int steps)
{
	
	unsigned int j=0;
	for(j=0;j<steps;j++)
	{
	f++;
	if(f>3)
		f=0;
	PORTB=full[f];
	WaitMs(speed);
	}
}

//Rotate the stepper clockwise in full stepping mode
void cw(unsigned int steps)
{
	unsigned int j=0;
	for(j=0;j<steps;j++)
	{
	f--;
	if(f==255)
		f=3;
	PORTB=full[f];
	WaitMs(speed);

	}
}


/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void WaitMs(unsigned int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 16380 (16k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 327; ++i)
			asm("nop");
	}
}


