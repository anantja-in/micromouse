/*	UART.c
*
*	This code implements UART communication between your PC and AVR board.
*	This is a simple ECHO program. So what you send from the Hyperterminal
*	of the PC will be received by the uC and resend back, i.e it will echo
*	back the data. In this program USART is configured to 9600bps, 8-bit
*	character size and no parity. You need to configure the hyperterminal
*	connection with this settings.
*	
*	The "special function registers" used in this program are UBRR, UCSRA
*	UCSRB, UCSRC and UDR.
*
*	UBBR will store the baud balue which is dependent on the transmission
*	speed or baud rate. You can either use the formulae which isgiven in
*	the datasheet, or you can refer to table68(page 169) in the ATmega16
*	datasheet. Note that UBRR is a 16-bit register consisting of two 8-bit
*	registers UBRRL and UBRRH.
* 
*	UCSRA, UCSRB & UCSRC are USART Control and Status registers. See the
*	datasheet for the values you need to give for the appropiate settings
*	Set the RXEN and TXEN bits to start the USART. We have also set the
*	RXCIE bit to enable RX(recieve) complete interrupt. So whenever your
*	uC completes the data recieve, it jumps to the RX complete ISR. You
*	can do what you want to do with the received data in the ISR. Don't
*	forget to enable the global interupt, untill which your interupt will
*	not work.
*	
*	UDR is the USART I/O data register which contains both the received
*	data and transmit data..
*
*/
#include <avr/io.h>
#include<avr/interrupt.h>

/*Macros definition*/
#define BIT(x)	(1 << (x))			//Set a particular bit mask
#define CHECKBIT(x,b) x&b			//Checks bit status
#define SETBIT(x,b) x|=b;			//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

unsigned char Recv;

int main (void)
{
	//Initialize UART with 8-bit character sizes,no parity,1 stop bit 
	UBRRL=0xA0;	//Write UBRRL register first
	UBRRH=0x01;	//then write the UBBRH register 
	UCSRB=0x98;	//Set RXCIE, RXEN, TXEN bits
   	UCSRC=0x86; 	//Set URSEL,UCSZ0,UCSZ1
   	 
	sei();		//Enable Global Interrupt 
	
	while(1)	
	{
	}   
}

//RX complete Interrup routine..put here all the code that you want to do with the received data..
ISR(USART_RXC_vect)
{
	Recv=UDR;
	UDR=Recv;
}
