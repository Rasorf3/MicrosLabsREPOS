/*
 * USART_Module.h
 *
 * Created: 4/12/2025 9:07:34 PM
 *  Author: rasor
 */ 


#ifndef USART_MODULE_H_
#define USART_MODULE_H_

#include <stdint.h>
#include <stdbool.h>

// Pines de TX/RX (port D)
#define TX_PORT       PD1
#define RX_PORT       PD0

// Tamaño de buffers circulares
#define USART_TX_BUFFER_SIZE  64
#define USART_RX_BUFFER_SIZE  64

/**
 * Inicializa la UART:
 *  - 8 bits, sin paridad, 1 stop bit
 *  - 9600 bps @ 16 MHz
 *  - Habilita interrupciones RX y Data Register Empty
 */
void USART_Init(void);

/**
 * Envía 'len' bytes apuntados por 'data' de forma asíncrona.
 * Devuelve true si todo cabe en el buffer y se inició el envío.
 * Si el buffer TX está lleno devuelve false.
 */
bool USART_SendBuffer(const uint8_t *data, uint8_t len);

/**
 * Intenta leer un byte recibido.
 * Si hay dato disponible devuelve true y lo escribe en *c.
 * Si no hay nada devuelve false.
 */
bool USART_ReceiveByte(uint8_t *c);

#endif // USART_MODULE_H_
