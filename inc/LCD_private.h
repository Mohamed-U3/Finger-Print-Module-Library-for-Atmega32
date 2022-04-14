/*
 * LCD_private.h
 *
 * Created: 3/23/2022 5:23:34 PM
 *  Author: mmyra
 */ 

#include <xc.h>

#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

#define uchar		unsigned char
#define uint 		unsigned int

#define RSLow		(LCDPORT&=~(1<<rs))
#define RSHigh	 	(LCDPORT|= (1<<rs))
#define RWLow 		(LCDPORT&=~(1<<rw))
#define ENLow		(LCDPORT&=~(1<<en))
#define ENHigh		(LCDPORT|= (1<<en))


#endif /* LCD_PRIVATE_H_ */