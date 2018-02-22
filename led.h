#ifndef __LED_H__
#define __LED_H__

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
	 
/* GPIO Pin identifier */
typedef struct _GPIO_PIN {
  XMC_GPIO_PORT_t *port;
  uint8_t         pin;
} GPIO_PIN;

void LED_Initialize (void) ;
void LED_Uninitialize (void) ;

void LED_On (uint8_t num) ;
void LED_Off (uint8_t num);
void LED_Toggle (uint8_t num);

#ifdef __cplusplus
}
#endif
#endif /*__LED_H__ */
