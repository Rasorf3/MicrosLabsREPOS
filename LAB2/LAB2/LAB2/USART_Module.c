/*
 * USART_Module.c
 *
 * Created: 4/12/2025 9:07:42 PM
 *  Author: rasor
 */ 
#include "USART_Module.h"

unsigned char buffer_pos = 0;   // Posici?n actual en el buffer

void USART_Init()
{
	DDRD |= (1 << TX_PORT);
	DDRD &= ~(1 << RX_PORT);
	
	//CONTROL AND STATUS REGISTER UCSRB
	//UCSRB |= (1 << RXCIE); // RX Interrupt enable
	//UCSRB |= (1 << TXCIE); // TX Interrupt enable
	//UCSRB |= (1 << UDRIE); // USART Data Register Empty Interrupt Enable
	
	UCSR0B |= (1 << TXEN0); // Enable TX
	UCSR0B |= (1 << RXEN0); // Enable RX
	
	
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	//Baud Rate Register UBRRL UBRRH
	// 9600 Baud Rate 16MHZ clock
	UBRR0H = 0x00;
	UBRR0L = 0x67;
	
	//Control & Status Register UCSRA
	UCSR0A = 0x00;
}
int isUSARTudrEmpty()
{
	if((UCSR0A & (1 << UDRE0))){
		return 1;
	}
	else{
		return 0;
	}
}
void setUSARTudr(unsigned char dataTX)
{
	while(!isUSARTudrEmpty());
	UDR0 = dataTX;
}
void sendStringUSART(char *str)
{
	while (*str) {
		setUSARTudr(*str);
		str++;
	}
}
int isUSARTrxComplete()
{
	if ((UCSR0A & (1 << RXC0)))
	return 1;
	else
	return 0;
}

int isUSARTerror()
{
	if (((UCSR0A & (1 << FE0)) | (UCSR0A & (1 << DOR0)) | (UCSR0A & (1 << UPE0))))
	return 1;
	else
	return 0;
}

unsigned char getUSARTdata()
{
	while (!isUSARTrxComplete());
	//while(!isUSARTerror());
	return UDR0;
}
void UART_receive_string() {
	char received_char;
	buffer_pos = 0;
	memset(buffer, 0, BUFFER_SIZE); // Limpiar buffer

	do {
		received_char = getUSARTdata(); // Lee car?cter
		if (received_char != '\n' && received_char != '\r' && buffer_pos < BUFFER_SIZE - 1) {
			buffer[buffer_pos++] = received_char; // Almacena en buffer
		}
	} while (received_char != '\n' && received_char != '\r' && buffer_pos < BUFFER_SIZE - 1);

	buffer[buffer_pos] = '\0'; // Terminador nulo (para que sea string v?lido en C)
}