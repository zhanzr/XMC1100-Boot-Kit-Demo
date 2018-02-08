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

#include "slist.h"

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;
char g_Buf[256];

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

void FailSafePOR(void)
{
	while(1)
	{
		LED_On(0);
		SimpleDelay(100000);
		LED_Off(0);
		SimpleDelay(100000);
	}
}

/* Constants necessary for RAM test (RAM_END is word aligned ) */
#define	RAM_SIZE		0x3FFC
#define RAM_START  (uint32_t *)(0x20000000)  
//#define RAM_START  (uint32_t *)(0x20000800)
#define RAM_END    (uint32_t *)(0x20003FFC)
//#define RAM_END    (uint32_t *)(0x20003FFC)
/* These are the direct and inverted data (pattern) used during the RAM
test, performed using March C- Algorithm */
#define BCKGRND     ((uint32_t)0x00000000uL)
#define INV_BCKGRND ((uint32_t)0xFFFFFFFFuL)

#define RT_RAM_BLOCKSIZE      ((uint32_t)6u)  /* Number of RAM words tested at once */

#define RAM_BLOCKSIZE         ((uint32_t)16)
static const uint8_t RAM_SCRMBL[RAM_BLOCKSIZE] = {0u,1u,3u,2u,4u,5u,7u,6u,8u,9u,11u,10u,12u,13u,15u,14u};
static const uint8_t RAM_REVSCRMBL[RAM_BLOCKSIZE] = {1u,0u,2u,3u,5u,4u,6u,7u,9u,8u,10u,11u,13u,12u,14u,15u};

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/**
  * @brief  This function verifies that RAM is functional,
  *   using the March C- algorithm.
  * @param :  None
  * @retval : ErrorStatus = (ERROR, SUCCESS)
  */
ErrorStatus FullRamMarchC(void)
{
      ErrorStatus Result = SUCCESS;
      uint32_t *p;       /* RAM pointer */
      uint32_t j;        /* Index for RAM physical addressing */
      
      uint32_t ra= __return_address(); /* save return address (as it will be destroyed) */

   /* ---------------------------- STEP 1 ----------------------------------- */
   /* Write background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p++)
   {
      /* Scrambling not important when there's no consecutive verify and write */
      *p = BCKGRND;
   }

   /* ---------------------------- STEP 2 ----------------------------------- */
   /* Verify background and write inverted background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE)
   {
      for (j = 0u; j < RAM_BLOCKSIZE; j++)
      {
         if ( *(p + (uint32_t)RAM_SCRMBL[j]) != BCKGRND)
         {
            Result = ERROR;
         }
         *(p + (uint32_t)RAM_SCRMBL[j]) = INV_BCKGRND;
      }
   }

   /* ---------------------------- STEP 3 ----------------------------------- */
   /* Verify inverted background and write background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE)
   {
      for (j = 0u; j < RAM_BLOCKSIZE; j++)
      {
         if ( *(p + (uint32_t)RAM_SCRMBL[j]) != INV_BCKGRND)
         {
            Result = ERROR;
         }
         *(p + (uint32_t)RAM_SCRMBL[j]) = BCKGRND;
      }
   }

   /* ---------------------------- STEP 4 ----------------------------------- */
   /* Verify background and write inverted background with addresses decreasing */
   for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE)
   {
      for (j = 0u; j < RAM_BLOCKSIZE; j++)
      {
         if ( *(p - (uint32_t)RAM_REVSCRMBL[j]) != BCKGRND)
         {
            Result = ERROR;
         }
         *(p - (uint32_t)RAM_REVSCRMBL[j]) = INV_BCKGRND;
      }
   }

   /* ---------------------------- STEP 5 ----------------------------------- */
   /* Verify inverted background and write background with addresses decreasing */
   for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE)
   {
      for (j = 0u; j < RAM_BLOCKSIZE; j++)
      {
         if ( *(p - (uint32_t)RAM_REVSCRMBL[j]) != INV_BCKGRND)
         {
            Result = ERROR;
         }
         *(p - (uint32_t)RAM_REVSCRMBL[j]) = BCKGRND;
      }
   }

   /* ---------------------------- STEP 6 ----------------------------------- */
   /* Verify background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p++)
   {
      if (*p != BCKGRND)
      {
         Result = ERROR;    /* No need to take into account scrambling here */
      }
   }

  /* Restore destroyed return address back into the stack (all the content is destroyed).
     Next line of code supposes the {r4-r5,pc} for Keil(ARMCC 5.06) registers
     only was saved into stack by this test so their restored values are not valid: 
     => optiomizations at caller must be switched off as caller cannot relay on r4-r7 values!!!
	 The return opcode would be
	 POP {r4-r5,pc}
	 or
	 POP {r4-r7,pc}
	 depending on the version of the compiler.
	 So it is necessary to skip the registers(r4-r5, or r4-r7), only restore the return address to the 
	 corrupted stack.*/
   *((uint32_t *)(__current_sp()) + 2u) = ra;

   return(Result);
}

extern void Reset_Handler(void);
//This test is destructive and will initialize the whole RAM to zero.
void MemtestFunc(void)
{
	//It seems not necessary to call this function as we only use the default clock setting
//	SystemInit();
	
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
	
	LED_Initialize();

  /* --------------------- Variable memory functional test -------------------*/
  /* WARNING: Stack is zero-initialized when exiting from this routine */
  if (FullRamMarchC() != SUCCESS)
  {  
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'T');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'L');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
		
		FailSafePOR();
  }
	else
	{
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'T');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'K');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
	}
	
	Reset_Handler();
}

void print_list_content(XMC_LIST_t* sl)
{
	SList* pL = (SList *)*sl;
	
	uint32_t len = SLIST_GetLength(sl);
	printf("List Length:%u\n", len);
	
	for(uint32_t i=0; i<len; ++i, pL = pL->next)
	{
		printf("%u\t", *(uint32_t*)pL->pVal);
	}
	printf("\n");
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
	
  printf ("Data Structure Study For XMC1100 Bootkit by Automan @ Infineon BBS @%u Hz\n",
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
	
	//Demo for SList
	XMC_LIST_t intList;
	XMC_LIST_Init(&intList);
	print_list_content(&intList);
	
	printf("Now Add\n");
	uint32_t a = 1;
	SList nodeA = {.pVal = &a};
	SLIST_Add(&intList, &nodeA);
	print_list_content(&intList);
	uint32_t b = 2;
	SList nodeB = {.pVal = &b};
	SLIST_Add(&intList, &nodeB);	
	print_list_content(&intList);
	uint32_t c = 3;
	SList nodeC = {.pVal = &c};
	SLIST_Add(&intList, &nodeC);
	print_list_content(&intList);
	
	printf("Now Remove\n");
	SLIST_Remove(&intList, &nodeA);
	print_list_content(&intList);
	SLIST_Remove(&intList, &nodeC);
	print_list_content(&intList);
	
	//Demo for XMC_PRIOARRAY_t
	
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
