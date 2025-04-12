/*
 * Timer0.h
 *
 * Created: 4/12/2025 1:08:06 PM
 *  Author: rasor
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_


#include <avr/io.h>
#include <stdint.h>


void Timer0_Init();

unsigned long int  Timer0_getTime();

unsigned char Timer0_getCount();

void Timer0_reset();


#endif /* TIMER0_H_ */