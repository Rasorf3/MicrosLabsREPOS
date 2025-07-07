/*
 * UltraSonicModule.h
 *
 * Created: 3/29/2025 8:17:06 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 

#define F_CPU 16000000UL

#ifndef ULTRASONICMODULE_H_
#define ULTRASONICMODULE_H_
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#define TRIGGER 2
#define ECHO 3

typedef enum
{
	STATE_IDLE,
	STATE_TRIGGER_SENT,
	STATE_MEASURING,
	STATE_READY,
}UltraState;

static uint16_t start_time; 
static uint16_t end_time;
volatile UltraState current_state;
volatile float distance_cm;

void UltraSonicInit();

void SendTrigger();

uint16_t  measure_pulse_width();

void UltraSonic_Display_Data(float average);

#endif /* ULTRASONICMODULE_H_ */