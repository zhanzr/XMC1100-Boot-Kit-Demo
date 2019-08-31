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

#include "list.h"

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

void TimerCallback( xTimerHandle pxtimer ) {
		LED_Toggle(0);
}

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line) {
	XMC_DEBUG("%s %s %u\n", msg, file, line);
  abort();
}

void print_list_content(List_t* p_fr_list) {    
    uint32_t len = listCURRENT_LIST_LENGTH(p_fr_list);
    XMC_DEBUG("List Length:%u\n", len);
    
	ListItem_t* item = listGET_HEAD_ENTRY(p_fr_list);
    for(uint32_t i=0; i<len; ++i) {  
			XMC_DEBUG("%u\t", (uint32_t)listGET_LIST_ITEM_VALUE(item));
			item = listGET_NEXT(item);
    }
    XMC_DEBUG("\n");
}

void test_slist(void) {
    List_t intList;
    vListInitialise(&intList);
    print_list_content(&intList);
     
    XMC_DEBUG("Add to list\n");
    uint32_t a = UINT8_MAX;
    ListItem_t nodeA;
		vListInitialiseItem(&nodeA);
		listSET_LIST_ITEM_VALUE(&nodeA, a);
    vListInsertEnd(&intList, &nodeA);
    print_list_content(&intList);
	
    XMC_DEBUG("Add to list\n");
    uint32_t b = UINT16_MAX;		
    ListItem_t nodeB;
		vListInitialiseItem(&nodeB);
		listSET_LIST_ITEM_VALUE(&nodeB, b);
    vListInsertEnd(&intList, &nodeB);
    print_list_content(&intList);
		
    XMC_DEBUG("Add to list\n");
    uint32_t c = UINT32_MAX;
    ListItem_t nodeC;
		vListInitialiseItem(&nodeC);
		listSET_LIST_ITEM_VALUE(&nodeC, c);
    vListInsertEnd(&intList, &nodeC);
    print_list_content(&intList);
		
    XMC_DEBUG("Now Remove\n");
    uxListRemove(&nodeA);
    print_list_content(&intList);
    uxListRemove(&nodeC);
    print_list_content(&intList);
}

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
							256,
							NULL,
							2,
							&g_task01_handle);

  /* definition and creation of myTask02 */
	xTaskCreate((TaskFunction_t)StartTask02,
							(const portCHAR *)"myTask02",
							128,
							NULL,
							3,
							&g_task02_handle);
							
	/* Create one Software Timer.*/
	g_timer = xTimerCreate("Timer", 
							200/ portTICK_PERIOD_MS,
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
  for(;;) {
		xQueueReceive(g_queue, &tmpTicks, portMAX_DELAY);
		
		printf("%s %u %i %u\n", 
		tskKERNEL_VERSION_NUMBER,
		tmpTicks,
		((int32_t)XMC1000_CalcTemperature()-273),
		SystemCoreClock
		);
	
		LED_Toggle(1);
		LED_Toggle(3);
		
		xSemaphoreTake(g_noti_sema, portMAX_DELAY);	

//		char tmpBuf[1024];
//		vTaskList(tmpBuf);
//		printf(tmpBuf);
		
		printf("Total Heap:%u\n", 
		configTOTAL_HEAP_SIZE);
		
		printf("Free Heap:%u\n",
		xPortGetFreeHeapSize());
				
		volatile StackType_t* tmp_top_stk = (((TCB_t *)g_task01_handle)->pxTopOfStack);
		volatile StackType_t* tmp_stk = (((TCB_t *)g_task01_handle)->pxStack);

		printf("task0 pTopStack:%p, pxStack:%p, free:%u Word\n", 
		tmp_top_stk,
		tmp_stk,
		((uint32_t)tmp_top_stk-(uint32_t)tmp_stk)/4);
		
	  tmp_top_stk = (((TCB_t *)g_task02_handle)->pxTopOfStack);
	  tmp_stk = (((TCB_t *)g_task02_handle)->pxStack);		
		printf("task2 pTopStack:%p, pxStack:%p, free:%u Word\n", 
		tmp_top_stk,
		tmp_stk,
		((uint32_t)tmp_top_stk-(uint32_t)tmp_stk)/4);

//		printf("minimum ever:%u\n",
//		xPortGetMinimumEverFreeHeapSize());
		
//		vTaskGetRunTimeStats(tmpBuf);
//		printf(tmpBuf);		

		printf("\n");
		
		test_slist();		
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
  for(;;) {
		LED_Toggle(2);

		xSemaphoreGive(g_noti_sema);		
		
		uint32_t tmpTick = getKernelSysTick();
		xQueueSend(g_queue, &tmpTick, 0);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
