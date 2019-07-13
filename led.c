#include "led.h"
#include <assert.h>
#include <xmc_gpio.h>

#define LED_COUNT 5

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Initialize(void) {
  XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
  XMC_GPIO_SetMode(XMC_GPIO_PORT0, 6, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
  XMC_GPIO_SetMode(XMC_GPIO_PORT0, 5, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);

  XMC_GPIO_SetMode(XMC_GPIO_PORT1, 4, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
  XMC_GPIO_SetMode(XMC_GPIO_PORT1, 5, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_On(uint8_t num) {
  //	assert(num<LED_COUNT);
  switch (num) {
  case 0:
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 7);
    break;

  case 1:
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 6);
    break;

  case 2:
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 5);
    break;

  case 3:
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 4);
    break;

  case 4:
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 5);
    break;

  default:
    break;
  }
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Off(uint8_t num) {
  //	assert(num<LED_COUNT);
  switch (num) {
  case 0:
    XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 7);
    break;

  case 1:
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 6);
    break;

  case 2:
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 5);
    break;

  case 3:
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 4);
    break;

  case 4:
    XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 5);
    break;

  default:
    break;
  }
}

void LED_Toggle(uint8_t num) {
  //	assert(num<LED_COUNT);

  switch (num) {
  case 0:
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT0, 7);
    break;

  case 1:
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT0, 6);
    break;

  case 2:
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT0, 5);
    break;

  case 3:
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 4);
    break;

  case 4:
    XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 5);
    break;

  default:
    break;
  }
}
