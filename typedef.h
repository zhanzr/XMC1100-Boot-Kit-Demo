/*! *****************************************************************************
 * @file            typedef.h
 * @brief           global types and definitions used in classB library
 * @version         $Rev: 651 $
 * @date            01. October 2014
 * $LastChangedDate: 2015-04-16 12:10:29 +0200 (Do, 16 Apr 2015) $
 *
 * @note            Copyright (C) 2016 Infineon Technologies AG. All rights reserved.
 *
 * @profile         global type definitions
 *
 * @par
 * Infineon Technologies AG (Infineon) is supplying this software for use with
 * Infineon’s microcontrollers. This file can be freely distributed within
 * development tools that are supporting such microcontrollers.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @originator      Hitex Development Tools GmbH
 * @author          gn
 * @bug             -
 * @attention       -
 * @compliance      MISRA rule 8.1 de-activated for MISRA check
 *                  MISRA 19.7 violation caused by makro usage is unavoidable
 *                  MISRA rule 19.10 de-activated for MISRA check
 *                  MISRA rule 19.11 de-activated for MISRA check
 * @warning         -
 * @todo            -
 *
 ******************************************************************************/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/*lint -save -e960 -e961 MISRA Rule 19.4, 19.7 accepted */
#ifndef Null
#define Null    (void *)0U
#endif

/*! MISRA rule 6.3 */
#ifndef float32
typedef float   float32;
#endif
#ifndef float64
typedef double  float64;
#endif

/* Define the standard boolean type */
typedef uint32_t boolean; 
#define FALSE ((boolean) 0)
#define TRUE  ((boolean) 1)

/** Bitmasks */
#define BITMASK_FF                 ((uint32_t) 0x000000FF)
#define BITMASK_1FF                ((uint32_t) 0x000001FF)
#define BITMASK_80000000           ((uint32_t) 0x80000000)
#define BITMASK_FFFFFFFF           ((uint32_t) 0xFFFFFFFF) 

/** Array Elements */
#define FIRST_ARRAY_ELEMENT   ((uint32_t) 0)
#define SECOND_ARRAY_ELEMENT  ((uint32_t) 1)
#define THIRD_ARRAY_ELEMENT   ((uint32_t) 2)
#define FOURTH_ARRAY_ELEMENT  ((uint32_t) 3)
#define FIFTH_ARRAY_ELEMENT   ((uint32_t) 4) 	
#define SIXTH_ARRAY_ELEMENT   ((uint32_t) 5) 	
#define SEVENTH_ARRAY_ELEMENT ((uint32_t) 6) 
#define EIGHTH_ARRAY_ELEMENT  ((uint32_t) 7) 	
#define NINTH_ARRAY_ELEMENT   ((uint32_t) 8) 	
#define TENTH_ARRAY_ELEMENT   ((uint32_t) 9) 	

#define PASSWORD_TO_UNLOCK_PRO_REG ((uint32_t) 0x000000C0)    /*!< Password to unlock protected registers */
#define PASSWORD_TO_LOCK_PRO_REG   ((uint32_t) 0x000000C3)    /*!< Password to lock protected registers */

/** Interrupt Priority */
#define NVIC_PRIORITY_0       ((uint32_t) 0)
#define NVIC_PRIORITY_1       ((uint32_t) 1)
#define NVIC_PRIORITY_2       ((uint32_t) 2)
#define NVIC_PRIORITY_3       ((uint32_t) 3)

/*! generic type for ClassB functional results */
typedef enum ClassB_EnumTestResultType
{
    ClassB_testFailed     = 0U,             /*!< test result failed replacement */
    ClassB_testPassed     = 1U,             /*!< test result passed replacement */
    ClassB_testInProgress = 2U              /*!< test is still in progress replacement */
} ClassB_EnumTestResultType;

typedef enum ClassB_EnumTestStateType
{
    ClassB_stateDone       = 0U,            /*!< test is done replacement */
    ClassB_statePending    = 1U,            /*!< test is pending replacement */
    ClassB_stateInProgress = 2U             /*!< test is still in progress replacement */
} ClassB_EnumTestStateType;

#include "critical_data.h"

/* CPU register test */
typedef struct ClassB_StructCPUTestType
{
    ClassB_EnumTestStateType  testState;    /*!< CPU register POST tests passed */
    ClassB_EnumTestResultType testResult;   /*!< CPU register POST test status (PASS/FAIL) */
} ClassB_StructCPUTestType;

/* RAM test modes */
typedef enum ClassB_EnumRamTestModeType
{
    ClassB_RAMtestNormal   = 0U,            /*!< test mode for RAM test without stack test */
    ClassB_RAMtestStack    = 1U             /*!< test mode for RAM test explicit stack test */
} ClassB_EnumRamTestModeType;

/* clock test */
typedef struct ClassB_StructClkTestType
{
    uint32_t                  Ratio;        /*!< ration of timer ticks to RTC ticks, defines the number of timer calls in the test*/
    uint32_t                  Tolerance;    /*!< tolerance of timer ticks, defines the deviation acceptable in the test*/
    uint32_t                  timerCounter; /*!< The counter Variable in the test */
    uint32_t                  baseTicks;    /*!< basic counter occurrences */
    uint32_t                  Min;          /*!< possible minimum value */
    uint32_t                  Max;          /*!< possible maximum value */
    ClassB_EnumTestResultType result;       /*!< result after test finished = (ClassB_EnumTestResultType + 1) */
} ClassB_StructClkTestType;

/* SPMon, flowcontrol */
typedef struct ClassB_StructFlowControlType
{
    Mod_StructCriticalUint32Type FlowData;  /*!< flags of the tasks */
    Mod_StructCriticalUint32Type Overflow;  /*!< overflow indication */
} ClassB_StructFlowControlType;

/* error messages and codes */
typedef enum ClassB_EnumProcIDType
{
    /** Watchdog_ID */
    ClassB_WDT_TEST_ID             = 1u,
    ClassB_WDT_SERVICE_ID          = 2u,
    /** Clock System */
    ClassB_CLK_ID                  = 10U,
    /** CPU Register IDs */
    ClassB_CPU_REG_ID              = 20u,
    ClassB_CPU_PC_ID               = 21U,
    /** RAM Parity */
    ClassB_PARITY_ID               = 30U,
    ClassB_PARITY_PSRAM_CHECK_ID   = 31u,
    ClassB_PARITY_DSRAM1_CHECK_ID  = 32u,
    ClassB_PARITY_DSRAM2_CHECK_ID  = 33u,
    /** Address Decoder */
    ClassB_AD_ID                   = 34U,
    /** RAM */
    ClassB_RAM_ID                  = 35U,
    /** Flash */
    ClassB_FLASH_ID                = 40U,
    /** FLOW_CONTROL_ID */
    ClassB_FLOW_CONTROL_ID         = 90u,
    ClassB_FAILSAFE_ID             = 91u,
    /* BIST IDs */
    ClassB_SPMON_ID                = 92u,
    ClassB_CPU_BIST_ID             = 100U,
    ClassB_PC_BIST_ID              = 200U,
    ClassB_RAM_BIST_ID             = 300U,
    ClassB_FLASH_BIST_ID           = 400U,
    ClassB_IRQ_BIST_ID             = 500U,
    ClassB_CLK_BIST_ID             = 600U,
    ClassB_FLASH_BIST_INIT_ID      = 700U
} ClassB_EnumProcIDType;

/* Interrupt test */
#define _VAL_A    ((uint32_t) 0x01234567)   /*!< values fixed to 32 bit regardless of compiler */
#define _VAL_B    ((uint32_t) 0x12345678)
#define _VAL_C    ((uint32_t) 0x23456789)
#define _VAL_D    ((uint32_t) 0x3456789A)
#define _VAL_E    ((uint32_t) 0x456789AB)
#define _VAL_F    ((uint32_t) 0x56789ABC)
#define _VAL_G    ((uint32_t) 0x6789ABCD)

/*!
 * @enum ClassB_EnumReturnValuesType
 * @brief reduces the possible return values on the type
 */
typedef enum ClassB_EnumReturnValuesType
{
    VAL_A = _VAL_A,                         /*!< hard coded return value for ClassB_PCTest__A function */
    VAL_B = _VAL_B,                         /*!< hard coded return value for ClassB_PCTest__B function */
    VAL_C = _VAL_C,                         /*!< hard coded return value for ClassB_PCTest__C function */
    VAL_D = _VAL_D,                         /*!< hard coded return value for ClassB_PCTest__D function */
    VAL_E = _VAL_E,                         /*!< hard coded return value for ClassB_PCTest__E function */
    VAL_F = _VAL_F,                         /*!< hard coded return value for ClassB_PCTest__F function */
    VAL_G = _VAL_G                          /*!< hard coded return value for ClassB_PCTest__G function */
}ClassB_EnumReturnValuesType;

/*!
 * @typedef    ClassB_PCTest_ReturnValues
 * @brief type declaration for the return values
 */
typedef enum ClassB_EnumReturnValuesType ClassB_PCTest_ReturnValues;

/*! @brief type structure for interrupt testing.
 *
 *  For each interrupt, which has to be tested, there has to be one instance of this structure.
 *  To initialize the struct, use the initialization function:
 *  \a ClassB_InitInterruptTest(IRQn_Type IRQn, uint8_t Mode, IRQTestData_t *CountSetup).
 */
typedef struct ClassB_StructIrqTestDataType
{
    IRQn_Type                 IRQIdx;            /*!< index (number) of the IRQ vector */
    uint16_t                  MinThres;          /*!< minimum threshold target of interrupt occurrences */
    uint16_t                  MaxThres;          /*!< maximum threshold target of interrupt occurrences */
    uint16_t                  EffectiveCount;    /*!< counter Variable*/
    ClassB_EnumTestResultType LastResult;        /*!< result Variable*/
} ClassB_StructIrqTestDataType;

/*! @brief type structure for interrupt testing.
 *
 *  For each interrupt, which has to be tested during BIST, there has to be one instance of this structure.
 *  To initialize the struct, use the initialization function:
 */
typedef struct ClassB_StructIrqBistTestDataType
{
    boolean             SelectIsrToTest;   /*!< This array enables the ISRs which are tested */    
    uint32_t            IsrMAxOccurrence;  /*!< This array counts the number of ISR occurrences during BIST */   
} ClassB_StructIrqBistTestDataType;

/*! @brief type structure for RAM testing.
 *
 *  Process parameter for the RAM test
 *  To initialize the struct, use the initialization function:
 *  \a ClassB_InitInterruptTest(IRQn_Type IRQn, uint8_t Mode, IRQTestData_t *CountSetup).
 */
typedef struct ClassB_StructRamTestDataType
{
    uint32_t                  StartAddress;      /*!< start address of the RAM to test */
    uint32_t                  Length;            /*!< length of the RAM to test */
    uint32_t                  NextAddress;       /*!< start address of the RAM to test */
    uint32_t                  RAMStart;          /*!< phys. Start address of the testable RAM */
    uint32_t                  RAMSize;           /*!< max. phys. size of the testable RAM */
    ClassB_EnumTestResultType LastResult;        /*!< result Variable*/
} ClassB_StructRamTestDataType;

/* Program Counter test */
typedef struct ClassB_StructPcDataType
{
    ClassB_PCTest_ReturnValues ret;
    uint32_t                   number;      /*!< action type */
} ClassB_StructPcDataType;

typedef ClassB_PCTest_ReturnValues (*ClassB_Exec_t)(ClassB_StructPcDataType *data);

/** generic callback function pointer */
typedef void (*ClassB_Callback_t) (ClassB_EnumTestResultType);

/*! generic callback function pointer */
typedef void (*ClassB_Callback_tt) (ClassB_StructFlowControlType*);

/**
 * FUNC_NULL_PTR
 * The macro FUNC_NULL_PTR is used to define the NULL_PTR value for the
 * function pointers
 */
#define FUNC_NULL_PTR ((ClassB_Callback_t)0u)

/* Flash test */
typedef struct ClassB_StructFlashSignType
{
    uint32_t CRC32Val;                     /*!< CRC32 value */
    uint32_t MemLength;                    /*!< memory length for calculation */
    uint32_t StartAddress;                 /*!< start address in Flash */
    uint32_t EndAddress;                   /*!< end address to test in Flash */
    uint32_t Status;                       /*!< status information of this structure */
} ClassB_StructFlashSignType;

#define MIN(a, b)          (((a) < (b)) ? (a) : (b))   /*!< macro returning smallest input */
#define MAX(a, b)          (((a) > (b)) ? (a) : (b))   /*!< macro returning biggest input */
#define CUT(a, b, c)       (MAX((a), MIN((b), (c))))   /*!< macro returning a (a>b>c),b (a<b<c), c(a<c<b) */
#define IsInRange(a, b, c) ((CUT((a), (b), (c)) == (b)) ? TRUE : FALSE) /*!< macro returning true if a<b<c */

#ifdef __cplusplus
}
#endif  /* __cplusplus */
/*lint -restore */
#endif  /* __TYPEDEF_H__ */

/************************************** EOF *********************************/
