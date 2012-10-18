#include<avr/io.h>
#include"LCD_1.h"

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit


int main(void)
{
	lcd_init();
	lcd_string("Anant Jain");
	while(1)
	{}
	return 0;
}
