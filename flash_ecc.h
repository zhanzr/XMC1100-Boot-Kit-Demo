/*! *****************************************************************************
 * @file            flash_ecc.h
 * @brief           IEC60335 Class B C-header Flash test API
 *                  internal FLASH POST and BIST internal FLASH test for all compiler
 * @version         $Rev: 651 $
 * @date            01. October 2014
 * $LastChangedDate: 2015-04-16 12:10:29 +0200 (Do, 16 Apr 2015) $
 *
 * @note            Copyright (C) 2016 Infineon Technologies AG. All rights reserved.
 *
 * @profile         API on Flash test POST and BIST library
 * @description     The file contains the C-API for Flash test on the Infineon
 *                  Cortex M0 device on base of the constant signature variant
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
 * @bug             none
 * @attention       none
 * @compliance      MISRA rule 8.1 de-activated for MISRA check
 * @warning         none
 * @todo            -
 *
 ******************************************************************************/

#ifndef __FLASH_ECC_H__
#define __FLASH_ECC_H__

/*! @addtogroup FLASH_ECC
 * @{
 */

#include "typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 ******************************************************************************
 * Macro definitions
 ******************************************************************************
 */
/* ROM function Table */
/*Start address of the ROM function table*/
#ifdef TESSY
    unsigned long TS_Rom_Function_Table[2];
    #define Fls_ROM_FUNCTION_TABLE_START    (unsigned long)TS_Rom_Function_Table
#else
#define FLASH_ROM_FUNCTION_TABLE_START   ((uint32_t) 0x00000100)
#endif

/*
 * This enumeration defines NVM ROM APIs return values for Fls app
 */
typedef enum FLASH_EnumRomNvmStatusType
{
    /** the function succeeded */
    FLASH_ROM_PASS               = (int32_t) 0x00010000U,
    /** generic error code */
    FLASH_ROM_E_FAIL             = (int32_t) 0x80010001U,
    /** source data not in RAM */
    FLASH_ROM_E_SRC_AREA_EXCCEED = (int32_t) 0x80010003U,
    /** source data is not 4 byte aligned */
    FLASH_ROM_E_SRC_ALIGNMENT    = (int32_t) 0x80010004U,
    /** NVM module cannot be physically accessed */
    FLASH_ROM_E_NVM_FAIL         = (int32_t) 0x80010005U,
    /** verification of written page not successful */
    FLASH_ROM_E_VERIFY           = (int32_t) 0x80010006U,
    /** destination data is not (completely) located in NVM */
    FLASH_ROM_E_DST_AREA_EXCEED  = (int32_t) 0x80010009U,
    /** destination data is not properly aligned */
    FLASH_ROM_E_DST_ALIGNMENT    = (int32_t) 0x80010010U
} FLASH_EnumRomNvmStatusType;

typedef enum FLASH_EnumEccTestModeType
{
    FLASH_ECC_TestMode_Reduced  = 0u,
    FLASH_ECC_TestMode_Full     = 1u
} FLASH_EnumEccTestModeType;


#define FLASH_ECCVERRRST_IDLESET            ((uint32_t) 0x3000)
/** Pointer to Erase Flash Page routine*/
#define FLASH_Erase                         ((uint32_t)(FLASH_ROM_FUNCTION_TABLE_START)
/** Pointer to Erase, Program & Verify Flash Page routine*/
#define FLASH_ProgVerify                    ((uint32_t)(FLASH_ROM_FUNCTION_TABLE_START + 0x04))
/** Macro for Erase Flash Page routine*/
#define FLASH_ErasePage                     ((uint32_t)(*((FLASH_EnumRomNvmStatusType (**) (uint32_t * src_add)) FLASH_Erase)))
/** Macro for ProgVerifyPage Flash Page routine*/
#define FLASH_ProgVerifyPage                (*((FLASH_EnumRomNvmStatusType (**) (uint32_t * src_add, uint32_t * dst_add))\
                                             FLASH_ProgVerify))
#define FLASH_PAGE_SIZE                     ((uint32_t) 256)

#define FLASH_WORD_SIZE                     (sizeof(uint32_t))
/*lint -restore */

#define FLASH_CMD_PROG_BLOCK                ((uint32_t) 0xA1)


/* one wordline including ECC */
typedef struct FLASH_StructEccWordType
{
    uint32_t    data_high_SecondWR;             /* second write value */
    uint32_t    data_low_SecondWR;              /* second write value */
    uint32_t    data_high_RB;                   /* read back value */
    uint32_t    data_low_RB;                    /* read back value */
} FLASH_StructEccWordType;

/* one page for ECC check */
typedef struct FLASH_StructEccPageType 
{
    uint32_t                ExpOkay;                        /* expected okay data (includes corrected single bit errors */
    uint32_t                ExpSingleBitErr;                /* expected single bit errors */
    uint32_t                ExpDoubleBitErr;                /* expected double bit errors */
    uint32_t                LimitToSBErr;                   /* expected initial words without double-Bit errors */
    uint32_t                ExpLinitSBErr;                  /* expected single bite errors in the initial word */
    uint32_t                WRInitValue_high;               /* initial first write value  */
    uint32_t                WRInitValue_low;                /* initial first write value  */
    FLASH_StructEccWordType PageData[64];
} FLASH_StructEccPageType;


/*!
 * @brief           public function to check the Flash ECC logic by reprogramming a Flash sector
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       ECC location must be configured in the file Classb_config.h and
 *                  flash sector must be free to reprogram
 * @traceability    SRS_008
 * @note            The function is placed in a specified code segment to force
 *                   the linker to locate it separately.
 *
 * @image html  ClassB_FLASHECC_Test_POST.jpg
 */
extern ClassB_EnumTestResultType ClassB_FLASHECC_Test_POST(void);

/*!
 * @brief           public function to check the Flash ECC logic
 * @param[in]       -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             POST ECC Test must be valid
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       The ECC test must be initialized running ther POST ECC Test
 * @traceability    SRS_010
 * @note            The function is placed in a specified code segment to force
 *                   the linker to locate it separately.
 *
 * @image html  ClassB_FLASHECC_Test_BIST.jpg
 */
extern ClassB_EnumTestResultType ClassB_FLASHECC_Test_BIST(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*!
 * @}
 */

#endif /* __FLASH_ECC_H__ */

/************************************** EOF *********************************/
