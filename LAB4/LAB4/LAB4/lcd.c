/*
 * lcd.c
 *
 * Created: 7/25/2025 10:26:27 AM
 *  Author: rasor
 */ 
/* Al inicio de lcd.c */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include "lcd.h"
#include <avr/io.h>
#include "FreeRTOS.h"

/* Cola global */
QueueHandle_t xLCDQueue = NULL;


static void lcdPulseEnable(void)
{
	portENTER_CRITICAL();            // cli()
	PORTC |=  (1 << PC1);            // EN = 1
	_delay_us(1);                    // ancho m?nimo de pulso
	PORTC &= ~(1 << PC1);            // EN = 0
	portEXIT_CRITICAL();             // sei()
	_delay_us(100);                  // espera post-pulse
}

/* Escritura de 8 bits at?mica */
static void lcdWrite8(uint8_t value)
{
	portENTER_CRITICAL();
	PORTB = (PORTB & ~0x3F) | (value & 0x3F);
	PORTD = (PORTD & ~((1<<PD2)|(1<<PD3)))
	| (((value & 0xC0) >> 4));
	lcdPulseEnable();
	portEXIT_CRITICAL();
}
void lcdSendCommand(uint8_t cmd)
{
	portENTER_CRITICAL();
	PORTC &= ~(1 << PC0);    // RS = 0
	_delay_us(1);
	lcdWrite8(cmd);
	portEXIT_CRITICAL();
}

void lcdSendData(uint8_t data)
{
	portENTER_CRITICAL();
	PORTC |=  (1 << PC0);    // RS = 1
	_delay_us(1);
	lcdWrite8(data);
	portEXIT_CRITICAL();
}


void lcdSetCursor(uint8_t row, uint8_t col)
{
	static const uint8_t rowAddr[4] = { 0x00, 0x40, 0x14, 0x54 };
	uint8_t addr = row < 4 ? rowAddr[row] + col : rowAddr[0];
	lcdSendCommand(0x80 | addr);
}

void lcdInit(void)
{
    /* PB0?PB5, PD2?PD3, PC0?PC1 como salidas */
    DDRB |=  0x3F;
    DDRD |= (1<<PD2)|(1<<PD3);
    DDRC |= (1<<PC0)|(1<<PC1);

    /* Fija RS=0 y EN=0 para evitar gpulsos fantasmah */
    PORTC &= ~((1<<PC0)|(1<<PC1));

    _delay_ms(40);
    /* c resto de inicializaci?n c */
	lcdSendCommand(0x38); /* 8-bit, 2 l?neas */
	lcdSendCommand(0x0C); /* Display ON, cursor OFF */
	lcdSendCommand(0x06); /* Entry mode */
	lcdSendCommand(0x01); /* Clear */
	_delay_ms(2);
}

void vTaskLCD(void *pv)
{
	LCDMessage_t msg;


	/* Crea la cola si no existe */
	if (xLCDQueue == NULL) {
		xLCDQueue = xQueueCreate(10, sizeof(LCDMessage_t));
		configASSERT(xLCDQueue);
	}

	for (;;)
	{
		if (xQueueReceive(xLCDQueue, &msg, portMAX_DELAY) == pdPASS)
		{
			/* Toggle LED para confirmar recepci?n */
			

			lcdSetCursor(msg.row, msg.col);
			for (char *p = msg.text; *p; p++) {
				lcdSendData((uint8_t)*p);
			}
		}
	}
}

