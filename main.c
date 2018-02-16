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
#include "lcd2004.h"
#include "flash_ecc.h"

#ifndef HZ
#define	HZ	1000
#endif

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

#define PARITY_DSRAM1_TEST_ADDR 0x20001000
#define PARITY_TIMEOUT     ((uint32_t) 0xDEADU)

#define __sectionClassB__               __attribute__((section(".ClassB_code")))
#define __sectionClassB_Data__          __attribute__((section(".ClassB_data")))
#define PRIVILEGED_VAR                  __attribute__((section(".data.privileged")))
#define __sectionClassB_CRCREF          __attribute__((section("._CRC_area"),used, noinline))
#define __ALWAYSINLINE
#define __UNUSED                        __attribute__((unused))

#define OPCODE           ((uint32_t) 0x46874823)    

#define EnterCriticalSection()          __disable_irq(); __DMB() /*!< enable atomic instructions */
#define ExitCriticalSection()           __DMB(); __enable_irq()  /*!< disable atomic instructions */

#ifdef TESSY
    extern void TS_TessyDummy (void* TS_var);
#endif

#ifdef TESSY
    #define TESSY_TESTCODE( x ) TS_TessyDummy( (x)) ;
#else
    #define TESSY_TESTCODE( x ) /* Tessy Dummy  */ ;
#endif

#define ClassB_Status_RAMTest_Pos       ((uint32_t) 0)
#define ClassB_Status_RAMTest_Msk       ((uint32_t) (0x00000001U << ClassB_Status_RAMTest_Pos))
#define ClassB_Status_ParityTest_Pos    ((uint32_t) 1)
#define ClassB_Status_ParityTest_Msk    ((uint32_t) (0x00000001U << ClassB_Status_ParityTest_Pos))
#define ClassB_Status_ParityReturn_Pos  ((uint32_t) 2)
#define ClassB_Status_ParityReturn_Msk  ((uint32_t) (0x00000001U << ClassB_Status_ParityReturn_Pos))

/*!
 * brief RAM test control states
 */
#define RAM_TEST_PENDING              ((uint32_t) 0x11)       /* test is pending, no started */
#define RAM_TEST_DONE                 ((uint32_t) 0x10)       /* test is complete */
#define RAM_TEST_PARITY_DATAP_PATTERN ((uint32_t) 0x20002000) /* Write to 32 byte locations with parity disabled */
#define RAM_NEXT_RAM_PAGE             ((uint32_t) 0x04)       /* Next RAM Page */

__sectionClassB_Data__
uint32_t ClassB_TrapMessage;


void SCU_0_IRQHandler(void)
{
    /* check Fault reason */
    uint32_t IrqStatus;
    /* read the interrupt status Register */
    IrqStatus = SCU_INTERRUPT->SRRAW;
    /* SCU Interrupts */
    /* check parity error */
    if (SCU_INTERRUPT_SRRAW_PESRAMI_Msk == (IrqStatus & SCU_INTERRUPT_SRRAW_PESRAMI_Msk))
    {
        SCU_INTERRUPT->SRMSK &= ~((uint32_t) SCU_INTERRUPT_SRMSK_PESRAMI_Msk);
        SCU_INTERRUPT->SRCLR  = ((uint32_t) SCU_INTERRUPT_SRCLR_PESRAMI_Msk);
        
        /* check parity error test enabled */
        if ( ClassB_Status_ParityTest_Msk == (ClassB_TrapMessage & ClassB_Status_ParityTest_Msk))
        {
            /* state the trap recognized for parity test function */
            ClassB_TrapMessage |= ((uint32_t) ClassB_Status_ParityReturn_Msk);
        }
        else
        {
            /* user code here */
        }
    }
    /* Loss of clock */
    if (SCU_INTERRUPT_SRRAW_LOCI_Msk == ((uint32_t) (IrqStatus & SCU_INTERRUPT_SRRAW_LOCI_Msk)))
    {
        /* clear the interrupt and mask */
        SCU_INTERRUPT->SRCLR  = ((uint32_t) SCU_INTERRUPT_SRCLR_LOCI_Msk);
        SCU_INTERRUPT->SRMSK &= ~((uint32_t) SCU_INTERRUPT_SRMSK_LOCI_Msk);
    }
    /* user code here */
}

extern void SCU_0_IRQHandler(void);
    #define OPCODE_ADDR ((uint32_t)0x20000040)
    #define TARGET_ADDR ((uint32_t)0x200000d0)
    #define TARGET      SCU_0_IRQHandler
void ClassB_setup_SCU_0_vector(void)
{
    uint32_t * pdestaddr = (uint32_t *)OPCODE_ADDR;

    /* copy the code */
    *pdestaddr = (uint32_t)OPCODE;
    /* copy the vector */
    pdestaddr  = (uint32_t *)TARGET_ADDR;
    *pdestaddr = (uint32_t)TARGET;
}

ClassB_EnumTestResultType ClassB_RAMTest_Parity(void)
{
    volatile uint32_t * TestParityDataPtr;
    volatile uint32_t   TimeoutCtr;
    volatile uint32_t   Temp __UNUSED = 0U;
    uint32_t            TestParityDataBackup;
    uint32_t            ReturnFlag;
    ClassB_EnumTestResultType result     = ClassB_testPassed;

    /* set flags that will be used by the SRAM parity trap */
    ClassB_TrapMessage = ClassB_Status_ParityTest_Msk;

    TimeoutCtr = PARITY_TIMEOUT;

    /*** DSRAM1 Parity Check ***/
    /* clear result flag that will be used by the parity trap */
    ClassB_TrapMessage &= ~((uint32_t) ClassB_Status_ParityReturn_Msk);

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
    do
    {
        /* Insert TESSY test code for simulating status from the exception on Parity tests */
        TESSY_TESTCODE(&ClassB_TrapMessage);
        /* See whether parity trap has occurred */
        ReturnFlag = ((uint32_t) (ClassB_TrapMessage & ClassB_Status_ParityReturn_Msk));

        /* Decrement counter */
        TimeoutCtr--;
    } while ((TimeoutCtr > 0U) && (ReturnFlag == 0U));  /* Not timed out? && Parity trap not occurred? */
    
    /*lint -save -e522 MISRA 2004 Rule 14.2 accepted */
    /* Has parity DSRAM1 check passed? */
    /* Has data error occurred? */
    if (TimeoutCtr == 0U) /* Timed out? */
    {
        result = ClassB_testFailed;
    }
    else
    {
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
    ClassB_TrapMessage &= ~((uint32_t) (ClassB_Status_ParityTest_Msk | ClassB_Status_ParityReturn_Msk));

    return(result);
}

#ifndef Null
#define Null    (void *)0U
#endif

#define ClassB_FLASH_SIZE 0x00032000
#define ClassB_FLASH_START 0x10001000
#define ClassB_TEST_POST_FLASH_ECC 1
#define ClassB_TEST_POST_FLASH_ECC_PAGE 0x10010000
#define SIZE2K                          ((uint32_t) 0x00000500)  /*!< definition for 2k area */
#define SIZE4K                          ((uint32_t) 0x00001000)  /*!< definition for 4k area */
#define SIZE8K                          ((uint32_t) 0x00002000)  /*!< definition for 8k area */
#define SIZE16K                         ((uint32_t) 0x00004000)  /*!< definition for 16k area */
#define SIZE32K                         ((uint32_t) 0x00008000)  /*!< definition for 32k area */
#define SIZE64K                         ((uint32_t) 0x00010000)  /*!< definition for 64k area */
#define SIZE128K                        ((uint32_t) 0x00020000)  /*!< definition for 128k area */
#define SIZE256K                        ((uint32_t) 0x00040000)  /*!< definition for 256k area */

/** ECC pattern uses 4 blocks:
*   1. Block to verify the content is available (2 DWords)
*   2. Block no errors                          (8 DWords)
*   3. Block to verify single bit correction    (10 DWords)
*   4. Block to verify double bit detection     (14 DWord)
*   orignal pattern from ECC Test on Infineon TriCore devices
*/
static const FLASH_StructEccPageType FLASH_ECC_pattern =
{
    .ExpOkay = 57U,                                        /* expected okay patterns     */
    .ExpSingleBitErr = 6U,                                         /* expected single bit errors */
    .ExpDoubleBitErr = 14U,                                        /* expected double bit errors */
    .LimitToSBErr = 10U,                                        /* words without double-Bit errors */
    .ExpLinitSBErr = 3U,                                         /* expected single bite errors in the initial word */
    .WRInitValue_high = 0xA15E3F09U, .WRInitValue_low = 0x83D5A9C3U,                   /* initial first write value  */
                                                /* Error Msk                       */
    .PageData = 
		{   /* second write values     expected readback values:      Nr  rd  nok SB DB */
        {0xFFFFFFFFU, 0xFFFFFFFFU, 0xA15E3F09U, 0x83D5A9C3U }, /* #1,  2,  0, 0, 0 */
        {0xFFFFFFFFU, 0xFFFFFFFFU, 0xA15E3F09U, 0x83D5A9C3U }, /* #2,  4,  0, 0, 0 */

        {0xA1FFFFFFU, 0xFFFFFFFFU, 0x215E3F09U, 0x83D5A9C1U }, /* #3,  5,  2, 1, 0 */
        {0x41FBFFFFU, 0xFFFFFFFFU, 0x015A3F09U, 0x83D5A9C1U }, /* #4,  5,  3, 1, 0 */
        {0xC7FFFFFFU, 0xFFFFFFFFU, 0x815E3F09U, 0x83D5A9C1U }, /* #5,  6,  4, 1, 0 */
        {0xBFFFFFFFU, 0x7FFFFFFFU, 0xA11E3F09U, 0x03D5A9C3U }, /* #6,  8,  4, 2, 0 */
        {0x487FFFFFU, 0xFFFFFFFFU, 0x005E3F09U, 0x83D5A9C1U }, /* #7,  9,  4, 2, 0 */
        {0xFFFFFFFFU, 0xFFFFFF7FU, 0xA15E3F09U, 0x83D5A945U }, /* #8,  10, 6, 2, 0 */
        {0x137694A0U, 0x60775400U, 0x01561400U, 0x00550400U }, /* #9,  12, 6, 3, 0 */
        {0xFEFFFF9FU, 0xFFFFFFFFU, 0xA05E3F08U, 0x83D5A9C3U }, /* #10, 13, 7, 3, 0 */
        {0x1FF9FE00U, 0x2D7215A0U, 0x01583E00U, 0x01500180U }, /* #11, 15, 7, 3, 1 */
        {0xFFBFFFFCU, 0xBFFFFFFFU, 0xA11E3F08U, 0x83D5A9C3U }, /* #12, 17, 7, 3, 3 */
        {0x77B28F2CU, 0x4A1251B5U, 0x21120F28U, 0x02100181U }, /* #13, 19, 7, 4, 3 */
        {0xFEA92400U, 0x07FFFFFFU, 0xA0082400U, 0x03D5A9C3U }, /* #14, 21, 7, 4, 3 */
        {0xFBD7FFFFU, 0xFFFFFFFFU, 0xA1563F09U, 0x83D5A9C3U }, /* #15, 23, 7, 4, 3 */
        {0xFFE4FFFFU, 0xFFFFFFFFU, 0xA1443F89U, 0x83D5A9C3U }, /* #16, 25, 7, 5, 3 */
        {0xFFFC3FFFU, 0xFFFFFFFFU, 0xA15C3F09U, 0x83D5A9C3U }, /* #17, 27, 7, 5, 4 */
        {0xEA9BFFFFU, 0xFFFFFFFFU, 0xA01A3F09U, 0x83D5A9C3U }, /* #18, 29, 7, 5, 5 */
        {0xFFFEDCFFU, 0xDFFFF01FU, 0xA15E1C09U, 0x83D5A003U }, /* #19, 31, 7, 5, 6 */
        {0xE0D3FC0DU, 0xB0FFFFFFU, 0xA0523C09U, 0x80D5A9C3U }, /* #20, 33, 7, 5, 7 */
        {0xFFBF7FFFU, 0xFFFFFFFFU, 0xA11E3F09U, 0x83D5A9C3U }, /* #21, 35, 7, 5, 8 */
        {0xEB8BFFFFU, 0xFFFFFFFFU, 0xA10A3F09U, 0x83D5A9C3U }, /* #22, 37, 7, 5, 9 */
        {0xFFEE467FU, 0xFFFFFFFFU, 0xA14E0609U, 0x83D5A9C3U }, /* #23, 39, 7, 5, 10 */
        {0xFFFE47BFU, 0xFFFFFFFFU, 0xA15E0709U, 0x83D5A9C3U }, /* #24, 41, 7, 5, 11 */
        {0xFFEE7E7FU, 0xFFFFFFFFU, 0xA14E3E09U, 0x83D5A9C3U }, /* #25, 43, 7, 5, 12 */
        {0xFFFE67F0U, 0x3FFFFFFFU, 0xA15E2700U, 0x03D5A9C3U }, /* #26, 45, 7, 5, 13 */
        {0xFFF7F7FFU, 0xFFFFFFFFU, 0xA1563709U, 0x83D5A9C3U }, /* #27, 47, 7, 5, 13 */
        {0xF6F7C0BFU, 0xFFFFFFFFU, 0xA0560009U, 0x83D5A9C3U }, /* #28, 49, 7, 5, 13 */
        {0xF6D7FFBFU, 0xFFFFFFFFU, 0xA2563F09U, 0x83D5A9C3U }, /* #29, 51, 7, 6, 13 */
        {0xEFE37FDDU, 0xFFFFFFFFU, 0xA1423F09U, 0x83D5A9C3U }, /* #30, 53, 7, 6, 13 */
        {0xF6C7FFFFU, 0xFFFFFFFFU, 0xA0463F09U, 0x83D5A9C3U }, /* #31, 55, 7, 6, 13 */
        {0xFEB77FFFU, 0xFFFFFFFFU, 0xA0163F09U, 0x83D5A9C3U }, /* #32, 57, 7, 6, 14 */
    }
};

static ClassB_EnumTestResultType ClassB_FLASHECC_VerifyPage(void)
{
#ifdef TESSY
	#undef	ClassB_TEST_POST_FLASH_ECC_PAGE
	#define ClassB_TEST_POST_FLASH_ECC_PAGE	(unsigned long)TS_ClassB_TEST_POST_FLASH_ECC_PAGE
#endif
    /*lint -save -e923 -e946 MISRA Rule 11.4, 17.2 accepted */
    register uint32_t* DataPtr = (uint32_t*)(ClassB_TEST_POST_FLASH_ECC_PAGE);
    /*lint -restore */
    return ((FLASH_ECC_pattern.WRInitValue_low  == DataPtr[FIRST_ARRAY_ELEMENT]) &&
            (FLASH_ECC_pattern.WRInitValue_high == DataPtr[SECOND_ARRAY_ELEMENT]))
            ?  ClassB_testPassed : ClassB_testFailed;
}


__sectionClassB__
/*! @endcond */
static ClassB_EnumTestResultType ClassB_FLASHECC_ClearStatus(void)
{
    ClassB_EnumTestResultType Result = ClassB_testFailed;

    /* Input parameter check */
    if ((NVM != Null) && (SCU_INTERRUPT != Null))
    {
        /* clear error status*/
        /* reset ECC2READ,ECC1READ in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMPROG_RSTECC_Pos);
        /* reset Write protocol error in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMPROG_RSTVERR_Pos);
        /* clear Write protection in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMSTATUS_WRPERR_Pos);
        /* clear Verification Error in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMPROG_RSTVERR_Pos);
        /*lint -save -e923 MISRA Rule 11.1, 11.3 accepted */
        /** clear interrupts */
        SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_FLECC2I_Pos);

        Result = ClassB_testPassed;
    }
    else
    {
        Result = ClassB_testFailed;
    }
    /*lint -restore */
    return(Result);
}

__sectionClassB__
/*! @endcond */
static ClassB_EnumTestResultType ClassB_FLASHECC_ReadPage(uint8_t* CoutDBErr, FLASH_EnumEccTestModeType TestMode)
{
    uint32_t*                 lAddress;
    uint32_t                  readback  = 0u;
    uint32_t                  ECC_Index = 0u;
    const                     FLASH_StructEccWordType* ECC_Word;
    uint16_t*                 FlashStatus;
    ClassB_EnumTestResultType result = ClassB_testPassed;
    uint32_t                  lCount;
    uint8_t                   lCntOK    = 0u;
    uint8_t                   lCntSBErr = 0u;

    if ((NVM != Null) && (CoutDBErr != Null))
    {
        /* reset values */
        *CoutDBErr = 0u;
        /*lint -save -e923 -e929 MISRA Rule 11.1, 11.4 accepted */
        FlashStatus = (uint16_t*)&NVM->NVMSTATUS;
        lAddress    = (uint32_t*)ClassB_TEST_POST_FLASH_ECC_PAGE;
        lCount      = ((FLASH_PAGE_SIZE/FLASH_WORD_SIZE)/2u);
        /* limit the read numbers to non double-bit error reads */
        if (TestMode == FLASH_ECC_TestMode_Reduced)
        {
            lCount  = (uint32_t)FLASH_ECC_pattern.LimitToSBErr;
        }
        else
        {
            /** Do nothing */
        }

        /*lint -restore */
        while ((lCount > 0u) && (result == ClassB_testPassed))
        {
            /* Reading the memory:
               will cause a trap if ECC double bit-error is present.
               Double-Bit Error interrupt is disabled so double-Bit errors are
               counted by evaluating the trap flags.
               One double bit error in a 32-Bit word will lead to one traps,
               as we read two 32-Bit values for the single-bit error we just
               look at the status bit after each read and check the proper
               correction by comparing the values
            */
            ECC_Word = &FLASH_ECC_pattern.PageData[ECC_Index];
            ECC_Index++;
            /* read and prepare next address */
            readback = *lAddress;
            lAddress++;
            /* compare */
            if (readback == ECC_Word->data_low_RB)
            {
                lCntOK++;   /* correct data read may be corrected by single bit correction */
            }
            else
            {
                /** Do nothing */
            }

            /* reading and clearing the error status register */
            lCntSBErr  += (*FlashStatus & NVM_NVMSTATUS_ECC1READ_Msk) >> NVM_NVMSTATUS_ECC1READ_Pos;
            *CoutDBErr += (*FlashStatus & NVM_NVMSTATUS_ECC2READ_Msk) >> NVM_NVMSTATUS_ECC2READ_Pos;
            result = ClassB_FLASHECC_ClearStatus();
            if (result == ClassB_testPassed)
            {
                /* read and prepare next address */
                readback = *lAddress;
                lAddress++;
                /* compare */
                if (readback == ECC_Word->data_high_RB)
                {
                    lCntOK++;   /* correct data read may be corrected by single bit correction */
                }
                else
                {
                    /** Do nothing */
                }

                /* reading and clearing the error status register */
                lCntSBErr  += (*FlashStatus & NVM_NVMSTATUS_ECC1READ_Msk) >> NVM_NVMSTATUS_ECC1READ_Pos;
                *CoutDBErr += (*FlashStatus & NVM_NVMSTATUS_ECC2READ_Msk) >> NVM_NVMSTATUS_ECC2READ_Pos;
                result = ClassB_FLASHECC_ClearStatus();
            }
            else
            {
                /** Do nothing */
            }

            lCount--;
        }

        if (result == ClassB_testPassed)
        {
            if (TestMode == FLASH_ECC_TestMode_Reduced)
            {   /* runtime test will not check for double bit errors */
                if (FLASH_ECC_pattern.ExpLinitSBErr != lCntSBErr)
                {
                    result = ClassB_testFailed;
                }
                else
                {
                    /** Do nothing */
                }
            }
            else
            {
                if ((FLASH_ECC_pattern.ExpSingleBitErr != lCntSBErr) || (FLASH_ECC_pattern.ExpOkay != lCntOK))
                {
                    result = ClassB_testFailed;
                }
                else
                {
                    /** Do nothing */
                }
            }
        }
        else
        {
            /** Do nothing */
        }
    }
    else
    {
        result     = ClassB_testFailed;
    }

    return (result);
}

static ClassB_EnumTestResultType ClassB_FLASHECC_ProgramPage(void)
{
    /*lint -save -e923 MISRA 2004 Advisory Rule 11.1 accepted */
    uint32_t                   lContent[FLASH_PAGE_SIZE/FLASH_WORD_SIZE]; /* 256 bytes buffer */
    uint32_t*                  lAddress   = (uint32_t*)ClassB_TEST_POST_FLASH_ECC_PAGE;
    ClassB_EnumTestResultType  result     = ClassB_testPassed;
    FLASH_EnumRomNvmStatusType lstatus;
    uint8_t                    ProtectedSectors;
    uint8_t                    ECCSector;
    uint8_t                    lCount;
    uint8_t                    lSCount;

    if (NVM != Null)
    {
        /* Enabling flash Idle State*/
        NVM->NVMPROG = FLASH_ECCVERRRST_IDLESET;
        /* reset ECC2READ,ECC1READ in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMPROG_RSTECC_Pos);
        /* reset Write protocol error in NVMSTATUS */
        SET_BIT(NVM->NVMPROG, NVM_NVMPROG_RSTVERR_Pos);

        /* check write protection */
        /* determine actual ECC test sector number */
        ECCSector = ((uint8_t)(ClassB_TEST_POST_FLASH_ECC_PAGE - ClassB_FLASH_START) / SIZE4K);
        /* check number of protected sectors */
        ProtectedSectors = ((uint8_t)(NVM->NVMCONF & NVM_NVMCONF_SECPROT_Msk) >> NVM_NVMCONF_SECPROT_Pos);
        /* check protection size exceeds test space */
        if (ECCSector < ProtectedSectors)
        {
            result = ClassB_testFailed;
        }
        else
        {
            /** Do nothing */
        }

        if (result == ClassB_testPassed)
        {
            /*lint -save -e661 accepted */
            /* prepare data */
            for (lCount = 0u; lCount < ((uint8_t)(FLASH_PAGE_SIZE/sizeof(uint32_t))); lCount+=2)
            {
                lContent[lCount]      = FLASH_ECC_pattern.WRInitValue_low;
                lContent[lCount + 1u] = FLASH_ECC_pattern.WRInitValue_high;
            }
            /*lint -restore */
            /* save the data to Flash */
            /*lint -save -e929 MISRA Rule 11.4 accepted */
            /*lint -e961 MISRA 2004 Advisory Rule Rule 12.13 accepted */
            lstatus = FLASH_ProgVerifyPage((uint32_t*)&lContent[0], lAddress);
            /*lint -restore */

            if (lstatus != FLASH_ROM_PASS)
            {
                result = ClassB_testFailed;
            }
            else    /* Flashing worked perfect and second flash content can be written */
            {
                lContent[4u] = ClassB_TEST_POST_FLASH_ECC_PAGE + FLASH_PAGE_SIZE;
                /* write the secondary data */
                lSCount = 0u;
                do {
                    /* get the data from the struct */
                    lCount = FIRST_ARRAY_ELEMENT;
                    lContent[FIRST_ARRAY_ELEMENT]  = FLASH_ECC_pattern.PageData[lSCount].data_low_SecondWR;
                    lContent[SECOND_ARRAY_ELEMENT] = FLASH_ECC_pattern.PageData[lSCount].data_high_SecondWR;
                    lSCount++;
                    lContent[THIRD_ARRAY_ELEMENT]  = FLASH_ECC_pattern.PageData[lSCount].data_low_SecondWR;
                    lContent[FOURTH_ARRAY_ELEMENT] = FLASH_ECC_pattern.PageData[lSCount].data_high_SecondWR;
                    lSCount++;

                    /* commit the write to the flash module */
                    NVM->NVMPROG = ((NVM_NVMPROG_RSTECC_Msk | NVM_NVMPROG_RSTVERR_Msk) | FLASH_CMD_PROG_BLOCK);
                    *lAddress = lContent[FIRST_ARRAY_ELEMENT];
                    lAddress++;
                    *lAddress = lContent[SECOND_ARRAY_ELEMENT];
                    lAddress++;
                    *lAddress = lContent[THIRD_ARRAY_ELEMENT];
                    lAddress++;
                    *lAddress = lContent[FOURTH_ARRAY_ELEMENT];
                    lAddress++;

                    while (NVM->NVMSTATUS & NVM_NVMSTATUS_BUSY_Msk)
                    {
                        TESSY_TESTCODE(&NVM->NVMSTATUS);
                    }
                    lContent[5u] = NVM->NVMSTATUS & NVM_NVMSTATUS_WRPERR_Msk;
                /*lint -e946 MISRA Rule 11.4, 17.2 accepted */
                } while ((lAddress < (uint32_t*)lContent[FIFTH_ARRAY_ELEMENT]) && (lContent[SIXTH_ARRAY_ELEMENT] == 0u));

                /* check write protection errors */
                if (NVM->NVMSTATUS & NVM_NVMSTATUS_WRPERR_Msk)
                {
                    result = ClassB_testFailed;
                }
                else
                {
                    /** Do nothing */
                }
                /* clear flags */
                NVM->NVMPROG = 0u;
                (void)ClassB_FLASHECC_ClearStatus();
            }
        }
        else
        {
            /** Do nothing */
        }
    }
    else
    {
        result     = ClassB_testFailed;
    }

    return (result);
    /*lint -restore */
}

ClassB_EnumTestResultType ClassB_FLASHECC_Test_POST(void)
{
    ClassB_EnumTestResultType Result = ClassB_testPassed;
    uint8_t             DBErrCount;

    /* Input parameter check */
    if ((NVM != Null) && (SCU_INTERRUPT != Null))
    {
        /*lint -save -e923 MISRA Rule 11.1, 11.3 accepted */
        /** clear INT_ON flag */
        CLR_BIT(NVM->NVMPROG, NVM_NVMCONF_INT_ON_Pos);
        /** disable ECC trap */
        CLR_BIT(SCU_INTERRUPT->SRMSK, SCU_INTERRUPT_SRMSK_FLECC2I_Pos);
        /** clear interrupts */
        SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_FLECC2I_Pos);

        /* check page already programmed */
        if (ClassB_FLASHECC_VerifyPage() == ClassB_testFailed)
        {
            /* programm with page reference */
            Result = ClassB_FLASHECC_ProgramPage();
        }
        else
        {
            /** Do nothing */
        }

        /* continue if verifcation and programming passed successfully */
        if (Result == ClassB_testPassed)
        {
            /* read the complete tested page and generate ECC errors */
            Result = ClassB_FLASHECC_ReadPage(&DBErrCount, FLASH_ECC_TestMode_Full);
        }
        else
        {
            /** Do nothing */
        }

        /* check function return */
        if (Result == ClassB_testPassed)
        {
            /* clear all errors */
            Result = ClassB_FLASHECC_ClearStatus();
            /** clear interrupt flags */
            SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_FLECC2I_Pos);
        }
        else
        {
            /** Do nothing */
        }
    #if (ENABLE_FLASH_ECC_TRAP == ClassB_Enabled)
        /** enable ECC trap */
        SET_BIT(SCU_INTERRUPT->SRMSK, SCU_INTERRUPT_SRMSK_FLECC2I_Pos);
    #endif  /* ENABLE_FLASH_ECC_TRAP */
    /*lint -restore */
    }
    else
    {
        Result = ClassB_testFailed;
    }

    /* terminate test and restore trap function for runtime */
    return(Result);
}

extern void Reset_Handler(void);
//This test is destructive and will initialize the whole RAM to zero.
void MemtestFunc(void)
{
	//Skip to reset handler for test
	Reset_Handler();

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

	//Parity Test

	if (ClassB_testFailed == ClassB_RAMTest_Parity())
	{
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'L');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');			
		FailSafePOR();
	}
	else
	{				
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'K');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
	}
		
 /* test ECC logic on FLASH */

	if (ClassB_testFailed == ClassB_FLASHECC_Test_POST())
	{
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'E');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'L');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');			
		FailSafePOR();
	}
	else
	{				
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'E');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'K');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
	}
		
	//March C
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

uint8_t line[4][21] = {
	"Debug (printf) Viewer",
	"ITM Stimulus Port",
	"STDERR, components",
	"through the ITM"
};

#define ITERATION_NUM    200
#define SIEVE_TEST_SIZE    8190

void sieve_test( void )
{       
	int     i,prime,k,count,iter;
	uint32_t Begin_Time,End_Time;
	char flags[SIEVE_TEST_SIZE + 1];

	printf( "%u iterations\n", ITERATION_NUM );

	Begin_Time = g_Ticks;

	for( iter = 1; iter <= ITERATION_NUM; iter++ )     /* do program 10 times  */
	{
					count = 0;                      /* prime counter        */
					for( i = 0; i <= SIEVE_TEST_SIZE; i++ )    /* set all flags true   */
									flags[i] = true;
					for( i = 0; i <= SIEVE_TEST_SIZE; i++ )
					{
									if( flags[i] )          /* found a prime        */
									{
													prime = i + i + 3; /* twice index + 3   */
													for(k=i+prime; k<=SIEVE_TEST_SIZE; k+=prime)
																	flags[k] = false;
																					/* kill all multiples   */
													count++;        /* primes found         */
									}
					}
	}

	End_Time = g_Ticks;

	printf( "%d primes.\n", count ); /* primes found on 10th pass   */

	printf( "Time: %.3f sec.\n", (End_Time - Begin_Time)/(HZ*1.0) );
}

int main(void)
{
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
	__IO XMC_RTC_TIME_t now_rtc_time;

  /* System timer configuration */
	g_Ticks = 0;	
  SysTick_Config(SystemCoreClock / HZ);
	
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
	
  printf ("Sieve For XMC1100 Bootkit by Automan @ Infineon BBS @%u Hz\n",
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
	
//	LCD_Initialize();
//	
//	LCD_displayL(0, 0, line[0]);
//	LCD_displayL(1, 0, line[1]);
//	LCD_displayL(2, 0, line[2]);
//	LCD_displayL(3, 0, line[3]);
			
	sieve_test();			

	while (1)
  {				
    LED_On(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}
		
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
//		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);

    LED_Off(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}		
  }
}
