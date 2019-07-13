#pragma once

#include <XMC1100.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN_TEMP_KELVIN      (233)
#define MAX_TEMP_KELVIN      (398)
#define ZERO_TEMP_KELVIN     (273)

uint32_t XMC1000_CalcTemperature(void);
uint32_t XMC1000_CalcTSEVAR(uint32_t temperature);
