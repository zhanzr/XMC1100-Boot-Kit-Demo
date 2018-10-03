#include <xmc_gpio.h>
#include "led.h"

/* LED GPIO Pins */
static const GPIO_PIN LED_PIN[] = 
{
  { XMC_GPIO_PORT0, 7 },
  { XMC_GPIO_PORT0, 5 },
  { XMC_GPIO_PORT0, 6 },
//  { XMC_GPIO_PORT1, 2 },
//  { XMC_GPIO_PORT1, 3 },
  { XMC_GPIO_PORT1, 4 },
  { XMC_GPIO_PORT1, 5 }
};

#define LED_COUNT (sizeof(LED_PIN)/sizeof(GPIO_PIN))


/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Initialize (void) 
{
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 5, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 6, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);

	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 4, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 5, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Uninitialize (void) 
{
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 5, XMC_GPIO_MODE_INPUT_TRISTATE);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 6, XMC_GPIO_MODE_INPUT_TRISTATE);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_INPUT_TRISTATE);

	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 4, XMC_GPIO_MODE_INPUT_TRISTATE);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 5, XMC_GPIO_MODE_INPUT_TRISTATE);
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_On (uint8_t num) 
{
	switch(num)
	{		
		case 0:
		XMC_GPIO_SetOutputHigh(LED_PIN[num].port, LED_PIN[num].pin);
		break;
		
		default:
		XMC_GPIO_SetOutputLow(LED_PIN[num].port, LED_PIN[num].pin);
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
void LED_Off (uint8_t num) 
{
	switch(num)
	{		
		case 0:
		XMC_GPIO_SetOutputLow(LED_PIN[num].port, LED_PIN[num].pin);
		break;
		
		default:
		XMC_GPIO_SetOutputHigh(LED_PIN[num].port, LED_PIN[num].pin);
		break;
	}
}

void LED_Toggle(uint8_t num) 
{
	XMC_GPIO_ToggleOutput(LED_PIN[num].port, LED_PIN[num].pin);
}
