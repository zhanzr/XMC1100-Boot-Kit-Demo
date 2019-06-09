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
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>

#include "arm_nnexamples_gru_test_data.h"
#include "arm_math.h"
#include "arm_nnfunctions.h"

#include "led.h"


/**
 * @defgroup GRUExample Gated Recurrent Unit Example
 *
 * \par Description:
 * \par
 * Demonstrates a gated recurrent unit (GRU) example with the use of fully-connected,
 * Tanh/Sigmoid activation functions.
 *
 * \par Model definition:
 * \par
 * GRU is a type of recurrent neural network (RNN). It contains two sigmoid gates and one hidden
 * state. 
 * \par
 * The computation can be summarized as:
 * <pre>z[t] = sigmoid( W_z &sdot; {h[t-1],x[t]} )
 * r[t] = sigmoid( W_r &sdot; {h[t-1],x[t]} ) 
 * n[t] = tanh( W_n &sdot; [r[t] &times; {h[t-1], x[t]} ) 
 * h[t] = (1 - z[t]) &times; h[t-1] + z[t] &times; n[t] </pre>
 * \image html GRU.gif "Gate Recurrent Unit Diagram"
 *
 * \par Variables Description:
 * \par
 * \li \c update_gate_weights, \c reset_gate_weights, \c hidden_state_weights are weights corresponding to update gate (W_z), reset gate (W_r), and hidden state (W_n).
 * \li \c update_gate_bias, \c reset_gate_bias, \c hidden_state_bias are layer bias arrays
 * \li \c test_input1, \c test_input2, \c test_history are the inputs and initial history
 *
 * \par
 * The buffer is allocated as:
 * \par
 * | reset | input | history | update | hidden_state |
 * \par
 * In this way, the concatination is automatically done since (reset, input) and (input, history)
 * are physically concatinated in memory.
 * \par
 *  The ordering of the weight matrix should be adjusted accordingly.
 *
  *
 * 
 * \par CMSIS DSP Software Library Functions Used:
 * \par
 * - arm_fully_connected_mat_q7_vec_q15_opt()
 * - arm_nn_activations_direct_q15()
 * - arm_mult_q15()
 * - arm_offset_q15()
 * - arm_sub_q15()
 * - arm_copy_q15()
 *
 * <b> Refer  </b>
 * \link arm_nnexamples_gru.cpp \endlink
 *
 */
 
#ifndef HZ
	#define	HZ	1000
#endif

#define UART_RX P1_3
#define UART_TX P1_2

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

__IO uint32_t g_Ticks;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = 
{	
  .data_bits = 8U,
  .stop_bits = 1U,
  .baudrate = 921600
};

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH1, (uint8_t)ch);
	return ch;
}


#define DIM_HISTORY 32
#define DIM_INPUT 32
#define DIM_VEC 64

#define USE_X4

#ifndef USE_X4
static q7_t update_gate_weights[DIM_VEC * DIM_HISTORY] = UPDATE_GATE_WEIGHT_X2;
static q7_t reset_gate_weights[DIM_VEC * DIM_HISTORY] = RESET_GATE_WEIGHT_X2;
static q7_t hidden_state_weights[DIM_VEC * DIM_HISTORY] = HIDDEN_STATE_WEIGHT_X2;
#else
static q7_t update_gate_weights[DIM_VEC * DIM_HISTORY] = UPDATE_GATE_WEIGHT_X4;
static q7_t reset_gate_weights[DIM_VEC * DIM_HISTORY] = RESET_GATE_WEIGHT_X4;
static q7_t hidden_state_weights[DIM_VEC * DIM_HISTORY] = HIDDEN_STATE_WEIGHT_X4;
#endif

static q7_t update_gate_bias[DIM_HISTORY] = UPDATE_GATE_BIAS;
static q7_t reset_gate_bias[DIM_HISTORY] = RESET_GATE_BIAS;
static q7_t hidden_state_bias[DIM_HISTORY] = HIDDEN_STATE_BIAS;

static q15_t test_input1[DIM_INPUT] = INPUT_DATA1;
static q15_t test_input2[DIM_INPUT] = INPUT_DATA2;
static q15_t test_history[DIM_HISTORY] = HISTORY_DATA;

q15_t     scratch_buffer[DIM_HISTORY * 4 + DIM_INPUT];

void gru_example(q15_t * scratch_input, uint16_t input_size, uint16_t history_size,
                 q7_t * weights_update, q7_t * weights_reset, q7_t * weights_hidden_state,
                 q7_t * bias_update, q7_t * bias_reset, q7_t * bias_hidden_state)
{
  q15_t    *reset = scratch_input;
  q15_t    *input = scratch_input + history_size;
  q15_t    *history = scratch_input + history_size + input_size;
  q15_t    *update = scratch_input + 2 * history_size + input_size;
  q15_t    *hidden_state = scratch_input + 3 * history_size + input_size;

  // reset gate calculation
  // the range of the output can be adjusted with bias_shift and output_shift
#ifndef USE_X4
  arm_fully_connected_mat_q7_vec_q15(input, weights_reset, input_size + history_size, history_size, 0, 15, bias_reset,
                                     reset, NULL);
#else
  arm_fully_connected_mat_q7_vec_q15_opt(input, weights_reset, input_size + history_size, history_size, 0, 15,
                                         bias_reset, reset, NULL);
#endif
  // sigmoid function, the size of the integer bit-width should be consistent with out_shift
  arm_nn_activations_direct_q15(reset, history_size, 0, ARM_SIGMOID);
  arm_mult_q15(history, reset, reset, history_size);

  // update gate calculation
  // the range of the output can be adjusted with bias_shift and output_shift
#ifndef USE_X4
  arm_fully_connected_mat_q7_vec_q15(input, weights_update, input_size + history_size, history_size, 0, 15,
                                     bias_update, update, NULL);
#else
  arm_fully_connected_mat_q7_vec_q15_opt(input, weights_update, input_size + history_size, history_size, 0, 15,
                                         bias_update, update, NULL);
#endif

  // sigmoid function, the size of the integer bit-width should be consistent with out_shift
  arm_nn_activations_direct_q15(update, history_size, 0, ARM_SIGMOID);

  // hidden state calculation
#ifndef USE_X4
  arm_fully_connected_mat_q7_vec_q15(reset, weights_hidden_state, input_size + history_size, history_size, 0, 15,
                                     bias_hidden_state, hidden_state, NULL);
#else
  arm_fully_connected_mat_q7_vec_q15_opt(reset, weights_hidden_state, input_size + history_size, history_size, 0, 15,
                                         bias_hidden_state, hidden_state, NULL);
#endif

  // tanh function, the size of the integer bit-width should be consistent with out_shift
  arm_nn_activations_direct_q15(hidden_state, history_size, 0, ARM_TANH);
  arm_mult_q15(update, hidden_state, hidden_state, history_size);

  // we calculate z - 1 here
  // so final addition becomes substraction
  arm_offset_q15(update, 0x8000, update, history_size);
  // multiply history
  arm_mult_q15(history, update, update, history_size);
  // calculate history_out
  arm_sub_q15(hidden_state, update, history, history_size);

  return;
}

int main(void) {
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
  
	/* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH1);

  /* Configure pins */
	XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);
	
  printf ("Gated Recurrent Unit Example XMC1100 by Automan @ Infineon BBS @%u Hz\n",
	SystemCoreClock	);
	
	LED_Initialize();
	
  printf("Start GRU execution\n");
  int       input_size = DIM_INPUT;
  int       history_size = DIM_HISTORY;

  // copy over the input data 
  arm_copy_q15(test_input1, scratch_buffer + history_size, input_size);
  arm_copy_q15(test_history, scratch_buffer + history_size + input_size, history_size);

  gru_example(scratch_buffer, input_size, history_size,
              update_gate_weights, reset_gate_weights, hidden_state_weights,
              update_gate_bias, reset_gate_bias, hidden_state_bias);
  printf("Complete first iteration on GRU\n");

  arm_copy_q15(test_input2, scratch_buffer + history_size, input_size);
  gru_example(scratch_buffer, input_size, history_size,
              update_gate_weights, reset_gate_weights, hidden_state_weights,
              update_gate_bias, reset_gate_bias, hidden_state_bias);
  printf("Complete second iteration on GRU\n");

	while (1) {				
//    LED_On(0);
//    LED_On(1);
//    LED_On(2);
//    LED_On(3);
    LED_On(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks) {
			__NOP();
			__WFI();
		}
		
//    LED_Off(0);
//    LED_Off(1);
//    LED_Off(2);
//    LED_Off(3);
    LED_Off(4);
		
		tmpTick = g_Ticks;
		while((tmpTick+2000) > g_Ticks) {
			__NOP();
			__WFI();
		}		
  }
}
