/*
 * spi.h
 *
 * Created: 4/07/2025 12:27:23 p. m.
 *  Author: rasor
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <util/delay.h>

#define ENC28J60_CS_PORT    PORTB
#define ENC28J60_CS_DDR     DDRB
#define ENC28J60_CS_PIN     PB4

void SPI_Init();
uint8_t SPI_Transfer(uint8_t data);

#endif