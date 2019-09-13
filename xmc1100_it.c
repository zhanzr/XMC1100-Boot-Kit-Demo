#include <XMC1100.h>
#include <xmc_scu.h>
#include <stdio.h>

extern __IO uint32_t g_Ticks;

void HardFault_Handler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void SVC_Handler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void PendSV_Handler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void SysTick_Handler(void)
{	
	g_Ticks++;
}


//RTC Alarm
void SCU_1_IRQHandler(void)
{
//	uint32_t lt = g_Ticks;

//	XMC_RTC_ClearEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);	 
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
