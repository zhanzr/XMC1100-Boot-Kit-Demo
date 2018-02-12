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

static inline void D0_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 3);	
}

static inline void D0_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 3);	
}

static inline void D1_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 4);	
}

static inline void D1_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 4);	
}

static inline void D2_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 12);	
}

static inline void D2_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 12);	
}

static inline void D3_H(void)
{
		XMC_GPIO_SetOutputHigh(XMC_GPIO_PORT0, 8);	
}

static inline void D3_L(void)
{
		XMC_GPIO_SetOutputLow(XMC_GPIO_PORT0, 8);	
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

static inline uint32_t ReadD0(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 3);
}

static inline uint32_t ReadD1(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 4);
}

static inline uint32_t ReadD2(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 12);
}

static inline uint32_t ReadD3(void)
{
	return XMC_GPIO_GetInput(XMC_GPIO_PORT0, 8);
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

static inline void DB8_Wr(uint8_t dat)
{
		(0==(dat&0x80))?D7_L():D7_H();
		(0==(dat&0x40))?D6_L():D6_H();
		(0==(dat&0x20))?D5_L():D5_H();
		(0==(dat&0x10))?D4_L():D4_H();
		(0==(dat&0x08))?D3_L():D3_H();
		(0==(dat&0x04))?D2_L():D2_H();
		(0==(dat&0x02))?D1_L():D1_H();
		(0==(dat&0x01))?D0_L():D0_H();	
}

static inline uint8_t DB8_Rd( void)
{
	uint8_t ret =
	((0==ReadD7())?0:0x80) | 
	((0==ReadD6())?0:0x40) | 
	((0==ReadD5())?0:0x20) | 
	((0==ReadD4())?0:0x10) | 
	((0==ReadD3())?0:0x08) | 
	((0==ReadD2())?0:0x04) | 
	((0==ReadD1())?0:0x02) | 
	((0==ReadD0())?0:0x01);
	
	return ret;
}

void LCD_WaitAvail(void)
{
	DB8_Wr(0xff);

	RS_L(); 
	RW_H();
	E_H();
	while(0!=ReadD7())
	{
		__NOP();
	}
	E_L();	
}

void LCD_WrCmd (uint8_t cmd)
{
	LCD_WaitAvail();
	RS_L(); 
	RW_L();   	
	DB8_Wr(cmd);
	E_H();
	E_L();
}

void LCD_WrDat (uint8_t dat)
{
	LCD_WaitAvail();
	RS_H(); 
	RW_L();   	
	DB8_Wr(dat);
	E_H();
	E_L();
}

void LCD_SetPos (uint8_t x, uint8_t y)
{	
	const static uint8_t pos_tab[] = {0x80, 0xc0, 0x94, 0xd4};
	
	LCD_WrCmd(pos_tab[x] + y);	
}

void LCD_Initialize (void) 
{
	//RS
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 0, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	//RW
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	//E
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	
	//DB0-DB7
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 3, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 4, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 12, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 8, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 9, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 0, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);	
	
	E_L();

	//Software PWM for contrast output, For 5V operation, a 0.5V output will get good display effect
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 10, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
}

void LCD_displayL(uint8_t x,uint8_t y,uint8_t *s)
{
	LCD_SetPos(x,y);
	while(*s)
	{
			LCD_WrDat(*s);
			s++;
	}
}
