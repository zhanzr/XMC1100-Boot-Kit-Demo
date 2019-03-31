#include <assert.h>
#include <xmc_gpio.h>
#include <xmc_spi.h>
#include "led.h"

/* LED GPIO Pins */
static const GPIO_PIN LED_PIN[] = 
{
  { XMC_GPIO_PORT0, 7 },
  { XMC_GPIO_PORT0, 5 },
  { XMC_GPIO_PORT0, 6 },

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
	assert(num<LED_COUNT);
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
	assert(num<LED_COUNT);
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

void LED_Toggle (uint8_t num) 
{
	assert(num<LED_COUNT);
	
	XMC_GPIO_ToggleOutput(LED_PIN[num].port, LED_PIN[num].pin);
}

//Soft SPI	
void CS_H(void){
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 9);	
}

void CS_L(void){
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 9);	
}

void SCLK_H(void){
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 8);	
}

void SCLK_L(void){
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 8);	
}

void MOSI_H(void){
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 0);	
}

void MOSI_L(void){
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 0);	
}

void MISO_H(void){
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);	
}

void MISO_L(void){
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);	
}


XMC_GPIO_CONFIG_t rx_pin_config;
XMC_GPIO_CONFIG_t tx_pin_config;
XMC_GPIO_CONFIG_t selo_pin_config;
XMC_GPIO_CONFIG_t clk_pin_config;	

/**
 * @brief SPI configuration structure
*/
XMC_SPI_CH_CONFIG_t spi_config =
{
  .baudrate = 500000,
  .bus_mode = XMC_SPI_CH_BUS_MODE_MASTER,
  .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
  .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};


void soft_spi_init(void){
//	//CS -> P0.9
//	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 9, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
//	//SCLK -> P0.8
//	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 8, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
//	
//	//MOSI -> P1.0
//	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 0, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

//	//MISO -> P1.1
////	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_INPUT_TRISTATE);
//	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

  /* Initialize and Start SPI */
  XMC_SPI_CH_Init(XMC_SPI0_CH0, &spi_config);
  XMC_SPI_CH_SetInputSource(XMC_SPI0_CH0, XMC_SPI_CH_INPUT_DIN0, USIC0_C0_DX0_P1_1);
	XMC_SPI_CH_SetBitOrderMsbFirst(XMC_SPI0_CH0);
	XMC_SPI_CH_SetSlaveSelectDelay(XMC_SPI0_CH0, 2);
	XMC_SPI_CH_EnableInterwordDelay(XMC_SPI0_CH0);
	XMC_SPI_CH_SetInterwordDelaySCLK(XMC_SPI0_CH0, 2);
	
  XMC_SPI_CH_Start(XMC_SPI0_CH0);

  /* GPIO Input pin configuration */
  rx_pin_config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
  XMC_GPIO_Init(XMC_GPIO_PORT1,1, &rx_pin_config);

  /* GPIO Output pin configuration */
  tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
  XMC_GPIO_Init(XMC_GPIO_PORT1,0, &tx_pin_config);

  /* GPIO Slave Select line pin configuration */
  selo_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6;
  XMC_GPIO_Init(XMC_GPIO_PORT0,9, &selo_pin_config);

  /* GPIO Clock pin configuration */
  clk_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT6;
  XMC_GPIO_Init(XMC_GPIO_PORT0,8, &clk_pin_config);	
}
