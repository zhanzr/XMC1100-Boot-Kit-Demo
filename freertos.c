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

void test_func0(void) {
	__NOP();
	
	return;
}

void test_func1(uint32_t a0_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	
	return;
}

void test_func2(uint32_t a0_u32, uint32_t a1_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);

	return;
}

void test_func2_u8(uint8_t a0_u8, uint8_t a1_u8) {
	__NOP();
	XMC_UNUSED_ARG(a0_u8);
	XMC_UNUSED_ARG(a1_u8);

	return;
}

void test_func3(uint32_t a0_u32, uint32_t a1_u32, uint32_t a2_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);
	XMC_UNUSED_ARG(a2_u32);

	return;
}

void test_func4(uint32_t a0_u32, uint32_t a1_u32, uint32_t a2_u32, uint32_t a3_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);
	XMC_UNUSED_ARG(a2_u32);
	XMC_UNUSED_ARG(a3_u32);

	return;
}

void test_func5(uint32_t a0_u32, uint32_t a1_u32, uint32_t a2_u32, uint32_t a3_u32, uint32_t a4_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);
	XMC_UNUSED_ARG(a2_u32);
	XMC_UNUSED_ARG(a3_u32);
	XMC_UNUSED_ARG(a4_u32);

	return;
}

void test_func6(uint32_t a0_u32, uint32_t a1_u32, uint32_t a2_u32, uint32_t a3_u32, uint32_t a4_u32, uint32_t a5_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);
	XMC_UNUSED_ARG(a2_u32);
	XMC_UNUSED_ARG(a3_u32);
	XMC_UNUSED_ARG(a4_u32);
	XMC_UNUSED_ARG(a5_u32);

	return;
}

void test_func7(uint32_t a0_u32, uint32_t a1_u32, uint32_t a2_u32, uint32_t a3_u32, uint32_t a4_u32, uint32_t a5_u32, uint32_t a6_u32) {
	__NOP();
	XMC_UNUSED_ARG(a0_u32);
	XMC_UNUSED_ARG(a1_u32);
	XMC_UNUSED_ARG(a2_u32);
	XMC_UNUSED_ARG(a3_u32);
	XMC_UNUSED_ARG(a4_u32);
	XMC_UNUSED_ARG(a5_u32);
	XMC_UNUSED_ARG(a6_u32);

	return;
}

uint8_t test_return_1(void) {
	__NOP();
	
	uint8_t ret = 0x11;
	return ret;
}

uint16_t test_return_2(void) {
	__NOP();
	
	uint16_t ret = 0x1122;
	return ret;
}

typedef struct str_three_byte {
	uint8_t a;
	uint8_t b;
	uint8_t c;
}three_byte;

three_byte test_return_3(void) {
	__NOP();
	
	three_byte ret = {.a = 0x11, .b = 0x22, .c = 0x33};
	return ret;
}

uint32_t test_return_4(void) {
	__NOP();
	
	uint32_t ret = 0x11223344;
	return ret;
}

typedef struct str_five_byte {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
}five_byte;

five_byte test_return_5(void) {
	__NOP();
	
	five_byte ret = {.a = 0x11, .b = 0x22, .c = 0x33, .d = 0x44, .e = 0x55};
	return ret;
}

uint64_t test_return_8(void) {
	__NOP();
	
	uint64_t ret = 0x1122334455667788;
	return ret;
}

typedef struct str_sixteen_byte {
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
}sixteen_byte;

sixteen_byte test_return_16(void) {
	__NOP();
	
	sixteen_byte ret = {.a = 0x11223344, .b = 0x55667788, .c = 0x99AABBCC, .d = 0xDDEEFF00};
	return ret;
}

typedef struct str_seventeen_byte {
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint8_t e;
}seventeen_byte;

seventeen_byte test_return_17(void) {
	__NOP();
	
	seventeen_byte ret = {.a = 0x11223344, .b = 0x55667788, .c = 0x99AABBCC, .d = 0xDDEEFF00, .e = 0x5A};
	return ret;
}

void test_eabi_0(void) {
	test_func0();
	test_func1(0x11);
	test_func2(0x11, 0x22);
	test_func2_u8(0x11, 0x22);
	test_func3(0x11, 0x22, 0x33);
	test_func4(0x11, 0x22, 0x33, 0x44);
	test_func5(0x11, 0x22, 0x33, 0x44, 0x55);
	test_func6(0x11, 0x22, 0x33, 0x44, 0x55, 0x66);
	test_func7(0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77);
	
	uint8_t t_1 = test_return_1();
	uint16_t t_2 = test_return_2();
	three_byte t_3 = test_return_3();
	uint32_t t_4 = test_return_4();
	five_byte t_5 = test_return_5();
	uint64_t t_8 = test_return_8();
	sixteen_byte t_16 = test_return_16();
	seventeen_byte t_17 = test_return_17();
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
							512,
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
		
//		test_slist();		
		test_eabi_0();
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
