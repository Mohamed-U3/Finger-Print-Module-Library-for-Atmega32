/*
 * main.c
 *
 * Created: 3/23/2022 8:09:16 PM
 * last modification : 4/14/2022 1:49:16 AM
 *  Author: Mohamed U3
 */
#define F_CPU 12000000UL
#include <xc.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "inc/LCD_interface.h"
#include "inc/UART_interface.h"
#include "inc/FP_interface.h"
#include "inc/keys_interface.h"

int main()
{
	UART_InterrptInit(57600);
	lcdbegin();
	keys_init();
	
	
	lcdclear();
	lcdprint("Loading.. ");
	_delay_ms(500);
	sendcmd2fb(FB_connect);
	_delay_ms(500);
	unsigned char options_conter = 0;
	lcdclear();
	
	while(1)
	{
		lcdfirstrow();
		lcdprint("Options: ");
		lcdwrite(0x7f,DATA);
		lcdwrite(0x7e,DATA);
		
		if(keys_Feedback() == keys_up)
		options_conter++;
		else if(keys_Feedback() == keys_down)
		options_conter--;
		else if(keys_Feedback() == keys_ok)
		{
			if(options_conter == 0)
			Search();
			else if(options_conter == 1)
			enroll();
			else if(options_conter == 2)
			sendcmd2fb(F_delete);
			else if(options_conter == 3)
			sendcmd2fb(FP_empty);
		}
		if(options_conter > 3)
		options_conter = 0;
		
		lcdsecondrow();
		if(options_conter == 0)
		lcdprint("-Scan           ");
		else if(options_conter == 1)
		lcdprint("-Enroll         ");
		else if(options_conter == 2)
		lcdprint("-Delete ID      ");
		else if(options_conter == 3)
		lcdprint("-Erase IDs      ");
		
		_delay_ms(100);
	}
}