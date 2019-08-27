#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>

#include <stdio.h>

extern __IO uint32_t g_Ticks;

void HardFault_Handler(void) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)'h');

	while(1)
	{;}
}

extern int __svc(0x02) svc_service_incr(int x);

// SVC handler - main code to handle processing
// Input parameter is stack frame starting address
// obtained from assembly wrapper.
void SVC_Handler_main(unsigned int * svc_args)
{
	// Stack frame contains:
	// r0, r1, r2, r3, r12, r14, the return address and xPSR
	// - Stacked R0 = svc_args[0]
	// - Stacked R1 = svc_args[1]
	// - Stacked R2 = svc_args[2]
	// - Stacked R3 = svc_args[3]
	// - Stacked R12 = svc_args[4]
	// - Stacked LR = svc_args[5]
	// - Stacked PC = svc_args[6]
	// - Stacked xPSR= svc_args[7]
	unsigned int svc_number;
	svc_number = ((char *)svc_args[6])[-2];
	switch(svc_number)
	{
		case 0:
			svc_args[0] = svc_args[0] + svc_args[1];
		break;

		case 1:
			svc_args[0] = svc_args[0] - svc_args[1];
		break;

		case 2:
			svc_args[0] = svc_args[0] + 1;
		break;

		case 3:
			printf("%s: %08X\n", __func__, svc_args[0]);
			break;
		
		default: // Unknown SVC request, cause a hardfault
			svc_service_incr(1);
		break;
	}
	return;
}

// SVC handler - Assembly wrapper to extract
// stack frame starting address
__asm void SVC_Handler(void) {
	IMPORT SVC_Handler_main
	
	MOVS r0, #4
	MOV r1, LR
	TST r0, r1
	BEQ stacking_used_MSP
	MRS R0, PSP // first parameter - stacking was using PSP
	B call_main_svc_handler
stacking_used_MSP
	MRS R0, MSP // first parameter - stacking was using MSP
call_main_svc_handler	
	LDR R1,=SVC_Handler_main
	BX R1
}

void PendSV_Handler(void) {
	printf("%s\n", __func__);
}

void SysTick_Handler(void)
{	
	g_Ticks++;
}


//RTC Alarm
void SCU_1_IRQHandler(void)
{
//	sprintf(g_Buf, "RTC ISR %08X\n", lt);
//	printf(g_Buf);  
}
                 
void SCU_2_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}
void ERU0_0_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void ERU0_1_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}
                   
void ERU0_2_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void ERU0_3_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void USIC0_0_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}        
       
void USIC0_1_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void USIC0_2_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void USIC0_3_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}        
       
void USIC0_4_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void USIC0_5_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void VADC0_C0_0_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}      
      
void VADC0_C0_1_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void CCU40_0_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void CCU40_1_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}   
      
void CCU40_2_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
}

void CCU40_3_IRQHandler(void)
{
	//printf(__func__);
	while(1)
	{;}
} 
