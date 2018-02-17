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

int stdout_putchar (int ch)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

static inline void SimpleDelay(uint32_t d)
{
	uint32_t t = d;
	while(--t)
	{
		__NOP();
	}
}

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line)
{
  printf("Assert:%s,%s,%u\n", msg, file, line);

	while(1)
	{
		LED_Toggle(1);
		SimpleDelay(100000);
	}
}

void thread1 (void const *argument)
{
	while(1)
	{
    LED_Toggle(4);	
		osDelay(500);
	}
}

void thread2 (void const *argument)
{
	__IO XMC_RTC_TIME_t now_rtc_time;
	uint32_t temp_k;
	int32_t temp_C;
	
	uint32_t tmpKTick = osKernelSysTick();
	
	while(1)
	{
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);	
//		printf("%u\n", osKernelSysTick()-tmpKTick);
//		tmpKTick = osKernelSysTick();		
		
		/* Calculate temperature of the chip in Kelvin */
		temp_k = XMC1000_CalcTemperature();
		/* Convert temperature to Celcius */
		temp_C = temp_k - 273;	
		printf("CoreTemp:%d 'C\n", temp_C);
		
		osDelay(5000);
	}
}

osThreadId thrdID1, thrdID2;

osThreadDef(thread1, osPriorityNormal, 1, 0); 
osThreadDef(thread2, osPriorityNormal, 1, 0); 

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
	
  XMC_RTC_Start();
	
	LED_Initialize();

	osThreadCreate(osThread(thread1), NULL);
	osThreadCreate(osThread(thread2), NULL);

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
