/*
 *  Stepper.h
 *	This code drives a bi-polar stepper motor connected to M3-M4 and M1-M2 of
 *	the AVR board. The arrays half[] and ful[] contains the half steppimg and full 
 *	stepping patterns, although only half stepping mode has been used. 
*/


#include <avr/io.h>
#include <avr/interrupt.h>


unsigned char halfR[8] = {8,9,1,5,4,6,2,10};
unsigned char halfL[8] = {128,144,16,80,64,96,32,160};
unsigned char fullR[4] ={5,6,10,9};
unsigned char fullL[4] ={80,96,160,144};

unsigned char hr=0,hl=0;

void WaitMs(unsigned int ms);

void motor_init() {
	DDRD=0xFF;
	DDRB=0xFF;
}	

void fwd(unsigned int steps, unsigned int speed) {
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hr++;
		if(hr>7)	hr=0;
		hl--;
		if(hl==255)	hl=7;
		PORTB=halfR[hr];
		PORTD=halfL[hl];
		WaitMs(speed);

}

void leftOnly(unsigned int steps, unsigned int speed) {
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hl--;
		if(hl==255)	hl=7;
		PORTD=halfL[hl];
		WaitMs(speed);
}

void rightOnly(unsigned int steps, unsigned int speed) {
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hr++;
		if(hr>7)	hr=0;
		PORTB=halfR[hr];
		WaitMs(speed);

}

void bwd(unsigned int steps, unsigned int speed) {
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hl++;
		if(hl>7)	hl=0;
		hr--;
		if(hr==255)	hr=7;
		PORTB=halfR[hr];
		PORTD=halfL[hl];
		WaitMs(speed);
}

// 74 steps per wheel for 90 deg turn
void right() {
	unsigned int steps = 74;
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hr--;
		if(hr==255)	hr=7;
		hl--;
		if(hl==255)	hl=7;
		PORTB=halfR[hr];
		PORTD=halfL[hl];
		WaitMs(15);

}
void left() {
	unsigned int steps = 74;
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hr++;
		if(hr>7)	hr=0;
		hl++;
		if(hl>7)	hl=0;
		PORTB=halfR[hr];
		PORTD=halfL[hl];
		WaitMs(15);

}
void uturn() {
	unsigned int steps = 148;
	unsigned int j=0;
	for(j=0;j<steps;j++) {
		hr++;
		if(hr>7)	hr=0;
		hl++;
		if(hl>7)	hl=0;
		PORTB=halfR[hr];
		PORTD=halfL[hl];
		WaitMs(15);
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



