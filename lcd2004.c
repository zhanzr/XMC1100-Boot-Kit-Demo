#include <XMC1100.h>
#include <xmc_gpio.h>
#include <xmc_ccu4.h>
#include <xmc_scu.h>

#include "lcd2004.h"
#include "lcd_pwm_vo.h"

static inline void SimpleDelay(uint32_t d)
{
	uint32_t t = d;
	while(--t)
	{
		__NOP();
	}
}

static inline void RS_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 0);	
}

static inline void RS_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 0);	
}

static inline void E_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 2);	
}

static inline void E_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 2);	
}

static inline void D4_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 9);	
}

static inline void D4_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 9);	
}

static inline void D5_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 1);	
}

static inline void D5_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 1);	
}

static inline void D6_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT1, 0);	
}

static inline void D6_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT1, 0);	
}

static inline void D7_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 8);	
}

static inline void D7_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 8);	
}

static inline void DB4_Wr(uint8_t dat)
{
		(0==(dat&0x08))?D7_L():D7_H();
		(0==(dat&0x04))?D6_L():D6_H();
		(0==(dat&0x02))?D5_L():D5_H();
		(0==(dat&0x01))?D4_L():D4_H();
}

void LCD_WrCmd_4 (uint8_t cmd)
{
	SimpleDelay(WAIT_AVAIL_DLY);

	RS_L(); 
	SimpleDelay(10);
//	RW_L();   	
	DB4_Wr(cmd>>4);
	SimpleDelay(10);
	E_H();
	SimpleDelay(10);
	E_L();
	SimpleDelay(10);
	DB4_Wr(cmd);
	SimpleDelay(10);
	E_H();
	SimpleDelay(10);
	E_L();	
}

void LCD_WrDat_4 (uint8_t dat)
{
	SimpleDelay(WAIT_AVAIL_DLY);
	
	RS_H(); 
	SimpleDelay(10);
//	RW_L();   	
	DB4_Wr(dat>>4);
	SimpleDelay(10);
	E_H();
	SimpleDelay(10);
	E_L();
	SimpleDelay(10);
	DB4_Wr(dat);
	SimpleDelay(10);
	E_H();
	SimpleDelay(10);
	E_L();		
}

void LCD_SetPos (uint8_t x, uint8_t y)
{	
	const static uint8_t pos_tab[] = {0x80, 0xc0, 0x94, 0xd4};
	
	LCD_WrCmd_4(pos_tab[x] + y);	
}

void LCD_Initialize (void) 
{
	//RS
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 0, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	//E
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	
	//DB4-DB7
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 9, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 0, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 8, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);	
	
	E_L();
	
	//Change to 4 bit mode
	LCD_WrCmd_4(0x28);
	LCD_WrCmd_4(0x28);
	LCD_WrCmd_4(0x28);
	
	LCD_WrCmd_4(0x08);
	LCD_WrCmd_4(0x01);	
	LCD_WrCmd_4(0x06);
	LCD_WrCmd_4(0x0c);
	
	LCD_PWM_VO_Init();	
}

void LCD_displayL(uint8_t l,uint8_t hori,uint8_t *s)
{
	LCD_SetPos(l, hori);
	while(*s)
	{
		LCD_WrDat_4(*s);
		s++;
	}
}
