/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "led.h"
#include "lcd2004.h"
#include "flash_ecc.h"
#include "XMC1000_TSE.h"

#include "cmsis_os.h"

#ifndef HZ
#define	HZ	1000
#endif

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = 
{	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 256000
};

XMC_RTC_CONFIG_t rtc_config =
{
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = 
{
	.year = 2018,
	.month = XMC_RTC_MONTH_JANUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 27,
	.hours = 15,
	.minutes = 40,
	.seconds = 55	
};


osMutexId g_uart_mutex;
osMutexDef (g_uart_mutex);

#define SEMA_PROTECT_UART	
int stdout_putchar (int ch)
{
#ifdef SEMA_PROTECT_UART	
	osMutexWait(g_uart_mutex, osWaitForever);	
#endif
	
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	
#ifdef SEMA_PROTECT_UART	
	osMutexRelease(g_uart_mutex);	
#endif
	
	return ch;
}

void SimpleDelay (uint32_t d)
{
	uint32_t t = d;
	while(--t)
	{
		__NOP();
	}
}	

void __svc(1) SVCDelay (uint32_t d);	
void __SVC_1 (uint32_t d)
{
  __disable_irq();
	uint32_t t = d;
	while(--t)
	{
		__NOP();
	}
  __enable_irq();	
}

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line)
{
  printf("Assert:%s,%s,%u\n", msg, file, line);

	while(1)
	{
		LED_Toggle(1);
		SimpleDelay(100000);
//		SVCDelay(100000);
	}
}

osThreadId g_Thread1;
osThreadId g_Thread2;
osThreadId g_RtcThread;

osSemaphoreId arrived1,arrived2;
osSemaphoreDef(arrived1);
osSemaphoreDef(arrived2);

typedef struct {
uint8_t LED0;
uint8_t LED1;
uint8_t LED2;
}memory_block_t;

osMessageQId g_QTemp;
osMessageQDef(g_QTemp,1,unsigned int);

osMessageQId g_QLED;
osMessageQDef(g_QLED,1,memory_block_t*);

osPoolDef(led_pool, 2, memory_block_t);
osPoolId( led_pool);

void thread1 (void const *argument)
{
	osEvent result;
	
	uint32_t tmpKTick = osKernelSysTick();
	g_QLED = osMessageCreate(osMessageQ(g_QLED),NULL);
	
	led_pool = osPoolCreate(osPool(led_pool));
	
	memory_block_t *led_data;
	led_data = (memory_block_t *) osPoolAlloc(led_pool);
	
	while(1)
	{
		osDelay(2000);
		//rendezvous
		osSemaphoreRelease(arrived1);
		osSemaphoreWait(arrived2,osWaitForever);		
//    LED_Toggle(2);	
		
		led_data->LED0 = tmpKTick&0x01;
		led_data->LED1 = tmpKTick&0x02;
		led_data->LED2 = tmpKTick&0x04;

		osMessagePut(g_QLED,(uint32_t)led_data,osWaitForever);
		
		printf("%u\n", osKernelSysTick()-tmpKTick);
		osSignalSet (g_Thread2, 0x01);
		tmpKTick = osKernelSysTick();		

		result = osMessageGet(g_QTemp,osWaitForever);	

		printf("CoreTemp:%d 'C\n", (int32_t)result.value.v);	
	}
}

void thread2 (void const *argument)
{
	uint32_t temp_k;
	int32_t temp_C;
	osEvent event; memory_block_t * received;
	
	g_QTemp = osMessageCreate(osMessageQ(g_QTemp),NULL);

	while(1)
	{		
		//rendezvous
		osSemaphoreRelease(arrived2);
		osSemaphoreWait(arrived1,osWaitForever);		
//    LED_Toggle(1);	
		
		event = osMessageGet(g_QLED, osWaitForever);
		received = (memory_block_t *)event.value.p;
		(received->LED0&0x01)?LED_On(0):LED_Off(0);
		(received->LED1&0x02)?LED_On(1):LED_Off(1);
		(received->LED2&0x04)?LED_On(2):LED_Off(2);
		osPoolFree(led_pool,received);
		
		/* Calculate temperature of the chip in Kelvin */
		temp_k = XMC1000_CalcTemperature();
		/* Convert temperature to Celcius */
		temp_C = temp_k - ZERO_TEMP_KELVIN;		
		
		osMessagePut(g_QTemp,temp_C,osWaitForever);
		//printf("CoreTemp:%d 'C\n", temp_C);	
		
		osSignalWait(0x01, osWaitForever);
	}
}

void RtcISRThread (void const *argument)
{
	__IO XMC_RTC_TIME_t now_rtc_time;
	
	while(1)
	{
		osSignalWait(0x01,osWaitForever);
		
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);	
	}
}

osThreadDef(thread1, osPriorityNormal, 1, 0); 
osThreadDef(thread2, osPriorityNormal, 1, 0); 
osThreadDef(RtcISRThread, osPriorityHigh, 1, 0); 

void os_idle_demon (void) {
	
	while(1)
	{
//    LED_Toggle(0);	
		SimpleDelay(1000000);
	}
}

/* OS Error Codes */
#define OS_ERROR_STACK_OVF      1
#define OS_ERROR_FIFO_OVF       2
#define OS_ERROR_MBX_OVF        3
#define OS_ERROR_TIMER_OVF      4
 
extern osThreadId svcThreadGetId (void);
/// \brief Called when a runtime error is detected
/// \param[in]   error_code   actual error code that has been detected
void os_error (uint32_t error_code) {
 
	osThreadId oId = svcThreadGetId();
	printf("[%08X]>\t%u\n", (uint32_t)oId, error_code);
  switch (error_code) {
    case OS_ERROR_STACK_OVF:
      /* Stack overflow detected for the currently running task. */
      /* Thread can be identified by calling svcThreadGetId().   */
      break;
    case OS_ERROR_FIFO_OVF:
      /* ISR FIFO Queue buffer overflow detected. */
      break;
    case OS_ERROR_MBX_OVF:
      /* Mailbox overflow detected. */
      break;
    case OS_ERROR_TIMER_OVF:
      /* User Timer Callback Queue overflow detected. */
      break;
  }
  for (;;);
}

/*
By default the CMSIS-RTOS scheduler will be running when main() is entered
and the main() function becomes the first active thread. Once in main(), we can
stop the scheduler task switching by calling osKernelInitialize (). While the
RTOS is halted we can create further threads and other RTOS objects. Once the
system is in a defined state we can restart the RTOS scheduler with
osKernelStart()
*/
int main(void)
{	
	osKernelInitialize();	
	
	/* Enable DTS */
	XMC_SCU_StartTempMeasurement();
	
  /*Initialize the UART driver */
	uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
	uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
 /* Configure UART channel */
  XMC_UART_CH_Init(XMC_UART0_CH1, &uart_config);
  XMC_UART_CH_SetInputSource(XMC_UART0_CH1, XMC_UART_CH_INPUT_RXD,USIC0_C1_DX0_P1_3);
  
	/* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH1);

  /* Configure pins */
	XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);
	
  printf ("CMSIS RTOS For XMC1100 @%u Hz\n",
	SystemCoreClock);
	printf("OSVer:%s,%u\n", osKernelSystemId, osKernelSysTickMicroSec(1));
	
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  XMC_SCU_INTERRUPT_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  NVIC_SetPriority(SCU_1_IRQn, 3);
  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	LED_Initialize();

	g_Thread1 = osThreadCreate(osThread(thread1), NULL);
	g_Thread2 = osThreadCreate(osThread(thread2), NULL);
	g_RtcThread = osThreadCreate(osThread(RtcISRThread), NULL);
	printf("%08X,%08X,%08X\n", 
	(uint32_t)g_Thread1,
	(uint32_t)g_Thread2,
	(uint32_t)g_RtcThread);
	
	g_uart_mutex = osMutexCreate(osMutex(g_uart_mutex));

	arrived1 =osSemaphoreCreate(osSemaphore(arrived1),0);
	arrived2 =osSemaphoreCreate(osSemaphore(arrived2),0);

	osKernelStart();
	
//	while (1)
//  {				
//    LED_Toggle(5);
//		
//		tmpTick = osKernelSysTick();
//		while((tmpTick+osKernelSysTickMicroSec(2000)) > osKernelSysTick())
//		{
//			__NOP();
//			__WFI();
//		}		
//  }
}
