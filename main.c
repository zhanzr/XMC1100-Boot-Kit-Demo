/*------------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: MDK ARMCC C Project Template for XMC1100 Bootkit
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <XMC1100.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include "led.h"
#include "serial.h"

#include "Driver_USART.h"

/* USART Driver */
extern ARM_DRIVER_USART Driver_USART1;
static ARM_DRIVER_USART *UARTdrv = &Driver_USART1; 

__IO uint32_t g_Ticks;

static uint8_t g_rcv_byte;

void myUART_callback(uint32_t event)
{
    switch (event)
    {
    case ARM_USART_EVENT_RECEIVE_COMPLETE:  
       UARTdrv->Receive(&g_rcv_byte, 1);
     break;
     
    case ARM_USART_EVENT_TRANSFER_COMPLETE:
    case ARM_USART_EVENT_SEND_COMPLETE:
    case ARM_USART_EVENT_TX_COMPLETE:
     
        break;
 
    case ARM_USART_EVENT_RX_TIMEOUT:
         __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
        break;
 
    case ARM_USART_EVENT_RX_OVERFLOW:
    case ARM_USART_EVENT_TX_UNDERFLOW:
        __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
        break;
    }
}
  
int main(void) {
	__IO uint32_t tmpTick;
	__IO uint32_t deltaTick;
	__IO uint32_t i=0;		
	
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / 1000);
	
  /*Initialize the UART driver */
  UARTdrv->Initialize(myUART_callback);
  /*Power up the UART peripheral */
  UARTdrv->PowerControl(ARM_POWER_FULL);
  /*Configure the USART to 9600 Bits/sec */
  UARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                   ARM_USART_DATA_BITS_8 |
                   ARM_USART_PARITY_NONE |
                   ARM_USART_STOP_BITS_1 , SERIAL_BAUDRATE);
   
  /* Enable the Transmitter/Receiver line */
  UARTdrv->Control (ARM_USART_CONTROL_TX, 1);
  UARTdrv->Control (ARM_USART_CONTROL_RX, 1);

  printf ("UART cmsis driver echo test @%u Hz\n",
	SystemCoreClock	);

	UARTdrv->Receive(&g_rcv_byte, 1);
	
	LED_Initialize();
	
	while (1) {		
			if(0 != g_rcv_byte) {					
				Driver_USART1.Send(&g_rcv_byte, 1);
				g_rcv_byte = 0;
			}
  }
}
