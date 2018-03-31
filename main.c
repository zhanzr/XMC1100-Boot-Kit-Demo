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

#include <signal.h>

#include "led.h"

#include "flash_ecc.h"

//The value of the content
#define ClassB_Ref_ChkSum 0xEAE3F110

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

void SystemCoreClockSetup(void)
{
	XMC_SCU_CLOCK_CONFIG_t clock_config =
	{
		.rtc_src = XMC_SCU_CLOCK_RTCCLKSRC_DCO2,
		.pclk_src = XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK,
		.fdiv = 0, 
		.idiv = 1
	 };

	XMC_SCU_CLOCK_Init(&clock_config);
	
//  SystemCoreClockUpdate();
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
		LED_On(1);
		SimpleDelay(100000);
		LED_Off(1);
		SimpleDelay(100000);
	}
}

//re-implement 
void __rt_raise(int signal, int type)
{
	if((signal == SIGFPE) && (type == DIVBYZERO))
	{
		//Catch the DIVBYZERO error
		printf("Catched %i %i\n", signal, DIVBYZERO);
	}
}

extern int $Super$$__aeabi_idiv0(void);
int $Sub$$__aeabi_idiv0(void)
{
	int res;	

	// call the original __aeabi_idiv0 function    
	res = $Super$$__aeabi_idiv0();

	printf("idiv %i\n", res);	
	
	return res+1;
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
	
  printf ("Divide by 0 test For XMC1100 Bootkit by Automan @ Infineon BBS @%u Hz\n",
	SystemCoreClock	);

	printf("CM%u, %08X\n",
		__CORTEX_M,
		SCB->CPUID);
		
	#ifdef __MICROLIB
	printf("With Microlib\n");
	#else
	printf("With StandardLib\n");
	#endif
	
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
	
//  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);
//  XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
//  NVIC_SetPriority(SCU_1_IRQn, 3);
//  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	LED_Initialize();
	
	printf("\nuint8_t test\n");
	{
		uint8_t a = 10;
		uint8_t b = 0;
		uint8_t c = a/b;
		printf("%u / %u = %u\n", a, b, c);
	}

	printf("\nint8_t test\n");
	{
		int8_t a = 10;
		int8_t b = 0;
		int8_t c = a/b;
		printf("%i / %i = %i\n", a, b, c);
	}
	
	printf("\nuint16_t test\n");
	{
		uint16_t a = 10;
		uint16_t b = 0;
		uint16_t c = a/b;
		printf("%u / %u = %u\n", a, b, c);
	}

	printf("\nint16_t test\n");
	{
		int16_t a = 10;
		int16_t b = 0;
		int16_t c = a/b;
		printf("%i / %i = %i\n", a, b, c);
	}
	
	printf("\nuint32_t test\n");
	{
		uint32_t a = 10;
		uint32_t b = 0;
		uint32_t c = a/b;
		printf("%u / %u = %u\n", a, b, c);
	}

	printf("\nint32_t test\n");
	{
		int32_t a = 10;
		int32_t b = 0;
		int32_t c = a/b;
		printf("%i / %i = %i\n", a, b, c);
	}	
	
	while (1)
  {				
//    LED_On(0);
//    LED_On(1);
//    LED_On(2);
//    LED_On(3);
    LED_On(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}
		
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
//		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);

//    LED_Off(0);
//    LED_Off(1);
//    LED_Off(2);
//    LED_Off(3);
    LED_Off(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}		
  }
}
