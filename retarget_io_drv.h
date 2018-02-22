#ifndef __RETARGET_IO_DRV_H__
#define __RETARGET_IO_DRV_H__

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#ifdef __cplusplus
 extern "C" {
#endif
	 
#if defined(__ARMCC_VERSION)
int stdout_putchar (int ch);
#else
int _write (int fd, const void *buf, size_t count);
#endif

#ifdef __cplusplus
}
#endif

#endif
