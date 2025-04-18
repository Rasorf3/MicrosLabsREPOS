/*
 * Timer1.c
 *
 * Created: 2/04/2025 2:16:30 p. m.
 *  Author: rasor
 */ 
#include "Timer1.h"

#include <avr/io.h>

// Función para configurar Timer1
void Timer1_Init() {
	TCCR1A = 0;             // Modo normal
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);   // Prescaler 1024
	TCCR1B &= ~(1 << WGM13);
	TCCR1B &= ~(1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);
}

unsigned long int  Timer1_getTime()
{
	unsigned long int tiempo_us = (unsigned long int)Timer1_getCount() * 64;
	return tiempo_us;
}

unsigned int Timer1_getCount()
{
	return TCNT1;
}

void Timer1_reset()
{
	TCNT1 = 0;
}