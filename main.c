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

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

static inline void simpe_delay(uint32_t d) {
	uint32_t t = d;
	while(--t) {
		__NOP();
	}
}

void FailSafePOR(void) {
	while(1) {
		LED_On(0);
		simpe_delay(100000);
		LED_Off(0);
		simpe_delay(100000);
	}
}

/* Constants necessary for RAM test (RAM_END is word aligned ) */
#define	RAM_SIZE		0x3FFC
#define RAM_START  (uint32_t *)(0x20000000)  
#define RAM_END    (uint32_t *)((uint32_t)RAM_START + RAM_SIZE)

/* These are the direct and inverted data (pattern) used during the RAM
test, performed using March C- Algorithm */
#define BCKGRND     ((uint32_t)0x00000000uL)
#define INV_BCKGRND ((uint32_t)~BCKGRND)

static const uint8_t RAM_SCRMBL[] = {0u,1u,3u,2u,4u,5u,7u,6u,8u,9u,11u,10u,12u,13u,15u,14u};
static const uint8_t RAM_REVSCRMBL[] = {1u,0u,2u,3u,5u,4u,6u,7u,9u,8u,10u,11u,13u,12u,14u,15u};
#define RAM_BLOCKSIZE         (sizeof(RAM_SCRMBL))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/**
  * @brief  This function verifies that RAM is functional,
  *   using the March C- algorithm.
  * @param :  None
  * @retval : ErrorStatus = (ERROR, SUCCESS)
  */
ErrorStatus FullRamMarchC(void) {
      ErrorStatus Result = SUCCESS;
      uint32_t *p;       /* RAM pointer */
      uint32_t j;        /* Index for RAM physical addressing */
      
      uint32_t ra= __return_address(); /* save return address (as it will be destroyed) */

   /* ---------------------------- STEP 1 ----------------------------------- */
   /* Write background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p++) {
      /* Scrambling not important when there's no consecutive verify and write */
      *p = BCKGRND;
   }

   /* ---------------------------- STEP 2 ----------------------------------- */
   /* Verify background and write inverted background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE) {
      for (j = 0u; j < RAM_BLOCKSIZE; j++) {
         if ( *(p + (uint32_t)RAM_SCRMBL[j]) != BCKGRND) {
            Result = ERROR;
         }
         *(p + (uint32_t)RAM_SCRMBL[j]) = INV_BCKGRND;
      }
   }

   /* ---------------------------- STEP 3 ----------------------------------- */
   /* Verify inverted background and write background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p += RAM_BLOCKSIZE) {
      for (j = 0u; j < RAM_BLOCKSIZE; j++) {
         if ( *(p + (uint32_t)RAM_SCRMBL[j]) != INV_BCKGRND) {
            Result = ERROR;
         }
         *(p + (uint32_t)RAM_SCRMBL[j]) = BCKGRND;
      }
   }

   /* ---------------------------- STEP 4 ----------------------------------- */
   /* Verify background and write inverted background with addresses decreasing */
   for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE) {
      for (j = 0u; j < RAM_BLOCKSIZE; j++) {
         if ( *(p - (uint32_t)RAM_REVSCRMBL[j]) != BCKGRND) {
            Result = ERROR;
         }
         *(p - (uint32_t)RAM_REVSCRMBL[j]) = INV_BCKGRND;
      }
   }

   /* ---------------------------- STEP 5 ----------------------------------- */
   /* Verify inverted background and write background with addresses decreasing */
   for (p = RAM_END; p > RAM_START; p -= RAM_BLOCKSIZE) {
      for (j = 0u; j < RAM_BLOCKSIZE; j++) {
         if ( *(p - (uint32_t)RAM_REVSCRMBL[j]) != INV_BCKGRND) {
            Result = ERROR;
         }
         *(p - (uint32_t)RAM_REVSCRMBL[j]) = BCKGRND;
      }
   }

   /* ---------------------------- STEP 6 ----------------------------------- */
   /* Verify background with addresses increasing */
   for (p = RAM_START; p <= RAM_END; p++) {
      if (*p != BCKGRND) {
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

#define EnterCriticalSection()          __disable_irq(); __DMB() /*!< enable atomic instructions */
#define ExitCriticalSection()           __DMB(); __enable_irq()  /*!< disable atomic instructions */

#define ClassB_Status_RAMTest_Pos       ((uint32_t) 0)
#define ClassB_Status_RAMTest_Msk       ((uint32_t) (0x00000001U << ClassB_Status_RAMTest_Pos))
#define ClassB_Status_ParityTest_Pos    ((uint32_t) 1)
#define ClassB_Status_ParityTest_Msk    ((uint32_t) (0x00000001U << ClassB_Status_ParityTest_Pos))
#define ClassB_Status_ParityReturn_Pos  ((uint32_t) 2)
#define ClassB_Status_ParityReturn_Msk  ((uint32_t) (0x00000001U << ClassB_Status_ParityReturn_Pos))

/*! generic type for ClassB functional results */
typedef enum ClassB_EnumTestResultType
{
    ClassB_testFailed     = 0U,             /*!< test result failed replacement */
    ClassB_testPassed     = 1U,             /*!< test result passed replacement */
    ClassB_testInProgress = 2U              /*!< test is still in progress replacement */
} ClassB_EnumTestResultType;

uint32_t ClassB_TrapMessage;

void SCU_0_IRQHandler(void) {
    /* check Fault reason */
    uint32_t IrqStatus;
    /* read the interrupt status Register */
    IrqStatus = SCU_INTERRUPT->SRRAW;
    /* SCU Interrupts */
    /* check parity error */
    if (SCU_INTERRUPT_SRRAW_PESRAMI_Msk == (IrqStatus & SCU_INTERRUPT_SRRAW_PESRAMI_Msk)) {
        SCU_INTERRUPT->SRMSK &= ~((uint32_t) SCU_INTERRUPT_SRMSK_PESRAMI_Msk);
        SCU_INTERRUPT->SRCLR  = ((uint32_t) SCU_INTERRUPT_SRCLR_PESRAMI_Msk);
        
        /* check parity error test enabled */
        if ( ClassB_Status_ParityTest_Msk == (ClassB_TrapMessage & ClassB_Status_ParityTest_Msk)) {
          /* state the trap recognized for parity test function */
					ClassB_TrapMessage |= ((uint32_t) ClassB_Status_ParityReturn_Msk);								
					XMC_UART_CH_Transmit(XMC_UART0_CH1, '$');
        } else {
          /* user code here */
					XMC_UART_CH_Transmit(XMC_UART0_CH1, '*');
        }
    }
    /* Loss of clock */
    if (SCU_INTERRUPT_SRRAW_LOCI_Msk == ((uint32_t) (IrqStatus & SCU_INTERRUPT_SRRAW_LOCI_Msk))) {
        /* clear the interrupt and mask */
        SCU_INTERRUPT->SRCLR  = ((uint32_t) SCU_INTERRUPT_SRCLR_LOCI_Msk);
        SCU_INTERRUPT->SRMSK &= ~((uint32_t) SCU_INTERRUPT_SRMSK_LOCI_Msk);
    }
    /* user code here */
}

//0x20000040 4823      LDR      r0,[pc,#140]  ; @0x200000D0
//0x20000042 4687      MOV      pc,r0
#define OPCODE           ((uint32_t) 0x46874823)    
void ClassB_setup_SCU_0_vector(void) {
    /* copy the code */
    *((uint32_t *)0x20000040) = (uint32_t)OPCODE;
    /* copy the vector */
    *((uint32_t *)0x200000d0) = (uint32_t)SCU_0_IRQHandler;
}

ClassB_EnumTestResultType ClassB_RAMTest_Parity(void) {
#define RAM_TEST_PARITY_DATAP_PATTERN ((uint32_t) 0x20002000) /* Write to 32 byte locations with parity disabled */
	
    volatile uint32_t   TimeoutCtr;
    volatile uint32_t   Temp = 0U;
    uint32_t            TestParityDataBackup;
    uint32_t            ReturnFlag;
    ClassB_EnumTestResultType result     = ClassB_testPassed;

    /* set flags that will be used by the SRAM parity trap */
    ClassB_TrapMessage = ClassB_Status_ParityTest_Msk;

    TimeoutCtr = PARITY_TIMEOUT;

    /*** DSRAM1 Parity Check ***/
    /* clear result flag that will be used by the parity trap */
    ClassB_TrapMessage &= ~((uint32_t) ClassB_Status_ParityReturn_Msk);

    /* Store existing contents */
    TestParityDataBackup = *(volatile uint32_t *)PARITY_DSRAM1_TEST_ADDR;

    /* enable SCU0 interrupt */
    /* clear and mask the parity test interrupt */
    SET_BIT(SCU_INTERRUPT->SRCLR, SCU_INTERRUPT_SRCLR_PESRAMI_Pos);
    SET_BIT(SCU_INTERRUPT->SRMSK, SCU_INTERRUPT_SRMSK_PESRAMI_Pos);
    /*lint -save -e10 -e534 MISRA 2004 Rule 16.10 accepted */
    /* make sure the interrupt vector is available */
    ClassB_setup_SCU_0_vector();
    __enable_irq();
		
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '>');
    /* enable the NVIC vector */
    NVIC_EnableIRQ(SCU_0_IRQn);
    
    /* Enable inverted parity for next SRAM write */
    SCU_GENERAL->PMTSR |= SCU_GENERAL_PMTSR_MTENS_Msk;

    /* Write to 32 bit locations with parity inverted */
    *(volatile uint32_t *)PARITY_DSRAM1_TEST_ADDR = RAM_TEST_PARITY_DATAP_PATTERN;
    /*lint -restore */

    /* disable Parity test */
    SCU_GENERAL->PMTSR = 0;

    /*lint -save -e838 not used variable accepted */
    /* Create parity error by reading location */
    Temp = *(volatile uint32_t *)PARITY_DSRAM1_TEST_ADDR;
    /*lint -restore */

    /* Check whether parity trap has occurred (before timeout expires) */
    do {
			/* Insert TESSY test code for simulating status from the exception on Parity tests */
			&ClassB_TrapMessage;
			/* See whether parity trap has occurred */
			ReturnFlag = ((uint32_t) (ClassB_TrapMessage & ClassB_Status_ParityReturn_Msk));

			/* Decrement counter */
			TimeoutCtr--;
			XMC_UART_CH_Transmit(XMC_UART0_CH1, '.');
    } while ((TimeoutCtr > 0U) && (ReturnFlag == 0U));  /* Not timed out? && Parity trap not occurred? */
    
    /*lint -save -e522 MISRA 2004 Rule 14.2 accepted */
    /* Has parity DSRAM1 check passed? */
    /* Has data error occurred? */
    if (TimeoutCtr == 0U) {
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
    *(volatile uint32_t *)PARITY_DSRAM1_TEST_ADDR = TestParityDataBackup;

    /* Exit critical section */
    ExitCriticalSection();

    /* clear the test flags */
    ClassB_TrapMessage &= ~((uint32_t) (ClassB_Status_ParityTest_Msk | ClassB_Status_ParityReturn_Msk));

    return(result);
}

extern void Reset_Handler(void);
//This test is destructive and will initialize the whole RAM to zero.
void MemtestFunc(void) {	
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
	if (ClassB_testFailed == ClassB_RAMTest_Parity()) {
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'F');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');			
		FailSafePOR();
	} else {				
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'P');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
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
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
		
		FailSafePOR();
  }
	else
	{
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'R');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'T');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, 'O');
		XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
	}
	
	Reset_Handler();
}

int main(void) {
	__IO uint32_t tmpTick;
	
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
	
  printf ("RAM parity test For XMC1100 Bootkit by Automan @%u Hz %p\n",
	SystemCoreClock, &ClassB_TrapMessage	);
	
	LED_Initialize();
	
	while (1) {				
    LED_On(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+1000) > g_Ticks) {
			__WFI();
		}

    LED_Off(4);

		tmpTick = g_Ticks;
		while((tmpTick+1000) > g_Ticks) {
			__WFI();
		}		
  }
}
