/******************************************************************************
File:			XMC1000_tseRoutine.c

	===========================================================================
	Copyright (C) 2012 � Infineon Technologies AG (IFX).
	All rights reserved.
	This document contains proprietary information belonging to IFX here.
	Passing on and copying of this document, and communication of its
	contents is not permitted without prior written authorisation.
	===========================================================================

******************************************************************************/
#include "XMC1000_TSE.h"

// fast square root algorithm taken from 
// Otto Peter: Prozessor zieht Wurzel, c't 1990, Heft 1, page 300-306
#define N_BITS 32
#define MAX_BIT ((N_BITS + 1) / 2 - 1)

#define TRUE 1
#define FALSE 0

uint32_t *tse_k1_ptr;
uint32_t *tse_k3_ptr;
uint32_t *tse_k2_ptr;
uint16_t *tse_min_ptr;
uint16_t *tse_max_ptr;
int8_t *tse_corrdata_ptr;

/**************************************************************************//**
Purpose:        Implementation of fast square root algorithm after Otto Peter's
                (Otto Peter: Prozessor zieht Wurzel, c't 1990, Heft 1, page 300-306)
 
@requirements   None   
@param          uint32_t    integer number unsigned
@return         square root
******************************************************************************/
uint32_t sqrt_5(uint32_t x)
{
    uint32_t xroot, m2, x2;

    xroot = 0;
    m2 = 1 << MAX_BIT * 2;
    do
    {
		x2 = xroot + m2;
		xroot >>= 1;
		if(x2<=x)
		{
			x -= x2; xroot += m2;
		}
    }while (m2 >>= 2);

    if(xroot < x) return xroot + 1;		// rounding, necessary

    return xroot;
}



#define MIN_TEMP_KELVIN      (233)
#define MAX_TEMP_KELVIN      (398)
#define ZERO_TEMP_KELVIN     (273)

/**************************************************************************//**
Purpose:        Implementation of CalcTemperature function
   
@requirements   None   
@param          None          
@return         uint32_t

Detailed description: The function is used to convert the ANATSEMON values to Kelvin. 

******************************************************************************/
uint32_t XMC1000_CalcTemperature(void)
{
    uint32_t rx;
    int32_t  ik1, ik3, ik2;
    int32_t  h1, h3;
    
    uint16_t tempConfigRecord;
    uint32_t calcTemperature;
    int8_t  tse_corrdata;

    tse_k1_ptr = (uint32_t*)0x10000F20;
    tse_k3_ptr = (uint32_t*)0x10000F24;
    tse_k2_ptr = (uint32_t*)0x10000F28;
    ik1 = (uint32_t) *tse_k1_ptr; //ANA_TSE_k1
    ik3 = (uint32_t) *tse_k3_ptr; //ANA_TSE_k3
    ik2 = (uint32_t) *tse_k2_ptr; //ANA_TSE_k2

    rx  = (uint32_t) SCU_ANALOG->ANATSEMON; //SCU_ANATSEMON.TSE_MON;

    /** Check the lower limit, if below use configANATSEMON_min as reference value */
    tse_min_ptr = (uint16_t*)0x10000F2C; //ANATSEMON_min
    tempConfigRecord = (uint16_t) *tse_min_ptr;
    if(rx > tempConfigRecord)
    {
        rx = tempConfigRecord;
    }

    /** Check the higher limit, if above use configANATSEMON_max as reverence value */
    tse_max_ptr = (uint16_t*)0x10000F2E;
    tempConfigRecord = (uint16_t) *tse_max_ptr; //ANATSEMON_max;
    if(rx < tempConfigRecord)
    {
        rx = tempConfigRecord;
    }

    h1 = rx * ik3;          // 2^44 rx*k3
    h1 = h1 - ik2;          // 2^42 rx*k3-k2

    h3 = ik1 * ik1;         // 2^42 k1^2
    h3 = h3 + h1;           // 2^42 k1^2+4*(rx*k3-k2)

    h3 = sqrt_5(h3);        // 2^23 sqrt(k1^2+4*(rx*k3-k2))
    h3 = h3 - ik1;          // 2^23 sqrt(k1^2+4*(rx*k3-k2))-k1
    h3 <<= 16;              // 2^40 (sqrt(k1^2+4*(rx*k3-k2))-k1)/2
    h1 >>= 7;               // 2^39 rx*k3-k2
    h3 = h3 / h1;           // 2^1  (sqrt(k1^2+4*(rx*k3-k2))-k1)/(2*(rx*k3-k2))
	   
    calcTemperature = (h3>>1)+(h3&0x1);// rounding

    /* Apply the temperature correction to compensate the low quality TSE sensor */
    if(calcTemperature < MIN_TEMP_KELVIN) 
    {
        /* If calculated temperature is bellow -45C, no correction can be done */
    	calcTemperature = MIN_TEMP_KELVIN;
    }
    if (calcTemperature > MAX_TEMP_KELVIN)
    {
        /* If calculated temperature is above 125C, no correction can be done */
    	calcTemperature = MAX_TEMP_KELVIN;
    }

    /* If the calculated temperature is in the expected range, add the correction and return the value */
    tse_corrdata_ptr = (int8_t*)0x10000F34;
    tse_corrdata = *tse_corrdata_ptr;
    tse_corrdata_ptr += calcTemperature - MIN_TEMP_KELVIN;
    tse_corrdata = *tse_corrdata_ptr;
    return (calcTemperature + tse_corrdata);//TSE_CorrData[calcTemperature - MIN_TEMP_KELVIN]
}

/**************************************************************************//**
Purpose:        Implementation of CalcTSEVAR function
  
@requirements   None   
@param          None
@return         uint32_t

Detailed description: The function is used to convert Kelvin temperatures to ANATSEMON values.   

If the conversion is not possible, the function returns 0x00

******************************************************************************/
uint32_t XMC1000_CalcTSEVAR(uint32_t temperature)
{
    int32_t h0, h1, h2;
    int32_t ik1, ik3, ik2;

    uint32_t searchStatus;
    uint16_t i;
    
    int8_t corrdata;

    searchStatus = FALSE;

    tse_corrdata_ptr = (int8_t*)0x10000F34;

    /* Lookup for the TSE sensor value corresponding to the real temperature; if nothing is found, use the user value. */
    for(i = 0; i <= (MAX_TEMP_KELVIN - MIN_TEMP_KELVIN); i++)
    {
    	corrdata = (int8_t) *tse_corrdata_ptr;
    	if(temperature == (MIN_TEMP_KELVIN + i + corrdata)) //TSE_CorrData[i]
        {        
            temperature = MIN_TEMP_KELVIN + i;
            searchStatus = TRUE;
            break;
        }
    	tse_corrdata_ptr++;
    }

    if(searchStatus == FALSE)
    {
        return 0;
    }

    tse_k1_ptr = (uint32_t*)0x10000F20;
    tse_k3_ptr = (uint32_t*)0x10000F24;
    tse_k2_ptr = (uint32_t*)0x10000F28;
    ik1 = (uint32_t) *tse_k1_ptr; //ANA_TSE_k1
    ik3 = (uint32_t) *tse_k3_ptr; //ANA_TSE_k3
    ik2 = (uint32_t) *tse_k2_ptr; //ANA_TSE_k2

    h0 = temperature * temperature;             //      t^2
    h0 >>= 2;               // 2^-2 t^2
    h1 = ik3 * h0;          // 2^28 k3*t^2
    h1 >>= 12;              // 2^16 k3*t^2
    h2 = ik2 >> 12;         // 2^32 k2
    h2 = h2 * h0;           // 2^30 k2*t^2  
    h0 = ik1 * temperature;           // 2^21 k1*t
    h0 <<= 9;               // 2^30 k1*t
    h0 = h2 - h0;           // 2^30 -k1*t+k2*t^2
    h0 = h0 + (1<<30);      // 2^30 1-k1*t+k2*t^2
    h0 = h0 / h1;           // 2^14 (1-k1*t+k2*t^2)/(k3*t^2)

    return h0;
}
