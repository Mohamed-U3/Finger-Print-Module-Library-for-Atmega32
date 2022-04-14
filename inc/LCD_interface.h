/*
 * LCD_interface.h
 *
 * Created: 3/23/2022 5:21:46 PM
 *  Author: mmyra
 */ 

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

#define LCDPORTDIR 	DDRB
#define LCDPORT 	PORTB
#define rs 			0
#define rw			1
#define en 			2

enum
{
	CMD=0,
	DATA,
};

void lcdwrite(char ch,char r);
void lcdprint(const char *str);
void lcdgotoxy(int x, int y);
void lcdclear();
void lcdfirstrow();
void lcdsecondrow();
void lcdbegin();


#endif /* LCD_INTERFACE_H_ */