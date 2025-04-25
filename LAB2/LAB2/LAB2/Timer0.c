/*
 * Timer0.c
 *
 * Created: 4/12/2025 1:07:55 PM
 *  Author: rasor
 */ 
#include "Timer0.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>


void Timer0_Init()
{
	TCCR0A = 0; //Modo normal
	TCCR0B = 0; //Stop Timer
	
	TIMSK0 |= (1 << TOIE0);
	
	TCCR0B |= (1 << CS02) | (0 << CS01) | (1<< CS00); //preescaler 1024 Cada TICK 64us
	TCNT0 = 0; //Reiniciar Contador
	overflow_count0 = 0; //Reiniciar OverFlow
}
ISR(TIMER0_OVF_vect)
{
	overflow_count0++;
}
unsigned long int  Timer0_getTime()
{
	unsigned long int tiempo_us = ((unsigned long int)overflow_count0 * 16320UL) + ((unsigned long int)Timer0_getCount() * 64UL);
	return tiempo_us;
}
unsigned char Timer0_milis(unsigned short int TimeMilis)
{
	if(TimeMilis <= (unsigned short int)(Timer0_getTime()/1000))
	{
		return 1;
	}
	else
	{
		return 0;
	}
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