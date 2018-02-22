/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C/C++ Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "rtc.h"
#include "led.h"
#include "hal.h"
#include "LiquidCrystal.h"
#include "retarget_io_drv.h"

#ifndef HZ
#define	HZ	1000
#endif

using namespace std;

__IO uint32_t g_Ticks;

//Re-implement any functions that require re-implementation.
namespace std {
  struct __FILE
  {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
  };
  FILE __stdout;
  FILE __stdin;
  FILE __stderr;
  int fgetc(FILE *f)
  {
    /* Your implementation of fgetc(). */
    return 0;
  }
  int fputc(int c, FILE *stream)
  {
		XMC_UART_CH_Transmit(XMC_UART0_CH1, c);
		return c;
  }
  int ferror(FILE *stream)
  {
    fputc('E', stream);
    fputc('\n', stream);
		
		return -1;
  }
  long int ftell(FILE *stream)
  {
    fputc('T', stream);
    fputc('\n', stream);
		
		return 0;
  }
  int fclose(FILE *f)
  {
    /* Your implementation of fclose(). */
    return 0;
  }
  int fseek(FILE *f, long nPos, int nMode)
  {
    /* Your implementation of fseek(). */
    return 0;
  }
  int fflush(FILE *f)
  {
    /* Your implementation of fflush(). */    
    return 0;
  }
}

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line)
{
  printf("Assert:%s,%s,%u\n", msg, file, line);

	while(1)
	{
		LED_Toggle(1);
		LiquidCrystal::SimpleDelay(100000);
	}
}

//uint8_t line[4][21]={
//	"https://www.infineon",
//	"-autoeco.com",
//	"Happy Spring Festvl",
//	"by Automan"};                               

int main(void)
{
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	uint8_t testBuf[21];
	
	__IO XMC_RTC_TIME_t now_rtc_time;

  /* System timer configuration */
  SysTick_Config(SystemCoreClock / HZ);
	
	uart_init();
	
  printf("LCD2004 4bit C++ driver For XMC1100 Bootkit. Cortex M0 Rev:%u, @%u Hz\n",
	__CM0_REV,
	SystemCoreClock);
	printf("CPUID:%08X, MPU:%u\n", SCB->CPUID, __MPU_PRESENT);
	
	RTC_Initialize();
	
	LED_Initialize();
	
	LiquidCrystal lcd;
	
//	lcd.Display(0,0,line[0]);
//	lcd.Display(1,0,line[1]);
//	lcd.Display(2,0,line[2]);
//	lcd.Display(3,0,line[3]);
	
	while (1)
  {				
    LED_Toggle(0);
    LED_Toggle(1);
    LED_Toggle(2);
		
		HAL_Delay(2000);

		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
		sprintf((char*)testBuf, "%02d:%02d:%02d", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);
		
		lcd.Display(rand()%LiquidCrystal::LINE_NUM, 10, testBuf);
		cout<<testBuf<<endl;

    LED_Toggle(3);
    LED_Toggle(4);
		
		HAL_Delay(2000);
  }
	
	return 0;
}
