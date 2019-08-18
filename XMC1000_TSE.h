#ifndef __XMC1000_TSE_H__
#define __XMC1000_TSE_H__

#include <XMC1100.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32_t XMC1000_CalcTemperature(void);
uint32_t XMC1000_CalcTSEVAR(uint32_t temperature);

#endif	//__XMC1000_TSE_H__
