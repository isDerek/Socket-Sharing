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

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'LPC54608J512'
- !!package 'LPC54608J512ET180'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.0.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

#define IOCON_PIO_DIGITAL_ADC       0x0000u   /*!< Enables digital function as ADC*/
#define IOCON_PIO_DIGITAL_EN        0x0100u   /*!< Enables digital function */
#define IOCON_PIO_FUNC0               0x00u   /*!< Selects pin function 0 */
#define IOCON_PIO_FUNC1               0x01u   /*!< Selects pin function 1 */
#define IOCON_PIO_FUNC4               0x04u   /*!< Selects pin function 4 */
#define IOCON_PIO_FUNC5               0x05u   /*!< Selects pin function 5 */
#define IOCON_PIO_FUNC3               0x03u   /*!< Selects pin function 3 */
#define IOCON_PIO_FUNC6               0x06u   /*!< Selects pin function 6 */
#define IOCON_PIO_FUNC7               0x07u   /*!< Selects pin function 7 */
#define IOCON_PIO_INPFILT_OFF       0x0200u   /*!< Input filter disabled */
#define IOCON_PIO_INV_DI              0x00u   /*!< Input function is not inverted */
#define IOCON_PIO_MODE_INACT          0x00u   /*!< No addition pin function */
#define IOCON_PIO_MODE_PULLUP         0x20u   /*!< Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI        0x00u   /*!< Open drain is disabled */
#define IOCON_PIO_SLEW_FAST         0x0400u   /*!< Fast mode, slew rate control is disabled */
#define IOCON_PIO_SLEW_STANDARD       0x00u   /*!< Standard mode, output slew rate control is enabled */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port 1 */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port 0 */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port 1 */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port 1 */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port 0 */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port 4 */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port 0 */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port 0 */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port 4 */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port 4 */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port 4 */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port 4 */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port 4 */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port 4 */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port 0 */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port 0 */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port 0 */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port 3 */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port 3 */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port 0 */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port 0 */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port 0 */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port 2 */
#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port 0 */
#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port 0 */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port 0 */
#define PIN30_IDX                       30u   /*!< Pin number for pin 30 in a port 0 */
#define PIN31_IDX                       31u   /*!< Pin number for pin 30 in a port 0 */
#define PORT0_IDX                        0u   /*!< Port index */
#define PORT1_IDX                        1u   /*!< Port index */
#define PORT2_IDX                        2u   /*!< Port index */
#define PORT3_IDX                        3u   /*!< Port index */
#define PORT4_IDX                        4u   /*!< Port index */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: B13, peripheral: FLEXCOMM0, signal: RXD_SDA_MOSI, pin_signal: PIO0_29/FC0_RXD_SDA_MOSI/CTIMER2_MAT3/SCT0_OUT8/TRACEDATA(2), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: fast, open_drain: disabled}
  - {pin_num: A2, peripheral: FLEXCOMM0, signal: TXD_SCL_MISO, pin_signal: PIO0_30/FC0_TXD_SCL_MISO/CTIMER0_MAT0/SCT0_OUT9/TRACEDATA(1), mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: fast, open_drain: disabled}
  - {pin_num: B14, peripheral: ENET, signal: 'ENET_TXD, 0', pin_signal: PIO4_8/ENET_TXD0/FC2_SCK/USB0_OVERCURRENTN/USB0_UP_LED/SCT0_GPI1, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: B9, peripheral: ENET, signal: ENET_RX_DV, pin_signal: PIO4_10/ENET_RX_DV/FC2_TXD_SCL_MISO/USB1_OVERCURRENTN/USB1_UP_LED/SCT0_GPI3, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A9, peripheral: ENET, signal: 'ENET_RXD, 0', pin_signal: PIO4_11/ENET_RXD0/FC2_CTS_SDA_SSEL0/USB0_IDVALUE/SCT0_GPI4, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: A6, peripheral: ENET, signal: 'ENET_RXD, 1', pin_signal: PIO4_12/ENET_RXD1/FC2_RTS_SCL_SSEL1/SCT0_GPI5, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: B6, peripheral: ENET, signal: ENET_TX_EN, pin_signal: PIO4_13/ENET_TX_EN/CTIMER4_MAT0/SCT0_GPI6, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: B5, peripheral: ENET, signal: ENET_RX_CLK, pin_signal: PIO4_14/ENET_RX_CLK/CTIMER4_MAT1/FC9_SCK/SCT0_GPI7, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: A4, peripheral: ENET, signal: ENET_MDC, pin_signal: PIO4_15/ENET_MDC/CTIMER4_MAT2/FC9_RXD_SDA_MOSI, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: C4, peripheral: ENET, signal: ENET_MDIO, pin_signal: PIO4_16/ENET_MDIO/CTIMER4_MAT3/FC9_TXD_SCL_MISO, mode: inactive, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: E14, peripheral: ENET, signal: 'ENET_TXD, 1', pin_signal: PIO0_17/FC4_SSEL2/SD_CARD_DET_N/SCT0_GPI7/SCT0_OUT0/EMC_OEN/ENET_TXD1, mode: inactive, invert: disabled,
    glitch_filter: disabled, slew_rate: standard, open_drain: disabled}
  - {pin_num: H11, peripheral: GPIO, signal: 'PIO2, 26', pin_signal: PIO2_26/LCD_VD(8)/FC3_SCK/CTIMER2_CAP1, mode: pullUp, invert: disabled, glitch_filter: disabled,
    slew_rate: standard, open_drain: disabled}
  - {pin_num: P2, peripheral: SWD, signal: SWO, pin_signal: PIO0_10/FC6_SCK/CTIMER2_CAP2/CTIMER2_MAT0/FC1_TXD_SCL_MISO/SWO/ADC0_0, mode: inactive, invert: disabled,
    glitch_filter: disabled, open_drain: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  const uint32_t port1_pin10_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_TXD1 */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN10_IDX, port1_pin10_config); /* PORT1 PIN10 (coords: E14) is configured as ENET_TXD1 */
  const uint32_t port1_pin3_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO2_26 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN3_IDX, port1_pin3_config); /* PORT1 PIN3 (coords: H11) is configured as PIO1_3 */
  const uint32_t port1_pin14_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_RX_DV */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN14_IDX, port1_pin14_config); /* PORT1 PIN14 (coords: B9) is configured as ENET_RX_DV */
  const uint32_t port1_pin12_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_RXD0 */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN12_IDX, port1_pin12_config); /* PORT1 PIN12 (coords: A9) is configured as ENET_RXD0 */
  const uint32_t port1_pin13_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_RXD1 */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN13_IDX, port1_pin13_config); /* PORT1 PIN13 (coords: A6) is configured as ENET_RXD1 */
  const uint32_t port0_pin15_config = (
    IOCON_PIO_FUNC7 |                                        /* Pin is configured as ENET_TX_EN */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN15_IDX, port0_pin15_config); /* PORT0 PIN15 (coords: B6) is configured as ENET_TX_EN */
  const uint32_t port0_pin7_config = (
    IOCON_PIO_FUNC7 |                                        /* Pin is configured as ENET_RX_CLK */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN7_IDX, port0_pin7_config); /* PORT0 PIN17 (coords: B5) is configured as ENET_RX_CLK */
  const uint32_t port1_pin16_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_MDC */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN16_IDX, port1_pin16_config); /* PORT1 PIN16 (coords: A4) is configured as ENET_MDC */
  const uint32_t port1_pin23_config = (
    IOCON_PIO_FUNC4 |                                        /* Pin is configured as ENET_MDIO */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN23_IDX, port1_pin23_config); /* PORT1 PIN23 (coords: C4) is configured as ENET_MDIO */
  const uint32_t port1_pin9_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as ENET_TXD0 */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN9_IDX, port1_pin9_config); /* PORT1 PIN9 (coords: B14) is configured as ENET_TXD0 */
	
	
/*************************** Usart  Gpio****************************************************/	
	const uint32_t port1_pin24_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC4_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN24_IDX, port1_pin24_config); /* PORT1 PIN24 (coords: B13) is configured as FC2_RXD_SDA_MOSI */  
	const uint32_t port1_pin25_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC4_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN25_IDX, port1_pin25_config); /* PORT1 PIN25 (coords: B13) is configured as FC2_TXD_SDA_MOSI */  
	const uint32_t port1_pin21_config = (
    IOCON_PIO_FUNC5 |                                        /* Pin is configured as FC4_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN21_IDX, port1_pin21_config); /* PORT1 PIN21 (coords: B13) is configured as FC4_RXD_SDA_MOSI */  
	const uint32_t port0_pin16_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as FC4_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN16_IDX, port0_pin16_config); /* PORT0 PIN16 (coords: B13) is configured as FC4_TXD_SDA_MOSI */  
//  const uint32_t port0_pin8_config = (
//    IOCON_PIO_FUNC3 |                                        /* Pin is configured as FC4_RXD_SDA_MOSI */
//    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
//    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
//    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
//    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
//    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
//    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
//  );
//  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN8_IDX, port0_pin8_config); /* PORT0 PIN8 (coords: B13) is configured as FC5_RXD_SDA_MOSI */  
//	const uint32_t port0_pin9_config = (
//    IOCON_PIO_FUNC3 |                                        /* Pin is configured as FC4_TXD_SDA_MOSI */
//    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
//    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
//    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
//    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
//    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
//    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
//  );
//  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN9_IDX, port0_pin9_config); /* PORT0 PIN9 (coords: B13) is configured as FC5_TXD_SDA_MOSI */
	const uint32_t port0_pin20_config = (
    IOCON_PIO_FUNC7 |                                        /* Pin is configured as FC1_RXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN20_IDX, port0_pin20_config); /* PORT3 PIN21 (coords: B13) is configured as FC7_RXD_SDA_MOSI */  
	const uint32_t port0_pin19_config = (
    IOCON_PIO_FUNC7 |                                        /* Pin is configured as FC1_TXD_SDA_MOSI */
    IOCON_PIO_MODE_INACT |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN19_IDX, port0_pin19_config); /* PORT0 PIN19 (coords: B13) is configured as FC7_TXD_SDA_MOSI */
	
/******************************************************************************************************************/	
	
	
	
/**********************************************I2C GPIO***********************************************************/
	const uint32_t port1_pin5_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as PIO1_5 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN5_IDX, port1_pin5_config); /* PORT1 PIN5 (coords: E7) is configured as PIO1_5_I2C_SDA */
	const uint32_t port0_pin30_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as PIO0_30 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN30_IDX, port0_pin30_config); /* PORT0 PIN30 (coords: E7) is configured as PIO0_30_I2C_CLK */
	const uint32_t port0_pin13_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as PIO0_13 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN13_IDX, port0_pin13_config); /* PORT0 PIN13 (coords: E7) is configured as PIO0_13_I2C_SDA */
	const uint32_t port0_pin14_config = (
    IOCON_PIO_FUNC1 |                                        /* Pin is configured as PIO0_26 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN14_IDX, port0_pin14_config); /* PORT0 PIN14 (coords: E7) is configured as PIO0_14_I2C_CLK */
	/***************************************************************************************************************/
	
	
  /***********************************************PWM GPIO********************************************************/		
	const uint32_t port0_pin17_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO0_25 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN17_IDX, port0_pin17_config); /* PORT0 PIN17 (coords: E7) is configured as PIO0_17_PWM /PIO0_17*/
	/****************************************************************************************************************/
	/***********************************************ADC GPIO******************************************************/	
  const uint32_t port0_pin10_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
		IOCON_PIO_DIGITAL_EN |                                  /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN10_IDX, port0_pin10_config); /* PORT0 PIN10 (coords: P2) is configured as ADC0/PIO0_10 */	
	const uint32_t port1_pin0_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO1_0 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN0_IDX, port1_pin0_config); /* PORT1 PIN0 (coords: E7) is configured as PIO1_0/ADC0_6 */

	/****************************************************************************************************************/	
  const uint32_t port0_pin8_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as FC5_RXD_SDA_MOSI */
    IOCON_PIO_MODE_PULLUP |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN8_IDX, port0_pin8_config); /* PORT0 PIN8 (coords: B13) is configured as FC5_RXD_SDA_MOSI */  
	const uint32_t port0_pin9_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as FC5_TXD_SDA_MOSI */
    IOCON_PIO_MODE_PULLUP |                                   /* No addition pin function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                    /* Fast mode, slew rate control is disabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN9_IDX, port0_pin9_config); /* PORT0 PIN9 (coords: B13) is configured as FC5_TXD_SDA_MOSI */
	
  const uint32_t port0_pin2_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO0_24 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN2_IDX, port0_pin2_config); /* PORT0 PIN2 (coords: E7) is configured as PIO0_2 */
	const uint32_t port0_pin3_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO0_23 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN3_IDX, port0_pin3_config); /* PORT0 PIN3 (coords: E7) is configured as PIO0_3 */
	const uint32_t port0_pin21_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO0_28 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN21_IDX, port0_pin21_config); /* PORT0 PIN21 (coords: E7) is configured as PIO0_21 */
	const uint32_t port0_pin22_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO0_27 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN22_IDX, port0_pin22_config); /* PORT0 PIN22(coords: E7) is configured as PIO0_22 */
	const uint32_t port0_pin24_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN24_IDX, port0_pin24_config); /* PORT0 PIN24 (coords: E7) is configured as PIO0_24 */
	const uint32_t port0_pin25_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN25_IDX, port0_pin25_config); /* PORT0 PIN25 (coords: E7) is configured as PIO0_25 */
	const uint32_t port0_pin28_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN28_IDX, port0_pin28_config); /* PORT0 PIN28(coords: E7) is configured as PIO0_28 */	
	const uint32_t port0_pin29_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT0_IDX, PIN29_IDX, port0_pin29_config); /* PORT0 PIN29 (coords: E7) is configured as PIO0_29 */
	const uint32_t port1_pin22_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN22_IDX, port1_pin22_config); /* PORT1 PIN22 (coords: E7) is configured as PIO1_22 */	
	const uint32_t port1_pin29_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN29_IDX, port1_pin29_config); /* PORT1 PIN29(coords: E7) is configured as PIO1_29 */	
	const uint32_t port1_pin30_config = (
    IOCON_PIO_FUNC0 |                                        /* Pin is configured as PIO3_29 */
    IOCON_PIO_MODE_PULLUP |                                  /* Selects pull-up function */
    IOCON_PIO_INV_DI |                                       /* Input function is not inverted */
    IOCON_PIO_DIGITAL_EN |                                   /* Enables digital function */
    IOCON_PIO_INPFILT_OFF |                                  /* Input filter disabled */
    IOCON_PIO_SLEW_STANDARD |                                /* Standard mode, output slew rate control is enabled */
    IOCON_PIO_OPENDRAIN_DI                                   /* Open drain is disabled */
  );
  IOCON_PinMuxSet(IOCON, PORT1_IDX, PIN30_IDX, port1_pin30_config); /* PORT1 PIN30 (coords: E7) is configured as PIO1_30 */
	
	

	
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
