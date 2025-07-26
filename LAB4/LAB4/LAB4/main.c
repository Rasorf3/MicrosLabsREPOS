/*
 * templateFRTOS.c
 *
 * Created: 14/05/2013 20:10:58
 *  Author: eBlady
 */ 


//FreeRTOS include files

#include "FreeRTOS.h"
#include "task.h"
#include "configModule.h"

int main(void)
{
	
	vInit();

	
	vCreatingTasks(tskIDLE_PRIORITY + 2);

	
	vTaskStartScheduler();

	
}



