/*
 *
 * Copyright (c) 2015-2016, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *
 */

/**
 * @file RTE_Device.h
 * @date 30 June, 2016
 * @version 1.1.0
 *
 * @brief RTE Device Configuration for Infineon XMC1xxx_T38
 *
 * History
 *
 * Version 1.0.0 
 *  Initial version
 *
 * Version 1.0.1 
 *  Fix pin assignment
 *
 * Version 1.1.0
 * - [New] SAI drivers 
 * - [Changed] Limit RX and TX FIFO sum of single channel to 32 entries
 * - [Fixed] Several pin configurations fixed
 */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

#include "xmc_device.h"
#include "xmc1_gpio_map.h"
#include "xmc1_usic_map.h"

#define NO_FIFO 0
#define FIFO_SIZE_2 1
#define FIFO_SIZE_4 2
#define FIFO_SIZE_8 3
#define FIFO_SIZE_16 4
#define FIFO_SIZE_32 5

// <e> UART0 (Universal asynchronous receiver transmitter) [Driver_USART0]
// <i> Configuration settings for Driver_USART0 in component ::Drivers:UART
#define RTE_UART0                      0

//   <o> UART0_TX Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P1_5 <5=>P2_0 <6=>P2_1
#define RTE_UART0_TX_ID                0

#if    (RTE_UART0_TX_ID == 0)
#define RTE_UART0_TX_PORT              P0_14
#define RTE_UART0_TX_AF                P0_14_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 1)
#define RTE_UART0_TX_PORT              P0_15
#define RTE_UART0_TX_AF                P0_15_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 2)
#define RTE_UART0_TX_PORT              P1_0
#define RTE_UART0_TX_AF                P1_0_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 3)
#define RTE_UART0_TX_PORT              P1_1
#define RTE_UART0_TX_AF                P1_1_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 4)
#define RTE_UART0_TX_PORT              P1_5
#define RTE_UART0_TX_AF                P1_5_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 5)
#define RTE_UART0_TX_PORT              P2_0
#define RTE_UART0_TX_AF                P2_0_AF_U0C0_DOUT0
#elif  (RTE_UART0_TX_ID == 6)
#define RTE_UART0_TX_PORT              P2_1
#define RTE_UART0_TX_AF                P2_1_AF_U0C0_DOUT0
#else
#error "Invalid UART0_TX Pin Configuration!"
#endif

//   <o> UART0_RX Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P2_0 <5=>P2_1 <6=>P2_2
#define RTE_UART0_RX_ID                0

#if    (RTE_UART0_RX_ID == 0)
#define RTE_UART0_RX_PORT              P0_14
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P0_14
#elif  (RTE_UART0_RX_ID == 1)
#define RTE_UART0_RX_PORT              P0_15
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P0_15
#elif  (RTE_UART0_RX_ID == 2)
#define RTE_UART0_RX_PORT              P1_0
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P1_0
#elif  (RTE_UART0_RX_ID == 3)
#define RTE_UART0_RX_PORT              P1_1
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P1_1
#elif  (RTE_UART0_RX_ID == 4)
#define RTE_UART0_RX_PORT              P2_0
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P2_0
#elif  (RTE_UART0_RX_ID == 5)
#define RTE_UART0_RX_PORT              P2_1
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_P2_1
#elif  (RTE_UART0_RX_ID == 6)
#define RTE_UART0_RX_PORT              P2_2
#define RTE_UART0_RX_INPUT             USIC0_C0_DX0_DX3INS
#else
#error "Invalid UART0_RX Pin Configuration!"
#endif

//   <o> UART0_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_UART0_RX_FIFO_SIZE_ID                4

#if    (RTE_UART0_RX_FIFO_SIZE_ID == 0)
#define RTE_UART0_RX_FIFO_SIZE         NO_FIFO
#define RTE_UART0_RX_FIFO_SIZE_NUM     0
#elif  (RTE_UART0_RX_FIFO_SIZE_ID == 1)
#define RTE_UART0_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_UART0_RX_FIFO_SIZE_NUM     2
#elif  (RTE_UART0_RX_FIFO_SIZE_ID == 2)
#define RTE_UART0_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_UART0_RX_FIFO_SIZE_NUM     4
#elif  (RTE_UART0_RX_FIFO_SIZE_ID == 3)
#define RTE_UART0_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_UART0_RX_FIFO_SIZE_NUM     8
#elif  (RTE_UART0_RX_FIFO_SIZE_ID == 4)
#define RTE_UART0_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_UART0_RX_FIFO_SIZE_NUM     16
#elif  (RTE_UART0_RX_FIFO_SIZE_ID == 5)
#define RTE_UART0_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_UART0_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid UART0_RX FIFO SIZE Configuration!"
#endif

//   <o> UART0_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_UART0_TX_FIFO_SIZE_ID                4

#if    (RTE_UART0_TX_FIFO_SIZE_ID == 0)
#define RTE_UART0_TX_FIFO_SIZE         NO_FIFO
#define RTE_UART0_TX_FIFO_SIZE_NUM     0
#elif  (RTE_UART0_TX_FIFO_SIZE_ID == 1)
#define RTE_UART0_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_UART0_TX_FIFO_SIZE_NUM     2
#elif  (RTE_UART0_TX_FIFO_SIZE_ID == 2)
#define RTE_UART0_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_UART0_TX_FIFO_SIZE_NUM     4
#elif  (RTE_UART0_TX_FIFO_SIZE_ID == 3)
#define RTE_UART0_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_UART0_TX_FIFO_SIZE_NUM     8
#elif  (RTE_UART0_TX_FIFO_SIZE_ID == 4)
#define RTE_UART0_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_UART0_TX_FIFO_SIZE_NUM     16
#elif  (RTE_UART0_TX_FIFO_SIZE_ID == 5)
#define RTE_UART0_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_UART0_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid UART0_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> UART1 (Universal asynchronous receiver transmitter) [Driver_USART1]
// <i> Configuration settings for Driver_USART1 in component ::Drivers:UART
#define RTE_UART1                      1

//   <o> UART1_TX Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <5=>P2_10 <6=>P2_11        
#define RTE_UART1_TX_ID                2

#if    (RTE_UART1_TX_ID == 0)
#define RTE_UART1_TX_PORT              P0_6
#define RTE_UART1_TX_AF                P0_6_AF_U0C1_DOUT0
#elif  (RTE_UART1_TX_ID == 1)
#define RTE_UART1_TX_PORT              P0_7
#define RTE_UART1_TX_AF                P0_7_AF_U0C1_DOUT0
#elif  (RTE_UART1_TX_ID == 2)
#define RTE_UART1_TX_PORT              P1_2
#define RTE_UART1_TX_AF                P1_2_AF_U0C1_DOUT0
#elif  (RTE_UART1_TX_ID == 3)
#define RTE_UART1_TX_PORT              P1_3
#define RTE_UART1_TX_AF                P1_3_AF_U0C1_DOUT0
#elif  (RTE_UART1_TX_ID == 5)
#define RTE_UART1_TX_PORT              P2_10
#define RTE_UART1_TX_AF                P2_10_AF_U0C1_DOUT0
#elif  (RTE_UART1_TX_ID == 6)
#define RTE_UART1_TX_PORT              P2_11
#define RTE_UART1_TX_AF                P2_11_AF_U0C1_DOUT0
#else
#error "Invalid UART1_TX Pin Configuration!"
#endif

//   <o> UART1_RX Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <4=>P2_10 <5=>P2_11 <6=>P2_1
#define RTE_UART1_RX_ID                3

#if    (RTE_UART1_RX_ID == 0)
#define RTE_UART1_RX_PORT              P0_6
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P0_6
#elif  (RTE_UART1_RX_ID == 1)
#define RTE_UART1_RX_PORT              P0_7
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P0_7
#elif  (RTE_UART1_RX_ID == 2)
#define RTE_UART1_RX_PORT              P1_2
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P1_2
#elif  (RTE_UART1_RX_ID == 3)
#define RTE_UART1_RX_PORT              P1_3
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P1_3
#elif  (RTE_UART1_RX_ID == 4)
#define RTE_UART1_RX_PORT              P2_10
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P2_10
#elif  (RTE_UART1_RX_ID == 5)
#define RTE_UART1_RX_PORT              P2_11
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_P2_11
#elif  (RTE_UART1_RX_ID == 6)
#define RTE_UART1_RX_PORT              P2_1
#define RTE_UART1_RX_INPUT             USIC0_C1_DX0_DX3INS
#else
#error "Invalid UART1_RX Pin Configuration!"
#endif

//   <o> UART1_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_UART1_RX_FIFO_SIZE_ID                4

#if    (RTE_UART1_RX_FIFO_SIZE_ID == 0)
#define RTE_UART1_RX_FIFO_SIZE         NO_FIFO
#define RTE_UART1_RX_FIFO_SIZE_NUM     0
#elif  (RTE_UART1_RX_FIFO_SIZE_ID == 1)
#define RTE_UART1_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_UART1_RX_FIFO_SIZE_NUM     2
#elif  (RTE_UART1_RX_FIFO_SIZE_ID == 2)
#define RTE_UART1_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_UART1_RX_FIFO_SIZE_NUM     4
#elif  (RTE_UART1_RX_FIFO_SIZE_ID == 3)
#define RTE_UART1_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_UART1_RX_FIFO_SIZE_NUM     8
#elif  (RTE_UART1_RX_FIFO_SIZE_ID == 4)
#define RTE_UART1_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_UART1_RX_FIFO_SIZE_NUM     16
#elif  (RTE_UART1_RX_FIFO_SIZE_ID == 5)
#define RTE_UART1_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_UART1_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid UART1_RX FIFO SIZE Configuration!"
#endif

//   <o> UART1_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_UART1_TX_FIFO_SIZE_ID                4

#if    (RTE_UART1_TX_FIFO_SIZE_ID == 0)
#define RTE_UART1_TX_FIFO_SIZE         NO_FIFO
#define RTE_UART1_TX_FIFO_SIZE_NUM     0
#elif  (RTE_UART1_TX_FIFO_SIZE_ID == 1)
#define RTE_UART1_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_UART1_TX_FIFO_SIZE_NUM     2
#elif  (RTE_UART1_TX_FIFO_SIZE_ID == 2)
#define RTE_UART1_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_UART1_TX_FIFO_SIZE_NUM     4
#elif  (RTE_UART1_TX_FIFO_SIZE_ID == 3)
#define RTE_UART1_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_UART1_TX_FIFO_SIZE_NUM     8
#elif  (RTE_UART1_TX_FIFO_SIZE_ID == 4)
#define RTE_UART1_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_UART1_TX_FIFO_SIZE_NUM     16
#elif  (RTE_UART1_TX_FIFO_SIZE_ID == 5)
#define RTE_UART1_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_UART1_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid UART1_TX FIFO SIZE Configuration!"
#endif
// </e>

// <e> SPI0 (Serial peripheral interface) [Driver_SPI0]
// <i> Configuration settings for Driver_SPI0 in component ::Drivers:SPI
#define RTE_SPI0                      0

//   <o> SPI0 TX: MOSI(master) MISO(slave) Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P1_5 <5=>P2_0 <6=>P2_1
#define RTE_SPI0_TX_ID                0

#if    (RTE_SPI0_TX_ID == 0)
#define RTE_SPI0_TX_PORT              P0_14
#define RTE_SPI0_TX_AF                P0_14_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 1)
#define RTE_SPI0_TX_PORT              P0_15
#define RTE_SPI0_TX_AF                P0_15_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 2)
#define RTE_SPI0_TX_PORT              P1_0
#define RTE_SPI0_TX_AF                P1_0_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 3)
#define RTE_SPI0_TX_PORT              P1_1
#define RTE_SPI0_TX_AF                P1_1_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 4)
#define RTE_SPI0_TX_PORT              P1_5
#define RTE_SPI0_TX_AF                P1_5_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 5)
#define RTE_SPI0_TX_PORT              P2_0
#define RTE_SPI0_TX_AF                P2_0_AF_U0C0_DOUT0
#elif  (RTE_SPI0_TX_ID == 6)
#define RTE_SPI0_TX_PORT              P2_1
#define RTE_SPI0_TX_AF                P2_1_AF_U0C0_DOUT0
#else
#error "Invalid SPI0_TX Pin Configuration!"
#endif

//   <o> SPI0 RX MISO(master) MOSI(slave) Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P2_0 <5=>P2_1
#define RTE_SPI0_RX_ID                0

#if    (RTE_SPI0_RX_ID == 0)
#define RTE_SPI0_RX_PORT              P0_14
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P0_14
#elif  (RTE_SPI0_RX_ID == 1)
#define RTE_SPI0_RX_PORT              P0_15
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P0_15
#elif  (RTE_SPI0_RX_ID == 2)
#define RTE_SPI0_RX_PORT              P1_0
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P1_0
#elif  (RTE_SPI0_RX_ID == 3)
#define RTE_SPI0_RX_PORT              P1_1
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P1_1
#elif  (RTE_SPI0_RX_ID == 4)
#define RTE_SPI0_RX_PORT              P2_0
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P2_0
#elif  (RTE_SPI0_RX_ID == 5)
#define RTE_SPI0_RX_PORT              P2_1
#define RTE_SPI0_RX_INPUT             USIC0_C0_DX0_P2_1
#else
#error "Invalid SPI0_RX Pin Configuration!"
#endif

//   <o> SPI0_CLK OUTPUT Pin <0=>P0_7 <1=>P0_8 <2=>P0_14 <4=>P2_0  
#define RTE_SPI0_CLK_OUTPUT_ID                0

#if    (RTE_SPI0_CLK_OUTPUT_ID == 0)
#define RTE_SPI0_CLK_OUTPUT_PORT              P0_7
#define RTE_SPI0_CLK_AF                       P0_7_AF_U0C0_SCLKOUT
#elif  (RTE_SPI0_CLK_OUTPUT_ID == 1)
#define RTE_SPI0_CLK_OUTPUT_PORT              P0_8
#define RTE_SPI0_CLK_AF                       P0_8_AF_U0C0_SCLKOUT
#elif  (RTE_SPI0_CLK_OUTPUT_ID == 2)
#define RTE_SPI0_CLK_OUTPUT_PORT              P0_14
#define RTE_SPI0_CLK_AF                       P0_14_AF_U0C0_SCLKOUT
#elif  (RTE_SPI0_CLK_OUTPUT_ID == 4)
#define RTE_SPI0_CLK_OUTPUT_PORT              P2_0
#define RTE_SPI0_CLK_AF                       P2_0_AF_U0C0_SCLKOUT
#else
#error "Invalid SPI0 CLOCK OUTPUT Pin Configuration!"
#endif
//   <h> SPI0_SLAVE SELECT Pins 
// <e> SLAVE SELECT LINE 0
// <i> Enable slave select line 0
#define RTE_SPI0_SLAVE_SELECT_LINE0 0

//   <o> SPI0_SLAVE SELECT LINE 0 Pin <0=>P0_0 <1=>P0_9 <2=>P1_4 
#define RTE_SPI0_SLAVE_SELECT_LINE_0_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_LINE_0_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_LINE_0_PORT              P0_0
#define RTE_SPI0_SLAVE_SELECT_LINE_0_AF                P0_0_AF_U0C0_SELO0
#elif  (RTE_SPI0_SLAVE_SELECT_LINE_0_ID == 1)
#define RTE_SPI0_SLAVE_SELECT_LINE_0_PORT              P0_9
#define RTE_SPI0_SLAVE_SELECT_LINE_0_AF                P0_9_AF_U0C0_SELO0
#elif  (RTE_SPI0_SLAVE_SELECT_LINE_0_ID == 2)
#define RTE_SPI0_SLAVE_SELECT_LINE_0_PORT              P1_4
#define RTE_SPI0_SLAVE_SELECT_LINE_0_AF                P1_4_AF_U0C0_SELO0
#else
#error "Invalid SPI0 SLAVE SELECT LINE 0 Pin Configuration!"
#endif
// </e>

// <e> SLAVE SELECT LINE 1
// <i> Enable slave select line 1
#define RTE_SPI0_SLAVE_SELECT_LINE1 0 

//   <o> SPI0_SLAVE SELECT LINE 1 Pin <0=>P0_10 <1=>P1_5
#define RTE_SPI0_SLAVE_SELECT_LINE_1_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_LINE_1_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_LINE_1_PORT              P0_10
#define RTE_SPI0_SLAVE_SELECT_LINE_1_AF                P0_10_AF_U0C0_SELO1
#elif    (RTE_SPI0_SLAVE_SELECT_LINE_1_ID == 1)
#define RTE_SPI0_SLAVE_SELECT_LINE_1_PORT              P1_5
#define RTE_SPI0_SLAVE_SELECT_LINE_1_AF                P1_5_AF_U0C0_SELO1
#else
#error "Invalid SPI0 SLAVE SELECT LINE 1 Pin Configuration!"
#endif
// </e>

// <e> SLAVE SELECT LINE 2
// <i> Enable slave select line 1
#define RTE_SPI0_SLAVE_SELECT_LINE2 0
//   <o> SPI0_SLAVE SELECT LINE 2 Pin <0=>P0_11
#define RTE_SPI0_SLAVE_SELECT_LINE_2_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_LINE_2_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_LINE_2_PORT              P0_11
#define RTE_SPI0_SLAVE_SELECT_LINE_2_AF                P0_11_AF_U0C0_SELO2
#else
#error "Invalid SPI0 SLAVE SELECT LINE 2 Pin Configuration!"
#endif
// </e>

// <e> SLAVE SELECT LINE 3
// <i> Enable slave select line 3
#define RTE_SPI0_SLAVE_SELECT_LINE3 0
//   <o> SPI0_SLAVE SELECT LINE 3 Pin <0=>P0_12 
#define RTE_SPI0_SLAVE_SELECT_LINE_3_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_LINE_3_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_LINE_3_PORT              P0_12
#define RTE_SPI0_SLAVE_SELECT_LINE_3_AF                P0_12_AF_U0C0_SELO3
#else
#error "Invalid SPI0 SLAVE SELECT LINE 3 Pin Configuration!"
#endif
// </e>

// <e> SLAVE SELECT LINE 4
// <i> Enable slave select line 4
#define RTE_SPI0_SLAVE_SELECT_LINE4 0
//   <o> SPI0_SLAVE SELECT LINE 4 Pin <0=>P0_13 
#define RTE_SPI0_SLAVE_SELECT_LINE_4_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_LINE_4_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_LINE_4_PORT              P0_13
#define RTE_SPI0_SLAVE_SELECT_LINE_4_AF                P0_13_AF_U0C0_SELO4
#else
#error "Invalid SPI0 SLAVE SELECT LINE 4 Pin Configuration!"
#endif
// </e>

#define RTE_SPI0_SLAVE_SELECT_LINE5 0 

// </h>
//   <o> SPI0_CLK INPUT Pin <0=>P0_14 <1=>P0_8 <2=>P0_7 <3=>P1_1 <4=>P2_0 
#define RTE_SPI0_CLK_INPUT_ID                0

#if    (RTE_SPI0_CLK_INPUT_ID == 0)
#define RTE_SPI0_CLK_INPUT_PORT              P0_14
#define RTE_SPI0_CLK_INPUT                   USIC0_C0_DX1_P0_14
#elif  (RTE_SPI0_CLK_INPUT_ID == 1)
#define RTE_SPI0_CLK_INPUT_PORT              P0_8
#define RTE_SPI0_CLK_INPUT                   USIC0_C0_DX1_P0_8
#elif  (RTE_SPI0_CLK_INPUT_ID == 2)
#define RTE_SPI0_CLK_INPUT_PORT              P0_7
#define RTE_SPI0_CLK_INPUT                   USIC0_C0_DX1_P0_7
#elif  (RTE_SPI0_CLK_INPUT_ID == 3)
#define RTE_SPI0_CLK_INPUT_PORT              P1_1
#define RTE_SPI0_CLK_INPUT                   USIC0_C0_DX1_P1_1
#elif  (RTE_SPI0_CLK_INPUT_ID == 4)
#define RTE_SPI0_CLK_INPUT_PORT              P2_0
#define RTE_SPI0_CLK_INPUT                   USIC0_C0_DX1_P2_0
#else
#error "Invalid SPI0 CLOCK INPUT Pin Configuration!"
#endif

//   <o> RTE_SPI0_SLAVE_SELECT INPUT Pin <0=>P0_0 <1=>P0_9 <2=>P0_10 <3=>P0_11 <4=>P0_12 <5=>P0_13
#define RTE_SPI0_SLAVE_SELECT_INPUT_ID                0

#if    (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 0)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_0
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_0
#elif  (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 1)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_9
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_9
#elif  (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 2)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_10
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_10
#elif  (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 3)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_11
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_11
#elif  (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 4)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_12
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_12
#elif  (RTE_SPI0_SLAVE_SELECT_INPUT_ID == 5)
#define RTE_SPI0_SLAVE_SELECT_INPUT_PORT              P0_13
#define RTE_SPI0_SLAVE_SELECT_INPUT                   USIC0_C0_DX2_P0_13
#else
#error "Invalid SPI0 SLAVE SELECT INPUT Pin Configuration!"
#endif

//   <o> SPI0_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_SPI0_RX_FIFO_SIZE_ID                4

#if    (RTE_SPI0_RX_FIFO_SIZE_ID == 0)
#define RTE_SPI0_RX_FIFO_SIZE         NO_FIFO
#define RTE_SPI0_RX_FIFO_SIZE_NUM     0
#elif  (RTE_SPI0_RX_FIFO_SIZE_ID == 1)
#define RTE_SPI0_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_SPI0_RX_FIFO_SIZE_NUM     2
#elif  (RTE_SPI0_RX_FIFO_SIZE_ID == 2)
#define RTE_SPI0_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_SPI0_RX_FIFO_SIZE_NUM     4
#elif  (RTE_SPI0_RX_FIFO_SIZE_ID == 3)
#define RTE_SPI0_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_SPI0_RX_FIFO_SIZE_NUM     8
#elif  (RTE_SPI0_RX_FIFO_SIZE_ID == 4)
#define RTE_SPI0_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_SPI0_RX_FIFO_SIZE_NUM     16
#elif  (RTE_SPI0_RX_FIFO_SIZE_ID == 5)
#define RTE_SPI0_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_SPI0_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid SPI0_RX FIFO SIZE Configuration!"
#endif

//   <o> SPI0_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_SPI0_TX_FIFO_SIZE_ID                4

#if    (RTE_SPI0_TX_FIFO_SIZE_ID == 0)
#define RTE_SPI0_TX_FIFO_SIZE         NO_FIFO
#define RTE_SPI0_TX_FIFO_SIZE_NUM     0
#elif  (RTE_SPI0_TX_FIFO_SIZE_ID == 1)
#define RTE_SPI0_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_SPI0_TX_FIFO_SIZE_NUM     2
#elif  (RTE_SPI0_TX_FIFO_SIZE_ID == 2)
#define RTE_SPI0_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_SPI0_TX_FIFO_SIZE_NUM     4
#elif  (RTE_SPI0_TX_FIFO_SIZE_ID == 3)
#define RTE_SPI0_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_SPI0_TX_FIFO_SIZE_NUM     8
#elif  (RTE_SPI0_TX_FIFO_SIZE_ID == 4)
#define RTE_SPI0_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_SPI0_TX_FIFO_SIZE_NUM     16
#elif  (RTE_SPI0_TX_FIFO_SIZE_ID == 5)
#define RTE_SPI0_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_SPI0_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid SPI0_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> SPI1 (Serial peripheral interface) [Driver_SPI1]
// <i> Configuration settings for Driver_SPI1 in component ::Drivers:SPI
#define RTE_SPI1                      0

//   <o> SPI1 TX MOSI(master) MISO(slave) <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <5=>P2_10 <6=>P2_11   
#define RTE_SPI1_TX_ID                0

#if    (RTE_SPI1_TX_ID == 0)
#define RTE_SPI1_TX_PORT              P0_6
#define RTE_SPI1_TX_AF                P0_6_AF_U0C1_DOUT0
#elif  (RTE_SPI1_TX_ID == 1)
#define RTE_SPI1_TX_PORT              P0_7
#define RTE_SPI1_TX_AF                P0_7_AF_U0C1_DOUT0
#elif  (RTE_SPI1_TX_ID == 2)
#define RTE_SPI1_TX_PORT              P1_2
#define RTE_SPI1_TX_AF                P1_2_AF_U0C1_DOUT0
#elif  (RTE_SPI1_TX_ID == 3)
#define RTE_SPI1_TX_PORT              P1_3
#define RTE_SPI1_TX_AF                P1_3_AF_U0C1_DOUT0
#elif  (RTE_SPI1_TX_ID == 5)
#define RTE_SPI1_TX_PORT              P2_10
#define RTE_SPI1_TX_AF                P2_10_AF_U0C1_DOUT0
#elif  (RTE_SPI1_TX_ID == 6)
#define RTE_SPI1_TX_PORT              P2_11
#define RTE_SPI1_TX_AF                P2_11_AF_U0C1_DOUT0
#else
#error "Invalid SPI1_TX Pin Configuration!"
#endif

//   <o> SPI1 RX MISO(master) MOSI(slave) Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <4=>P2_10 <5=>P2_11
#define RTE_SPI1_RX_ID                0

#if    (RTE_SPI1_RX_ID == 0)
#define RTE_SPI1_RX_PORT              P0_6
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P0_6
#elif  (RTE_SPI1_RX_ID == 1)
#define RTE_SPI1_RX_PORT              P0_7
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P0_7
#elif  (RTE_SPI1_RX_ID == 2)
#define RTE_SPI1_RX_PORT              P1_2
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P1_2
#elif  (RTE_SPI1_RX_ID == 3)
#define RTE_SPI1_RX_PORT              P1_3
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P1_3
#elif  (RTE_SPI1_RX_ID == 4)
#define RTE_SPI1_RX_PORT              P2_10
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P2_10
#elif  (RTE_SPI1_RX_ID == 5)
#define RTE_SPI1_RX_PORT              P2_11
#define RTE_SPI1_RX_INPUT             USIC0_C1_DX0_P2_11
#else
#error "Invalid SPI1_RX Pin Configuration!"
#endif

//   <o> SPI1_CLK OUTPUT Pin <0=>P0_8 <1=>P1_3 <2=>P1_4 <3=>P2_1 <4=>P2_11
#define RTE_SPI1_CLK_OUTPUT_ID                0

#if    (RTE_SPI1_CLK_OUTPUT_ID == 0)
#define RTE_SPI1_CLK_OUTPUT_PORT              P0_8
#define RTE_SPI1_CLK_AF                       P0_8_AF_U0C1_SCLKOUT
#elif  (RTE_SPI1_CLK_OUTPUT_ID == 1)
#define RTE_SPI1_CLK_OUTPUT_PORT              P1_3
#define RTE_SPI1_CLK_AF                       P1_3_AF_U0C1_SCLKOUT
#elif  (RTE_SPI1_CLK_OUTPUT_ID == 2)
#define RTE_SPI1_CLK_OUTPUT_PORT              P1_4
#define RTE_SPI1_CLK_AF                       P1_4_AF_U0C1_SCLKOUT
#elif  (RTE_SPI1_CLK_OUTPUT_ID == 3)
#define RTE_SPI1_CLK_OUTPUT_PORT              P2_1
#define RTE_SPI1_CLK_AF                       P2_1_AF_U0C1_SCLKOUT
#elif  (RTE_SPI1_CLK_OUTPUT_ID == 4)
#define RTE_SPI1_CLK_OUTPUT_PORT              P2_11
#define RTE_SPI1_CLK_AF                       P2_11_AF_U0C1_SCLKOUT
#else
#error "Invalid SPI1 CLOCK OUTPUT Pin Configuration!"
#endif

// <h> SPI1_SLAVE SELECT Pins
// <e> SLAVE SELECT LINE 0
// <i> Enable slave select line 0
#define RTE_SPI1_SLAVE_SELECT_LINE0 0

//   <o> SPI1_SLAVE SELECT LINE 0 Pin <0=>P0_0 <1=>P0_9 <2=>P1_1
#define RTE_SPI1_SLAVE_SELECT_LINE_0_ID                0

#if    (RTE_SPI1_SLAVE_SELECT_LINE_0_ID == 0)
#define RTE_SPI1_SLAVE_SELECT_LINE_0_PORT              P0_0
#define RTE_SPI1_SLAVE_SELECT_LINE_0_AF                P0_0_AF_U0C1_SELO0
#elif  (RTE_SPI1_SLAVE_SELECT_LINE_0_ID == 1)
#define RTE_SPI1_SLAVE_SELECT_LINE_0_PORT              P0_9
#define RTE_SPI1_SLAVE_SELECT_LINE_0_AF                P0_9_AF_U0C1_SELO0
#elif  (RTE_SPI1_SLAVE_SELECT_LINE_0_ID == 2)
#define RTE_SPI1_SLAVE_SELECT_LINE_0_PORT              P1_1
#define RTE_SPI1_SLAVE_SELECT_LINE_0_AF                P1_1_AF_U0C1_SELO0
#else
#error "Invalid SPI1 SLAVE SELECT LINE 0 Pin Configuration!"
#endif
// </e>
// <e> SLAVE SELECT LINE 1
// <i> Enable slave select line 1
#define RTE_SPI1_SLAVE_SELECT_LINE1 0

//   <o> SPI1_SLAVE SELECT LINE 1 Pin <0=>P0_10 <1=>P1_4 
#define RTE_SPI1_SLAVE_SELECT_LINE_1_ID                0

#if    (RTE_SPI1_SLAVE_SELECT_LINE_1_ID == 0)
#define RTE_SPI1_SLAVE_SELECT_LINE_1_PORT              P0_10
#define RTE_SPI1_SLAVE_SELECT_LINE_1_AF                P0_10_AF_U0C1_SELO1
#elif  (RTE_SPI1_SLAVE_SELECT_LINE_1_ID == 1)
#define RTE_SPI1_SLAVE_SELECT_LINE_1_PORT              P1_4
#define RTE_SPI1_SLAVE_SELECT_LINE_1_AF                P1_4_AF_U0C1_SELO1
#else
#error "Invalid SPI1 SLAVE SELECT LINE 1 Pin Configuration!"
#endif
// </e>
// <e> SLAVE SELECT LINE 2
// <i> Enable slave select line 2
#define RTE_SPI1_SLAVE_SELECT_LINE2 0

//   <o> SPI1_SLAVE SELECT LINE 2 Pin <0=>P0_11 <1=>P1_5 
#define RTE_SPI1_SLAVE_SELECT_LINE_2_ID                0

#if    (RTE_SPI1_SLAVE_SELECT_LINE_2_ID == 0)
#define RTE_SPI1_SLAVE_SELECT_LINE_2_PORT              P0_11
#define RTE_SPI1_SLAVE_SELECT_LINE_2_AF                P0_11_AF_U0C1_SELO2
#elif  (RTE_SPI1_SLAVE_SELECT_LINE_2_ID == 1)
#define RTE_SPI1_SLAVE_SELECT_LINE_2_PORT              P1_5
#define RTE_SPI1_SLAVE_SELECT_LINE_2_AF                P1_5_AF_U0C1_SELO2
#else
#error "Invalid SPI1 SLAVE SELECT LINE 2 Pin Configuration!"
#endif
// </e>

#define RTE_SPI1_SLAVE_SELECT_LINE3 0
// </h>

//   <o> SPI1_CLK INPUT Pin <0=>P1_3 <1=>P0_8 <2=>P0_7 <3=>P2_11
#define RTE_SPI1_CLK_INPUT_ID                0

#if    (RTE_SPI1_CLK_INPUT_ID == 0)
#define RTE_SPI1_CLK_INPUT_PORT              P1_3
#define RTE_SPI1_CLK_INPUT                   USIC0_C1_DX1_P1_3
#elif  (RTE_SPI1_CLK_INPUT_ID == 1)
#define RTE_SPI1_CLK_INPUT_PORT              P0_8
#define RTE_SPI1_CLK_INPUT                   USIC0_C1_DX1_P0_8
#elif  (RTE_SPI1_CLK_INPUT_ID == 2)
#define RTE_SPI1_CLK_INPUT_PORT              P0_7
#define RTE_SPI1_CLK_INPUT                   USIC0_C1_DX1_P0_7
#elif  (RTE_SPI1_CLK_INPUT_ID == 3)
#define RTE_SPI1_CLK_INPUT_PORT              P2_11
#define RTE_SPI1_CLK_INPUT                   USIC0_C1_DX1_P2_11
#else
#error "Invalid SPI1 CLOCK INPUT Pin Configuration!"
#endif

//   <o> RTE_SPI1_SLAVE_SELECT INPUT Pin <0=>P0_0 <1=>P0_9 <2=>P0_10 <3=>P0_11 <4=>P1_1 <5=>P2_0
#define RTE_SPI1_SLAVE_SELECT_INPUT_ID                0

#if    (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 0)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P0_0
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P0_0
#elif  (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 1)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P0_9
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P0_9
#elif  (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 2)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P0_10
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P0_10
#elif  (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 3)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P0_11
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P0_11
#elif  (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 4)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P1_1
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P1_1
#elif  (RTE_SPI1_SLAVE_SELECT_INPUT_ID == 5)
#define RTE_SPI1_SLAVE_SELECT_INPUT_PORT              P2_0
#define RTE_SPI1_SLAVE_SELECT_INPUT                   USIC0_C1_DX2_P2_0
#else
#error "Invalid SPI1 SLAVE SELECT INPUT Pin Configuration!"
#endif

//   <o> SPI1_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_SPI1_RX_FIFO_SIZE_ID                4

#if    (RTE_SPI1_RX_FIFO_SIZE_ID == 0)
#define RTE_SPI1_RX_FIFO_SIZE         NO_FIFO
#define RTE_SPI1_RX_FIFO_SIZE_NUM     0
#elif  (RTE_SPI1_RX_FIFO_SIZE_ID == 1)
#define RTE_SPI1_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_SPI1_RX_FIFO_SIZE_NUM     2
#elif  (RTE_SPI1_RX_FIFO_SIZE_ID == 2)
#define RTE_SPI1_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_SPI1_RX_FIFO_SIZE_NUM     4
#elif  (RTE_SPI1_RX_FIFO_SIZE_ID == 3)
#define RTE_SPI1_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_SPI1_RX_FIFO_SIZE_NUM     8
#elif  (RTE_SPI1_RX_FIFO_SIZE_ID == 4)
#define RTE_SPI1_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_SPI1_RX_FIFO_SIZE_NUM     16
#elif  (RTE_SPI1_RX_FIFO_SIZE_ID == 5)
#define RTE_SPI1_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_SPI1_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid SPI1_RX FIFO SIZE Configuration!"
#endif

//   <o> SPI1_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_SPI1_TX_FIFO_SIZE_ID                4

#if    (RTE_SPI1_TX_FIFO_SIZE_ID == 0)
#define RTE_SPI1_TX_FIFO_SIZE         NO_FIFO
#define RTE_SPI1_TX_FIFO_SIZE_NUM     0
#elif  (RTE_SPI1_TX_FIFO_SIZE_ID == 1)
#define RTE_SPI1_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_SPI1_TX_FIFO_SIZE_NUM     2
#elif  (RTE_SPI1_TX_FIFO_SIZE_ID == 2)
#define RTE_SPI1_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_SPI1_TX_FIFO_SIZE_NUM     4
#elif  (RTE_SPI1_TX_FIFO_SIZE_ID == 3)
#define RTE_SPI1_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_SPI1_TX_FIFO_SIZE_NUM     8
#elif  (RTE_SPI1_TX_FIFO_SIZE_ID == 4)
#define RTE_SPI1_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_SPI1_TX_FIFO_SIZE_NUM     16
#elif  (RTE_SPI1_TX_FIFO_SIZE_ID == 5)
#define RTE_SPI1_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_SPI1_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid SPI1_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> I2C0 (Inter-Integrated circuit) [Driver_I2C0]
// <i> Configuration settings for Driver_I2C0 in component ::Drivers:I2C
#define RTE_I2C0                      0

//   <o> I2C0_TX Pin Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P1_5 <5=>P2_0 <6=>P2_1     
#define RTE_I2C0_TX_ID                      0

#if    (RTE_I2C0_TX_ID == 0)
#define RTE_I2C0_TX_PORT              P0_14
#define RTE_I2C0_TX_AF                P0_14_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 1)
#define RTE_I2C0_TX_PORT              P0_15
#define RTE_I2C0_TX_AF                P0_15_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 2)
#define RTE_I2C0_TX_PORT              P1_0
#define RTE_I2C0_TX_AF                P1_0_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 3)
#define RTE_I2C0_TX_PORT              P1_1
#define RTE_I2C0_TX_AF                P1_1_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 4)
#define RTE_I2C0_TX_PORT              P1_5
#define RTE_I2C0_TX_AF                P1_5_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 5)
#define RTE_I2C0_TX_PORT              P2_0
#define RTE_I2C0_TX_AF                P2_0_AF_U0C0_DOUT0
#elif  (RTE_I2C0_TX_ID == 6)
#define RTE_I2C0_TX_PORT              P2_1
#define RTE_I2C0_TX_AF                P2_1_AF_U0C0_DOUT0
#else
#error "Invalid I2C0_TX Pin Configuration!"
#endif

//   <o> I2C0_RX Pin Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P2_0 <5=>P2_1
#define RTE_I2C0_RX_ID                0

#if    (RTE_I2C0_RX_ID == 0)
#define RTE_I2C0_RX_PORT              P0_14
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P0_14
#elif  (RTE_I2C0_RX_ID == 1)
#define RTE_I2C0_RX_PORT              P0_15
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P0_15
#elif  (RTE_I2C0_RX_ID == 2)
#define RTE_I2C0_RX_PORT              P1_0
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P1_0
#elif  (RTE_I2C0_RX_ID == 3)
#define RTE_I2C0_RX_PORT              P1_1
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P1_1
#elif  (RTE_I2C0_RX_ID == 4)
#define RTE_I2C0_RX_PORT              P2_0
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P2_0
#elif  (RTE_I2C0_RX_ID == 5)
#define RTE_I2C0_RX_PORT              P2_1
#define RTE_I2C0_RX_INPUT             USIC0_C0_DX0_P2_1
#else
#error "Invalid I2C0_RX Pin Configuration!"
#endif

//   <o> I2C0_CLK OUTPUT Pin <0=>P0_7 <1=>P0_8 <2=>P0_14 <4=>P2_0  
#define RTE_I2C0_CLK_OUTPUT_ID                0

#if    (RTE_I2C0_CLK_OUTPUT_ID == 0)
#define RTE_I2C0_CLK_OUTPUT_PORT              P0_7
#define RTE_I2C0_CLK_AF                       P0_7_AF_U0C0_SCLKOUT 
#elif  (RTE_I2C0_CLK_OUTPUT_ID == 1)
#define RTE_I2C0_CLK_OUTPUT_PORT              P0_8
#define RTE_I2C0_CLK_AF                       P0_8_AF_U0C0_SCLKOUT
#elif  (RTE_I2C0_CLK_OUTPUT_ID == 2)
#define RTE_I2C0_CLK_OUTPUT_PORT              P0_14
#define RTE_I2C0_CLK_AF                       P0_14_AF_U0C0_SCLKOUT
#elif  (RTE_I2C0_CLK_OUTPUT_ID == 4)
#define RTE_I2C0_CLK_OUTPUT_PORT              P2_0
#define RTE_I2C0_CLK_AF                       P2_0_AF_U0C0_SCLKOUT
#else
#error "Invalid I2C0 CLOCK OUTPUT Pin Configuration!"
#endif

//   <o> I2C0_CLK INPUT Pin  <0=>P0_14 <1=>P0_8 <2=>P0_7 <3=>P1_1 <4=>P2_0 
#define RTE_I2C0_CLK_INPUT_ID                0

#if    (RTE_I2C0_CLK_INPUT_ID == 0)
#define RTE_I2C0_CLK_INPUT_PORT              P0_14
#define RTE_I2C0_CLK_INPUT                   USIC0_C0_DX1_P0_14
#elif  (RTE_I2C0_CLK_INPUT_ID == 1)
#define RTE_I2C0_CLK_INPUT_PORT              P0_8
#define RTE_I2C0_CLK_INPUT                   USIC0_C0_DX1_P0_8
#elif  (RTE_I2C0_CLK_INPUT_ID == 2)
#define RTE_I2C0_CLK_INPUT_PORT              P0_7
#define RTE_I2C0_CLK_INPUT                   USIC0_C0_DX1_P0_7
#elif  (RTE_I2C0_CLK_INPUT_ID == 3)
#define RTE_I2C0_CLK_INPUT_PORT              P1_1
#define RTE_I2C0_CLK_INPUT                   USIC0_C0_DX1_P1_1
#elif  (RTE_I2C0_CLK_INPUT_ID == 4)
#define RTE_I2C0_CLK_INPUT_PORT              P2_0
#define RTE_I2C0_CLK_INPUT                   USIC0_C0_DX1_P2_0
#else
#error "Invalid I2C0 CLOCK INPUT Pin Configuration!"
#endif

//   <o> I2C0_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_I2C0_RX_FIFO_SIZE_ID                4

#if    (RTE_I2C0_RX_FIFO_SIZE_ID == 0)
#define RTE_I2C0_RX_FIFO_SIZE         NO_FIFO
#define RTE_I2C0_RX_FIFO_SIZE_NUM     0
#elif  (RTE_I2C0_RX_FIFO_SIZE_ID == 1)
#define RTE_I2C0_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2C0_RX_FIFO_SIZE_NUM     2
#elif  (RTE_I2C0_RX_FIFO_SIZE_ID == 2)
#define RTE_I2C0_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2C0_RX_FIFO_SIZE_NUM     4
#elif  (RTE_I2C0_RX_FIFO_SIZE_ID == 3)
#define RTE_I2C0_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2C0_RX_FIFO_SIZE_NUM     8
#elif  (RTE_I2C0_RX_FIFO_SIZE_ID == 4)
#define RTE_I2C0_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2C0_RX_FIFO_SIZE_NUM     16
#elif  (RTE_I2C0_RX_FIFO_SIZE_ID == 5)
#define RTE_I2C0_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2C0_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2C0_RX FIFO SIZE Configuration!"
#endif

//   <o> I2C0_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_I2C0_TX_FIFO_SIZE_ID                4

#if    (RTE_I2C0_TX_FIFO_SIZE_ID == 0)
#define RTE_I2C0_TX_FIFO_SIZE         NO_FIFO
#define RTE_I2C0_TX_FIFO_SIZE_NUM     0
#elif  (RTE_I2C0_TX_FIFO_SIZE_ID == 1)
#define RTE_I2C0_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2C0_TX_FIFO_SIZE_NUM     2
#elif  (RTE_I2C0_TX_FIFO_SIZE_ID == 2)
#define RTE_I2C0_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2C0_TX_FIFO_SIZE_NUM     4
#elif  (RTE_I2C0_TX_FIFO_SIZE_ID == 3)
#define RTE_I2C0_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2C0_TX_FIFO_SIZE_NUM     8
#elif  (RTE_I2C0_TX_FIFO_SIZE_ID == 4)
#define RTE_I2C0_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2C0_TX_FIFO_SIZE_NUM     16
#elif  (RTE_I2C0_TX_FIFO_SIZE_ID == 5)
#define RTE_I2C0_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2C0_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2C0_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> I2C1 (Inter-Integrated circuit) [Driver_I2C1]
// <i> Configuration settings for Driver_I2C1 in component ::Drivers:I2C
#define RTE_I2C1                      0

//   <o> I2C1_TX Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <5=>P2_10 <6=>P2_11   
#define RTE_I2C1_TX_ID                0

#if    (RTE_I2C1_TX_ID == 0)
#define RTE_I2C1_TX_PORT              P0_6
#define RTE_I2C1_TX_AF                P0_6_AF_U0C1_DOUT0
#elif  (RTE_I2C1_TX_ID == 1)
#define RTE_I2C1_TX_PORT              P0_7
#define RTE_I2C1_TX_AF                P0_7_AF_U0C1_DOUT0
#elif  (RTE_I2C1_TX_ID == 2)
#define RTE_I2C1_TX_PORT              P1_2
#define RTE_I2C1_TX_AF                P1_2_AF_U0C1_DOUT0
#elif  (RTE_I2C1_TX_ID == 3)
#define RTE_I2C1_TX_PORT              P1_3
#define RTE_I2C1_TX_AF                P1_3_AF_U0C1_DOUT0
#elif  (RTE_I2C1_TX_ID == 5)
#define RTE_I2C1_TX_PORT              P2_10
#define RTE_I2C1_TX_AF                P2_10_AF_U0C1_DOUT0
#elif  (RTE_I2C1_TX_ID == 6)
#define RTE_I2C1_TX_PORT              P2_11
#define RTE_I2C1_TX_AF                P2_11_AF_U0C1_DOUT0
#else
#error "Invalid I2C1_TX Pin Configuration!"
#endif

//   <o> I2C1_RX Pin Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <4=>P2_10 <5=>P2_11
#define RTE_I2C1_RX_ID                0

#if    (RTE_I2C1_RX_ID == 0)
#define RTE_I2C1_RX_PORT              P0_6
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P0_6
#elif  (RTE_I2C1_RX_ID == 1)
#define RTE_I2C1_RX_PORT              P0_7
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P0_7
#elif  (RTE_I2C1_RX_ID == 2)
#define RTE_I2C1_RX_PORT              P1_2
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P1_2
#elif  (RTE_I2C1_RX_ID == 3)
#define RTE_I2C1_RX_PORT              P1_3
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P1_3
#elif  (RTE_I2C1_RX_ID == 4)
#define RTE_I2C1_RX_PORT              P2_10
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P2_10
#elif  (RTE_I2C1_RX_ID == 5)
#define RTE_I2C1_RX_PORT              P2_11
#define RTE_I2C1_RX_INPUT             USIC0_C1_DX0_P2_11
#else
#error "Invalid I2C1_RX Pin Configuration!"
#endif

//   <o> I2C1_CLK OUTPUT <0=>P0_8 <1=>P1_3 <2=>P1_4 <3=>P2_1 <4=>P2_11
#define RTE_I2C1_CLK_OUTPUT_ID                0

#if    (RTE_I2C1_CLK_OUTPUT_ID == 0)
#define RTE_I2C1_CLK_OUTPUT_PORT              P0_8
#define RTE_I2C1_CLK_AF                       P0_8_AF_U0C1_SCLKOUT
#elif  (RTE_I2C1_CLK_OUTPUT_ID == 1)
#define RTE_I2C1_CLK_OUTPUT_PORT              P1_3
#define RTE_I2C1_CLK_AF                       P1_3_AF_U0C1_SCLKOUT
#elif  (RTE_I2C1_CLK_OUTPUT_ID == 2)
#define RTE_I2C1_CLK_OUTPUT_PORT              P1_4
#define RTE_I2C1_CLK_AF                       P1_4_AF_U0C1_SCLKOUT
#elif  (RTE_I2C1_CLK_OUTPUT_ID == 3)
#define RTE_I2C1_CLK_OUTPUT_PORT              P2_1
#define RTE_I2C1_CLK_AF                       P2_1_AF_U0C1_SCLKOUT
#elif  (RTE_I2C1_CLK_OUTPUT_ID == 4)
#define RTE_I2C1_CLK_OUTPUT_PORT              P2_11
#define RTE_I2C1_CLK_AF                       P2_11_AF_U0C1_SCLKOUT
#else
#error "Invalid I2C1 CLOCK OUTPUT Pin Configuration!"
#endif

//   <o> I2C1_CLK INPUT Pin <0=>P1_3 <1=>P0_8 <1=>P0_7 <2=>P2_11
#define RTE_I2C1_CLK_INPUT_ID                0

#if    (RTE_I2C1_CLK_INPUT_ID == 0)
#define RTE_I2C1_CLK_INPUT_PORT              P1_3
#define RTE_I2C1_CLK_INPUT                   USIC0_C1_DX1_P1_3
#elif  (RTE_I2C1_CLK_INPUT_ID == 1)
#define RTE_I2C1_CLK_INPUT_PORT              P0_8
#define RTE_I2C1_CLK_INPUT                   USIC0_C1_DX1_P0_8
#elif  (RTE_I2C1_CLK_INPUT_ID == 2)
#define RTE_I2C1_CLK_INPUT_PORT              P0_7
#define RTE_I2C1_CLK_INPUT                   USIC0_C1_DX1_P0_7
#elif  (RTE_I2C1_CLK_INPUT_ID == 3)
#define RTE_I2C1_CLK_INPUT_PORT              P2_11
#define RTE_I2C1_CLK_INPUT                   USIC0_C1_DX1_P2_11
#else
#error "Invalid I2C1 CLOCK INPUT Pin Configuration!"
#endif

//   <o> I2C1_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_I2C1_RX_FIFO_SIZE_ID                4

#if    (RTE_I2C1_RX_FIFO_SIZE_ID == 0)
#define RTE_I2C1_RX_FIFO_SIZE         NO_FIFO
#define RTE_I2C1_RX_FIFO_SIZE_NUM     0
#elif  (RTE_I2C1_RX_FIFO_SIZE_ID == 1)
#define RTE_I2C1_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2C1_RX_FIFO_SIZE_NUM     2
#elif  (RTE_I2C1_RX_FIFO_SIZE_ID == 2)
#define RTE_I2C1_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2C1_RX_FIFO_SIZE_NUM     4
#elif  (RTE_I2C1_RX_FIFO_SIZE_ID == 3)
#define RTE_I2C1_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2C1_RX_FIFO_SIZE_NUM     8
#elif  (RTE_I2C1_RX_FIFO_SIZE_ID == 4)
#define RTE_I2C1_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2C1_RX_FIFO_SIZE_NUM     16
#elif  (RTE_I2C1_RX_FIFO_SIZE_ID == 5)
#define RTE_I2C1_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2C1_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2C1_RX FIFO SIZE Configuration!"
#endif

//   <o> I2C1_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
#define RTE_I2C1_TX_FIFO_SIZE_ID                4

#if    (RTE_I2C1_TX_FIFO_SIZE_ID == 0)
#define RTE_I2C1_TX_FIFO_SIZE         NO_FIFO
#define RTE_I2C1_TX_FIFO_SIZE_NUM     0
#elif  (RTE_I2C1_TX_FIFO_SIZE_ID == 1)
#define RTE_I2C1_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2C1_TX_FIFO_SIZE_NUM     2
#elif  (RTE_I2C1_TX_FIFO_SIZE_ID == 2)
#define RTE_I2C1_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2C1_TX_FIFO_SIZE_NUM     4
#elif  (RTE_I2C1_TX_FIFO_SIZE_ID == 3)
#define RTE_I2C1_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2C1_TX_FIFO_SIZE_NUM     8
#elif  (RTE_I2C1_TX_FIFO_SIZE_ID == 4)
#define RTE_I2C1_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2C1_TX_FIFO_SIZE_NUM     16
#elif  (RTE_I2C1_TX_FIFO_SIZE_ID == 5)
#define RTE_I2C1_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2C1_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2C1_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> I2S0 (Inter-Integrated Sound) [Driver_SAI0]
// <i> Configuration settings for Driver_SAI0 in component ::Drivers:SAI
#define RTE_I2S0                      0

//   <o> I2S0_TX Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P1_5 <5=>P2_0 <6=>P2_1
//   <i> Transmit data for I2S0
#define RTE_I2S0_TX_ID                0

#if    (RTE_I2S0_TX_ID == 0)
#define RTE_I2S0_TX_PORT              P0_14
#define RTE_I2S0_TX_AF                P0_14_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 1)
#define RTE_I2S0_TX_PORT              P0_15
#define RTE_I2S0_TX_AF                P0_15_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 2)
#define RTE_I2S0_TX_PORT              P1_0
#define RTE_I2S0_TX_AF                P1_0_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 3)
#define RTE_I2S0_TX_PORT              P1_1
#define RTE_I2S0_TX_AF                P1_1_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 4)
#define RTE_I2S0_TX_PORT              P1_5
#define RTE_I2S0_TX_AF                P1_5_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 5)
#define RTE_I2S0_TX_PORT              P2_0
#define RTE_I2S0_TX_AF                P2_0_AF_U0C0_DOUT0
#elif  (RTE_I2S0_TX_ID == 6)
#define RTE_I2S0_TX_PORT              P2_1
#define RTE_I2S0_TX_AF                P2_1_AF_U0C0_DOUT0
#else
#error "Invalid I2S0_TX Pin Configuration!"
#endif

//   <o> I2S0_RX Pin <0=>P0_14 <1=>P0_15 <2=>P1_0 <3=>P1_1 <4=>P2_0 <5=>P2_1
//   <i> Receive data for I2S0
#define RTE_I2S0_RX_ID                0

#if    (RTE_I2S0_RX_ID == 0)
#define RTE_I2S0_RX_PORT              P0_14
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P0_14
#elif  (RTE_I2S0_RX_ID == 1)
#define RTE_I2S0_RX_PORT              P0_15
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P0_15
#elif  (RTE_I2S0_RX_ID == 2)
#define RTE_I2S0_RX_PORT              P1_0
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P1_0
#elif  (RTE_I2S0_RX_ID == 3)
#define RTE_I2S0_RX_PORT              P1_1
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P1_1
#elif  (RTE_I2S0_RX_ID == 4)
#define RTE_I2S0_RX_PORT              P2_0
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P2_0
#elif  (RTE_I2S0_RX_ID == 5)
#define RTE_I2S0_RX_PORT              P2_1
#define RTE_I2S0_RX_INPUT             USIC0_C0_DX0_P2_1
#else 
#error "Invalid I2S0_RX Pin Configuration!"
#endif

//   <o> I2S0_CLK OUTPUT Pin <0=>P0_7 <1=>P0_8 <2=>P0_14 <4=>P2_0
//   <i> Transmit clock for I2S0
#define RTE_I2S0_CLK_OUTPUT_ID                0

#if    (RTE_I2S0_CLK_OUTPUT_ID == 0)
#define RTE_I2S0_CLK_OUTPUT_PORT              P0_7
#define RTE_I2S0_CLK_AF                       P0_7_AF_U0C0_SCLKOUT
#elif  (RTE_I2S0_CLK_OUTPUT_ID == 1)
#define RTE_I2S0_CLK_OUTPUT_PORT              P0_8
#define RTE_I2S0_CLK_AF                       P0_8_AF_U0C0_SCLKOUT
#elif  (RTE_I2S0_CLK_OUTPUT_ID == 2)
#define RTE_I2S0_CLK_OUTPUT_PORT              P0_14
#define RTE_I2S0_CLK_AF                       P0_14_AF_U0C0_SCLKOUT
#elif  (RTE_I2S0_CLK_OUTPUT_ID == 4)
#define RTE_I2S0_CLK_OUTPUT_PORT              P2_0
#define RTE_I2S0_CLK_AF                       P2_0_AF_U0C0_SCLKOUT
#else 
#error "Invalid I2S0_CLK Output Pin Configuration!"
#endif

//   <o> I2S0_CLK INPUT Pin <0=>P0_7 <1=>P0_8 <2=>P0_14 <3=>P1_1 <4=>P2_0
//   <i> Receive clock for I2S0
#define RTE_I2S0_CLK_INPUT_ID                0

#if    (RTE_I2S0_CLK_INPUT_ID == 0)
#define RTE_I2S0_CLK_INPUT_PORT              P0_7
#define RTE_I2S0_CLK_INPUT                   USIC0_C0_DX1_P0_7
#elif  (RTE_I2S0_CLK_INPUT_ID == 1)
#define RTE_I2S0_CLK_INPUT_PORT              P0_8
#define RTE_I2S0_CLK_INPUT                   USIC0_C0_DX1_P0_8
#elif  (RTE_I2S0_CLK_INPUT_ID == 2)
#define RTE_I2S0_CLK_INPUT_PORT              P0_14
#define RTE_I2S0_CLK_INPUT                   USIC0_C0_DX1_P0_14
#elif  (RTE_I2S0_CLK_INPUT_ID == 3)
#define RTE_I2S0_CLK_INPUT_PORT              P1_1
#define RTE_I2S0_CLK_INPUT                   USIC0_C0_DX1_P1_1
#elif  (RTE_I2S0_CLK_INPUT_ID == 4)
#define RTE_I2S0_CLK_INPUT_PORT              P2_0
#define RTE_I2S0_CLK_INPUT                   USIC0_C0_DX1_P2_0
#else 
#error "Invalid I2S0_CLK Input Pin Configuration!"
#endif

//   <o> I2S0_WS OUTPUT Pin <0=>P0_0 <1=>P0_9 <2=>P1_4
//   <i> Transmit word select for I2S0
#define RTE_I2S0_WS_OUTPUT_ID                0

#if     (RTE_I2S0_WS_OUTPUT_ID == 0)
#define RTE_I2S0_WS_OUTPUT_PORT              P0_0
#define RTE_I2S0_WS_AF                       P0_0_AF_U0C0_SELO0
#elif   (RTE_I2S0_WS_OUTPUT_ID == 1)
#define RTE_I2S0_WS_OUTPUT_PORT              P0_9
#define RTE_I2S0_WS_AF                       P0_9_AF_U0C0_SELO0
#elif   (RTE_I2S0_WS_OUTPUT_ID == 2)
#define RTE_I2S0_WS_OUTPUT_PORT              P1_4
#define RTE_I2S0_WS_AF                       P1_4_AF_U0C0_SELO0
#else 
#error "Invalid I2S0_WS Output Pin Configuration!"
#endif

//   <o> I2S0_WS INPUT Pin <0=>P0_0 <1=>P0_9 <2=>P0_10 <3=>P0_11 <4=>P0_12 <5=>P0_13
//   <i> Receive word select for I2S0
#define RTE_I2S0_WS_INPUT_ID                 0

#if    (RTE_I2S0_WS_INPUT_ID == 0)
#define RTE_I2S0_WS_INPUT_PORT               P0_0
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_0
#elif  (RTE_I2S0_WS_INPUT_ID == 1)
#define RTE_I2S0_WS_INPUT_PORT               P0_9
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_9
#elif  (RTE_I2S0_WS_INPUT_ID == 2)
#define RTE_I2S0_WS_INPUT_PORT               P0_10
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_10
#elif  (RTE_I2S0_WS_INPUT_ID == 3)
#define RTE_I2S0_WS_INPUT_PORT               P0_11
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_11
#elif  (RTE_I2S0_WS_INPUT_ID == 4)
#define RTE_I2S0_WS_INPUT_PORT               P0_12
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_12
#elif  (RTE_I2S0_WS_INPUT_ID == 5)
#define RTE_I2S0_WS_INPUT_PORT               P0_13
#define RTE_I2S0_WS_INPUT_ID_INPUT           USIC0_C0_DX2_P0_13
#else
#error "Invalid I2S0_WS Input Pin Configuration!"
#endif

// <e> Enable MCLK output
#define RTE_I2S0_MCLK_OUTPUT_ENABLE          0
//   <o> I2S0_MCLK OUTPUT Pin <0=>P0_11
#define RTE_I2S0_MCLK_OUTPUT_ID              0
#if    (RTE_I2S0_MCLK_OUTPUT_ID == 0)
#define RTE_I2S0_MCLK_OUTPUT_PORT            P0_11
#define RTE_I2S0_MCLK_AF                     P0_11_AF_U0C0_MCLKOUT
#else
#error "Invalid I2S0_MCLK Output Pin Configuration!"
#endif
// </e>

//   <o> I2S0_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
//   <i> Receive FIFO size for I2S0 
#define RTE_I2S0_RX_FIFO_SIZE_ID                4

#if    (RTE_I2S0_RX_FIFO_SIZE_ID == 0)
#define RTE_I2S0_RX_FIFO_SIZE         NO_FIFO
#define RTE_I2S0_RX_FIFO_SIZE_NUM     0
#elif  (RTE_I2S0_RX_FIFO_SIZE_ID == 1)
#define RTE_I2S0_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2S0_RX_FIFO_SIZE_NUM     2
#elif  (RTE_I2S0_RX_FIFO_SIZE_ID == 2)
#define RTE_I2S0_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2S0_RX_FIFO_SIZE_NUM     4
#elif  (RTE_I2S0_RX_FIFO_SIZE_ID == 3)
#define RTE_I2S0_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2S0_RX_FIFO_SIZE_NUM     8
#elif  (RTE_I2S0_RX_FIFO_SIZE_ID == 4)
#define RTE_I2S0_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2S0_RX_FIFO_SIZE_NUM     16
#elif  (RTE_I2S0_RX_FIFO_SIZE_ID == 5)
#define RTE_I2S0_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2S0_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2S0_RX FIFO SIZE Configuration!"
#endif

//   <o> I2S0_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
//   <i> Transmit FIFO size for I2S0 
#define RTE_I2S0_TX_FIFO_SIZE_ID                4

#if    (RTE_I2S0_TX_FIFO_SIZE_ID == 0)
#define RTE_I2S0_TX_FIFO_SIZE         NO_FIFO
#define RTE_I2S0_TX_FIFO_SIZE_NUM     0
#elif  (RTE_I2S0_TX_FIFO_SIZE_ID == 1)
#define RTE_I2S0_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2S0_TX_FIFO_SIZE_NUM     2
#elif  (RTE_I2S0_TX_FIFO_SIZE_ID == 2)
#define RTE_I2S0_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2S0_TX_FIFO_SIZE_NUM     4
#elif  (RTE_I2S0_TX_FIFO_SIZE_ID == 3)
#define RTE_I2S0_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2S0_TX_FIFO_SIZE_NUM     8
#elif  (RTE_I2S0_TX_FIFO_SIZE_ID == 4)
#define RTE_I2S0_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2S0_TX_FIFO_SIZE_NUM     16
#elif  (RTE_I2S0_TX_FIFO_SIZE_ID == 5)
#define RTE_I2S0_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2S0_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2S0_TX FIFO SIZE Configuration!"
#endif
//</e>

// <e> I2S1 (Inter-Integrated Sound) [Driver_I2S1]
// <i> Configuration settings for Driver_I2S1 in component ::Drivers:I2S
#define RTE_I2S1                      0

//   <o> I2S1_TX Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <5=>P2_10 <6=>P2_11
//   <i> Transmit data for I2S1
#define RTE_I2S1_TX_ID                0

#if    (RTE_I2S1_TX_ID == 0)
#define RTE_I2S1_TX_PORT              P0_6
#define RTE_I2S1_TX_AF                P0_6_AF_U0C1_DOUT0
#elif  (RTE_I2S1_TX_ID == 1)
#define RTE_I2S1_TX_PORT              P0_7
#define RTE_I2S1_TX_AF                P0_7_AF_U0C1_DOUT0
#elif  (RTE_I2S1_TX_ID == 2)
#define RTE_I2S1_TX_PORT              P1_2
#define RTE_I2S1_TX_AF                P1_2_AF_U0C1_DOUT0
#elif  (RTE_I2S1_TX_ID == 3)
#define RTE_I2S1_TX_PORT              P1_3
#define RTE_I2S1_TX_AF                P1_3_AF_U0C1_DOUT0
#elif  (RTE_I2S1_TX_ID == 5)
#define RTE_I2S1_TX_PORT              P2_10
#define RTE_I2S1_TX_AF                P2_10_AF_U0C1_DOUT0
#elif  (RTE_I2S1_TX_ID == 6)
#define RTE_I2S1_TX_PORT              P2_11
#define RTE_I2S1_TX_AF                P2_11_AF_U0C1_DOUT0
#else
#error "Invalid I2S1_TX Pin Configuration!"
#endif

//   <o> I2S1_RX Pin <0=>P0_6 <1=>P0_7 <2=>P1_2 <3=>P1_3 <4=>P2_10 <5=>P2_11
//   <i> Receive data for I2S1
#define RTE_I2S1_RX_ID                0

#if    (RTE_I2S1_RX_ID == 0)
#define RTE_I2S1_RX_PORT              P0_6
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P0_6
#elif  (RTE_I2S1_RX_ID == 1)
#define RTE_I2S1_RX_PORT              P0_7
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P0_7
#elif  (RTE_I2S1_RX_ID == 2)
#define RTE_I2S1_RX_PORT              P1_2
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P1_2
#elif  (RTE_I2S1_RX_ID == 3)
#define RTE_I2S1_RX_PORT              P1_3
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P1_3
#elif  (RTE_I2S1_RX_ID == 4)
#define RTE_I2S1_RX_PORT              P2_10
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P2_10
#elif  (RTE_I2S1_RX_ID == 5)
#define RTE_I2S1_RX_PORT              P2_11
#define RTE_I2S1_RX_INPUT             USIC0_C1_DX0_P2_11
#else
#error "Invalid I2S1_RX Pin Configuration!"
#endif

//   <o> I2S1_CLK OUTPUT Pin <0=>P0_8 <1=>P1_3 <2=>P1_4 <3=>P2_1 <4=>P2_11
//   <i> Transmit clock for I2S1
#define RTE_I2S1_CLK_OUTPUT_ID                0

#if    (RTE_I2S1_CLK_OUTPUT_ID == 0)
#define RTE_I2S1_CLK_OUTPUT_PORT              P0_8
#define RTE_I2S1_CLK_AF                       P0_8_AF_U0C1_SCLKOUT
#elif  (RTE_I2S1_CLK_OUTPUT_ID == 1)
#define RTE_I2S1_CLK_OUTPUT_PORT              P1_3
#define RTE_I2S1_CLK_AF                       P1_3_AF_U0C1_SCLKOUT
#elif  (RTE_I2S1_CLK_OUTPUT_ID == 2)
#define RTE_I2S1_CLK_OUTPUT_PORT              P1_4
#define RTE_I2S1_CLK_AF                       P1_4_AF_U0C1_SCLKOUT
#elif  (RTE_I2S1_CLK_OUTPUT_ID == 3)
#define RTE_I2S1_CLK_OUTPUT_PORT              P2_1
#define RTE_I2S1_CLK_AF                       P2_1_AF_U0C1_SCLKOUT
#elif  (RTE_I2S1_CLK_OUTPUT_ID == 4)
#define RTE_I2S1_CLK_OUTPUT_PORT              P2_11
#define RTE_I2S1_CLK_AF                       P2_11_AF_U0C1_SCLKOUT
#else
#error "Invalid I2S1 CLOCK OUTPUT Pin Configuration!"
#endif

//   <o> I2S1_CLK INPUT Pin <0=>P0_7 <1=>P0_8 <2=>P1_3 <3=>P2_11
//   <i> Receive clock for I2S1
#define RTE_I2S1_CLK_INPUT_ID                0

#if    (RTE_I2S1_CLK_INPUT_ID == 0)
#define RTE_I2S1_CLK_INPUT_PORT              P0_7
#define RTE_I2S1_CLK_INPUT                   USIC0_C1_DX1_P0_7
#elif  (RTE_I2S1_CLK_INPUT_ID == 1)
#define RTE_I2S1_CLK_INPUT_PORT              P0_8
#define RTE_I2S1_CLK_INPUT                   USIC0_C1_DX1_P0_8
#elif  (RTE_I2S1_CLK_INPUT_ID == 2)
#define RTE_I2S1_CLK_INPUT_PORT              P1_3
#define RTE_I2S1_CLK_INPUT                   USIC0_C1_DX1_P1_3
#elif  (RTE_I2S1_CLK_INPUT_ID == 3)
#define RTE_I2S1_CLK_INPUT_PORT              P2_11
#define RTE_I2S1_CLK_INPUT                   USIC0_C1_DX1_P2_11
#else
#error "Invalid I2S1 CLOCK INPUT Pin Configuration!"
#endif

//   <o> I2S1_WS OUTPUT Pin <0=>P0_0 <1=>P0_9 <2=>P1_1
//   <i> Transmit word select for I2S1
#define RTE_I2S1_WS_OUTPUT_ID                0

#if     (RTE_I2S1_WS_OUTPUT_ID == 0)
#define RTE_I2S1_WS_OUTPUT_PORT              P0_0
#define RTE_I2S1_WS_AF                       P0_0_AF_U0C1_SELO0
#elif   (RTE_I2S1_WS_OUTPUT_ID == 1)
#define RTE_I2S1_WS_OUTPUT_PORT              P0_9
#define RTE_I2S1_WS_AF                       P0_9_AF_U0C1_SELO0
#elif   (RTE_I2S1_WS_OUTPUT_ID == 2)
#define RTE_I2S1_WS_OUTPUT_PORT              P1_1
#define RTE_I2S1_WS_AF                       P1_1_AF_U0C1_SELO0
#else
#error "Invalid I2S1_WS Output Pin Configuration!"
#endif

//   <o> I2S1_WS INPUT Pin <0=>P0_0 <1=>P0_9 <2=>P0_10 <3=>P0_11 <4=>P1_1 <5=>P2_0
//   <i> Receive word select for I2S1
#define RTE_I2S1_WS_INPUT_ID                 0

#if    (RTE_I2S1_WS_INPUT_ID == 0)
#define RTE_I2S1_WS_INPUT_PORT               P0_0
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P0_0
#elif  (RTE_I2S1_WS_INPUT_ID == 1)
#define RTE_I2S1_WS_INPUT_PORT               P0_9
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P0_9
#elif  (RTE_I2S1_WS_INPUT_ID == 2)
#define RTE_I2S1_WS_INPUT_PORT               P0_10
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P0_10
#elif  (RTE_I2S1_WS_INPUT_ID == 3)
#define RTE_I2S1_WS_INPUT_PORT               P0_11
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P0_11
#elif  (RTE_I2S1_WS_INPUT_ID == 4)
#define RTE_I2S1_WS_INPUT_PORT               P1_1
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P1_1
#elif  (RTE_I2S1_WS_INPUT_ID == 5)
#define RTE_I2S1_WS_INPUT_PORT               P2_0
#define RTE_I2S1_WS_INPUT_ID_INPUT           USIC0_C1_DX2_P2_0
#else
#error "Invalid I2S1_WS Input Pin Configuration!"
#endif

// <e> Enable MCLK output
#define RTE_I2S1_MCLK_OUTPUT_ENABLE          0
//   <o> I2S0_MCLK OUTPUT Pin <0=>P0_6 <1=>P0_15
#define RTE_I2S1_MCLK_OUTPUT_ID              0
#if    (RTE_I2S1_MCLK_OUTPUT_ID == 0)
#define RTE_I2S1_MCLK_OUTPUT_PORT            P0_6
#define RTE_I2S1_MCLK_AF                     P0_6_AF_U0C1_MCLKOUT
#elif  (RTE_I2S1_MCLK_OUTPUT_ID == 1)
#define RTE_I2S1_MCLK_OUTPUT_PORT            P0_15
#define RTE_I2S1_MCLK_AF                     P0_15_AF_U0C1_MCLKOUT
#else
#error "Invalid I2S1_MCLK Output Pin Configuration!"
#endif
// </e>

//   <o> I2S1_RX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
//   <i> Receive FIFO size for I2S1
#define RTE_I2S1_RX_FIFO_SIZE_ID                4

#if    (RTE_I2S1_RX_FIFO_SIZE_ID == 0)
#define RTE_I2S1_RX_FIFO_SIZE         NO_FIFO
#define RTE_I2S1_RX_FIFO_SIZE_NUM     0
#elif  (RTE_I2S1_RX_FIFO_SIZE_ID == 1)
#define RTE_I2S1_RX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2S1_RX_FIFO_SIZE_NUM     2
#elif  (RTE_I2S1_RX_FIFO_SIZE_ID == 2)
#define RTE_I2S1_RX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2S1_RX_FIFO_SIZE_NUM     4
#elif  (RTE_I2S1_RX_FIFO_SIZE_ID == 3)
#define RTE_I2S1_RX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2S1_RX_FIFO_SIZE_NUM     8
#elif  (RTE_I2S1_RX_FIFO_SIZE_ID == 4)
#define RTE_I2S1_RX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2S1_RX_FIFO_SIZE_NUM     16
#elif  (RTE_I2S1_RX_FIFO_SIZE_ID == 5)
#define RTE_I2S1_RX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2S1_RX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2S1_RX FIFO SIZE Configuration!"
#endif

//   <o> I2S1_TX FIFO ENTRIES <0=>0 <1=>2 <2=>4 <3=>8 <4=>16 <5=>32
//   <i> Transmit FIFO size for I2S1
#define RTE_I2S1_TX_FIFO_SIZE_ID                4

#if    (RTE_I2S1_TX_FIFO_SIZE_ID == 0)
#define RTE_I2S1_TX_FIFO_SIZE         NO_FIFO
#define RTE_I2S1_TX_FIFO_SIZE_NUM     0
#elif  (RTE_I2S1_TX_FIFO_SIZE_ID == 1)
#define RTE_I2S1_TX_FIFO_SIZE         FIFO_SIZE_2
#define RTE_I2S1_TX_FIFO_SIZE_NUM     2
#elif  (RTE_I2S1_TX_FIFO_SIZE_ID == 2)
#define RTE_I2S1_TX_FIFO_SIZE         FIFO_SIZE_4
#define RTE_I2S1_TX_FIFO_SIZE_NUM     4
#elif  (RTE_I2S1_TX_FIFO_SIZE_ID == 3)
#define RTE_I2S1_TX_FIFO_SIZE         FIFO_SIZE_8
#define RTE_I2S1_TX_FIFO_SIZE_NUM     8
#elif  (RTE_I2S1_TX_FIFO_SIZE_ID == 4)
#define RTE_I2S1_TX_FIFO_SIZE         FIFO_SIZE_16
#define RTE_I2S1_TX_FIFO_SIZE_NUM     16
#elif  (RTE_I2S1_TX_FIFO_SIZE_ID == 5)
#define RTE_I2S1_TX_FIFO_SIZE         FIFO_SIZE_32
#define RTE_I2S1_TX_FIFO_SIZE_NUM     32
#else
#error "Invalid I2S1_TX FIFO SIZE Configuration!"
#endif
//</e>

#if ((RTE_UART0+RTE_I2C0+RTE_SPI0+RTE_I2S0)>1)
#error "Choose just one Driver_I2C/SPI/UART0/SAI0 driver !"
#elif ((RTE_UART1+RTE_I2C1+RTE_SPI1+RTE_I2S1)>1)
#error "Choose just one Driver_I2C/SPI/UART1/SAI1 driver !"
#endif


#if (((RTE_UART0_RX_FIFO_SIZE_NUM*RTE_UART0)+\
      (RTE_I2C0_RX_FIFO_SIZE_NUM*RTE_I2C0)+\
      (RTE_SPI0_RX_FIFO_SIZE_NUM*RTE_SPI0)+\
      (RTE_I2S0_RX_FIFO_SIZE_NUM*RTE_I2S0)+\
      (RTE_UART1_RX_FIFO_SIZE_NUM*RTE_UART1)+\
      (RTE_I2C1_RX_FIFO_SIZE_NUM*RTE_I2C1)+\
      (RTE_SPI1_RX_FIFO_SIZE_NUM*RTE_SPI1)+\
      (RTE_I2S1_RX_FIFO_SIZE_NUM*RTE_I2S1)+\
      (RTE_UART0_TX_FIFO_SIZE_NUM*RTE_UART0)+\
      (RTE_I2C0_TX_FIFO_SIZE_NUM*RTE_I2C0)+\
      (RTE_SPI0_TX_FIFO_SIZE_NUM*RTE_SPI0)+\
      (RTE_I2S0_TX_FIFO_SIZE_NUM*RTE_I2S0)+\
      (RTE_UART1_TX_FIFO_SIZE_NUM*RTE_UART1)+\
      (RTE_I2C1_TX_FIFO_SIZE_NUM*RTE_I2C1)+\
      (RTE_SPI1_TX_FIFO_SIZE_NUM*RTE_SPI1)+\
      (RTE_I2S1_TX_FIFO_SIZE_NUM*RTE_I2S1))>64)
#error "Just 64 positions in a FIFO shared by RTE_x0 and RTE_x1 !"
#endif

#endif  /* __RTE_DEVICE_H */
