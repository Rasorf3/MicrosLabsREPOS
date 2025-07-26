/*
 * configModule.h
 *
 * Created: 22/07/2025 9:49:29 a. m.
 *  Author: rasor
 */ 



#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
void vInit(void);

void vCreatingTasks(UBaseType_t uxBasePriority);

#endif // CONFIG_MODULE_H


