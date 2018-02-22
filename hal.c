#include "hal.h"

extern __IO uint32_t g_Ticks;

void HAL_Delay(uint32_t t)
{
	uint32_t tmpTick = g_Ticks;
	while((tmpTick+t) > g_Ticks)
	{
		__NOP();
	}		
}

uint32_t HAL_GetTick(void)
{
	return g_Ticks;
}
