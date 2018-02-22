#ifndef __RTC_H__
#define	__RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#include <assert.h>

#include <xmc1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>

void RTC_Initialize (void);

#ifdef __cplusplus
}
#endif

#endif
