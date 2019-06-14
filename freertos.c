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
#include <limits.h>
#include <dspfns.h> 
#include <c55x.h>       // include TI C55x intrinsics
#endif

#include <xmc_gpio.h>
#include <xmc_rtc.h>
#include "led.h"
#include "XMC1000_Tse.h"

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
static inline int inHandlerMode (void)  __pure {
  return __get_IPSR() != 0;
}

uint32_t getKernelSysTick(void) {
  if (inHandlerMode()) {
    return xTaskGetTickCountFromISR();
  } else {
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

void TimerCallback( xTimerHandle pxtimer ) {
	uint32_t ra = __return_address();
		
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
	uint32_t ra = __return_address();
  /* Create the thread(s) */
  /* definition and creation of defaultTask */
	xTaskCreate((TaskFunction_t)StartDefaultTask,
							(const portCHAR *)"defaultTask",
							384,
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

void gnu_builtin_test(void) {
	int res = __builtin_bcmp(gnu_builtin_test, gnu_builtin_test, sizeof(void(*)(void)));
	
	struct V {
	char buf1[10];
	int b;
	char buf2[10]; 
	} var;
	char* p = &var.buf1[1];
	char*	q = (char*)&var.b;

//	/* Here the object p points to is var.  */
//	assert (__builtin_object_size (p, 0) == sizeof (var) - 1);
//	/* The subobject p points to is var.buf1.  */
//	assert (__builtin_object_size (p, 1) == sizeof (var.buf1) - 1);
//	/* The object q points to is var.  */
//	assert (__builtin_object_size (q, 0)
//					== (char *) (&var + 1) - (char *) &var.b);
//	/* The subobject q points to is var.b.  */
//	assert (__builtin_object_size (q, 1) == sizeof (var.b));
	
	printf("size check %i %i %i %i\n",
	__builtin_object_size (p, 0),
	__builtin_object_size (p, 1),
	__builtin_object_size (q, 0),
	__builtin_object_size (q, 1));
		
	printf("malloc %p builtin_malloc %p\n", 
	malloc,
	__builtin_malloc
	);
	
		printf("clz %p __builtin_clz %p\n", 
	__clz,
	__builtin_clz
	);
	
		printf("memcmp %p __builtin_bcmp %p\n", 
	memcmp,
	__builtin_bcmp
	);	
	uint8_t* p_malloc_10000 = __builtin_alloca(10000);
	
	__NOP();
}

	//TI C55X emulation
//	if(true){		
//	int32_t c, d, e;
//    d = __qadd(a, b);     // ARM intrinsic saturating add
//    e = _lsadd(a, b);     // TI C55x saturating add
//    return c == d == e;   // returns 1
//	}
	
void vApplicationIdleHook( void ) {
	uint32_t ra = __return_address();
	__WFI();
}

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
void StartDefaultTask(void const * argument)
{
	uint32_t ra = __return_address();
	uint32_t tmpTicks;	
	
	uint32_t tmp_sp = __current_sp();
	
	__yield();
	
  /* Infinite loop */
  for(;;)
  {
		xQueueReceive(g_queue, &tmpTicks, portMAX_DELAY);
		
		printf("%s %u %u %u\n", 
		tskKERNEL_VERSION_NUMBER,
		tmpTicks,
		SystemCoreClock,
		__clz(tmpTicks)
		);
	
		int32_t tmp_i32 = (int32_t)XMC1000_CalcTemperature_soft();
		printf("die temp:%i\n", tmp_i32 - 273);
		
		XMC_RTC_TIME_t current_time;
		XMC_RTC_GetTime(&current_time);
		printf("RTC %02u:%02u:%02u\n", current_time.hours, current_time.minutes, current_time.seconds);
		
		LED_Toggle(1);
		
		xSemaphoreTake(g_noti_sema, portMAX_DELAY);	

		char tmpBuf[1024];
		vTaskList(tmpBuf);
		printf(tmpBuf);
		
		printf("Total Heap:%u\n", 
		configTOTAL_HEAP_SIZE);
		
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
				
		printf("minimum ever:%u\n",
		xPortGetMinimumEverFreeHeapSize());
		
//		vTaskGetRunTimeStats(tmpBuf);
//		printf(tmpBuf);		

	gnu_builtin_test();

		printf("\n");
  }
}

/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
void StartTask02(void const * argument)
{
	uint32_t ra = __return_address();
  /* Infinite loop */
  for(;;) {
		LED_Toggle(2);

		xSemaphoreGive(g_noti_sema);		
		
		uint32_t tmpTick = getKernelSysTick();
		xQueueSend(g_queue, &tmpTick, 0);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
