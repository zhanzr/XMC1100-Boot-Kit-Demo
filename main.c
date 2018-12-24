/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "led.h"

#include "cmsis_os.h"

#define	HZ	(1000)

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

extern bool g_uart_rx_flag;
extern uint8_t g_uart_rx_ch;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = 
{	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 256000
};

XMC_RTC_CONFIG_t rtc_config =
{
  .time.seconds = 5U,
  .prescaler = 0x7fffU
};     

XMC_RTC_TIME_t init_rtc_time = 
{
	.year = 2018,
	.month = XMC_RTC_MONTH_JANUARY,
	.daysofweek = XMC_RTC_WEEKDAY_TUESDAY,
	.days = 27,
	.hours = 15,
	.minutes = 40,
	.seconds = 55	
};

int stdout_putchar (int ch)
{
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}

int SER_GetChar (void) {
	if(g_uart_rx_flag)
	{
		g_uart_rx_flag = false;
		return (int)g_uart_rx_ch;
	}
	else
	{
		return (int)XMC_UART_CH_GetReceivedData(XMC_UART0_CH1);
	}	
}

void SystemCoreClockSetup(void)
{
	XMC_SCU_CLOCK_CONFIG_t clock_config =
	{
		.rtc_src = XMC_SCU_CLOCK_RTCCLKSRC_DCO2,
		.pclk_src = XMC_SCU_CLOCK_PCLKSRC_DOUBLE_MCLK,
		.fdiv = 0, 
		.idiv = 1
	 };

	XMC_SCU_CLOCK_Init(&clock_config);
	
//  SystemCoreClockUpdate();
}

#define RED     0                     /* I/O Pin:  RED    lamp output        */
#define YELLOW  1                     /* I/O Pin:  YELLOW lamp output        */
#define GREEN   2                     /* I/O Pin:  GREEN  lamp output        */
//#define STOP    4                     /* I/O Pin:  STOP   lamp output        */
//#define WALK    5                     /* I/O Pin:  WALK   lamp output        */

const char menu[] = 
  "\r\n"
  "+********* TRAFFIC LIGHT CONTROLLER using RTX kernel *********+\r\n"
  "| This program is a simple Traffic Light Controller. Between  |\r\n"
  "| start time and end time the system controls a traffic light |\r\n"
  "| with pedestrian self-service.  Outside of this time range   |\r\n"
  "| the YELLOW caution lamp is blinking.                        |\r\n"
  "+ command -+ syntax -----+ function --------------------------+\r\n"
  "| Display  | D           | display times                      |\r\n"
  "| Time     | T hh:mm:ss  | set clock time                     |\r\n"
  "| Start    | S hh:mm:ss  | set start time                     |\r\n"
  "| End      | E hh:mm:ss  | set end time                       |\r\n"
  "+----------+-------------+------------------------------------+\r\n";


extern void getline (char *, int);    /* external function:  input line      */

osThreadId tid_command;               /* Thread id of thread: command        */
osThreadId tid_blinking;              /* Thread id of thread: blinking       */
osThreadId tid_lights;                /* Thread id of thread: lights         */
osThreadId tid_getesc;                /* Thread id of thread: getesc         */
  
struct time  {                        /* structure of the time record        */
  uint8_t hour;                       /* hour                                */
  uint8_t min;                        /* minute                              */
  uint8_t sec;                        /* second                              */
};

struct time c_time = { 12,  0,  0 };   /* storage for clock time values       */
struct time stime = {  7, 30,  0 };   /* storage for start time values       */
struct time etime = { 18, 30,  0 };   /* storage for end   time values       */

char cmdline[16];                     /* storage for command input line      */
struct time rtime;                    /* temporary storage for entry time    */
bool display_time = false;            /* flag signal cmd state display_time  */
bool escape;                          /* flag: mark ESCAPE character enteRED */

#define ESC  0x1B                     /* ESCAPE character code               */

void init (void const *argument);     /* Function prototypes                 */
void c_clock (void const *argument) ;
void get_escape (void const *argument);
void command (void const *argument);
void blinking (void const *argument);
void lights (void const *argument);

osThreadDef(command,    osPriorityNormal, 1, 2000);
osThreadDef(blinking,   osPriorityNormal, 1, 0);
osThreadDef(lights,     osPriorityNormal, 1, 0);
osThreadDef(get_escape, osPriorityNormal, 1, 0);

osTimerDef(clock1s, c_clock);
osTimerId  clock1s;


/*---------------------------------------------------------------------------
  Set Lights function
  controls LEDs and LCD display
 *---------------------------------------------------------------------------*/
void SetLights (uint32_t light, uint32_t on) {		
	printf("%d->%d\n", light, on);

  if (on == 0) {
    LED_Off (light);
  }
  else {
    LED_On (light);
  }
}

int main(void)
{
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
	__IO XMC_RTC_TIME_t now_rtc_time;

  /* System timer configuration */
  SysTick_Config(SystemCoreClock / HZ);
	
  /*Initialize the UART driver */
	uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7;
	uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
 /* Configure UART channel */
  XMC_UART_CH_Init(XMC_UART0_CH1, &uart_config);
  XMC_UART_CH_SetInputSource(XMC_UART0_CH1, XMC_UART_CH_INPUT_RXD,USIC0_C1_DX0_P1_3);
  
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	0);																															
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	1);	
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	2);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	3);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	4);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//	5);	
//	
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	0);																															
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	1);	
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	2);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	3);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	4);
//	XMC_UART_CH_SelectInterruptNodePointer(XMC_UART0_CH1,
//	XMC_UART_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//	5);	
	
	XMC_UART_CH_EnableEvent(XMC_UART0_CH1, XMC_UART_CH_EVENT_RECEIVE_START);

  NVIC_SetPriority(USIC0_0_IRQn, 3);
  NVIC_EnableIRQ(USIC0_0_IRQn);
  NVIC_SetPriority(USIC0_1_IRQn, 3);
  NVIC_EnableIRQ(USIC0_1_IRQn);
  NVIC_SetPriority(USIC0_2_IRQn, 3);
  NVIC_EnableIRQ(USIC0_2_IRQn);
  NVIC_SetPriority(USIC0_3_IRQn, 3);
  NVIC_EnableIRQ(USIC0_3_IRQn);
  NVIC_SetPriority(USIC0_4_IRQn, 3);
  NVIC_EnableIRQ(USIC0_4_IRQn);	
  NVIC_SetPriority(USIC0_5_IRQn, 3);
  NVIC_EnableIRQ(USIC0_5_IRQn);	
	
	/* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH1);

  /* Configure pins */
	XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);
	
  printf ("Interactive For XMC1100 Bootkit by Automan @ Infineon BBS @%u Hz\n",
	SystemCoreClock	);
	
	//RTC
  XMC_RTC_Init(&rtc_config);
	
	XMC_RTC_SetTime(&init_rtc_time);
	
//  XMC_RTC_EnableEvent(XMC_RTC_EVENT_PERIODIC_SECONDS);
//  XMC_SCU_INTERRUPT_EnableEvent(XMC_SCU_INTERRUPT_EVENT_RTC_PERIODIC);
//  NVIC_SetPriority(SCU_1_IRQn, 3);
//  NVIC_EnableIRQ(SCU_1_IRQn);
  XMC_RTC_Start();
	
	LED_Initialize();
	
	clock1s = osTimerCreate(osTimer(clock1s), osTimerPeriodic, NULL);                                          
  if (clock1s) osTimerStart(clock1s, 1000);                                          

  osDelay(500);
                                          /* start command thread            */
  tid_command = osThreadCreate(osThread(command), NULL);
                                          /* start lights thread             */
  tid_lights  = osThreadCreate(osThread(lights), NULL);
                                          /* start keyread thread            */							
	osDelay(osWaitForever);

	while (1)
  {				
//    LED_On(0);
//    LED_On(1);
//    LED_On(2);
//    LED_On(3);
    LED_On(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}
		
		XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
//		printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);

//    LED_Off(0);
//    LED_Off(1);
//    LED_Off(2);
//    LED_Off(3);
    LED_Off(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks)
		{
			__NOP();
			__WFI();
		}		
  }
}


/*----------------------------------------------------------------------------
  c_clock: Periodic 1s Timer Callback
 *---------------------------------------------------------------------------*/
void c_clock (void const *argument) {

  if (++c_time.sec == 60) {            /* calculate the second                */
    c_time.sec = 0;
    if (++c_time.min == 60) {          /* calculate the minute                */
      c_time.min = 0;
      if (++c_time.hour == 24) {       /* calculate the hour                  */
        c_time.hour = 0;
      }
    }
  }
  if (display_time) {                 /* if command_status == display_time   */
                                      /* signal time changed to command      */
    osSignalSet(tid_command, 0x0001);
  }
}

/*----------------------------------------------------------------------------
  readtime: convert line input to time values & store in rtime
 *---------------------------------------------------------------------------*/
char readtime (char *buffer) {
  int args;                                    /* number of arguments        */
  int hour,min,sec;

  rtime.sec = 0;                               /* preset second              */
  args = sscanf (buffer, "%d:%d:%d",           /* scan input line for        */
                         &hour, &min, &sec);   /* hour, minute and second    */
  
  if (hour > 23  ||  min > 59  ||              /* check for valid inputs     */
      sec > 59   ||  args < 2        ) {
     printf ("\r\n*** ERROR: INVALID TIME FORMAT\r\n");
     return (0);
  }
  rtime.hour = hour;                           /* setting a new time: hour   */
  rtime.min  = min;                            /* setting a new time: min    */
  rtime.sec  = sec;                            /* setting a new time: sec    */
  return (1);
}

/*----------------------------------------------------------------------------
  Thread 5 'get_escape': check if ESC (escape character) was entered
 *---------------------------------------------------------------------------*/
void get_escape (void const *argument) {
  while (1)  {                                /* endless loop                */
    if (SER_GetChar () == ESC) {              /* If ESC entered, set flag    */ 
      escape = true;                          /* 'escape', set event flag of */
      osSignalSet(tid_command, 0x0002);       /* thread 'command'            */
    }
  }
}

/*----------------------------------------------------------------------------
  Thread 1 'command': command processor
 *---------------------------------------------------------------------------*/
void command (void const *argument) {
  int i;

  printf (menu);                              /* display command menu        */
  while (1) {                                 /* endless loop                */
    printf ("\r\nCommand: ");                 /* display prompt              */
    #ifdef __USE_FFLUSH
    fflush (stdout); 
    #endif
    getline (cmdline, sizeof (cmdline));      /* get command line input      */

    for (i = 0; cmdline[i] != 0; i++) {       /* convert to uppercase        */
      cmdline[i] = (char) toupper((int)cmdline[i]);
    }

    for (i = 0; cmdline[i] == ' '; i++);      /* skip blanks                 */

    switch (cmdline[i]) {                     /* proceed to command function */
      case 'D':                               /* Display Time Command        */
        printf ("Start Time: %02d:%02d:%02d    "
                "End Time: %02d:%02d:%02d\r\n",
                 stime.hour, stime.min, stime.sec,
                 etime.hour, etime.min, etime.sec);
        printf ("                        type ESC to abort\r");
        #ifdef __USE_FFLUSH
        fflush (stdout);
        #endif

        tid_getesc = osThreadCreate(osThread(get_escape), NULL);
                                              /* ESC check in display loop   */
        escape = false;                       /* clear escape flag           */
        display_time = true;                  /* set display time flag       */
        osSignalClear(tid_command, 0x0003);
                                              /* clear pending signals       */
        while (!escape) {                     /* while no ESC enteRED        */
          printf ("Clock Time: %02d:%02d:%02d\r",   /* display time          */
                   c_time.hour, c_time.min, c_time.sec);
          #ifdef __USE_FFLUSH
          fflush (stdout);
          #endif
          osSignalWait(0, osWaitForever);     /* wait for time change or ESC */
        }

        osThreadTerminate(tid_getesc);        /* terminate 'get_escape'      */
        display_time = false;                 /* clear display time flag     */
        printf ("\r\n\r\n");
        break;

      case 'T':                               /* Set Time Command            */
        if (readtime (&cmdline[i+1])) {       /* read time input and         */
          c_time.hour = rtime.hour;            /* store in 'c_time'            */
          c_time.min  = rtime.min;
          c_time.sec  = rtime.sec;
        }
        break;

      case 'E':                               /* Set End Time Command        */
        if (readtime (&cmdline[i+1]))  {      /* read time input and         */
          etime.hour = rtime.hour;              /* store in 'end'              */
          etime.min  = rtime.min;
          etime.sec  = rtime.sec;
        }
        break;

      case 'S':                               /* Set Start Time Command      */
        if (readtime (&cmdline[i+1]))  {      /* read time input and         */
          stime.hour = rtime.hour;            /* store in 'start'            */
          stime.min  = rtime.min;
          stime.sec  = rtime.sec;
        }
        break;

      default:                                /* Error Handling              */
        printf (menu);                        /* display command menu        */
        break;
    }   
  }
}

/*----------------------------------------------------------------------------
  signalon: check if c_clock time is between start and end
 *---------------------------------------------------------------------------*/
char signalon (void) {
  if (memcmp (&stime, &etime, sizeof (struct time)) < 0)  {
    if (memcmp (&stime, &c_time, sizeof (struct time)) < 0  &&
        memcmp (&c_time, &etime, sizeof (struct time)) < 0) {
      return (1);
    }
  }
  else { 
    if (memcmp (&etime, &c_time, sizeof (stime)) > 0  &&
        memcmp (&c_time, &stime, sizeof (stime)) > 0) {
      return (1);
    }
  }
  return (0);                                 /* signal off, blinking on     */
}

/*----------------------------------------------------------------------------
  Thread 2 'blinking': runs if current time is outside start & end time 
 *---------------------------------------------------------------------------*/
void blinking (void const *argument) {        /* blink YELLOW light          */
  SetLights (RED, 0);                         /* all lights off              */
  SetLights (YELLOW, 0);
  SetLights (GREEN, 0);
//  SetLights (STOP, 0);
//  SetLights (WALK, 0);
  while (1) {                                 /* endless loop                */
    SetLights (YELLOW, 1);                    /* YELLOW light on             */
    osDelay(300);                             /* wait for timeout: 300ms     */
    SetLights (YELLOW, 0);                    /* YELLOW light off            */
    osDelay(300);                             /* wait for timeout: 300ms     */
    if (signalon ()) {                        /* if blinking time over       */
      tid_lights = osThreadCreate(osThread(lights), NULL);
                                              /* start lights                */
      return;                                 /* and STOP blinking           */
    }
  }
}

/*----------------------------------------------------------------------------
  Thread 3 'lights': executes if current time is between start & end time
 *---------------------------------------------------------------------------*/
void lights (void const *argument) {          /* traffic light operation     */
  SetLights (RED, 1);                         /* RED & STOP lights on        */
//  SetLights (STOP, 1);
  SetLights (YELLOW, 0);
  SetLights (GREEN, 0);
//  SetLights (WALK, 0);
  while (1) {                                 /* endless loop                */
    osDelay(500);                             /* wait for timeout: 500ms     */
    if (!signalon ()) {                       /* if traffic signal time over */
      tid_blinking = osThreadCreate(osThread(blinking), NULL);
                                              /* start blinking              */
      return;                                 /* STOP lights                 */
    }
    SetLights (YELLOW, 1);
    osDelay(500);                             /* wait for timeout: 500ms     */
    SetLights (RED, 0);                       /* GREEN light for cars        */
    SetLights (YELLOW, 0);
    SetLights (GREEN, 1);
    osSignalClear(tid_lights, 0x0010);
    osDelay(500);                             /* wait for timeout: 500ms     */
    osSignalWait(0x0010, 7500);               /* wait for event with timeout */
    SetLights (YELLOW, 1);                    /* timeout value: 7.5s         */
    SetLights (GREEN, 0);
    osDelay(500);                             /* wait for timeout: 500ms     */
    SetLights (RED, 1);                       /* RED light for cars          */
    SetLights (YELLOW, 0);
    osDelay(500);                             /* wait for timeout: 500ms     */
//    SetLights (STOP, 0);                      /* GREEN light for WALKers     */    
//    SetLights (WALK, 1);
//    osDelay(2500);                            /* wait for timeout: 2.5s      */
//    SetLights (STOP, 1);                      /* RED light for WALKers       */        
//    SetLights (WALK, 0);
  }
}
