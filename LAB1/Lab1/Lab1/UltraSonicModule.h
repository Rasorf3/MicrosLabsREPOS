/*
 * UltraSonicModule.h
 *
 * Created: 3/29/2025 8:17:06 PM
 *  Author: Luis Felipe Holchor Virgen
 */ 


#ifndef ULTRASONICMODULE_H_
#define ULTRASONICMODULE_H_
#include <avr/io.h>
#include <util/delay.h>
#define TRIGGER 4
#define ECHO 5

void UltraSonicInit();
float GetDistance();




#endif /* ULTRASONICMODULE_H_ */