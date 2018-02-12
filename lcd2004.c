#include <xmc_gpio.h>
#include "lcd2004.h"

static inline void RS_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 0);	
}

static inline void RS_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 0);	
}

static inline void RW_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 1);	
}

static inline void RW_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 1);	
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
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 7);	
}

static inline void D7_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 7);	
}

static inline uint32_t ReadD4(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 9);
}

static inline uint32_t ReadD5(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT1, 1);
}

static inline uint32_t ReadD6(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT1, 0);
}

static inline uint32_t ReadD7(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 7);
}

static inline void DB4_Wr(uint8_t dat)
{
		(0==(dat&0x08))?D7_L():D7_H();
		(0==(dat&0x04))?D6_L():D6_H();
		(0==(dat&0x02))?D5_L():D5_H();
		(0==(dat&0x01))?D4_L():D4_H();
}

void LCD_WaitAvail(void)
{
	DB4_Wr(0xff);

	RS_L(); 
	RW_H();
	E_H();
	while(0!=ReadD7())
	{
		__NOP();
	}
	E_L();	
}

void LCD_WrCmd_4 (uint8_t cmd)
{
	LCD_WaitAvail();
	__NOP();__NOP();

	RS_L(); 
	RW_L();   	
	DB4_Wr(cmd>>4);
	E_H();
	E_L();
	
	__NOP();__NOP();

	DB4_Wr(cmd);
	E_H();
	E_L();	
}

void LCD_WrDat_4 (uint8_t dat)
{
	LCD_WaitAvail();
	__NOP();__NOP();
	
	RS_H(); 
	RW_L();   	
	DB4_Wr(dat>>4);
	E_H();
	E_L();

	__NOP();__NOP();

	DB4_Wr(dat);
	E_H();
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
	//RW
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	//E
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	
	//DB4-DB7
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 9, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 0, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);	
	
	E_L();

	//Software PWM for contrast output, For 5V operation, a 0.5V output will get good display effect
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 10, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	
	//Change to 4 bit mode
	LCD_WrCmd_4(0x28);
	LCD_WrCmd_4(0x08);
	LCD_WrCmd_4(0x01);	
	LCD_WrCmd_4(0x06);
	LCD_WrCmd_4(0x0c);
}

void LCD_displayL(uint8_t x,uint8_t y,uint8_t *s)
{
	LCD_SetPos(x,y);
	while(*s)
	{
			LCD_WrDat_4(*s);
			s++;
	}
}
