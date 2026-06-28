#ifndef __XMC1000_TSE_H__
#define __XMC1000_TSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <XMC1100.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN_TEMP_KELVIN      (233)
#define MAX_TEMP_KELVIN      (398)
#define ZERO_TEMP_KELVIN     (273)

uint32_t XMC1000_CalcTemperatureUser(void);
uint32_t XMC1000_CalcTSEVARUser(uint32_t temperature);

#ifdef __cplusplus
}
#endif

#endif /* __XMC1000_TSE_H__ */
