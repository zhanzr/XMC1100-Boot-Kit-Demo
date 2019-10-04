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

__IO uint32_t g_ticks;

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

#define	TEST_LOOP_N	((uint8_t)255)

// CRC16 Test Vector
uint8_t test_vec0[]= {0x63 }; // 0xBD35
uint8_t test_vec1[]= {0x8C }; // 0xB1F4
uint8_t test_vec2[]= {0x7D }; // 0x4ECA
uint8_t test_vec3[]= {0xAA, 0xBB, 0xCC }; // 0x6CF6
uint8_t test_vec4[]= {0x00, 0x00, 0xAA, 0xBB, 0xCC }; // 0xB166

const uint8_t test_vec_bench[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};

uint16_t UpdateCRC(uint16_t CRC_acc, uint8_t* p_input, uint32_t len) {
#define POLY 0x1021

	uint16_t CRC_acc_cal = CRC_acc;
	uint8_t i;
	uint32_t idx;

	for(idx = 0; idx < len; ++idx) {
		// Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
		// with no carries)
		CRC_acc_cal = CRC_acc_cal ^ (p_input[idx] << 8);
		// "Divide" the poly into the dividend using CRC XOR subtraction
		// CRC_acc_cal holds the "remainder" of each divide
		//
		// Only complete this division for 8 bits since input is 1 byte
		for (i = 0; i < 8; i++) {
			// Check if the MSB is set (if MSB is 1, then the POLY can "divide"
			// into the "dividend")
			if ((CRC_acc_cal & 0x8000) == 0x8000) {
				// if so, shift the CRC value, and XOR "subtract" the poly
				CRC_acc_cal = CRC_acc_cal << 1;
				CRC_acc_cal ^= POLY;
			} else {
				// if not, just shift the CRC value
				CRC_acc_cal = CRC_acc_cal << 1;
			}
		}
	}

	return CRC_acc_cal;
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
	
	// CRC16 benchmark 

	uint8_t loop_cnt;
	uint32_t tick0;
	uint32_t tick1;

//Soft CRC16 test
	uint16_t res0 = UpdateCRC(0xffff, test_vec0, sizeof(test_vec0));
	printf("Ref value:%04X\n", res0);

	uint16_t res1 = UpdateCRC(0xffff, test_vec1, sizeof(test_vec1));
	printf("Ref value:%04X\n", res1);

	uint16_t res2 = UpdateCRC(0xffff, test_vec2, sizeof(test_vec2));
	printf("Ref value:%04X\n", res2);

	uint16_t res3 = UpdateCRC(0xffff, test_vec3, sizeof(test_vec3));
	printf("Ref value:%04X\n", res3);

	uint16_t res4 = UpdateCRC(0xffff, test_vec4, sizeof(test_vec4));
	printf("Ref value:%04X\n", res4);

	uint16_t res_bench = UpdateCRC(0xffff, (uint8_t*)test_vec_bench, sizeof(test_vec_bench));
	printf("Ref value:%04X\n", res_bench);   
	
		tick0 = g_ticks;
		loop_cnt = TEST_LOOP_N;
		while(loop_cnt--) {
			res_bench = UpdateCRC(0xffff, (uint8_t*)test_vec_bench, sizeof(test_vec_bench));
		}

		tick1 = g_ticks;
		printf("%u %u \t %u\n", tick0, tick1, tick1-tick0);
		
  shell_init(cmd_table, my_shell_init);

	while (1) {				
    shell_state_machine();
  }
}
