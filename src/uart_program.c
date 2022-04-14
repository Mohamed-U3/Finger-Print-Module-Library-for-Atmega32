/*
 * uart_program.c
 *
 * Created: 3/31/2022 5:38:00 PM
 *  Author: mmyra
 */ 

#include <xc.h>
#include "../inc/UART_interface.h"
#include "avr/interrupt.h"

ISR(USART_RXC_vect)
{
	//Read the data from buffer
	rcvData[cont++] = UDR;
	//Clear the interrupt flag
	UCSRA|=(1<<RXC);
}

void clearArray(unsigned char *str)
{
	while(*str)
	{
		*str = 0;
		*str++;
	}
}

void UART_Init(unsigned long baud)
{
	unsigned int UBRR;
	/*Baud rate calculator*/
	UBRR = (F_CPU/(16*baud))-1;
	UBRRH = (unsigned char)(UBRR>>8);
	UBRRL = (unsigned char)UBRR;
	/*Enable the transmitter and receiver with receiver
	complete interrupt*/
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/*asynchronous mode, 8-bit, 1-stop bit*/
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

void UART_InterrptInit(unsigned long baud)
{
	unsigned int UBRR;
	/*Baud rate calculator*/
	UBRR = (F_CPU/(16*baud))-1;
	UBRRH = (unsigned char)(UBRR>>8);
	UBRRL = (unsigned char)UBRR;
	/*Enable the transmitter and receiver with receiver
	complete interrupt*/
	UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
	/*asynchronous mode, 8-bit, 1-stop bit*/
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	ei();
}

unsigned char UART_Receive()
{
	while ((UCSRA & (1 << RXC)) == 0);	// Wait till data is received
	return(UDR);						// Return the byte
}

void UART_Transmit(unsigned char data)
{
	/*Stay here until the buffer is empty*/
	while(!(UCSRA&(1<<UDRE)));
	/*Put the data into the buffer*/
	UDR=data;
}

void UART_Array(const char *data,int size)
{
	for(int i = 0; i<size; i++)
	{
		UART_Transmit(*data);
		*data++;
	}
}

void UART_String(const char *data)
{
	while(*data) UART_Transmit(*data++);
}