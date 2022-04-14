/*
 * uart_interface.h
 *
 * Created: 3/31/2022 5:40:17 PM
 *  Author: mmyra
 */ 


#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#define F_CPU 12000000UL

volatile char cont;
volatile char rcvData[15];

void clearArray(unsigned char *str);

void UART_Init(unsigned long baud);
void UART_InterrptInit(unsigned long baud);
unsigned char UART_Receive();
void UART_Array(const char *data,int size);
void UART_Transmit(unsigned char data);
void UART_String(const char *data);

#endif /* UART_INTERFACE_H_ */