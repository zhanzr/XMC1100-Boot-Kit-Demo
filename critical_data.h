/*! *****************************************************************************
 * @file            critical_data.h
 * @brief           ClassB header source for secure data storage
 *                  POST and BIST secure data storage for all compiler
 * @version         $Rev: 651 $
 * @date            01. October 2014
 * $LastChangedDate: 2015-04-16 12:10:29 +0200 (Do, 16 Apr 2015) $
 *
 * @note            Copyright (C) 2016 Infineon Technologies AG. All rights reserved.
 *
 * @profile         Critical data storage types and runtime library
 * @description     This file contains API to store, verify and read critical data
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
 * @compliance      MISRA 6.3 violation caused by basic types on typedefs
 *                  MISRA rule 8.1 de-activated for MISRA check
 *                  MISRA 19.7 violation caused by functions redefinition is unavoidable
 *                  MISRA 19.10 rule disabled caused by the define
 *                  MISRA 19.11 rule disabled caused by the define
 * @warning         -
 * @todo            -
 *
 ******************************************************************************/

#ifndef __CRITCAL_DATA_H__
#define __CRITCAL_DATA_H__

/*! @addtogroup CRITICAL_DATA
 * @{
 */

#include "typedef.h"
/*lint -save -e961 MISRA 2004 Advisory Rule 19.7 accepted */
/*lint -e768 -e769 enumeration constant unused accepted */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*! @file critical_data.h
 */

/*!
 * @brief           typedef for a critical floating point variable with 64 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */

typedef struct Mod_StructCriticalDoubleType
{
    float64 inverse;       /*!< The complement of the data content of the critical variable */
    float64 data;          /*!< Data content of the critical variable */
} Mod_StructCriticalDoubleType;

/*!
 * @brief           typedef for a critical floating point variable with 32 bits.
 *
 *                  This variable has to be checked with the macro
 *                   \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalFloatType
{
    float32 inverse;    /*!< The complement of the data content of the critical variable */
    float32 data;       /*!< Data content of the critical variable */
} Mod_StructCriticalFloatType;

/*!
 * @brief           typedef for a critical unsigned integer variable with 64 bits.
 *
 *                 This variable has to be checked with the macro
 *                 \a ClassB_CriticalDataValidate(criticalVar)
 *                 \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalUint64Type
{
    uint64_t inverse;       /*!< The complement of the data content of the critical variable */
    uint64_t data;          /*!< Data content of the critical variable */
} Mod_StructCriticalUint64Type;

/*!
 * @brief           typedef for a critical signed integer variable with 64 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalInt64Type
{
    int64_t inverse;        /*!< The complement of the data content of the critical variable */
    int64_t data;           /*!< Data content of the critical variable */
} Mod_StructCriticalInt64Type;

/*!
 * @brief           typedef for a critical unsigned integer variable with 32 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note

 */
typedef struct Mod_StructCriticalUint32Type
{
    uint32_t inverse;         /*!< The complement of the data content of the critical variable */
    uint32_t data;            /*!< Data content of the critical variable */
} Mod_StructCriticalUint32Type;

/*!
 * @brief           typedef for a critical signed integer variable with 32 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalInt32Type
{
    int32_t inverse;          /*!< The complement of the data content of the critical variable */
    int32_t data;             /*!< Data content of the critical variable */
} Mod_StructCriticalInt32Type;

/*!
 * @brief           typedef for a critical unsigned integer variable with 16 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalUint16Type
{
    uint16_t inverse;         /*!< The complement of the data content of the critical variable */
    uint16_t data;            /*!< Data content of the critical variable */
} Mod_StructCriticalUint16Type;

/*!
 * @brief           typedef for a critical signed integer variable with 16 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalInt16Type
{
    int16_t inverse;          /*!< The complement of the data content of the critical variable */
    int16_t data;             /*!< Data content of the critical variable */
} Mod_StructCriticalInt16Type;

/*!
 * @brief           typedef for a critical unsigned integer variable with 8 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalUint8Type
{
    uint8_t inverse;          /*!< The complement of the data content of the critical variable */
    uint8_t data;             /*!< Data content of the critical variable */
} Mod_StructCriticalUint8Type;

/*!
 * @brief           typedef for a critical signed integer variable with 8 bits.
 *
 *                  This variable has to be checked with the macro
 *                  \a ClassB_CriticalDataValidate(criticalVar)
 *                  \see ClassB_CriticalDataValidate(criticalVar)
 * @param           -
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 */
typedef struct Mod_StructCriticalInt8Type
{
    int8_t inverse;           /*!< The complement of the data content of the critical variable */
    int8_t data;              /*!< Data content of the critical variable */
} Mod_StructCriticalInt8Type;

/*!
 * @brief           Macro to check critical variables.
 * @param[in]       criticalVar  target variable of critical type
 * @return          passed or failed. See \link #ClassB_EnumTestResultType ClassB_EnumTestResultType \endlink.
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instance such a critical variable, the macro \a ClassB_CriticalDataInit(value)
 *                   has to be used to initialize the struct.
 * @traceability    SRS_014
 * @note
 *
 * @image html ClassB_CriticalDataValidate.jpg
 */
#define ClassB_CriticalDataValidate(criticalVar) \
    ((criticalVar.data == ~(criticalVar.inverse)) ? (ClassB_testPassed) : (ClassB_testFailed))

/*!
 * @brief           Macro to read critical variables.
 * @param[in]       criticalVar  target variable of critical type
 * @return          void
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instanciate such a critical variable, use the macro with initial value
 * @traceability    SRS_014
 * @note
 *
 * @image html ClassB_CriticalDataPop.jpg
 */
#define ClassB_CriticalDataPop(criticalVar) \
    (criticalVar.data)

/*!
 * @brief           Macro to write critical variables.
 * @param[in]       criticalVar  target variable of critical type
 * @param[out]      value     new value
 * @return          void
 * @pre             None
 * @post            None
 * @sync            Synchronous
 * @reentrancy      non - reentrant
 * @attention       To instanciate such a critical variable, use the macro with initial value
 * @traceability    SRS_014
 * @note
 *
 * @image html ClassB_CriticalDataPush.jpg
 */
#define ClassB_CriticalDataPush(criticalVar, value) { \
    criticalVar.inverse = ~value;                     \
    criticalVar.data    = value;                      \
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*!
 * @}
 */
/*lint -restore */
#endif  /* __CRITCAL_DATA_H__ */

/************************************** EOF *********************************/
