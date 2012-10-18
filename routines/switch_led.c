/*	switch_led.c
*
*	This code glows the LED D2 on PORTB PIN6 i.e PB6 of the AVR board, if switch1
*	is turned on else it is off.	
*	The LED D2 is connected to PB6 on the Atmga chip, and is wired so that
*	it will be ON when the output is high and OFF when the output is low.
*	The Switch S1 is connected to PB4.
*	The "special function registers" used in this program are:
*		DDRB -- Port B Data Direction Register
*			A high (1) value indicates output, low (0) is input.
*			We set the 6th bit in DDRB to configure the PB6 pin as output pin.
*			We clear the 4th bit in DDRB to configure the PB4 pin as input pin.
*		PORTB -- Port B Data Register
*			This is used to set the output value for each of the
*			eight port B pins.
*		PINB --- PORTB input register, used to read inputs in PORTB pins 
*/

#include<avr/io.h>
#include<avr/interrupt.h>

/*Macros definition*/
#define BIT(x)	(1 << (x))			//Set a particular bit mask
#define CHECKBIT(x,b) (x&b)			//Checks bit status
#define SETBIT(x,b) x|=b;			//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

int main(void)
{
	SETBIT(DDRB,BIT(6))			//Sets the 6th pin in DDRB register to make PB6 a output pin
	CLEARBIT(DDRB,BIT(4))		//Clears the 4th pin in DDRB register to make PB4 a input pin
	while(1)
	{
		if(CHECKBIT(PINB,BIT(4)))	//Check 4th bit in PINB register
			SETBIT(PORTB,BIT(6))	//Sets PORTB6 as high i.e. LED on AVR Board Glows
		else
			CLEARBIT(PORTB,BIT(6))	//Clears PORTB6, PB6 goes low i.e tur led off
	}
	return 0;
}
