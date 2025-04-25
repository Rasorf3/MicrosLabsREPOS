/*
 * Timer0.h
 *
 * Created: 4/12/2025 1:08:06 PM
 *  Author: rasor
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

volatile unsigned char overflow_count0;

void Timer0_Init();

unsigned long int  Timer0_getTime();

unsigned char Timer0_getCount();

void Timer0_reset();

unsigned char Timer0_milis(unsigned short int TimeMilis);

void Timer0_Stop();

void Timer0_Start();

#endif /* TIMER0_H_ */