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
	uint8_t ret = ((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 7))?0:0x80) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT1, 0))?0:0x40) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT1, 1))?0:0x20) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 9))?0:0x10) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 8))?0:0x08) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 12))?0:0x04) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 4))?0:0x02) | 
	((0==XMC_GPIO_GetInput(XMC_GPIO_PORT0, 3))?0:0x01);
	
	return ret;
}

void LCD_WaitAvail(void)
{
	DB8_Wr(0xff);

	E_L();
	RS_L(); 
	RW_H();
	E_H();
	while((DB8_Rd()&0x80)==0x80)
	{
		__NOP();
	}
	E_L();
	RS_H(); 
	RW_L();   	
}

void LCD_WrCmd (uint8_t cmd)
{
	LCD_WaitAvail();
	E_L();
	RS_L(); 
	RW_L();   	
	DB8_Wr(cmd);
	E_H();
	E_L();
	RS_H(); 
	RW_H();
}

void LCD_WrDat (uint8_t dat)
{
	LCD_WaitAvail();
	E_L();
	RS_H(); 
	RW_L();   	
	DB8_Wr(dat);
	E_H();
	E_L();
	RS_L(); 
	RW_H();
}

const static uint8_t pos_tab[] = {0x80, 0xc0, 0x94, 0xd4};
void LCD_SetPos (uint8_t x, uint8_t y)
{	
	LCD_WrCmd(pos_tab[x] + y);	
}

void LCD_Initialize (void) 
{
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 0, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 1, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 2, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 3, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 4, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 12, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 8, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 9, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 1, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT1, 0, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 7, XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN);	
	
	XMC_GPIO_SetMode(XMC_GPIO_PORT0, 10, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);
	
	LCD_WrCmd(0x38);                                                         //设置8位格式，2行，5x7
	LCD_WrCmd(0x08);                                                         //Tune off screen                               
	LCD_WrCmd(0x01);                                                         //设定输入方式，增量不移位
	LCD_WrCmd(0x06);                                                         //整体显示，关光标，不闪烁
	LCD_WrCmd(0x0c); 	
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
