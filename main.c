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
#include <math.h>
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
#include "serial.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = SERIAL_BAUDRATE
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

extern SemaphoreHandle_t g_noti_sema_stdin;

void USIC0_0_IRQHandler(void) {
  static uint8_t data;

  data = XMC_UART_CH_GetReceivedData(SERIAL_UART);

  ring_buffer_put(&serial_buffer, data);	
	
	BaseType_t tmp_HigherPriorityTaskWoken = pdFAIL;
	xSemaphoreGiveFromISR(g_noti_sema_stdin, &tmp_HigherPriorityTaskWoken);		
}

void MX_FREERTOS_Init(void);

void vApplicationTickHook( void ) {
}
	
int main(void) {		
  /*Initialize the UART driver */
	uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
	uart_rx.mode = XMC_GPIO_MODE_INPUT_PULL_UP;
//	uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
 /* Configure UART channel */
  XMC_UART_CH_Init(SERIAL_UART, &uart_config);
  XMC_UART_CH_SetInputSource(SERIAL_UART, XMC_UART_CH_INPUT_RXD, SERIAL_RX_INPUT);
  
  /* Set service request for receive interrupt */
  XMC_USIC_CH_SetInterruptNodePointer(SERIAL_UART, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, 0U);
  XMC_USIC_CH_SetInterruptNodePointer(SERIAL_UART, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, 0U);

  /*Set priority and enable NVIC node for receive interrupt*/
  NVIC_SetPriority(SERIAL_RX_IRQN, 3);
  NVIC_EnableIRQ(SERIAL_RX_IRQN);

  XMC_UART_CH_EnableEvent(SERIAL_UART, XMC_UART_CH_EVENT_STANDARD_RECEIVE | XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE);
	
	/* Start UART channel */
  XMC_UART_CH_Start(SERIAL_UART);

  /* Configure pins */
	XMC_GPIO_Init(SERIAL_TX_PIN, &uart_tx);
  XMC_GPIO_Init(SERIAL_RX_PIN, &uart_rx);
	
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
	
	while(1) { ; }
}
