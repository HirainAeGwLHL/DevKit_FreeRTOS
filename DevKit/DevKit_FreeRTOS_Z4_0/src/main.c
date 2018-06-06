/*==================================================================================================
*
*   (c) Copyright 2015 Freescale Semiconductor Inc.
*
*   This program is free software; you can redistribute it and/or modify it under
*   the terms of the GNU General Public License (version 2) as published by the
*   Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.
*
*   ***************************************************************************
*   >>!   NOTE: The modification to the GPL is included to allow you to     !<<
*   >>!   distribute a combined work that includes FreeRTOS without being   !<<
*   >>!   obliged to provide the source code for proprietary components     !<<
*   >>!   outside of the FreeRTOS kernel.                                   !<<
*   ***************************************************************************
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
==================================================================================================*/

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bsp.h"
#include "hw_platform.h"

/* Standard demo includes. */

#define mainLED_DELAY                        ( ( TickType_t ) 500 / portTICK_PERIOD_MS )
#define mainLED_TASK_PRIORITY                ( tskIDLE_PRIORITY + 1 )


extern void SystemInit(void);

void vLEDTask( void *pvParameters );


#if configUSE_TICK_HOOK > 0
void vApplicationTickHook(void);

volatile uint32_t ui32_ms_cnt = 0;

void vApplicationTickHook(void)
{
  // increments every 1ms if configCPU_CLOCK_HZ is set correctly
  ui32_ms_cnt++;

  /* The full demo includes a software timer demo/test that requires
  prodding periodically from the tick interrupt. */
  vTimerPeriodicISRTests();

  /* Call the periodic queue overwrite from ISR demo. */
  vQueueOverwritePeriodicISRDemo();

  /* Call the periodic event group from ISR demo. */
  vPeriodicEventGroupsProcessing();

  /* Use task notifications from an interrupt. */
  xNotifyTaskFromISR();

  /* Use mutexes from interrupts. */
  vInterruptSemaphorePeriodicTest();
}
#endif

void vLEDTask(void *pvParameters)
{
	unsigned int led = (unsigned int) pvParameters;

	for (;;)
	{
    /* Not very exiting - just delay... */
    vTaskDelay((500UL / portTICK_PERIOD_MS));

    /* set the GIO pin to 1 */
    Led_On(led);

    /* delay */
    vTaskDelay((500UL / portTICK_PERIOD_MS));

    /* clear the GIO pin */
    Led_Off(led);
	}
}


int main(void)
{
	BSP_Init();

	OS_PlatformInit();

	xTaskCreate(vLEDTask, (const char * const ) "LedTask1", configMINIMAL_STACK_SIZE, (void*) 0x0, mainLED_TASK_PRIORITY,
			NULL);
	xTaskCreate(vLEDTask, (const char * const ) "LedTask2", configMINIMAL_STACK_SIZE, (void*) 0x1, mainLED_TASK_PRIORITY,
			NULL);
	xTaskCreate(vLEDTask, (const char * const ) "LedTask3", configMINIMAL_STACK_SIZE, (void*) 0x2, mainLED_TASK_PRIORITY,
			NULL);
	xTaskCreate(vLEDTask, (const char * const ) "LedTask4", configMINIMAL_STACK_SIZE, (void*) 0x3, mainLED_TASK_PRIORITY,
			NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();
	// Will only get here if there was insufficient memory to create the idle
	// task.
	for (;;)
		;
}
