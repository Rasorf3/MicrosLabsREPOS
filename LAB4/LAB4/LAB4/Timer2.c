/*
 * Timer2.c
 *
 * Created: 4/12/2025 11:43:07 AM
 *  Author: rasor
 */ 
#include "Timer2.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

void Timer2_Init()
{
	TCCR2A = 0; //Modo normal
	
	
	TIMSK2 |= (1 << TOIE2);
	TCCR2B = (0 << CS22) | (1 << CS21) | (0<< CS20); //preescaler 8 Cada TICK 0.5us
	TCNT2 = 0; //Reiniciar Timer
	overflow_count2 = 0; //Reiniciar Overflow
}

unsigned int Timer2_getTime()
{

	unsigned int tiempo_us = (overflow_count2 * 128) + (unsigned int)(Timer2_getCount() * 0.5);
	return tiempo_us;
}
ISR(TIMER2_OVF_vect)
{
	overflow_count2++;
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

void Timer2_Stop()
{
	TCCR2B = 0;
}

void Timer2_Start()
{
	TCCR2B = (0 << CS22) | (1 << CS21) | (0<< CS20);
}