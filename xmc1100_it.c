#include <stdio.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>

#include "led.h"

extern __IO uint32_t g_Ticks;

bool g_uart_rx_flag;
uint8_t g_uart_rx_ch;

void HardFault_Handler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

//void SVC_Handler(void)
//{
//	printf(__FUNCTION__);
//	while(1)
//	{;}
//}

//void PendSV_Handler(void)
//{
//	printf(__FUNCTION__);
//	while(1)
//	{;}
//}

//void SysTick_Handler(void)
//{	
//	g_Ticks++;
//}


//RTC Alarm
void SCU_1_IRQHandler(void)
{
	uint32_t lt = g_Ticks;

	XMC_RTC_ClearEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);	
  
//	sprintf(g_Buf, "RTC ISR %08X\n", lt);
//	printf(g_Buf);  
}
                 
void SCU_2_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}
void ERU0_0_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void ERU0_1_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}
                   
void ERU0_2_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void ERU0_3_IRQHandler(void)
{
	LED_Toogle(4);
	printf(__FUNCTION__);
	while(1)
	{;}
}

void USIC0_0_IRQHandler(void)
{
	LED_Toogle(4);

	g_uart_rx_flag = true;
  g_uart_rx_ch = (uint8_t)XMC_UART_CH_GetReceivedData(XMC_UART0_CH1);
	
	XMC_UART_CH_Transmit(XMC_UART0_CH1, g_uart_rx_ch);

	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}        
       
void USIC0_1_IRQHandler(void)
{
	LED_Toogle(4);
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'B');

//	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}

void USIC0_2_IRQHandler(void)
{
	LED_Toogle(4);
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'C');

//	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}

void USIC0_3_IRQHandler(void)
{
	LED_Toogle(4);
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'C');

//	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}        
       
void USIC0_4_IRQHandler(void)
{
	LED_Toogle(4);
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'D');

//	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}

void USIC0_5_IRQHandler(void)
{
	LED_Toogle(4);
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'E');

//	XMC_UART_CH_ClearStatusFlag(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);
}

void VADC0_C0_0_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}      
      
void VADC0_C0_1_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void CCU40_0_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void CCU40_1_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}   
      
void CCU40_2_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void CCU40_3_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
} 
