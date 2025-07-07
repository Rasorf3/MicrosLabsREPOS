/*
 * USART_Module.h
 *
 * Created: 4/12/2025 9:07:34 PM
 *  Author: rasor
 */ 


#ifndef USART_MODULE_H_
#define USART_MODULE_H_



#define TX_PORT 1
#define RX_PORT 0

volatile unsigned char txFlag;
volatile unsigned char rxFlag;
volatile unsigned char dataRX;
volatile unsigned char indexBuffer;

void USART_Init();
void setUSARTudr(unsigned char dataTX);
void sendStringUSART(char *str);
unsigned char getUSARTdata();


#endif /* USART_MODULE_H_ */