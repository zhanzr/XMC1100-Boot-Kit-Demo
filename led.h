#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <xmc1_gpio.h>

/* GPIO Pin identifier */
typedef struct _GPIO_PIN {
  XMC_GPIO_PORT_t *port;
  uint8_t         pin;
} GPIO_PIN;

void LED_Initialize (void) ;
void LED_Uninitialize (void) ;

void LED_On (uint8_t num) ;
void LED_Off (uint8_t num);
void LED_Toggle(uint8_t num);

#endif //LED_H
