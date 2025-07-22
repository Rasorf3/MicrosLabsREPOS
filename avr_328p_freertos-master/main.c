/*
 * templateFRTOS.c
 *
 * Created: 14/05/2013 20:10:58
 *  Author: eBlady
 */ 


//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"


//////////////////////////////////////////////////////////////////////////////////
// Funci?n PRINCIPAL
int main(void)
{
	// Inicializar el micro
	vInit();

	// Arrancar las tareas con un nivel de prioridad dado.
	vStartLEDFlashTask(tskIDLE_PRIORITY+1);

	// Arrancar, el planificador de tareas.
	vTaskStartScheduler();

	return 0;
}

