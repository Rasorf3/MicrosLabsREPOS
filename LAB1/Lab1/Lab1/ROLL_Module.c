/*
 * ROLL_Module.c
 *
 * Created: 4/5/2025 9:52:08 PM
 *  Author: rasor
 */ 
#include "ROLL_Module.h"

int ReadRollPin()
{
	if(PIND & (1 << ROLL_PIN))
	{
		return 1;
	}
	return 0;
}
void Roll_Init()
{
	DDRD &= ~(1 << ROLL_PIN);
}