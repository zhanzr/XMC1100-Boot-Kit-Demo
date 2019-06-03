/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */ 
#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "LiquidCrystal.h"

using namespace std;
#else
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#endif

#include <xmc_gpio.h>
#include "led.h"
#include "XMC1000_TSE.h"


/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t g_task01_handle;
TaskHandle_t g_task02_handle;
SemaphoreHandle_t g_noti_sema;
QueueHandle_t g_queue;
TimerHandle_t g_timer;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* Determine whether we are in thread mode or handler mode. */
static int inHandlerMode (void)
{
  return __get_IPSR() != 0;
}

uint32_t getKernelSysTick(void)
{
  if (inHandlerMode()) {
    return xTaskGetTickCountFromISR();
  }
  else {
    return xTaskGetTickCount();
  }
}

extern void xPortSysTickHandler(void);

void freertos_tick_handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
	#endif  /* INCLUDE_xTaskGetSchedulerState */  
			xPortSysTickHandler();
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
		}
	#endif  /* INCLUDE_xTaskGetSchedulerState */  
}

void TimerCallback( xTimerHandle pxtimer )
{
		LED_Toggle(0);
}

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
	xTaskCreate((TaskFunction_t)StartDefaultTask,
							(const portCHAR *)"defaultTask",
							512,
							NULL,
							2,
							&g_task01_handle);

  /* definition and creation of myTask02 */
	xTaskCreate((TaskFunction_t)StartTask02,
							(const portCHAR *)"myTask02",
							64,
							NULL,
							3,
							&g_task02_handle);
							
	/* Create one Software Timer.*/
	g_timer = xTimerCreate("Timer", 
							2000/ portTICK_PERIOD_MS,
							pdTRUE,
							0,
							TimerCallback);
	xTimerStart( g_timer, 0);

	/* Create the notification semaphore and set the initial state. */
	vSemaphoreCreateBinary(g_noti_sema);
	vQueueAddToRegistry(g_noti_sema, "Notification Semaphore");
	xSemaphoreTake(g_noti_sema, 0);

	/* Create a queue*/
	g_queue = xQueueCreate(2, sizeof(uint32_t));
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
void StartDefaultTask(void const * argument)
{
	uint32_t tmpTicks;	
		
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(g_queue, &tmpTicks, portMAX_DELAY);
		
		printf("%s %u %i %u\n", 
		tskKERNEL_VERSION_NUMBER,
		tmpTicks,
		((int32_t)XMC1000_CalcTemperature()-273),
		SystemCoreClock
		);
	
		LED_Toggle(1);
		
		xSemaphoreTake(g_noti_sema, portMAX_DELAY);	

		char tmpBuf[1024];
		vTaskList(tmpBuf);
		printf(tmpBuf);
		
		printf("Total Heap:%u\n", 
		configTOTAL_HEAP_SIZE);
		
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
				
		//Test malloc 1 byte
	  uint8_t* p_test_1_byte = pvPortMalloc(1);
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
		
		//Test malloc 100 bytes
	  uint8_t* p_test_2_byte = pvPortMalloc(100);
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
		
		printf("ptr1:%p, ptr2:%p\n",
		p_test_1_byte, p_test_2_byte);
		
		vPortFree(p_test_1_byte);
		vPortFree(p_test_2_byte);
		
		printf("ptr1:%p, ptr2:%p\n",
		p_test_1_byte, p_test_2_byte);
		
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
		
		printf("minimum ever:%u\n",
		xPortGetMinimumEverFreeHeapSize());
		
//		vTaskGetRunTimeStats(tmpBuf);
//		printf(tmpBuf);		

		printf("\n\n\n\n\n\n\n");
  }
}

/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
void StartTask02(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {
		LED_Toggle(2);

		xSemaphoreGive(g_noti_sema);		
		
		uint32_t tmpTick = getKernelSysTick();
		xQueueSend(g_queue, &tmpTick, 0);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
