#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>

#include <stdio.h>

void HardFault_Handler(void)
{
	printf(__FUNCTION__);
	while(1)
	{;}
}

void freertos_tick_handler(void);
void SysTick_Handler(void) {	
	freertos_tick_handler();	
}


//RTC Alarm
void SCU_1_IRQHandler(void){	
	while(1)
	{;}
}
                 
void SCU_2_IRQHandler(void)
{
	while(1)
	{;}
}
void ERU0_0_IRQHandler(void)
{
	while(1)
	{;}
}

void ERU0_1_IRQHandler(void)
{
	while(1)
	{;}
}
                   
void ERU0_2_IRQHandler(void)
{
	while(1)
	{;}
}

void ERU0_3_IRQHandler(void)
{
	while(1)
	{;}
}

void USIC0_0_IRQHandler(void)
{
	while(1)
	{;}
}        
       
void USIC0_1_IRQHandler(void)
{
	while(1)
	{;}
}

void USIC0_2_IRQHandler(void)
{
	while(1)
	{;}
}

void USIC0_3_IRQHandler(void)
{
	while(1)
	{;}
}        
       
void USIC0_4_IRQHandler(void)
{
	while(1)
	{;}
}

void USIC0_5_IRQHandler(void)
{
	while(1)
	{;}
}

void VADC0_C0_0_IRQHandler(void)
{
	while(1)
	{;}
}      
      
void VADC0_C0_1_IRQHandler(void)
{
	while(1)
	{;}
}

void CCU40_0_IRQHandler(void)
{
	while(1)
	{;}
}

void CCU40_1_IRQHandler(void)
{
	while(1)
	{;}
}   
      
void CCU40_2_IRQHandler(void)
{
	while(1)
	{;}
}

void CCU40_3_IRQHandler(void)
{
	while(1)
	{;}
} 
