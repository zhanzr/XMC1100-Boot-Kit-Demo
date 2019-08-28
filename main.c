/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

__IO uint32_t g_Ticks;

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

int stderr_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

void ttywrch (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
}

int __svc(0x00) svc_service_add(int x, int y);

int __svc(0x01) svc_service_sub(int x, int y);

int __svc(0x02) svc_service_incr(int x);

int __svc(0x03) svc_service_print_u32(uint32_t val32);

int __svc(0x04) svc_loop(uint32_t val32);

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line) {
	printf("%s %s %u\n", msg, file, line);
  abort();
}

int main(void) {
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / 1000);
	
	NVIC_SetPriority(SVCall_IRQn, 0x2);
	NVIC_SetPriority(PendSV_IRQn, 0x3);
  
	printf ("XMC1100 test @%u Hz\n",
	SystemCoreClock	);
	
	printf("assert 1\n");
	assert(1);
	printf("assert 0\n");
	assert(0);
	
	printf("XMC_ASSERT 1\n");
	XMC_ASSERT("test zero", 1);
	
	printf("XMC_ASSERT 0\n");
	XMC_ASSERT("test zero", 0);	
	
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

	svc_loop(0);

	while (1) {
  }
}
