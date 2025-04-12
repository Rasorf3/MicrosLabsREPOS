/*
 * Timer0.c
 *
 * Created: 4/12/2025 1:07:55 PM
 *  Author: rasor
 */ 
#include "Timer0.h"

volatile unsigned int overflow_count0 = 0;

void Timer0_Init()
{
	TCCR0A = 0; //Modo normal
	TCCR0B |= (1 << CS02) | (0 << CS01) | (1<< CS00); //preescaler 1024 Cada TICK 64us
	TCCR0B &= ~(1 << WGM02);
	TCNT0 = 0; //Reiniciar Contador
	TIFR0 |= (1 << TOV0); //Reiniciar OVERFLOW FLAG
}

unsigned long int  Timer0_getTime()
{
	if(TIFR0 & (1 << TOV0))
	{
		overflow_count0++;
		TIFR0 |= (1 << TOV0);
	}
	unsigned long int tiempo_us = (overflow_count0 * 16320) + (Timer0_getCount() * 64);
	return tiempo_us;
}

unsigned char Timer0_getCount()
{
	return TCNT0;
}

void Timer0_reset()
{
	TCNT0 = 0;
	overflow_count0 = 0;
}