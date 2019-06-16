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
#include <xmc_vadc.h>

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

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 921600
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


/***************************************************************************
 * CONFIGURATION
 **************************************************************************/

/* VADC group data configuration. No configuration needed, standard values
 * are used. */

/* Data configuration for background source. */
const XMC_VADC_BACKGROUND_CONFIG_t g_bgn_handle = {
		.asctrl =0,
		.asmr = 0,
		.conv_start_mode = XMC_VADC_STARTMODE_WFS,
		.req_src_priority = XMC_VADC_GROUP_RS_PRIORITY_0,

};

/* VADC Global resources data configuration. No configuration needed,
 * standard values are used. */
 const XMC_VADC_GLOBAL_CONFIG_t g_global_handle = {
		.wait_for_read_mode = 1,
		.class0.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
		.class0.sample_time_std_conv = 0,
};

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

void MX_FREERTOS_Init(void);

int main(void) {	
	uint32_t ra = __return_address();

  /* System timer configuration */
  SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);
	
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
	
	 /* ****************************** VADC ****************************** */
    /* Provide clock to VADC and initialize the VADC global registers. */
    XMC_VADC_GLOBAL_Init(VADC, &g_global_handle);

    /* Calibrate the VADC. Make sure you do this after all used VADC groups
     * are set to normal operation mode. */
    XMC_VADC_GLOBAL_StartupCalibration(VADC);

    /* Initialize the background source hardware. The gating mode is set to
     * ignore to pass external triggers unconditionally.*/
    XMC_VADC_GLOBAL_BackgroundInit(VADC, &g_bgn_handle);

    /* Add a channel to the background source. */
    XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, 0, 5);
    /* Add a channel to the background source. */
    XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, 0, 6);
		
    /* Enables autoscan feature for continuous conversion. */
    XMC_VADC_GLOBAL_BackgroundEnableContinuousMode(VADC);

    /* Generate conversion request (load event). */
    XMC_VADC_GLOBAL_BackgroundTriggerConversion(VADC);


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
