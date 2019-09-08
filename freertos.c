/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
/* Private includes ----------------------------------------------------------*/
#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "LiquidCrystal.h"

using namespace std;
#else
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#endif

#include <xmc_gpio.h>
#include <xmc_rtc.h>
#include "led.h"
#include "XMC1000_Tse.h"
#include "ring_buffer.h"
#include "serial.h"
#include "shell.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "croutine.h"

#ifdef CLOCKS_PER_SEC
#undef CLOCKS_PER_SEC
#define	CLOCKS_PER_SEC	configTICK_RATE_HZ
#endif

/* Private variables ---------------------------------------------------------*/
TaskHandle_t g_task01_handle;
TaskHandle_t g_task02_handle;
TaskHandle_t g_task_shell_handle;
SemaphoreHandle_t g_noti_sema;
SemaphoreHandle_t g_noti_sema_stdin;
QueueHandle_t g_queue;
TimerHandle_t g_timer;

RING_BUFFER_DEF(serial_buffer, SERIAL_BUFFER_SIZE);

/* Private function prototypes -----------------------------------------------*/
/* Determine whether we are in thread mode or handler mode. */
static inline int inHandlerMode (void)  __pure {
  return __get_IPSR() != 0;
}

void _clock_init( void ) {
	//No need to reinitialize the clock
}

uint32_t getKernelSysTick(void) {
  if (inHandlerMode()) {
    return xTaskGetTickCountFromISR();
  } else {
    return xTaskGetTickCount();
  }
}

clock_t clock( void ) {
	return getKernelSysTick();
}

int frequency_of_primes (int n) {
  int i,j;
  int freq=n-1;
  for (i=2; i<=n; ++i) for (j=sqrt(i);j>1;--j) if (i%j==0) {--freq; break;}
  return freq;
}

void TimerCallback( xTimerHandle pxtimer ) {
	uint32_t ra = __return_address();
		
//	LED_Toggle(0);
}

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void shell_task(void const * argument);
	
void coTask(CoRoutineHandle_t handle,UBaseType_t uxIndex)
{
    static const TickType_t delay = 500 / portTICK_PERIOD_MS;
     
    crSTART(handle);
     
    while(1) {     	
			LED_Toggle(uxIndex);
//			crDELAY(handle,10);

			crDELAY(handle,delay);
    }
     
    crEND();
}

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* Create the thread(s) */
  /* definition and creation of defaultTask */
//	xTaskCreate((TaskFunction_t)StartDefaultTask,
//							(const portCHAR *)"defaultTask",
//							384,
//							NULL,
//							2,
//							&g_task01_handle);

//  /* definition and creation of myTask02 */
//	xTaskCreate((TaskFunction_t)StartTask02,
//							(const portCHAR *)"myTask02",
//							64,
//							NULL,
//							3,
//							&g_task02_handle);
							
	xTaskCreate((TaskFunction_t)shell_task,
							(const portCHAR *)"shell_task",
							384,
							NULL,
							2,
							&g_task_shell_handle);		
							
	BaseType_t ret_bt;
	ret_bt = xCoRoutineCreate(coTask, 0, 0);
	ret_bt = xCoRoutineCreate(coTask, 1, 1);
							
	/* Create one Software Timer.*/
	g_timer = xTimerCreate("Timer", 
							2000/ portTICK_PERIOD_MS,
							pdTRUE,
							0,
							TimerCallback);
	xTimerStart( g_timer, 0);

	/* Create the notification semaphore and set the initial state. */
	vSemaphoreCreateBinary(g_noti_sema);
	vQueueAddToRegistry(g_noti_sema, "Notification Semaphore");
	xSemaphoreTake(g_noti_sema, 0);
	
	vSemaphoreCreateBinary(g_noti_sema_stdin);
	vQueueAddToRegistry(g_noti_sema_stdin, "STDIN Semaphore");
//	xSemaphoreTake(g_noti_sema_stdin, 0);
	
	/* Create a queue*/
	g_queue = xQueueCreate(2, sizeof(uint32_t));
}

void vApplicationIdleHook( void ) {
	vCoRoutineSchedule();
}

extern void __Vectors(void);

extern void __Vectors_End;
#define __STEXT	(void(*)(void))&__Vectors_End
extern void Region$$Table$$Limit;
#define __ETEXT ((void(*)(void))&Region$$Table$$Limit) 
	
extern void __Vectors_Size(void);
extern void Reset_Handler(void);
extern void __main(void);

//#pragma import(__use_smaller_memcpy)

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
void StartDefaultTask(void const * argument) {
	uint32_t ra = __return_address();
	uint32_t tmpTicks;	
	
	uint32_t tmp_sp = __current_sp();
	
	__yield();
	
  /* Infinite loop */
  for(;;) {
		xQueueReceive(g_queue, &tmpTicks, portMAX_DELAY);
		
//		printf("%s %u %u %u\n", 
//		tskKERNEL_VERSION_NUMBER,
//		tmpTicks,
//		SystemCoreClock,
//		__clz(tmpTicks)
//		);
//	
		xSemaphoreTake(g_noti_sema, portMAX_DELAY);	

//		char tmpBuf[1024];
//		vTaskList(tmpBuf);
//		printf(tmpBuf);
//		
//		printf("Total Heap:%u\n", 
//		configTOTAL_HEAP_SIZE);
//		
//		printf("Free Heap:%u\n",
//		xPortGetFreeHeapSize());
//				
//		printf("minimum ever:%u\n",
//		xPortGetMinimumEverFreeHeapSize());
//		
//		printf("vect: %p vect_end: %p table_limit: %p\n", __Vectors, __STEXT, __ETEXT);
//		printf("vect_size: %p\n", __Vectors_Size);
//		printf("Reset_Handler: %p\n", Reset_Handler);
//		printf("__main: %p\n", __main);
//		printf("_clock_init: %p\n", _clock_init);
//        
//		printf("%u %u\n", CLOCKS_PER_SEC, clock());
						
//		vTaskGetRunTimeStats(tmpBuf);
//		printf(tmpBuf);		

//		printf("\n");
  }
}

/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
void StartTask02(void const * argument) {
	uint32_t ra = __return_address();
  /* Infinite loop */
  for(;;) {
		LED_Toggle(2);

		xSemaphoreGive(g_noti_sema);		
		
		uint32_t tmpTick = getKernelSysTick();
		xQueueSend(g_queue, &tmpTick, 0);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}


void led_cmd(int32_t argc, char **argv) {
  if (argc == 3) {
		uint8_t tmp_num = (uint8_t)atoi(argv[1]);
		if(tmp_num >= LED_TOTAL_NUM) {
			tmp_num = 0;
		}
		
		if (strcmp(argv[2], "on") == 0) {
			LED_On(tmp_num);	
		}	else if (strcmp(argv[2], "off") == 0) {
			LED_Off(tmp_num);	
		} else {
			shell_println("Argument not supported");
		}
  }
}

void help_cmd(int32_t argc, char **argv) {
  (void)argc;
  (void)argv;

  shell_help();
}

void rtc_cmd(int32_t argc, char **argv) {
	__IO XMC_RTC_TIME_t now_rtc_time;
	
  (void)argc;
  (void)argv;
	
	XMC_RTC_GetTime((XMC_RTC_TIME_t *)&now_rtc_time);
	printf("%02d:%02d:%02d\n", now_rtc_time.hours, now_rtc_time.minutes, now_rtc_time.seconds);
}

void dts_cmd(int32_t argc, char **argv) {
  (void)argc;
  (void)argv;
	
	int32_t tmp_i32 = (int32_t)XMC1000_CalcTemperature_soft();
	printf("DTS:%i\n", tmp_i32 - 273);
}

const shell_command_t cmd_table[] = {
  {"help", 0u, 0u, help_cmd, "Display this help message", ""},
  {"led", 2u, 2u, led_cmd, "Led control", "<num> <on|off>"},
  {"rtc", 0u, 0u, rtc_cmd, "Get RTC value", ""},
  {"dts", 0u, 0u, dts_cmd, "Get DTS value", ""},
  {0, 0u, 0u, 0, 0, 0}
};

void my_shell_init(void) {
  const char DELIMITER_STR[] = "************************************************";

  shell_println("\r\n%s", DELIMITER_STR);
  shell_println(" %s", "Shell Application");
  shell_println(DELIMITER_STR);
  shell_println(" Version %s", "1.0.0");
  shell_println(" Built %s", __DATE__ " at " __TIME__);

  shell_println("\n Enter 'help' for command list.");
  shell_println("%s\n", DELIMITER_STR);
}

extern SHELL_STATE_t shell_state;
extern uint32_t shell_cmdline_pos;
extern char shell_cmdline[SHELL_CMDLINE_SIZE];
extern const shell_command_t *shell_cmd_table;

void shell_task(void const * argument) {
	char *argv[SHELL_ARGS_MAX + 1u]; /* One extra for 0 terminator.*/
	int32_t argc;
	int32_t ch;

  shell_init(cmd_table, my_shell_init);

	while (1) {							
		switch (shell_state)
		{
			case SHELL_STATE_INIT:
				printf("%s", SHELL_PROMPT);
				fflush(stdout);
				shell_state = SHELL_STATE_GET_USER_INPUT;
				break;

			case SHELL_STATE_GET_USER_INPUT:
				
				xSemaphoreTake(g_noti_sema_stdin, portMAX_DELAY);	
				
				if (ring_buffer_avail(&serial_buffer) > 0) {
					ch = getchar();				
					if (ch != EOF)
					{
						if (((char)ch != SHELL_LF) && (shell_cmdline_pos < SHELL_CMDLINE_SIZE))
						{
							switch(ch)
							{
								case SHELL_BACKSPACE:
								case SHELL_DELETE:
								if (shell_cmdline_pos > 0U)
								{
									shell_cmdline_pos -= 1U;
									stdout_putchar(SHELL_BACKSPACE);
									stdout_putchar(' ');
									stdout_putchar(SHELL_BACKSPACE);
								}
								break;

								default:
									if ((shell_cmdline_pos + 1U) < SHELL_CMDLINE_SIZE)
									{
										/* Only printable characters. */
										if (((char)ch >= SHELL_SPACE) && ((char)ch <= SHELL_DELETE))
										{
											shell_cmdline[shell_cmdline_pos] = (char)ch;
											shell_cmdline_pos++;
											stdout_putchar((char)ch);
										}
									}
									break;
							}
						}
						else
						{
							shell_cmdline[shell_cmdline_pos] = '\0';

							stdout_putchar(SHELL_CR);
							stdout_putchar(SHELL_LF);

							shell_state = SHELL_STATE_EXEC_CMD;
						}
					}
				}
				break;

			case SHELL_STATE_EXEC_CMD:
				argc = shell_make_argv(shell_cmdline, argv);

				if (argc != 0)
				{
					const shell_command_t *cur_command = shell_cmd_table;
					while (cur_command->name)
					{
						if (strcasecmp(cur_command->name, argv[0]) == 0) /* Command is found. */
						{
							if (((argc - 1u) >= cur_command->min_args) && ((argc - 1u) <= cur_command->max_args))
							{
								 if (cur_command->cmd_ptr)
								 {
									 ((void(*)(int32_t cmd_ptr_argc, char **cmd_ptr_argv))(cur_command->cmd_ptr))(argc, argv);
								 }
							}
							else /* Wrong command syntax. */
							{
								shell_println(SHELL_ERR_SYNTAX, argv[0]);
							}

							break;
						}
						cur_command++;
					}

					if (cur_command->name == 0)
					{
						shell_println(SHELL_ERR_CMD, argv[0]);
					}
				}

				shell_state = SHELL_STATE_END_CMD;
				break;

			case SHELL_STATE_END_CMD:
				shell_state = SHELL_STATE_INIT;
				shell_cmdline_pos = 0u;
				shell_cmdline[0] = 0u;
				break;

			default:
					break;

		}	
  }
}
