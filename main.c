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
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "led.h"
#include "lcd2004.h"
#include "flash_ecc.h"
#include "XMC1000_TSE.h"

//#include "cmsis_os.h"

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

__IO uint32_t g_Ticks;

uint32_t HAL_GetTick(void)
{
	return g_Ticks;
}

//#define SEMA_PROTECT_UART	
int stdout_putchar (int ch)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	
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

void TestFunct(void)
{
	printf("CPUID:%08X\n", SCB->CPUID);
}

extern uint32_t asm_get_8bit_number(void);
extern uint32_t asm_get_xor(uint32_t in, uint32_t key);
extern void asm_direct_jump(void(*fptr)(void));

extern uint32_t asm_add2(uint32_t in);
extern uint32_t asm_simple_add(uint32_t i1, uint32_t i2);
extern uint32_t asm_pc_add(void);

int main(void)
{	
//	osKernelInitialize();	
	uint32_t tmpTick;

  /* System timer configuration */
	SysTick_Config(SystemCoreClock / HZ);	
	
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
	
  printf ("ASM Test For XMC1100 @%u Hz\n",
	SystemCoreClock);
//	printf("OSVer:%s,%u\n", osKernelSystemId, osKernelSysTickMicroSec(1));
	
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  XMC_SCU_INTERRUPT_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  NVIC_SetPriority(SCU_1_IRQn, 3);
  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	LED_Initialize();
	
	//Part 1: Move
	printf("ASM Test 1, Result:%u\n", asm_get_8bit_number());
	printf("ASM Test 2, Result:%08X\n", asm_get_xor(0x12345678, 0x34567890));
	printf("ASM Test 3, Direct Jump\n");
	printf("Before Jump.%08X\n", __get_MSP());
	asm_direct_jump(TestFunct);
	printf("Jump over.%08X\n", __get_MSP());
	
	//Part 2: Add
	printf("ASM Test 4, Result:%u\n", asm_add2(34));
	printf("ASM Test 5 Result:%u\n", asm_simple_add(123, 456));
	printf("ASM Test 6 Result:%u\n", asm_pc_add());
	
	while (1)
  {				
    LED_Toggle(4);
		
		tmpTick = HAL_GetTick();
		while((tmpTick+2000) > HAL_GetTick())
		{
			__NOP();
			__WFI();
		}		
  }
}
