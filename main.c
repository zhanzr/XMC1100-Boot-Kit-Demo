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

int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_incr(int x);

void __svc(0x03) svc_service_print_u32(uint32_t val32);

int main(void) {
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
	
	NVIC_SetPriority(SVCall_IRQn, 0x2);
	NVIC_SetPriority(PendSV_IRQn, 0x3);
  
	printf ("XMC1100 test @%u Hz\n",
	SystemCoreClock	);
	
	int x, y, z;
	
	x = 3; y = 5;
	z = svc_service_add(x, y);
	printf ("3+5 = %d \n", z);
	x = 9; y = 2;
	z = svc_service_sub(x, y);
	printf ("9-2 = %d \n", z);

	x = 3;
	z = svc_service_incr(x);
	printf ("3++ = %d \n", z);

	svc_service_print_u32((uint32_t)main);
	svc_service_print_u32((uint32_t)stdout_putchar);

	//Pending the PendSVC
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Set PendSV pending status

	while (1) {
  }
}
