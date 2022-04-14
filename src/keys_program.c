/*
 * keys_program.c
 *
 * Created: 4/11/2022 1:51:55 AM
 *  Author: mmyra
 */ 
#include <xc.h>
#include "../inc/keys_interface.h"


void keys_init()
{
	DDRA	= 0xF;	//configure as input
	PORTA	= 0xF;	//set the pins to internal pull up resistance
}

char keys_Feedback()
{
	if((PINA & (1<<0)) == 0)
	{
		return keys_up;
	}
	else if((PINA & (1<<1)) == 0)
	{
		return keys_down;
	}
	else if((PINA & (1<<2)) == 0)
	{
		return keys_ok;
	}
	else if((PINA & (1<<3)) == 0)
	{
		return keys_cancel;
	}
	else
	return keys_non;
}