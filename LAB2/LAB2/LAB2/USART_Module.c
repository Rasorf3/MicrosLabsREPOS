/*
 * USART_Module.c
 *
 * Created: 4/12/2025 9:07:42 PM
 *  Author: rasor
 */ 
#include "USART_Module.h"

void USART_Init()
{
	DDRD |= (1 << TX_PORT);
	DDRD &= ~(1 << RX_PORT);
	
	//CONTROL AND STATUS REGISTER UCSRB
	UCSR0B |= (1 << RXCIE0); // RX Interrupt enable
	UCSR0B |= (1 << TXCIE0); // TX Interrupt enable
	//UCSRB |= (1 << UDRIE); // USART Data Register Empty Interrupt Enable
	
	UCSR0B |= (1 << TXEN0); // Enable TX
	UCSR0B |= (1 << RXEN0); // Enable RX
	
	
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	//Baud Rate Register UBRRL UBRRH
	// 9600 Baud Rate 16MHZ clock
	UBRR0H = 0x00;
	UBRR0L = 0x67;
}

void setUSARTudr(unsigned char dataTX)
{
	UDR0 = dataTX;
}
void sendStringUSART(char *str)
{
	if(indexBuffer == 0)
	{
		setUSARTudr(str[indexBuffer]);
		indexBuffer++;
	}
	if(txFlag)
	{
		txFlag = 0;
		if(str[indexBuffer] != '\0')
		{
			setUSARTudr(str[indexBuffer]);
			indexBuffer++;
		}
		else
		{
			indexBuffer = 0;
			txFlag = 1;
		}
		
		
	}
}

unsigned char getUSARTdata()
{
	return UDR0;
}

ISR(USART_RX_vect)
{
	rxFlag = 1;
	dataRX = getUSARTdata();
	
}
ISR(USART_TX_vect)
{
	txFlag = 1;
}