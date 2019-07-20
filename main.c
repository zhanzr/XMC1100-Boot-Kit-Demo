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
#include <xmc_prng.h>
#include <xmc_rtc.h>

#include "XMC1000_TSE.h"

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

XMC_RTC_CONFIG_t rtc_config =
{
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = 
{
	.year = 2019,
	.month = XMC_RTC_MONTH_JANUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 17,
	.hours = 5,
	.minutes = 6,
	.seconds = 55	
};

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

int main(void) {
	uint16_t tmpRand;
	XMC_RTC_TIME_t now_rtc_time;
	XMC_PRNG_INIT_t	tmpPrngT;
	int32_t temp_k;
	int32_t temp_C;
			
	/* Enable DTS */
	XMC_SCU_StartTempMeasurement();
	
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
  
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
	
//  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);
//  XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
//  NVIC_SetPriority(SCU_1_IRQn, 3);
//  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	printf ("XMC1100 test @%u Hz\n",
	SystemCoreClock	);
	
	#ifdef __USE_ANSI_EXAMPLE_RAND
	printf("use ansi example rand\n");
	#else
	printf("use libc rand\n");
	#endif
	
	#ifdef __MICROLIB
	printf("Micro LibC\n");
	#else
	printf("Standard LibC\n");
	#endif
	
	uint32_t flash_addr;	
	uint32_t sram_addr;	
//Flash access alignment test	
	printf("Flash Access Test\n");
	flash_addr = 0x10001000;
	printf("[%08X], 1, %02X\n", flash_addr, *(uint8_t*)(flash_addr));
	printf("[%08X], 2, %04X\n", flash_addr, *(uint16_t*)(flash_addr));
	printf("[%08X], 4, %08X\n", flash_addr, *(uint32_t*)(flash_addr));
	
	flash_addr++;
	printf("[%08X], 1, %02X\n", flash_addr, *(uint8_t*)(flash_addr));
//	printf("[%08X], 2, %04X\n", flash_addr, *(uint16_t*)(flash_addr));
//	printf("[%08X], 4, %08X\n", flash_addr, *(uint32_t*)(flash_addr));	
	
	flash_addr++;
	printf("[%08X], 1, %02X\n", flash_addr, *(uint8_t*)(flash_addr));
	printf("[%08X], 2, %04X\n", flash_addr, *(uint16_t*)(flash_addr));
//	printf("[%08X], 4, %08X\n", flash_addr, *(uint32_t*)(flash_addr));	
	
	flash_addr++;
	printf("[%08X], 1, %02X\n", flash_addr, *(uint8_t*)(flash_addr));
//	printf("[%08X], 2, %04X\n", flash_addr, *(uint16_t*)(flash_addr));
//	printf("[%08X], 4, %08X\n", flash_addr, *(uint32_t*)(flash_addr));		
	
	//SRAM access alignment test sram_addr;
	printf("SRAM Access Test\n");
	sram_addr = 0x20000000;
	printf("[%08X], 1, %02X\n", sram_addr, *(uint8_t*)(sram_addr));
	printf("[%08X], 2, %04X\n", sram_addr, *(uint16_t*)(sram_addr));
	printf("[%08X], 4, %08X\n", sram_addr, *(uint32_t*)(sram_addr));
	
	sram_addr++;	
	printf("[%08X], 1, %02X\n", sram_addr, *(uint8_t*)(sram_addr));
//	printf("[%08X], 2, %04X\n", sram_addr, *(uint16_t*)(sram_addr));
//	printf("[%08X], 4, %08X\n", sram_addr, *(uint32_t*)(sram_addr));
		
	sram_addr++;	
	printf("[%08X], 1, %02X\n", sram_addr, *(uint8_t*)(sram_addr));
	printf("[%08X], 2, %04X\n", sram_addr, *(uint16_t*)(sram_addr));
//	printf("[%08X], 4, %08X\n", sram_addr, *(uint32_t*)(sram_addr));
	
	sram_addr++;	
	printf("[%08X], 1, %02X\n", sram_addr, *(uint8_t*)(sram_addr));
//	printf("[%08X], 2, %04X\n", sram_addr, *(uint16_t*)(sram_addr));
//	printf("[%08X], 4, %08X\n", sram_addr, *(uint32_t*)(sram_addr));	
	
	printf("All Test End\n");
	
	while (1) {
		/* Calculate temperature of the chip in Kelvin */
		temp_k = XMC1000_CalcTemperature();

		/* Convert temperature to Celcius */
		temp_C = (int32_t)temp_k - 273;

		printf ("die temp = %i 'C\n", temp_C);
		
		XMC_RTC_GetTime(&now_rtc_time);
		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);		
		uint32_t lockTick = g_Ticks;
		while((lockTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}			
  }
}
