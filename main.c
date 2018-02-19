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

#include "cmsis_os.h"

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

osMutexId g_uart_mutex;
osMutexDef (g_uart_mutex);

#define SEMA_PROTECT_UART	
int stdout_putchar (int ch)
{
#ifdef SEMA_PROTECT_UART	
	osMutexWait(g_uart_mutex, osWaitForever);	
#endif
	
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	
#ifdef SEMA_PROTECT_UART	
	osMutexRelease(g_uart_mutex);	
#endif
	
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
	printf("%s\n", __FUNCTION__);
	
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
//		LED_Toggle(1);
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

extern int32_t asm_sub20(int32_t in);
extern int32_t asm_simple_sub(int32_t i1, int32_t i2);
extern int32_t asm_get_neg(int32_t in);

extern uint32_t asm_simple_mul(uint32_t i1, uint32_t i2);
extern uint32_t asm_test_cmp(uint32_t i1, uint32_t i2);
extern uint32_t asm_test_cmn(uint32_t i1, uint32_t i2);
extern uint32_t asm_get_and(uint32_t in, uint32_t key);
extern uint32_t asm_get_or(uint32_t in, uint32_t key);
extern int32_t asm_get_not(int32_t in);
		  
extern uint32_t asm_logic_left(uint32_t in, uint32_t key);
extern uint32_t asm_logic_right(uint32_t in, uint32_t key);
extern int32_t asm_arithm_right(int32_t in, uint32_t key);
extern uint32_t asm_rotate_right(uint32_t in, uint32_t key);

uint32_t g_TestVar32;// __attribute__((at(0x20003FF8)));

extern uint32_t asm_ldr32(uint32_t* addr);
extern uint32_t asm_str32(uint32_t* addr, uint32_t v);
extern uint32_t asm_test_push_pop(uint32_t i1, uint32_t i2);

extern int32_t asm_s16ext(int16_t in);
extern int32_t asm_s8ext(int8_t in);
extern int32_t asm_u16ext(uint16_t in);
extern uint32_t asm_rev(uint32_t in);
extern uint32_t asm_rev16(uint32_t in);
extern uint32_t asm_revsh(uint32_t in);

extern void asm_svc_1(uint32_t in);
extern void asm_test_msr(uint32_t in);
extern uint32_t asm_test_mrs(void);



osThreadId g_Thread1;
osThreadId g_Thread2;
osThreadId g_RtcThread;

osSemaphoreId arrived1,arrived2;
osSemaphoreDef(arrived1);
osSemaphoreDef(arrived2);

typedef struct {
uint8_t LED0;
uint8_t LED1;
uint8_t LED2;
}memory_block_t;

osMessageQId g_QTemp;
osMessageQDef(g_QTemp,1,unsigned int);

osMessageQId g_QLED;
osMessageQDef(g_QLED,1,memory_block_t*);

osPoolDef(led_pool, 2, memory_block_t);
osPoolId( led_pool);

void thread1 (void const *argument)
{
	osEvent result;
	
	uint32_t tmpKTick = osKernelSysTick();
	g_QLED = osMessageCreate(osMessageQ(g_QLED),NULL);
	
	led_pool = osPoolCreate(osPool(led_pool));
	
	memory_block_t *led_data;
	led_data = (memory_block_t *) osPoolAlloc(led_pool);
	
	while(1)
	{
		osDelay(1000);
    LED_On(0);	
    LED_On(1);	
    LED_On(2);	
    LED_On(3);	
    LED_On(4);	
		//delieberately inject an error
    LED_On(5);			
		osDelay(1000);
    LED_Off(0);	
    LED_Off(1);	
    LED_Off(2);	
    LED_Off(3);	
    LED_Off(4);				
		//rendezvous
		osSemaphoreRelease(arrived1);
		osSemaphoreWait(arrived2,osWaitForever);		
//    LED_Toggle(2);	
		
		led_data->LED0 = tmpKTick&0x01;
		led_data->LED1 = tmpKTick&0x02;
		led_data->LED2 = tmpKTick&0x04;

		osMessagePut(g_QLED,(uint32_t)led_data,osWaitForever);
		
		printf("%u\n", osKernelSysTick()-tmpKTick);
		osSignalSet (g_Thread2, 0x01);
		tmpKTick = osKernelSysTick();		

		result = osMessageGet(g_QTemp,osWaitForever);	

		printf("CoreTemp:%d 'C\n", (int32_t)result.value.v);	
//    LED_Toggle(0);	
//    LED_Toggle(1);	
//    LED_Toggle(2);	
//    LED_Toggle(3);	
//    LED_Toggle(4);			
	}
}

void thread2 (void const *argument)
{
	uint32_t temp_k;
	int32_t temp_C;
	osEvent event; memory_block_t * received;
	
	g_QTemp = osMessageCreate(osMessageQ(g_QTemp),NULL);

	while(1)
	{		
		//rendezvous
		osSemaphoreRelease(arrived2);
		osSemaphoreWait(arrived1,osWaitForever);		
//    LED_Toggle(1);	
		
		event = osMessageGet(g_QLED, osWaitForever);
		received = (memory_block_t *)event.value.p;
//		(received->LED0&0x01)?LED_On(0):LED_Off(0);
//		(received->LED1&0x02)?LED_On(1):LED_Off(1);
//		(received->LED2&0x04)?LED_On(2):LED_Off(2);
		osPoolFree(led_pool,received);
		
		/* Calculate temperature of the chip in Kelvin */
		temp_k = XMC1000_CalcTemperature();
		/* Convert temperature to Celcius */
		temp_C = temp_k - ZERO_TEMP_KELVIN;		
		
		osMessagePut(g_QTemp,temp_C,osWaitForever);
		//printf("CoreTemp:%d 'C\n", temp_C);	
		
		osSignalWait(0x01, osWaitForever);
	}
}

void RtcISRThread (void const *argument)
{
	__IO XMC_RTC_TIME_t now_rtc_time;
	char testBuf[21];
	
	while(1)
	{
		osSignalWait(0x01,osWaitForever);
		
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
//		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);	
		sprintf(testBuf, "%02d:%02d:%02d", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);	
		printf(testBuf);
		LCD_displayL(0, 0, (uint8_t*)testBuf);
	}
}

osThreadDef(thread1, osPriorityNormal, 1, 0); 
osThreadDef(thread2, osPriorityNormal, 1, 0); 
osThreadDef(RtcISRThread, osPriorityHigh, 1, 0); 

void os_idle_demon (void) {
	
	while(1)
	{
		__WFI();
//    LED_Toggle(0);	
//    LED_Toggle(1);	
//    LED_Toggle(2);	
//    LED_Toggle(3);	
//    LED_Toggle(4);	
//		SimpleDelay(1000000);
	}
}

/* OS Error Codes */
#define OS_ERROR_STACK_OVF      1
#define OS_ERROR_FIFO_OVF       2
#define OS_ERROR_MBX_OVF        3
#define OS_ERROR_TIMER_OVF      4
 
extern osThreadId svcThreadGetId (void);
/// \brief Called when a runtime error is detected
/// \param[in]   error_code   actual error code that has been detected
void os_error (uint32_t error_code) {
 
	osThreadId oId = svcThreadGetId();
	printf("[%08X]>\t%u\n", (uint32_t)oId, error_code);
  switch (error_code) {
    case OS_ERROR_STACK_OVF:
      /* Stack overflow detected for the currently running task. */
      /* Thread can be identified by calling svcThreadGetId().   */
      break;
    case OS_ERROR_FIFO_OVF:
      /* ISR FIFO Queue buffer overflow detected. */
      break;
    case OS_ERROR_MBX_OVF:
      /* Mailbox overflow detected. */
      break;
    case OS_ERROR_TIMER_OVF:
      /* User Timer Callback Queue overflow detected. */
      break;
  }
  for (;;);
}

int main(void)
{	
//	osKernelInitialize();	
	uint32_t tmpTick;

	osKernelInitialize(); 
  /* System timer configuration */
//	SysTick_Config(SystemCoreClock / HZ);	
	
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
	printf("Part 1\n");
	printf("ASM Test 1 Result:%u\n", asm_get_8bit_number());
	printf("ASM Test 2 Result:%08X\n", asm_get_xor(0x12345678, 0x34567890));
	printf("ASM Test 3 Direct Jump\n");
	printf("Before Jump.%08X\n", __get_MSP());
	asm_direct_jump(TestFunct);
	printf("Jump over.%08X\n", __get_MSP());
	
	//Part 2: Add
	printf("Part 2\n");
	printf("ASM Test 4 Result:%u\n", asm_add2(34));
	printf("ASM Test 5 Result:%u\n", asm_simple_add(123, 456));
	printf("ASM Test 6 Result:%u\n", asm_pc_add());
	
	//Part 3: Sub
	printf("Part 3\n");
	printf("ASM Test 7 Result:%d\n", asm_sub20(34));
	printf("ASM Test 8 Result:%d\n", asm_simple_sub(123, 456));
	printf("ASM Test 9 Result:%d\n", asm_get_neg(1024));

	//Part 4: Multiply, Compare, Logic
	printf("Part 4\n");
	printf("ASM Test 10 Result:%u\n", asm_simple_mul(123, 456));
	printf("ASM Test 11 Result:%u\n", asm_test_cmp(123, 456));
	printf("ASM Test 12 Result:%u\n", asm_test_cmn(123, 456));
	printf("ASM Test 13 Result:%08X\n", asm_get_and(0x12345678, 0x34567890));
	printf("ASM Test 14 Result:%08X\n", asm_get_or(0x12345678, 0x34567890));
	printf("ASM Test 15 Result:%08X\n", asm_get_not(0x12345678));

	//Test Addition/Mulitiplication Cycles
#define	TEST_ADD_MUL_NUM	100000
	//If the muliplication takes similar cycles, it is a single cycle multiplication implementation
	tmpTick = osKernelSysTick();
	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	{
		asm_simple_add(i, 456);
	}
	tmpTick = osKernelSysTick()-tmpTick;
	printf("%u\n", tmpTick);
	tmpTick = osKernelSysTick();
	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	{
		asm_simple_mul(i, 456);
	}
	tmpTick = osKernelSysTick()-tmpTick;
	printf("%u\n", tmpTick);
	
	printf("Part 5\n");
	//Part 5: Shift, Rotate
	printf("ASM Test 16 Result:%08X\n", asm_logic_left(0x80000001, 2));
	printf("ASM Test 17 Result:%08X\n", asm_logic_right(0x80000001, 2));
	printf("ASM Test 18 Result:%08X\n", asm_arithm_right(0x80000001, 2));
	printf("ASM Test 19 Result:%08X\n", asm_rotate_right(0x80000001, 2));

	//Part 6: Test Load, Store
	printf("Part 6\n");
	g_TestVar32 = 0x12345678;
	printf("ASM Test 20 Result:%08X\n", asm_ldr32(&g_TestVar32));
	asm_str32(&g_TestVar32, 0x78904563);	
	printf("ASM Test 21 Result:%08X\n", asm_ldr32(&g_TestVar32));
	printf("ASM Test 22 Result:%u\n", asm_test_push_pop(123, 456));

	//Part 7: Test Extend, Reverse
	printf("Part 7\n");
	printf("ASM Test 23 Result:%08X\n", asm_s16ext((int16_t)0x8001));
	printf("ASM Test 24 Result:%08X\n", asm_s8ext((int8_t)0xC4));
	printf("ASM Test 25 Result:%08X\n", asm_u16ext((uint16_t)0x8001));
	printf("ASM Test 26 Result:%08X\n", asm_rev(0x123456C8));
	printf("ASM Test 27 Result:%08X\n", asm_rev16(0x123456C8));
	printf("ASM Test 28 Result:%08X\n", asm_revsh(0x123456C8));
	
	//Part 8: Test SVC, MSR, MRS
	printf("Part 8\n");	
	printf("ASM Test 29, Before SVC #1\n");
	asm_svc_1(1000);
	printf("After SVC #1\n");
	
	printf("ASM Test 30 Result:%08X\n", asm_test_mrs());
	printf("ASM Test 31 Tick:%u\n", SysTick->VAL);
	asm_test_msr(0x00000001);
	uint32_t p1 = asm_test_mrs();
	asm_test_msr(0x00000000);
	uint32_t p2 = asm_test_mrs();
	printf("%08X\t%08X\n", p1, p2);
		
	g_Thread1 = osThreadCreate(osThread(thread1), NULL);
	g_Thread2 = osThreadCreate(osThread(thread2), NULL);
	g_RtcThread = osThreadCreate(osThread(RtcISRThread), NULL);
	printf("%08X,%08X,%08X\n", 
	(uint32_t)g_Thread1,
	(uint32_t)g_Thread2,
	(uint32_t)g_RtcThread);
	
	g_uart_mutex = osMutexCreate(osMutex(g_uart_mutex));

	arrived1 =osSemaphoreCreate(osSemaphore(arrived1),0);
	arrived2 =osSemaphoreCreate(osSemaphore(arrived2),0);

	LCD_Initialize();

	osKernelStart();

//	while (1)
//  {				
//    LED_Toggle(4);
//		
//		tmpTick = osKernelSysTick();
//		while((tmpTick+osKernelSysTickMicroSec(2000)) > osKernelSysTick())
//		{
//			__NOP();
//			__WFI();
//		}		
//  }
}
