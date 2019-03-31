#pragma once

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

void CS_H(void);

void CS_L(void);

void SCLK_H(void);

void SCLK_L(void);

void MOSI_H(void);

void MOSI_L(void);

void MISO_H(void);

void MISO_L(void);

void soft_spi_init(void);
