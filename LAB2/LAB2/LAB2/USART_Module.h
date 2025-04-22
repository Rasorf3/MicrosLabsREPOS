/*
 * USART_Module.h
 *
 * Created: 4/12/2025 9:07:34 PM
 *  Author: rasor
 */ 


#ifndef USART_MODULE_H_
#define USART_MODULE_H_

#include <avr/io.h>

#define TX_PORT 1
#define RX_PORT 0

#define BUFFER_SIZE 64 // Tama?o m?ximo del buffer
char buffer[BUFFER_SIZE]; // Buffer para almacenar la cadena

void USART_Init();
int isUSARTudrEmpty();
void setUSARTudr(unsigned char dataTX);
void sendStringUSART(char *str);
unsigned char getUSARTdata();
int isUSARTerror();
int isUSARTrxComplete();
void UART_receive_string();

#endif /* USART_MODULE_H_ */