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
  .baudrate = 921600
};

int stdout_putchar (int ch)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

int main(void)
{
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
	__IO XMC_RTC_TIME_t now_rtc_time;

  /* System timer configuration */
  SysTick_Config(SystemCoreClock / 1000);
	
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
	
  printf ("Dhrystone For XMC1100 Bootkit by Automan @ Infineon BBS @%u Hz\n",
	SystemCoreClock	);
	
	LED_Initialize();
			
	while (1) {				
    LED_Toggle(0);
    LED_Toggle(1);
    LED_Toggle(2);
    LED_Toggle(3);
    LED_Toggle(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}		
  }
}
