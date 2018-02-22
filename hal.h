#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
	 
void HAL_Delay(uint32_t t);

uint32_t HAL_GetTick(void);

#ifdef __cplusplus
}
#endif
#endif /*__HAL_H__ */
