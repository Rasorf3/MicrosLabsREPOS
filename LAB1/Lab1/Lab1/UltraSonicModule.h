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

#define TRIGGER 2
#define ECHO 3

void UltraSonicInit();
float GetDistance();
unsigned int measure_pulse_width();
float TrashPercent(float distance);
void UltraSonic_Display_Data();

#endif /* ULTRASONICMODULE_H_ */