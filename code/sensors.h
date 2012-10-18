/*
 *  sensors.h
 *  
 *
 *  Created by Anant on 03/08/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

#include <avr/io.h>
#include "stepper.h"

char sense() {
	// PINS C0, C1, C3 recieve data through ADC 
	// ADCMUX = 0x40, 0x41, 0x42 accordingly
	// C0 = Data3 = Left Sensor
	char status=0;
	// status = 0b00000xyz where x,y,z respectively indicate the presence of L, F, R walls
	ADCSRA = 0xE7;
	
	// Check left wall
	ADMUX = 0x40;
	unsigned int a;
	while(!(ADCSRA & (1<<ADIF)));
	a=ADC;
	if(a>300)
		SETBIT(status,0x04)
		
	// Do the same for front wall
	ADMUX = 0x41;
	while(!(ADCSRA & (1<<ADIF)));
	a=ADC;
	if(a>300)
		SETBIT(status,0x02)
		
	// Do the same for right wall
	ADMUX = 0x42;
	while(!(ADCSRA & (1<<ADIF)));
	a=ADC;
	if(a>300)
		SETBIT(status,0x01)	
		
	return status;
}

void align() {
	unsigned int left;
	unsigned int right;
	
	ADCSRA = 0xE7;
	
	ADMUX = 0x40;
	while(!(ADCSRA & (1<<ADIF)));
		left=ADC;
	
	ADMUX = 0x42;
	while(!(ADCSRA & (1<<ADIF)));
		right=ADC;

	unsigned int diff;
	if(left>right)	diff=left-right;
	else			diff = right-left;
	
	if(diff>50) {
		if(left>right) {
			leftOnly(25,15);
			rightOnly(25,15);
		}
		else {
			rightOnly(25,15);
			leftOnly(25,15);
		}
	}
}
