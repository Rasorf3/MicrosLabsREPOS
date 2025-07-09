/*
 * i2c_async.c
 *
 * Created: 9/07/2025 11:12:10 a. m.
 *  Author: rasor
 */ 
#include "i2c_async.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define START_COND  ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE))
#define ENABLE_INT  ((1<<TWINT)|(1<<TWEN)|(1<<TWIE))
#define STOP_COND   ((1<<TWINT)|(1<<TWEN)|(1<<TWSTO))

static struct {
	volatile bool        busy;
	volatile uint8_t     addr;
	volatile uint8_t    *buf;
	volatile uint8_t     len;
	volatile uint8_t     idx;
	volatile i2c_callback_t cb;
} i2c_ctx;

void i2c_async_init(void) {
	TWSR = 0x00;      // prescaler = 1
	TWBR = 72;        // 100kHz @16MHz
	i2c_ctx.busy = false;
	sei();            // interrupciones globales
}

I2C_Result_t i2c_async_write(uint8_t address,
uint8_t *tx_buf, uint8_t len,
i2c_callback_t cb)
{
	if (i2c_ctx.busy) return I2C_BUSY;
	i2c_ctx.busy = true;
	i2c_ctx.addr = address & 0xFE; // write
	i2c_ctx.buf  = tx_buf;
	i2c_ctx.len  = len;
	i2c_ctx.idx  = 0;
	i2c_ctx.cb   = cb;
	TWCR = START_COND;
	return I2C_OK;
}

I2C_Result_t i2c_async_read(uint8_t address,
uint8_t *rx_buf, uint8_t len,
i2c_callback_t cb)
{
	if (i2c_ctx.busy) return I2C_BUSY;
	i2c_ctx.busy = true;
	i2c_ctx.addr = address | 0x01; // read
	i2c_ctx.buf  = rx_buf;
	i2c_ctx.len  = len;
	i2c_ctx.idx  = 0;
	i2c_ctx.cb   = cb;
	TWCR = START_COND;
	return I2C_OK;
}

ISR(TWI_vect)
{
	uint8_t status = TWSR & 0xF8;
	switch (status) {
		case 0x08: // START
		case 0x10: // REPEATED START
		TWDR = i2c_ctx.addr;
		TWCR = ENABLE_INT;
		break;

		case 0x18: // SLA+W ACK
		case 0x28: // DATA TX ACK
		if (i2c_ctx.idx < i2c_ctx.len) {
			TWDR = i2c_ctx.buf[i2c_ctx.idx++];
			TWCR = ENABLE_INT;
			} else {
			TWCR = STOP_COND;
			i2c_ctx.busy = false;
			i2c_ctx.cb(I2C_OK, NULL, 0);
		}
		break;

		case 0x40: // SLA+R ACK
		TWCR = (i2c_ctx.len>1
		? (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE)
		: ENABLE_INT);
		break;

		case 0x50: // DATA RX ACK
		i2c_ctx.buf[i2c_ctx.idx++] = TWDR;
		TWCR = (i2c_ctx.idx < i2c_ctx.len-1
		? (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE)
		: ENABLE_INT);
		break;

		case 0x58: // DATA RX NACK (último)
		i2c_ctx.buf[i2c_ctx.idx++] = TWDR;
		TWCR = STOP_COND;
		i2c_ctx.busy = false;
		i2c_ctx.cb(I2C_OK, i2c_ctx.buf, i2c_ctx.len);
		break;

		default: // error o NACK
		TWCR = STOP_COND;
		i2c_ctx.busy = false;
		i2c_ctx.cb(I2C_ERROR_BUS, NULL, 0);
		break;
	}
}
