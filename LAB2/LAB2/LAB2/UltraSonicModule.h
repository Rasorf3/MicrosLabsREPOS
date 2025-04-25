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

volatile uint16_t start_time; 
volatile uint16_t end_time;
volatile UltraState current_state;

void UltraSonicInit();

void SendTrigger();


float GetDistance();
uint16_t  measure_pulse_width();
float TrashPercent(float distance);
void UltraSonic_Display_Data(float average);
float UltraSonic_AvarageData(uint8_t counter,float dataUltra);
#endif /* ULTRASONICMODULE_H_ */