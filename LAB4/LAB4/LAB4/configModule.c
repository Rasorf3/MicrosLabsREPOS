/*
 * configModule.c
 *
 * Created: 22/07/2025 9:49:41 a. m.
 *  Author: rasor
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>        
#include "configModule.h"



static void lcdPulseEnable(void)
{
    PORTC |=  (1 << PC1);
    _delay_us(1);
    PORTC &= ~(1 << PC1);
    _delay_us(1);
}

static void lcdWrite(uint8_t v, uint8_t rs)
{
    if (rs) PORTC |=  (1 << PC0);
    else    PORTC &= ~(1 << PC0);

    PORTB = (PORTB & ~0x3F) | (v & 0x3F);
    PORTD = (PORTD & ~((1<<PD2)|(1<<PD3)))
         | (((v >> 6) & 1) << PD2)
         | (((v >> 7) & 1) << PD3);

    lcdPulseEnable();
}

static void lcdCommand(uint8_t cmd) { lcdWrite(cmd, 0); _delay_us(40); }
static void lcdData   (uint8_t dat) { lcdWrite(dat, 1); _delay_us(40); }

static void lcdClear(void)
{
    lcdCommand(0x01);
    _delay_ms(2);
}

static void lcdSetCursor(uint8_t col, uint8_t row)
{
    static const uint8_t row_off[4] = { 0x00, 0x40, 0x14, 0x54 };
    if (row > 3) row = 3;
    lcdCommand(0x80 | (col + row_off[row]));
}

static void lcdPrint(const char *s)
{
    while (*s) {
        lcdData(*s++);
    }
}

// ——— Tarea: medir y mostrar distancia ———

static void vTaskLCDControl(void *pvParameters)
{
	(void)pvParameters;
	char str_cm[6];

	for (;;)
	{
		uint32_t ticks = 0;
		uint8_t last = TCNT2, now;

		
		PORTC |=  (1 << PC2);
		_delay_us(15);
		PORTC &= ~(1 << PC2);

		while (!(PINC & (1 << PC3)));

		while (PINC & (1 << PC3))
		{
			now = TCNT2;
			if (now >= last)
			ticks += (now - last);
			else
			ticks += (256 - last) + now;
			last = now;
		}

		// 4) Calcular distancia (0.5µs/tick → cm = ticks/116)
		uint16_t dist_cm = ticks / 116;

		// 5) Mostrar en LCD
		lcdClear();
		lcdSetCursor(0, 0);
		lcdPrint("--------------------");
		lcdSetCursor(0, 1);
		lcdPrint("Dist: ");
		utoa(dist_cm, str_cm, 10);
		lcdPrint(str_cm);
		lcdPrint(" cm");
		dist_cm = 0;
		vTaskDelay(2000);
	}
}


void vInit(void)
{
    // LCD
    DDRB |=  0x3F;                     
    DDRD |= (1<<PD2)|(1<<PD3);         
    DDRC |= (1<<PC0)|(1<<PC1);        

    _delay_ms(20);
    lcdCommand(0x38);  _delay_ms(5);
    lcdCommand(0x38);  _delay_us(150);
    lcdCommand(0x38);
    lcdCommand(0x08);  lcdClear();
    lcdCommand(0x06);  lcdCommand(0x0C);

    // HC-SR04
    DDRC |=  (1<<PC2);                 // TRIG out
    DDRC &= ~(1<<PC3);                 // ECHO in
    // no pull-up

    // Timer2 prescaler=8 (0.5 µs/tick)
    TCCR2A = 0;
    TCCR2B = (1<<CS21);
    TCNT2  = 0;
}

void vCreatingTasks(UBaseType_t uxBasePriority)
{
    xTaskCreate(
        vTaskLCDControl,
        "LCDCtrl",
        configMINIMAL_STACK_SIZE + 200,
        NULL,
        uxBasePriority,
        NULL
    );
}






