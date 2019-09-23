/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include <cmsis_os.h>

#include "EventRecorder.h"

#include "led.h"
#include "serial.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

const char* test_string[2] = {
	"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF",
	"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
};

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {
    .data_bits = 8U, .stop_bits = 1U, .baudrate = SERIAL_BAUDRATE};

osMutexId uart_mutex;
osMutexDef (uart_mutex);

//int mutex_printf (const char *format, ...) {
//   osMutexWait(uart_mutex, osWaitForever);
//   va_list args;
//   int retval;
//   va_start (args, format);
//   retval = vfprintf (stdout, format, args);   
//   va_end (args);
//   osMutexRelease(uart_mutex);
//   return retval;
//}

void blink_func (void  const *argument) {
	uint32_t num = (uint32_t)argument;
	
	while(1) {
		LED_Toggle(num);
		
		osMutexWait(uart_mutex, osWaitForever);
		
		printf("\nTask Func %p Got num:%u, %u\n",
		blink_func,
		num,
		osKernelSysTick()
		);
		printf("\n");
		printf(test_string[num]);
		printf("\n%s\n", osKernelSystemId);
		printf("\n");
  
		osMutexRelease(uart_mutex);
		
		osSignalWait(1, osWaitForever);
	}
}

#define TEST_BLINK_NUM	2
osThreadId tid_blink[TEST_BLINK_NUM];                 
osThreadDef(blink_func, osPriorityNormal, TEST_BLINK_NUM, 0);

void singal_func (void  const *argument) {
	while(1) {
		osSignalSet(tid_blink[0], 1);
		osSignalSet(tid_blink[1], 1);
		osDelay(500);    
	}
}

osThreadId tid_signal;                 
osThreadDef(singal_func, osPriorityNormal, 1, 0);

void USIC0_0_IRQHandler(void) {
  static uint8_t data;

  data = XMC_UART_CH_GetReceivedData(SERIAL_UART);
}

int main(void) {
//  EventRecorderInitialize(EventRecordAll, 1);
	
  /* System timer configuration */
	LED_Initialize();
	
  /*Initialize the UART driver */
  uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
  uart_rx.mode = XMC_GPIO_MODE_INPUT_PULL_UP;
  //	uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
  /* Configure UART channel */
  XMC_UART_CH_Init(SERIAL_UART, &uart_config);
  XMC_UART_CH_SetInputSource(SERIAL_UART, XMC_UART_CH_INPUT_RXD,
                             SERIAL_RX_INPUT);

  /* Set service request for receive interrupt */
  XMC_USIC_CH_SetInterruptNodePointer(
      SERIAL_UART, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, 0U);
  XMC_USIC_CH_SetInterruptNodePointer(
      SERIAL_UART, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, 0U);

  /*Set priority and enable NVIC node for receive interrupt*/
  NVIC_SetPriority(SERIAL_RX_IRQN, 3);
  NVIC_EnableIRQ(SERIAL_RX_IRQN);

  XMC_UART_CH_EnableEvent(SERIAL_UART,
                          XMC_UART_CH_EVENT_STANDARD_RECEIVE |
                              XMC_UART_CH_EVENT_ALTERNATIVE_RECEIVE);

  /* Start UART channel */
  XMC_UART_CH_Start(SERIAL_UART);

  /* Configure pins */
  XMC_GPIO_Init(SERIAL_TX_PIN, &uart_tx);
  XMC_GPIO_Init(SERIAL_RX_PIN, &uart_rx);
	
	NVIC_SetPriority(SVCall_IRQn, 0x2);
	NVIC_SetPriority(PendSV_IRQn, 0x3);
  
	printf ("XMC1100 rtx @%u Hz %s\n",
	SystemCoreClock,
	osKernelSystemId);
	
	tid_blink[0] = osThreadCreate(osThread(blink_func), (void*)0);
	tid_blink[1] = osThreadCreate(osThread(blink_func), (void*)1);
	tid_signal = osThreadCreate(osThread(singal_func), NULL);

	uart_mutex = osMutexCreate(osMutex(uart_mutex));

  osKernelStart();
}
