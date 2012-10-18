/*	ADC.c
*
*	This code implements ADC conversion. The Test pot at PA7(ADC7) is used
*	to control the brightness of the led D2. The pot gives a variable voltage
*	at the ADC7 pin. This is converted to a digital value. This digital value
*	is then used to control the brightness of the LED. A simple PWM with
*	delay routines is used.
*
*
*	The "special function registers" for ADC are ADMUX, ADCSRA, ADCH, ADCL and
*	SFIOR.
*	
*	ADMUX is used to select which channel to use for ADC conversion. We used
*	ADC7 here. It also has a ADLAR bit, by setting which the ADC data value
*	becomes left adjusted.
*	
*	ADCSRA contain bits to enable ADC,start ADC, select ADC prescale clock,etc..
*	
*	ADCH and ADCL contains the ADC data..We used ADCH only coz we are stisfied
*	with 8-bit resolution oonly.
*
*	SFIOR contains bits to configure different modes..Default value sets the ADC
*	at free running mode..
*
*	The ADC ISR is executed when a ADC conversion completes. In the ISR, you can
*	do whatever you like with the ADC converted value stored in ADCH:L At the end
*	of the ISR routine you should set the ADSC (ADC start conversion) bit to again
*	start the ADC conversion. 
*	
*
*/


#include <avr/io.h>
#include<avr/interrupt.h>

/*Macros definition*/
#define BIT(x)	(1 << (x))		//Set a particular bit mask
#define CHECKBIT(x,b) x&b		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

unsigned char digital=100;

void Wait(unsigned char delay);

int main(void)
{
	//ADC Initialization
	ADMUX=0x27;	//We set ADLAR=1(left adjusted) and set the MUX bits to select ADC7 input.
	ADCSRA=0x8E;	//enable ADC and ADIE(Enable ADC interrupt)
	
	SETBIT(DDRB,BIT(6)); 	//Set the DDRB bit 6 to configure PB6 as output
	SETBIT(PORTB,BIT(6)); 	//Turn on the Led D2


	sei();			//Enable Global Interrupt
	SETBIT(ADCSRA,BIT(6));	//Start ADC conversion

	while(1)
	{
		//Here we generate PWM using a simple delay routine,
		//ON time is proportional to the digital value recieved
 		//from the ADC
		
		//ON TIME
		SETBIT(PORTB,BIT(6));	//turn on led
		Wait(digital);

		//OFF TIME
		CLEARBIT(PORTB,BIT(6));	//turn off led
		Wait(255-digital);
	
	}
	return 0;
}

//ADC conversion complete ISR
ISR(ADC_vect)
{
	digital=ADCH;	//Get the ADC converted value..Since we used Left adjusted, so we need to access the ADCH only
		
	SETBIT(ADCSRA,BIT(6));	//Start the next ADC conversion
}

//A delay routine
void Wait(unsigned char delay)
{
	unsigned char i;

	while (delay-- > 0)
	{
		for (i = 0; i < 65; ++i)
			asm("nop");
	}
}
