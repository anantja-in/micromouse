/*
 *  micromouse.c
 *  
 *  Created by Anant on 02/08/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#define BIT(x)	(1 << (x))
#define CHECKBIT(x,b) (x&b)		//Checks bit status
#define SETBIT(x,b) x|=b;		//Sets the particular bit
#define CLEARBIT(x,b) x&=~b;		//Sets the particular bit
#define TOGGLEBIT(x,b) x^=b;		//Toggles the particular bit

#include "stepper.h"
#include "LCD.h"
#include "sensors.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// Global vars

char flood[16][16];
char walls[16][16];

void reflood(char x, char y) {
	char flag = 0;
	char min=255;
	if(walls(x,y) & 0x08==0x00)
		if(flood(x,y+1) < flood(x,y)) {
			flag = 1; 
			if(min>flood(x,y+1)) min=flood(x,y+1);
		}
	if(walls(x,y) & 0x04==0x00)
		if(flood(x+1,y) < flood(x,y)) {
			flag = 1;
			if(min>flood(x+1,y)) min=flood(x+1,y);
		}
	if(walls(x,y) & 0x02==0x00)
		if(flood(x-1,y) < flood(x,y)) {
			flag = 1;
			if(min>flood(x-1,y)) min=flood(x-1,y);
		}
	if(walls(x,y) & 0x01==0x00)
		if(flood(x,y-1) < flood(x,y)) {
			flag = 1;
			if(min>flood(x,y-1)) min=flood(x,y-1);
		}
			
	if(flag==0)  { // flooding is reqd
		flood(x,y) = min+1;
		if(walls(x,y) & 0x08==0x00)
			reflood(x,y+1);
		if(walls(x,y) & 0x04==0x00)
			reflood(x+1,y);
		if(walls(x,y) & 0x02==0x00)
			reflood(x-1,y);
		if(walls(x,y) & 0x01==0x00)
			reflood(x,y-1);
	}
}	
int main(void) {
	motor_init();
	
	int x,y,x1,y1;
	char centreFound = 0;
	char flood[16][16];
	char walls[16][16];
	// Initialize flood values and wall data
	// Wall data: == 0 unvisited, no info
	// 0b1111news (first four bits indicate reliability of last four bits, respectively)
	
	for(x=0;x<16;x++)
		for(y=0;y<16;y++) {
		x1 = (x>7)? 15-x:x;
		y1 = (y>7)? 15-y:y;
		flood[x][y]=14-x1-y1;
		walls[x][y]=0;
	}
	walls[0][0] |= 0x11; // This south wall is the only unseen one by the mouse
	
	char curX, curY, curHead, t;
	curX = 0;
	curY = 0;
	curHead = 'N';
	
	while(centreFound == 0) {
	
		
		// Sense the walls and update wall map
		
		char status = sense();
		SETBIT(walls[curX][curY],0xF0)
		
		if(curHead=='N') {
			t = 0x04 & status;
			if(t==0x04) SETBIT(walls[curX][curY],0x02)
			t = 0x02 & status;
			if(t==0x02) SETBIT(walls[curX][curY],0x08)
			t = 0x01 & status;
			if(t==0x01) SETBIT(walls[curX][curY],0x04)
		}
		if(curHead=='E') {
			t = 0x04 & status;
			if(t==0x04) SETBIT(walls[curX][curY],0x08)
			t = 0x02 & status;
			if(t==0x02) SETBIT(walls[curX][curY],0x04)
			t = 0x01 & status;
			if(t==0x01) SETBIT(walls[curX][curY],0x01)
		}
		if(curHead=='W') {
			t = 0x04 & status;
			if(t==0x04) SETBIT(walls[curX][curY],0x01)
			t = 0x02 & status;
			if(t==0x02) SETBIT(walls[curX][curY],0x02)
			t = 0x01 & status;
			if(t==0x01) SETBIT(walls[curX][curY],0x08)
		}
		if(curHead=='S') {
			t = 0x04 & status;
			if(t==0x04) SETBIT(walls[curX][curY],0x04)
			t = 0x02 & status;
			if(t==0x02) SETBIT(walls[curX][curY],0x01)
			t = 0x01 & status;
			if(t==0x01) SETBIT(walls[curX][curY],0x02)
		}
		
		// Wall map updated
		
		reflood(curX,curY);
		// Re-flooded
		
		// Check alignment and align
		align();
		
		//Make the next move:
		char flag=0;
		
		// Either move north
		if(walls(x,y) & 0x08==0x00)
			if(flood(x,y+1) < flood(x,y) && flag==0) {
				if(curHead=='N') {
					fwd(212, 15);
				}
				else if (curHead=='E') {
					left();
					fwd(212,15);
				}
				else if (curHead=='W') {
					right();
					fwd(212,15);
				}
				else {
					uturn();
					fwd(212,15);
				}
				curHead = 'N';
				curY+=1;
				flag = 1;
		}
		
		// or move east
		if(walls(x,y) & 0x04==0x00)
			if(flood(x+1,y) < flood(x,y) && flag==0) {
				if(curHead=='E') {
					fwd(212, 15);
				}
				else if (curHead=='S') {
					left();
					fwd(212,15);
				}
				else if (curHead=='N') {
					right();
					fwd(212,15);
				}
				else {
					uturn();
					fwd(212,15);
				}
				curHead = 'E';
				curX+=1;
				flag = 1;
		}
		
		// or move west
		if(walls(x,y) & 0x02==0x00)
			if(flood(x-1,y) < flood(x,y) && flag==0) {
				if(curHead=='W') {
					fwd(212, 15);
				}
				else if (curHead=='N') {
					left();
					fwd(212,15);
				}
				else if (curHead=='S') {
					right();
					fwd(212,15);
				}
				else {
					uturn();
					fwd(212,15);
				}
				curHead = 'W';
				curX-=1;
				flag = 1;
		}
		// or move south
		if(walls(x,y) & 0x01==0x00)
			if(flood(x,y-1) < flood(x,y) && flag==0) {
				if(curHead=='S') {
					fwd(212, 15);
				}
				else if (curHead=='W') {
					left();
					fwd(212,15);
				}
				else if (curHead=='E') {
					right();
					fwd(212,15);
				}
				else {
					uturn();
					fwd(212,15);
				}
				curHead = 'S';
				curY-=1;
				flag = 1;
		}
		
		// display current X and Y on the LCD
		char str[32];
		unsigned int X = curX;
		X+=48;
		unsigned int Y = curY;
		Y+=48;
		
		str[0] = 'X';
		str[1] = ':';
		str[2] = ' ';
		str[4] = (X%10) + 48; 
		X/=10;
		str[3] = (char) X+48;
		
		str[5]= ' ';
		
		str[6] = 'Y';
		str[7] = ':';
		str[8] = ' ';
		str[10] = (Y%10) + 48; 
		Y/=10;
		str[11] = (char)Y+48;
		lcd_string(str);
		
		// display complete
		
		if((curX==7 || curX==8) && (curY==7 || curY==8))
			centreFound = 1;
	}
	
	
	return 0;
}

