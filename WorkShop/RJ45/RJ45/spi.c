/*
 * spi.c
 *
 * Created: 4/07/2025 12:27:31 p. m.
 *  Author: rasor
 */ 
#include "spi.h"

void SPI_Init() {
	// Configurar MOSI, SCK y CS como salidas, MISO como entrada
	DDRB |= (1 << PB5) | (1 << PB7) | (1 << ENC28J60_CS_PIN);
	DDRB &= ~(1 << PB6);
	
	// Habilitar SPI, Maestro, Clock = Fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	// Activar CS (inicialmente alto)
	ENC28J60_CS_PORT |= (1 << ENC28J60_CS_PIN);
}

uint8_t SPI_Transfer(uint8_t data) {
	SPDR = data; // Cargar dato en el registro SPI
	while (!(SPSR & (1 << SPIF))); // Esperar a que termine la transmisión
	return SPDR; // Leer dato recibido
}