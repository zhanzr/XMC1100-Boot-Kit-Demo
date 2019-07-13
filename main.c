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
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include <cmsis_os.h>

#include "led.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 921600U
};

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

void blink_func0 (void  const *argument) {
	while(1) {
		LED_Toggle(0);
	
		osSignalWait(1, osWaitForever);
	}
}

osThreadId tid_blink0;                 
osThreadDef(blink_func0, osPriorityNormal, 1, 0);

void singal_func (void  const *argument) {
	while(1) {
		osSignalSet(tid_blink0, 1);
		osDelay(200);    
	}
}

osThreadId tid_signal;                 
osThreadDef(singal_func, osPriorityNormal, 1, 0);

int main(void) {
  /* System timer configuration */
	LED_Initialize();
	
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
	
	NVIC_SetPriority(SVCall_IRQn, 0x2);
	NVIC_SetPriority(PendSV_IRQn, 0x3);
  
	printf ("XMC1100 test @%u Hz\n",
	SystemCoreClock	);
	
	tid_blink0 = osThreadCreate(osThread(blink_func0), NULL);
	tid_signal = osThreadCreate(osThread(singal_func), NULL);

  osKernelStart();

	while (1) {
  }
}
