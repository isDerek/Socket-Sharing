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

#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_flashiap.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_iocon.h"


/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/



#define IOCON_PIO_DIGITAL_EN        0x0100u   /*!< Enables digital function */
#define IOCON_PIO_FUNC1               0x01u   /*!< Selects pin function 1 */
#define IOCON_PIO_INPFILT_OFF       0x0200u   /*!< Input filter disabled */
#define IOCON_PIO_INV_DI              0x00u   /*!< Input function is not inverted */
#define IOCON_PIO_MODE_INACT          0x00u   /*!< No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI        0x00u   /*!< Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD       0x00u   /*!< Standard mode, output slew rate control is enabled */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port 0 */
#define PIN30_IDX                       30u   /*!< Pin number for pin 30 in a port 0 */
#define PORT0_IDX                        0u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins_Core0:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins_Core0
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins_Core0(void) {
  CLOCK_EnableClock(kCLOCK_Iocon);                           /* Enables the clock for the IOCON block. 0 = Disable; 1 = Enable.: 0x01u */

  const uint32_t port0_pin29_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC0_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN29_IDX, port0_pin29_config); /* PORT0 PIN29 (coords: B13) is configured as FC0_RXD_SDA_MOSI */
  const uint32_t port0_pin30_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC0_TXD_SCL_MISO */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN30_IDX, port0_pin30_config); /* PORT0 PIN30 (coords: A2) is configured as FC0_TXD_SCL_MISO */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t page_buf[FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES / sizeof(uint32_t)];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief Main function
*/
int main(void)
{
    uint32_t i, status;

    /* Board pin, clock, debug console init */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins_Core0();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();

    /* Clear screen*/
    PRINTF("%c[2J", 27);
    /* Set cursor location at [0,0] */
    PRINTF("%c[0;0H", 27);
    PRINTF("\f\r\nFLASHIAP example\r\n");
    PRINTF("\r\nWriting flash sector 1\n");
    /* Erase sector before writing */
    FLASHIAP_PrepareSectorForWrite(1, 1);
    FLASHIAP_EraseSector(1, 1, SystemCoreClock);
    /* Generate data to be written to flash */
    for (i = 0; i < FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES; i++)
    {
        *(((uint8_t *)(&page_buf[0])) + i) = i;
    }
    /* Program sector */
    for (i = 0; i < (FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES); i++)
    {
        FLASHIAP_PrepareSectorForWrite(1, 1);
        FLASHIAP_CopyRamToFlash(
            FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES + (i * FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES), &page_buf[0],
            FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
    }
    /* Verify sector contents */
    for (i = 0; i < (FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES); i++)
    {
        status = FLASHIAP_Compare(
            FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES + (i * FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES), &page_buf[0],
            FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES);

        if (status != kStatus_FLASHIAP_Success)
        {
            PRINTF("\r\nSector Verify failed\n");
            break;
        }
    }

    PRINTF("\r\nErasing flash sector 1\n");
    FLASHIAP_PrepareSectorForWrite(1, 1);
    FLASHIAP_EraseSector(1, 1, SystemCoreClock);
    status = FLASHIAP_BlankCheckSector(1, 1);
    if (status != kStatus_FLASHIAP_Success)
    {
        PRINTF("\r\nSector Erase failed");
    }

    PRINTF("\r\nErasing page 1 in flash sector 1\n");
    /* First write a page */
    FLASHIAP_PrepareSectorForWrite(1, 1);
    FLASHIAP_CopyRamToFlash(FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES, &page_buf[0],
                            FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES, SystemCoreClock);
    /* Erase page */
    FLASHIAP_PrepareSectorForWrite(1, 1);
    FLASHIAP_ErasePage(FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES,
                       FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES / FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES,
                       SystemCoreClock);

    /* Fill page buffer with erased state value */
    for (i = 0; i < FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES; i++)
    {
        *(((uint8_t *)(&page_buf[0])) + i) = 0xFF;
    }
    /* Verify Erase */
    status = FLASHIAP_Compare(FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES, (uint32_t *)(&page_buf[0]),
                              FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES);
    if (status != kStatus_FLASHIAP_Success)
    {
        PRINTF("\r\nPage Erase failed\n");
    }
    PRINTF("\r\nEnd of FLASHIAP Example \r\n");
    while (1)
    {
    }
}
