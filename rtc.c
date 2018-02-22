#include "rtc.h"

XMC_RTC_CONFIG_t rtc_config =
{
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = 
{
	.year = 2018,
	.month = XMC_RTC_MONTH_FEBRUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 20,
	.hours = 15,
	.minutes = 32,
	.seconds = 59	
};

void RTC_Initialize (void)
{
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  XMC_SCU_INTERRUPT_EnableEvent(XMC_RTC_EVENT_PERIODIC_MINUTES);
  NVIC_SetPriority(SCU_1_IRQn, 3);
  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();	
}
