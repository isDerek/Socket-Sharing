/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_usart.h"

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_USART USART0
#define DEMO_USART9 USART9	//added by derek 2017.10.24
#define DEMO_USART4 USART4	//added by derek 2017.10.24
#define DEMO_USART_CLK_SRC kCLOCK_Flexcomm0
#define DEMO_USART_CLK_FREQ CLOCK_GetFreq(kCLOCK_Flexcomm0)
#define DEMO_USART_CLK_FREQ4 CLOCK_GetFreq(kCLOCK_Flexcomm4) 	//added by derek 2017.10.24
#define DEMO_USART_CLK_FREQ9 CLOCK_GetFreq(kCLOCK_Flexcomm9)	//added by derek 2017.10.24
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler 
#define DEMO_USART_IRQHandler4 FLEXCOMM4_IRQHandler  //added by derek 2017.10.24
#define DEMO_USART_IRQHandler9 FLEXCOMM9_IRQHandler  //added by derek 2017.10.24
#define DEMO_USART_IRQn FLEXCOMM0_IRQn
#define DEMO_USART_IRQn4 FLEXCOMM4_IRQn		//added by derek 2017.10.24
#define DEMO_USART_IRQn9 FLEXCOMM9_IRQn   //added by derek 2017.10.24

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Usart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_USART_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(DEMO_USART))
    {
        data = USART_ReadByte(DEMO_USART);
        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
            demoRingBuffer[rxIndex] = data;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    usart_config_t config;

    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH4);  // ADDED BY DEREK 2017.10.24
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH9);  // ADDED BY DEREK 2017.10.24
    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.loopback = false;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);

    /* Send g_tipString out. */
    USART_WriteBlocking(DEMO_USART, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]));

    /* Enable RX interrupt. */
    USART_EnableInterrupts(DEMO_USART, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    EnableIRQ(DEMO_USART_IRQn);

    while (1)
    {
        /* Send data only when USART TX register is empty and ring buffer has data to send out. */
        while ((kUSART_TxFifoNotFullFlag & USART_GetStatusFlags(DEMO_USART)) && (rxIndex != txIndex))
        {
            USART_WriteByte(DEMO_USART, demoRingBuffer[txIndex]);
            txIndex++;
            txIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}
