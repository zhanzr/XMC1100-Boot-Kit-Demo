#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>

#include "cmsis_os.h"

extern osThreadId g_RtcThread;

extern __IO uint32_t g_Ticks;

//void SVC_Handler(void)
//{
//	XMC_UART_CH_Transmit(XMC_UART0_CH1, 'S');
//	XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
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

void HardFault_Handler(void)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, 'H');
	XMC_UART_CH_Transmit(XMC_UART0_CH1, '\n');
	while(1)
	{;}
}

//RTC Alarm
void SCU_1_IRQHandler(void)
{
	osSignalSet (g_RtcThread, 0x01);

	XMC_RTC_ClearEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);	
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
	printf(__FUNCTION__);
	while(1)
	{;}
}

void USIC0_0_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}        
       
void USIC0_1_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void USIC0_2_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void USIC0_3_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}        
       
void USIC0_4_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void USIC0_5_IRQHandler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
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
