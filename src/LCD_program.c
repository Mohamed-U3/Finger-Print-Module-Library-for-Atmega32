/*
 * LCD_program.c
 *
 * Created: 3/23/2022 5:20:48 PM
 *  Author: mmyra
 */ 

#include "../inc/LCD_private.h"
#include "../inc/LCD_interface.h"
#include <util/delay.h>

void lcdwrite(char ch,char r)
{
	LCDPORT=ch & 0xF0;
	RWLow;
	if(r == 1)
	RSHigh;
	else
	RSLow;
	ENHigh;
	_delay_ms(5);
	ENLow;
	_delay_ms(15);
	
	LCDPORT=ch<<4 & 0xF0;
	RWLow;
	if(r == 1)
	RSHigh;
	else
	RSLow;
	ENHigh;
	_delay_ms(5);
	ENLow;
	_delay_ms(15);
}

void lcdprint(const char *str)
{
	while(*str)
	{
		lcdwrite(*str++,DATA);
	}
}

void lcdgotoxy(int x, int y)
{
	y--;
	if (x==1)
	{
		lcdwrite(0x80+y, CMD);
	}
	if (x==2)
	{
		lcdwrite(0xC0+y, CMD);
	}
}

void lcdclear()
{
	lcdwrite(0x01, CMD);
}

void lcdfirstrow()
{
	lcdwrite(0x80, CMD);
}

void lcdsecondrow()
{
	lcdwrite(0xC0, CMD);
}

void lcdbegin()
{
	LCDPORTDIR	=	0xFF;
	uchar lcdcmd[5]={0x02,0x28,0x0E,0x06,0x01};
	uint i=0;
	for(i=0;i<5;i++)
	{
		lcdwrite(lcdcmd[i], CMD);
	}
	_delay_ms(200);
}