#include "retarget_io_drv.h"

#if defined(__ARMCC_VERSION)
int stdout_putchar (int ch)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}
#else
int _write (int fd, const void *buf, size_t count)
{
	for(uint32_t i=0; i<count; ++i)
	{
		XMC_UART_CH_Transmit(XMC_UART0_CH1, *(uint8_t*)buf+i);		
	}
	return count;
}
#endif
