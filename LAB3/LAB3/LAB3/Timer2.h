/*
 * Timer2.h
 *
 * Created: 4/12/2025 11:42:58 AM
 *  Author: rasor
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_

volatile unsigned int overflow_count2;

void Timer2_Init();

unsigned int Timer2_getTime();

unsigned char Timer2_getCount();

void Timer2_reset(); 

void Timer2_Stop();

void Timer2_Start();

#endif /* TIMER2_H_ */