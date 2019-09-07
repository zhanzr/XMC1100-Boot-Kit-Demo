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
#include "ring_buffer.h"
#include "serial.h"
#include "shell.h"

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

//extern ring_buffer_t serial_buffer;

RING_BUFFER_DEF(serial_buffer, SERIAL_BUFFER_SIZE);

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = SERIAL_BAUDRATE
};

XMC_RTC_CONFIG_t rtc_config = {
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = {
	.year = 2018,
	.month = XMC_RTC_MONTH_JANUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 27,
	.hours = 15,
	.minutes = 40,
	.seconds = 55	
};

void USIC0_0_IRQHandler(void) {
  static uint8_t data;

  data = XMC_UART_CH_GetReceivedData(SERIAL_UART);

  ring_buffer_put(&serial_buffer, data);	
}

void led_cmd(int32_t argc, char **argv) {
  if (argc == 3) {
		uint8_t tmp_num = (uint8_t)atoi(argv[1]);
		if(tmp_num >= LED_TOTAL_NUM) {
			tmp_num = 0;
		}
		
		if (strcmp(argv[2], "on") == 0) {
			LED_On(tmp_num);	
		}	else if (strcmp(argv[2], "off") == 0) {
			LED_Off(tmp_num);	
		} else {
			shell_println("Argument not supported");
		}
  }
}

void help_cmd(int32_t argc, char **argv) {
  (void)argc;
  (void)argv;

  shell_help();
}

void rtc_cmd(int32_t argc, char **argv) {
	__IO XMC_RTC_TIME_t now_rtc_time;
	
  (void)argc;
  (void)argv;
	
	XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
	printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);
}

const shell_command_t cmd_table[] =
{
  {"help", 0u, 0u, help_cmd, "Display this help message", ""},
  {"led", 2u, 2u, led_cmd, "Led control", "<num> <on|off>"},
  {"rtc", 0u, 0u, rtc_cmd, "Get RTC value", ""},
  {0, 0u, 0u, 0, 0, 0}
};

void my_shell_init(void)
{
  const char DELIMITER_STR[] = "************************************************";

  shell_println("\r\n%s", DELIMITER_STR);
  shell_println(" %s", "Shell Application");
  shell_println(DELIMITER_STR);
  shell_println(" Version %s", "1.0.0");
  shell_println(" Built %s", __DATE__ " at " __TIME__);

  shell_println("\n Enter 'help' for command list.");
  shell_println("%s\n", DELIMITER_STR);

}

int main(void) {
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / 1000);
	
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
	
  printf ("UART Shell test @%u Hz\n",
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
	
  shell_init(cmd_table, my_shell_init);

	while (1) {				
    shell_state_machine();
  }
}
