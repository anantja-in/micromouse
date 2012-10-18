/*	blinking_led.c
*
*	This code blinks the LED D2 on PORTB PIN6 i.e PB6 of the AVR board.
*
*	The LED D2 is connected to PB6 on the Atmel chip, and is wired so that
*	it will be ON when the output is high and OFF when the output is low.
*
*	The "special function registers" used in this program are:
*		DDRB -- Port B Data Direction Register
*			A high (1) value indicates output, low (0) is input.
*			We set the 6th bit in DDRB to configure the PB6 pin as output pin.
*		PORTB -- Port B Data Register
*			This is used to set the output value for each of the
*			eight port B pins. 
*	WaitMs() is used to produce a delay.
*/

#include<avr/io.h>
#include<avr/interrupt.h>

/*Macros definition*/
#define BIT(x)	(1 << (x))			//Set a particular bit mask
#define CHECKBIT(x,b) x&b			//Checks bit status
#define SETBIT(x,b) x|=b;			//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

void WaitMs(unsigned int ms);

int main(void)
{
	SETBIT(DDRB,BIT(6))			//Sets Direction of 6th pin of port B as output

	while(1)
	{
		SETBIT(PORTB,BIT(6))	//Sets PORTB6 as high i.e. turn on led
		WaitMs(200);			//delay for 200ms(0.2s)
		CLEARBIT(PORTB,BIT(6))	//Clears PORTB6(PB6 pin goes low) i.e turn off led
		WaitMs(200);			//delay for 200ms(0.2s)
	}
	return 0;
}

/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void WaitMs(unsigned int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 16380 (16k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 3276; ++i)
			asm("nop");
	}
}
