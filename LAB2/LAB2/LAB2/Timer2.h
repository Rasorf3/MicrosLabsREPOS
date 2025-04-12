/*
 * Timer2.h
 *
 * Created: 4/12/2025 11:42:58 AM
 *  Author: rasor
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_

#include <avr/io.h>



void Timer2_Init();

unsigned int Timer2_getTime();

unsigned char Timer2_getCount();

void Timer2_reset(); 

#endif /* TIMER2_H_ */