/*
 * USART_Module.c
 *
 * Created: 4/12/2025 9:07:42 PM
 *  Author: rasor
 */ 
#include "USART_Module.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// Buffers circulares
static volatile uint8_t tx_buf[USART_TX_BUFFER_SIZE];
static volatile uint8_t tx_head, tx_tail;

static volatile uint8_t rx_buf[USART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head, rx_tail;

void USART_Init(void)
{
	// Configuro pines
	DDRD |=  (1 << TX_PORT);   // TX como salida
	DDRD &= ~(1 << RX_PORT);   // RX como entrada

	// Habilito RX Complete Interrupt y UDR Empty Interrupt deshabilitado por ahora
	UCSR0B  = (1 << RXCIE0)    // RX complete ISR
	| (0 << UDRIE0)    // UDR Empty ISR (se habilita al enviar)
	| (1 << RXEN0)     // RX Enable
	| (1 << TXEN0);    // TX Enable

	// 8 bits, sin paridad, 1 stop bit
	UCSR0C  = (1 << UCSZ01) | (1 << UCSZ00);

	// Baud rate 9600 @ F_CPU=16MHz: UBRR = 103 decimal = 0x0067
	UBRR0H = 0x00;
	UBRR0L = 0x67;

	// Inicializo índices
	tx_head = tx_tail = 0;
	rx_head = rx_tail = 0;

	// Habilito interrupciones globales
	sei();
}

bool USART_SendBuffer(const uint8_t *data, uint8_t len)
{
	uint8_t i, next;

	// Intenta poner cada byte en el buffer
	for (i = 0; i < len; i++) {
		next = (tx_head + 1) % USART_TX_BUFFER_SIZE;
		if (next == tx_tail) {
			// buffer lleno
			return false;
		}
		tx_buf[tx_head] = data[i];
		tx_head = next;
	}

	// Habilito la interrupción Data Register Empty para arrancar envío
	UCSR0B |= (1 << UDRIE0);
	return true;
}

bool USART_ReceiveByte(uint8_t *c)
{
	if (rx_tail == rx_head) {
		// vacío
		return false;
	}
	*c = rx_buf[rx_tail];
	rx_tail = (rx_tail + 1) % USART_RX_BUFFER_SIZE;
	return true;
}

// **************************************************
// ISR: UDR Empty ? podemos cargar el siguiente byte TX
// **************************************************
ISR(USART_UDRE_vect)
{
	if (tx_tail == tx_head) {
		// nada que enviar: deshabilito ISR
		UCSR0B &= ~(1 << UDRIE0);
		} else {
		// carga siguiente byte
		UDR0 = tx_buf[tx_tail];
		tx_tail = (tx_tail + 1) % USART_TX_BUFFER_SIZE;
	}
}

// **************************************************
// ISR: RX Complete ? guardo UDR0 en buffer RX
// **************************************************
ISR(USART_RX_vect)
{
	uint8_t next = (rx_head + 1) % USART_RX_BUFFER_SIZE;
	uint8_t d    = UDR0;

	// Si no hay overflow, guardo
	if (next != rx_tail) {
		rx_buf[rx_head] = d;
		rx_head = next;
	}
}
