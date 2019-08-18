/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
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

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "led.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 921600
};

XMC_RTC_CONFIG_t rtc_config = {
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = {
	.year = 2019,
	.month = XMC_RTC_MONTH_JANUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 27,
	.hours = 15,
	.minutes = 40,
	.seconds = 55	
};

void vApplicationTickHook( void ) {
}

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

void MX_FREERTOS_Init(void);

int main(void) {
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
	__IO XMC_RTC_TIME_t now_rtc_time;

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
	
	XMC_SCU_StartTempMeasurement();
	
  printf ("FreeRTOS For XMC1100 Bootkit @%u Hz\n",
	SystemCoreClock	);
	
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
	
//  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);
//  XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
//  NVIC_SetPriority(SCU_1_IRQn, 3);
//  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	LED_Initialize();
			
  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */  
	vTaskStartScheduler();
	
	perror(__FUNCTION__);
	while(1) {				
			;
  }
}
