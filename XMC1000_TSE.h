#ifndef XMC1000_TSE_H
#define	XMC1000_TSE_H

#include <XMC1100.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32_t XMC1000_CalcTemperature_soft(void);
uint32_t XMC1000_CalcTSEVAR_soft(uint32_t temperature);

#endif //XMC1000_TSE_H
