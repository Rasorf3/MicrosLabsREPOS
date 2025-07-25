/*
 * configModule.h
 *
 * Created: 22/07/2025 9:49:29 a. m.
 *  Author: rasor
 */ 


#ifndef CONFIGMODULE_H_
#define CONFIGMODULE_H_

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

/* Prototipos */
void vInit(void);
void vCreatingTasks(UBaseType_t uxPriority);

#endif /* CONFIGMODULE_H_ */

