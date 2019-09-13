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
#include "serial.h"
#include "core_portme.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = SERIAL_BAUDRATE
};

void USIC0_0_IRQHandler(void) {
  static uint8_t data;

  data = XMC_UART_CH_GetReceivedData(SERIAL_UART);
}

//int main(void)
void original_main(void) {
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / HZ);
	
 /*Initialize the UART driver */
	uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
	uart_rx.mode = XMC_GPIO_MODE_INPUT_PULL_UP;
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
	
  printf ("Coremark @%u Hz\n", SystemCoreClock	);
	
	LED_Initialize();
			
//	while (1)
//  {				
//    LED_On(4);
//		
//		tmpTick = g_Ticks;
//		while((tmpTick+2000) > g_Ticks)
//		{
//			__NOP();
//			__WFI();
//		}
//		
//		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
////		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);

//    LED_Off(4);
//		
//		tmpTick = g_Ticks;
//		while((tmpTick+2000) > g_Ticks)
//		{
//			__NOP();
//			__WFI();
//		}		
//  }
}
