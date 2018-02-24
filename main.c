/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XMC1100.h>
#include <xmc_flash.h>
#include <xmc_gpio.h>
#include <xmc_rtc.h>
#include <xmc_scu.h>
#include <xmc_uart.h>

#include "lcd2004.h"
#include "led.h"

#ifndef HZ
#define HZ 1000
#endif

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {
    .data_bits = 8U, .stop_bits = 1U, .baudrate = 256000};

XMC_RTC_CONFIG_t rtc_config = {.time.seconds = 5U, .prescaler = 0x7fffU};

XMC_RTC_TIME_t init_rtc_time = {.year = 2018,
                                .month = XMC_RTC_MONTH_JANUARY,
                                .daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
                                .days = 27,
                                .hours = 15,
                                .minutes = 40,
                                .seconds = 55};

void SystemCoreClockSetup(void) {
  XMC_SCU_CLOCK_CONFIG_t clock_config = {
      .rtc_src = XMC_SCU_CLOCK_RTCCLKSRC_DCO2,
      .pclk_src = XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK,
      .fdiv = 0,
      .idiv = 1};

  XMC_SCU_CLOCK_Init(&clock_config);

  //  SystemCoreClockUpdate();
}

static inline void SimpleDelay(uint32_t d) {
  uint32_t t = d;
  while (--t) {
    __NOP();
  }
}

void XMC_AssertHandler(const char *const msg, const char *const file,
                       uint32_t line) {
  printf("Assert:%s,%s,%u\n", msg, file, line);

  while (1) {
    LED_Toggle(1);
    SimpleDelay(100000);
  }
}

void FailSafePOR(void) {
  while (1) {
    LED_Toggle(0);
    SimpleDelay(100000);
  }
}

/* Constants necessary for RAM test (RAM_END is word aligned ) */
#define RAM_SIZE 0x3FFC
#define RAM_START (uint32_t *)(0x20000000)
//#define RAM_START  (uint32_t *)(0x20000800)
#define RAM_END (uint32_t *)(0x20003FFC)
//#define RAM_END    (uint32_t *)(0x20003FFC)
/* These are the direct and inverted data (pattern) used during the RAM
test, performed using March C- Algorithm */
#define BCKGRND ((uint32_t)0x00000000uL)
#define INV_BCKGRND ((uint32_t)0xFFFFFFFFuL)

#define RT_RAM_BLOCKSIZE ((uint32_t)6u) /* Number of RAM words tested at once  \
                                         */

#define RAM_BLOCKSIZE ((uint32_t)16)
static const uint8_t RAM_SCRMBL[RAM_BLOCKSIZE] = {
    0u, 1u, 3u, 2u, 4u, 5u, 7u, 6u, 8u, 9u, 11u, 10u, 12u, 13u, 15u, 14u};
static const uint8_t RAM_REVSCRMBL[RAM_BLOCKSIZE] = {
    1u, 0u, 2u, 3u, 5u, 4u, 6u, 7u, 9u, 8u, 10u, 11u, 13u, 12u, 14u, 15u};

typedef enum { ERROR = 0, SUCCESS = !ERROR } ErrorStatus;

/**
 * @brief  This function verifies that RAM is functional,
 *   using the March C- algorithm.
 * @param :  None
 * @retval : ErrorStatus = (ERROR, SUCCESS)
 */
ErrorStatus FullRamMarchC(void) {
  ErrorStatus Result = SUCCESS;
  uint32_t *p; /* RAM pointer */
  uint32_t j;  /* Index for RAM physical addressing */

  uint32_t ra =
      __return_address(); /* save return address (as it will be destroyed) */

  /* ---------------------------- STEP 1 ----------------------------------- */
  /* Write background with addresses increasing */
  for (p = RAM_START; p <= RAM_END; p++) {
    /* Scrambling not important when there's no consecutive verify and write */
    *p = BCKGRND;
  }

  /* ---------------------------- STEP 2 ----------------------------------- */
  /* Verify background and write inverted background with addresses increasing
   */
  for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE) {
    for (j = 0u; j < RAM_BLOCKSIZE; j++) {
      if (*(p + (uint32_t)RAM_SCRMBL[j]) != BCKGRND) {
        Result = ERROR;
      }
      *(p + (uint32_t)RAM_SCRMBL[j]) = INV_BCKGRND;
    }
  }

  /* ---------------------------- STEP 3 ----------------------------------- */
  /* Verify inverted background and write background with addresses increasing
   */
  for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE) {
    for (j = 0u; j < RAM_BLOCKSIZE; j++) {
      if (*(p + (uint32_t)RAM_SCRMBL[j]) != INV_BCKGRND) {
        Result = ERROR;
      }
      *(p + (uint32_t)RAM_SCRMBL[j]) = BCKGRND;
    }
  }

  /* ---------------------------- STEP 4 ----------------------------------- */
  /* Verify background and write inverted background with addresses decreasing
   */
  for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE) {
    for (j = 0u; j < RAM_BLOCKSIZE; j++) {
      if (*(p - (uint32_t)RAM_REVSCRMBL[j]) != BCKGRND) {
        Result = ERROR;
      }
      *(p - (uint32_t)RAM_REVSCRMBL[j]) = INV_BCKGRND;
    }
  }

  /* ---------------------------- STEP 5 ----------------------------------- */
  /* Verify inverted background and write background with addresses decreasing
   */
  for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE) {
    for (j = 0u; j < RAM_BLOCKSIZE; j++) {
      if (*(p - (uint32_t)RAM_REVSCRMBL[j]) != INV_BCKGRND) {
        Result = ERROR;
      }
      *(p - (uint32_t)RAM_REVSCRMBL[j]) = BCKGRND;
    }
  }

  /* ---------------------------- STEP 6 ----------------------------------- */
  /* Verify background with addresses increasing */
  for (p = RAM_START; p <= RAM_END; p++) {
    if (*p != BCKGRND) {
      Result = ERROR; /* No need to take into account scrambling here */
    }
  }

  /* Restore destroyed return address back into the stack (all the content is
     destroyed). Next line of code supposes the {r4-r5,pc} for Keil(ARMCC 5.06)
     registers only was saved into stack by this test so their restored values
     are not valid:
     => optiomizations at caller must be switched off as caller cannot relay on
     r4-r7 values!!! The return opcode would be POP {r4-r5,pc} or POP {r4-r7,pc}
         depending on the version of the compiler.
         So it is necessary to skip the registers(r4-r5, or r4-r7), only restore
     the return address to the corrupted stack.*/
  *((uint32_t *)(__current_sp()) + 2u) = ra;

  return (Result);
}

#define PARITY_DSRAM1_TEST_ADDR 0x20001000
#define PARITY_TIMEOUT ((uint32_t)0xDEADU)

#define __sectionClassB__ __attribute__((section(".ClassB_code")))
#define __sectionClassB_Data__ __attribute__((section(".ClassB_data")))
#define PRIVILEGED_VAR __attribute__((section(".data.privileged")))
#define __sectionClassB_CRCREF                                                 \
  __attribute__((section("._CRC_area"), used, noinline))
#define __ALWAYSINLINE
#define __UNUSED __attribute__((unused))

#define OPCODE ((uint32_t)0x46874823)

#define EnterCriticalSection()                                                 \
  __disable_irq();                                                             \
  __DMB() /*!< enable atomic instructions */
#define ExitCriticalSection()                                                  \
  __DMB();                                                                     \
  __enable_irq() /*!< disable atomic instructions */

#ifdef TESSY
extern void TS_TessyDummy(void *TS_var);
#endif

#ifdef TESSY
#define TESSY_TESTCODE(x) TS_TessyDummy((x));
#else
#define TESSY_TESTCODE(x) /* Tessy Dummy  */ ;
#endif

#define ClassB_Status_RAMTest_Pos ((uint32_t)0)
#define ClassB_Status_RAMTest_Msk                                              \
  ((uint32_t)(0x00000001U << ClassB_Status_RAMTest_Pos))
#define ClassB_Status_ParityTest_Pos ((uint32_t)1)
#define ClassB_Status_ParityTest_Msk                                           \
  ((uint32_t)(0x00000001U << ClassB_Status_ParityTest_Pos))
#define ClassB_Status_ParityReturn_Pos ((uint32_t)2)
#define ClassB_Status_ParityReturn_Msk                                         \
  ((uint32_t)(0x00000001U << ClassB_Status_ParityReturn_Pos))

/*!
 * brief RAM test control states
 */
#define RAM_TEST_PENDING ((uint32_t)0x11) /* test is pending, no started */
#define RAM_TEST_DONE ((uint32_t)0x10)    /* test is complete */
#define RAM_TEST_PARITY_DATAP_PATTERN                                          \
  ((uint32_t)0x20002000) /* Write to 32 byte locations with parity disabled */
#define RAM_NEXT_RAM_PAGE ((uint32_t)0x04) /* Next RAM Page */

/*! generic type for ClassB functional results */
typedef enum ClassB_EnumTestResultType {
  ClassB_testFailed = 0U,    /*!< test result failed replacement */
  ClassB_testPassed = 1U,    /*!< test result passed replacement */
  ClassB_testInProgress = 2U /*!< test is still in progress replacement */
} ClassB_EnumTestResultType;

__sectionClassB_Data__ uint32_t ClassB_TrapMessage;

void SCU_0_IRQHandler(void) {
  /* check Fault reason */
  uint32_t IrqStatus;
  /* read the interrupt status Register */
  IrqStatus = SCU_INTERRUPT->SRRAW;
  /* SCU Interrupts */
  /* check parity error */
  if (SCU_INTERRUPT_SRRAW_PESRAMI_Msk ==
      (IrqStatus & SCU_INTERRUPT_SRRAW_PESRAMI_Msk)) {
    SCU_INTERRUPT->SRMSK &= ~((uint32_t)SCU_INTERRUPT_SRMSK_PESRAMI_Msk);
    SCU_INTERRUPT->SRCLR = ((uint32_t)SCU_INTERRUPT_SRCLR_PESRAMI_Msk);

    /* check parity error test enabled */
    if (ClassB_Status_ParityTest_Msk ==
        (ClassB_TrapMessage & ClassB_Status_ParityTest_Msk)) {
      /* state the trap recognized for parity test function */
      ClassB_TrapMessage |= ((uint32_t)ClassB_Status_ParityReturn_Msk);
    } else {
      /* user code here */
    }
  }
  /* Loss of clock */
  if (SCU_INTERRUPT_SRRAW_LOCI_Msk ==
      ((uint32_t)(IrqStatus & SCU_INTERRUPT_SRRAW_LOCI_Msk))) {
    /* clear the interrupt and mask */
    SCU_INTERRUPT->SRCLR = ((uint32_t)SCU_INTERRUPT_SRCLR_LOCI_Msk);
    SCU_INTERRUPT->SRMSK &= ~((uint32_t)SCU_INTERRUPT_SRMSK_LOCI_Msk);
  }
  /* user code here */
}

extern void SCU_0_IRQHandler(void);
#define OPCODE_ADDR ((uint32_t)0x20000040)
#define TARGET_ADDR ((uint32_t)0x200000d0)
#define TARGET SCU_0_IRQHandler
void ClassB_setup_SCU_0_vector(void) {
  uint32_t *pdestaddr = (uint32_t *)OPCODE_ADDR;

  /* copy the code */
  *pdestaddr = (uint32_t)OPCODE;
  /* copy the vector */
  pdestaddr = (uint32_t *)TARGET_ADDR;
  *pdestaddr = (uint32_t)TARGET;
}

ClassB_EnumTestResultType ClassB_RAMTest_Parity(void) {
  volatile uint32_t *TestParityDataPtr;
  volatile uint32_t TimeoutCtr;
  volatile uint32_t Temp __UNUSED = 0U;
  uint32_t TestParityDataBackup;
  uint32_t ReturnFlag;
  ClassB_EnumTestResultType result = ClassB_testPassed;

  /* set flags that will be used by the SRAM parity trap */
  ClassB_TrapMessage = ClassB_Status_ParityTest_Msk;

  TimeoutCtr = PARITY_TIMEOUT;

  /*** DSRAM1 Parity Check ***/
  /* clear result flag that will be used by the parity trap */
  ClassB_TrapMessage &= ~((uint32_t)ClassB_Status_ParityReturn_Msk);

  /* Point to test area */
  TestParityDataPtr = (volatile uint32_t *)PARITY_DSRAM1_TEST_ADDR;
  /* Store existing contents */
  TestParityDataBackup = *TestParityDataPtr;

  /* enable SCU0 interrupt */
  /* clear and mask the parity test interrupt */
  SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_PESRAMI_Pos);
  SET_BIT(SCU_INTERRUPT->SRMSK, SCU_INTERRUPT_SRMSK_PESRAMI_Pos);
  /*lint -save -e10 -e534 MISRA 2004 Rule 16.10 accepted */
  /* make sure the interrupt vector is available */
  ClassB_setup_SCU_0_vector();
  __enable_irq();
  /*lint -restore */
  /* enable the NVIC vector */
  NVIC_EnableIRQ(SCU_0_IRQn);

  /* Enable inverted parity for next SRAM write */
  SCU_GENERAL->PMTSR |= SCU_GENERAL_PMTSR_MTENS_Msk;

  /*lint -save -e2 Unclosed Quote MISRA 2004 Rule 1.2 accepted */
  /* Write to 32 bit locations with parity inverted */
  *TestParityDataPtr = RAM_TEST_PARITY_DATAP_PATTERN;
  /*lint -restore */

  /* disable Parity test */
  SCU_GENERAL->PMTSR = 0;

  /*lint -save -e838 not used variable accepted */
  /* Create parity error by reading location */
  Temp = *TestParityDataPtr;
  /*lint -restore */

  /* Check whether parity trap has occurred (before timeout expires) */
  do {
    /* Insert TESSY test code for simulating status from the exception on Parity
     * tests */
    TESSY_TESTCODE(&ClassB_TrapMessage);
    /* See whether parity trap has occurred */
    ReturnFlag =
        ((uint32_t)(ClassB_TrapMessage & ClassB_Status_ParityReturn_Msk));

    /* Decrement counter */
    TimeoutCtr--;
  } while (
      (TimeoutCtr > 0U) &&
      (ReturnFlag == 0U)); /* Not timed out? && Parity trap not occurred? */

  /*lint -save -e522 MISRA 2004 Rule 14.2 accepted */
  /* Has parity DSRAM1 check passed? */
  /* Has data error occurred? */
  if (TimeoutCtr == 0U) /* Timed out? */
  {
    result = ClassB_testFailed;
  } else {
    /* The parity trap has occurred (ReturnFlag != 0), i.e. check passed */
  }
  /*lint -restore */

  /* double clear interrupt flags */
  SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_PESRAMI_Pos);
  /* disable interrupt flags */
  CLR_BIT(SCU_INTERRUPT->SRMSK, SCU_INTERRUPT_SRMSK_PESRAMI_Pos);

  /* Restore existing contents */
  *TestParityDataPtr = TestParityDataBackup;

  /* Exit critical section */
  ExitCriticalSection();

  /* clear the test flags */
  ClassB_TrapMessage &= ~((uint32_t)(ClassB_Status_ParityTest_Msk |
                                     ClassB_Status_ParityReturn_Msk));

  return (result);
}

extern void Reset_Handler(void);
// This test is destructive and will initialize the whole RAM to zero.
void MemtestFunc(void) {
  // It seems not necessary to call this function as we only use the default
  // clock setting
  //	SystemInit();

  /*Initialize the UART driver */
  uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
  uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
  /* Configure UART channel */
  XMC_UART_CH_Init(XMC_UART0_CH1, &uart_config);
  XMC_UART_CH_SetInputSource(XMC_UART0_CH1, XMC_UART_CH_INPUT_RXD,
                             USIC0_C1_DX0_P1_3);
  /* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH1);
  /* Configure pins */
  XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);

  LED_Initialize();

  // Parity Test

  if (ClassB_testFailed == ClassB_RAMTest_Parity()) {
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'L');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
    FailSafePOR();
  } else {
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'K');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
  }

  // March C
  /* --------------------- Variable memory functional test -------------------*/
  /* WARNING: Stack is zero-initialized when exiting from this routine */
  if (FullRamMarchC() != SUCCESS) {
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'T');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'L');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');

    FailSafePOR();
  } else {
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'T');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, 'K');
    XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
  }

  Reset_Handler();
}

uint8_t line[4][21] = {__DATE__, __TIME__, "Happy Spring Festvl", "by Automan"};

int main(void) {
  __IO uint32_t tmpTick;
  __IO uint32_t deltaTick;
  __IO uint32_t i = 0;

  __IO XMC_RTC_TIME_t now_rtc_time;

  /* System timer configuration */
  SysTick_Config(SystemCoreClock / HZ);

  /*Initialize the UART driver */
  uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
  uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
  /* Configure UART channel */
  XMC_UART_CH_Init(XMC_UART0_CH1, &uart_config);
  XMC_UART_CH_SetInputSource(XMC_UART0_CH1, XMC_UART_CH_INPUT_RXD,
                             USIC0_C1_DX0_P1_3);

  /* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH1);

  /* Configure pins */
  XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);

  printf("LCD2004 4bit driver For XMC1100 Bootkit. Cortex M0 Rev:%u, @%u Hz\n",
         __CM0_REV, SystemCoreClock);
  printf("CPUID:%08X, MPU:%u\n", SCB->CPUID, __MPU_PRESENT);
  // RTC
  XMC_RTC_Init(&rtc_config);
  XMC_RTC_SetTime(&init_rtc_time);

  //  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);
  //  XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
  //  NVIC_SetPriority(SCU_1_IRQn, 3);
  //  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();

  LED_Initialize();

  LCD_Initialize();

  //	tmpTick = g_Ticks;
  //	for(uint16_t i=0; i<100; ++i)
  //	{
  LCD_displayL(0, 0, line[0]);
  LCD_displayL(1, 0, line[1]);
  LCD_displayL(2, 0, line[2]);
  LCD_displayL(3, 0, line[3]);
  //	}
  //	tmpTick = g_Ticks-tmpTick;
  //	printf("Ticks:%u\n", tmpTick);

  while (1) {
    //    LED_On(0);
    //    LED_On(1);
    //    LED_On(2);
    //    LED_On(3);
    LED_On(4);

    tmpTick = g_Ticks;
    while ((tmpTick + 2000) > g_Ticks) {
      __NOP();
      __WFI();
    }

    XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
    //		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes,
    //now_rtc_time.seconds);

    //    LED_Off(0);
    //    LED_Off(1);
    //    LED_Off(2);
    //    LED_Off(3);
    LED_Off(4);

    tmpTick = g_Ticks;
    while ((tmpTick + 2000) > g_Ticks) {
      __NOP();
      __WFI();
    }
  }
}
