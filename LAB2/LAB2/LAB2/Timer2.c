/*
 * Timer2.c
 *
 * Created: 4/12/2025 11:43:07 AM
 *  Author: rasor
 */ 
#include "Timer2.h"

volatile unsigned int overflow_count2 = 0;

void Timer2_Init()
{
	TCCR2A = 0; //Modo normal
	TCCR2B = (0 << CS22) | (1 << CS21) | (0<< CS20); //preescaler 8 Cada TICK 0.5us
	TCNT2 = 0; //Reiniciar Contador
	TIFR2 |= (1 << TOV2); //Reiniciar OVERFLOW FLAG
}

unsigned int Timer2_getTime()
{
	if(TIFR2 & (1 << TOV2))
	{
		overflow_count2++;
		TIFR2 |= (1 << TOV2);
	}
	unsigned long int tiempo_us = (overflow_count2 * 128) + (Timer2_getCount() * 0.5);
	return tiempo_us;
}

unsigned char Timer2_getCount()
{
	return TCNT2;
}

void Timer2_reset()
{
	TCNT2 = 0;
	overflow_count2 = 0;
}