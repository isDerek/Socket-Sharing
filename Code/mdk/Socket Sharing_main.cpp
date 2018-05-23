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

/*******************************************************************************
 * Includes
 ******************************************************************************/



extern "C"
{
#include "opt.h"
#if LWIP_NETCONN
#include "tcpecho.h"
#include "tcpip.h"
#include "ethernet.h"
#include "ethernetif.h"
#include "board.h"
#include "pin_mux.h"
#include <stdbool.h>
#include "lpc54608_api.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_adc.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
#include "fsl_usart.h"
#include "LPC54608_features.h"
#include "api.h"
#include "fsl_flashiap.h"
#include "fsl_common.h"
#include "fsl_iocon.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"
}
#include "cJSON.h"
#include "flashLayout.h"
#include "lib_crc16.h"
#include "Socket_Sharing.h"
#include "ota.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_ENET ENET
#define DEMO_ADC_BASE ADC0
#define DEMO_ADC_IRQ_ID ADC0_SEQA_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC ADC0_SEQA_IRQHandler

//volatile bool gAdcConvSeqAIntFlag;

#define DEMO_USART2 USART2	//added by derek 2017.10.24
#define DEMO_USART4 USART4	//added by derek 2017.10.24
#define DEMO_USART5 USART5 	//added by derek 2017.10.24 
#define DEMO_USART7 USART7	//added by derek 2017.10.24


#define DEMO_USART_CLK_SRC2 kCLOCK_Flexcomm2	//added by derek 2017.10.24
#define DEMO_USART_CLK_SRC4 kCLOCK_Flexcomm4	//added by derek 2017.10.24
#define DEMO_USART_CLK_SRC5 kCLOCK_Flexcomm5	//added by derek 2017.10.24
#define DEMO_USART_CLK_SRC7 kCLOCK_Flexcomm7	//added by derek 2017.10.24


#define DEMO_USART_CLK_FREQ2 CLOCK_GetFreq(kCLOCK_Flexcomm2) 	//added by derek 2017.10.24
#define DEMO_USART_CLK_FREQ4 CLOCK_GetFreq(kCLOCK_Flexcomm4) 	//added by derek 2017.10.24
#define DEMO_USART_CLK_FREQ5 CLOCK_GetFreq(kCLOCK_Flexcomm5) 	//added by derek 2017.10.24
#define DEMO_USART_CLK_FREQ7 CLOCK_GetFreq(kCLOCK_Flexcomm7)	//added by derek 2017.10.24

#define DEMO_USART2_IRQHandler FLEXCOMM2_IRQHandler  //added by derek 2017.10.24
#define DEMO_USART4_IRQHandler FLEXCOMM4_IRQHandler  //added by derek 2017.10.24
#define DEMO_USART5_IRQHandler FLEXCOMM5_IRQHandler  //added by derek 2017.10.24
#define DEMO_USART7_IRQHandler FLEXCOMM7_IRQHandler  //added by derek 2017.10.24

#define DEMO_USART_IRQn2 FLEXCOMM2_IRQn	//added by derek 2017.10.24
#define DEMO_USART_IRQn4 FLEXCOMM4_IRQn	//added by derek 2017.10.24
#define DEMO_USART_IRQn5 FLEXCOMM5_IRQn	//added by derek 2017.10.24
#define DEMO_USART_IRQn7 FLEXCOMM7_IRQn	//added by derek 2017.10.24

#define LED_W_PORT 0
#define LED_W_PIN  2
#define LED_B_PORT 0
#define LED_B_PIN  29
#define LED_G_PORT 1
#define LED_G_PIN  29
#define LED_Y_PORT 0
#define LED_Y_PIN  22
#define PIO1_30_PORT 1
#define PIO1_30_PIN 30
#define SW_PORT 0
#define SW_PIN 3
//#define RST_PORT  
//#define RST_PIN
#define PIO0_21_PORT 0
#define PIO0_21_PIN 21



#define PIO0_28_PORT 0
#define PIO0_28_PIN 28
#define RELAY_PORT 0
#define RELAY_PIN 24
#define PIO1_22_PORT 1
#define PIO1_22_PIN 22
#define PIO0_25_PORT 0
#define PIO0_25_PIN 25
#define PIO1_0_PORT 1
#define PIO1_0_PIN 0  

#define RS485_TX_PORT 0
#define RS485_TX_PIN 9
#define RS485_RX_PORT 0
#define RS485_RX_PIN 8
#define CP_PORT 0
#define CP_PIN 10
#define PWM_PORT 0
#define PWM_PIN 17

#define sourceClock         			CLOCK_GetFreq(kCLOCK_CoreSysClk)



 /*--DEFINES-----------------------------------------------------------------------------*/
#define RESET_TIMER 120

SystemEventHandle eventHandle;

struct netbuf *buf;
SocketInfo socketInfo;
int systemTimer = 0;
int resetTimer = 0;

volatile bool tcpwriteFlag = false;

char tempBuffer[256];//need to be fixed, save data flash use this buffer;
/*! @brief Ring buffer to save received data. */


/*******************************************************************************
* Prototypes
******************************************************************************/

struct netconn  *tcpsocket;
#define configSERVER_ADDR0    112    //10  //112
#define configSERVER_ADDR1    74   //86   //74
#define configSERVER_ADDR2    170  //136  //170
#define configSERVER_ADDR3    197  //14  //197

//#define configSERVER_ADDR0    10   //10  //112
//#define configSERVER_ADDR1    86   //86   //74
//#define configSERVER_ADDR2    136  //136  //170
//#define configSERVER_ADDR3    27  //14  //197

/*******************************************************************************
* Variables
******************************************************************************/

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 20
char regvalue[DEMO_RING_BUFFER_SIZE];
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t i; /* Index of the memory to save new arrived data. */
volatile bool BufferFullFlag = false;
/*--CONSTANTS---------------------------------------------------------------------------*/

const char* DEFAULT_SERVER_ADDRESS = "112.74.170.197";

#if  DEBUG_MODE 
	#warning  "debug mode! server port is :44441"
	const int DEFAULT_SERVER_PORT = 44441;
#else
	const int DEFAULT_SERVER_PORT = 11114;
#endif

char ECHO_SERVER_ADDRESS[20];
int ECHO_SERVER_PORT ;
ip_addr_t SERVERADDRESS;
struct netif fsl_netif0;	


int respond = 0; //added by derek 2018.3.16
bool switchAllPortStatusFlag = false;//added by derek 2018.3.20
extern char eeprombuff[4];
char versionSN [33] ="00000000000000000000000000000000";
bool getLatestFWFromServerFlag;
/*******************************************************************************
 * Code
 ******************************************************************************/

extern "C"
{

/****************************************************************************/

	void DEMO_USART2_IRQHandler(void)
{

		usart_readbyByte(DEMO_USART2);

}
	void DEMO_USART4_IRQHandler(void)
{

		usart_readbyByte(DEMO_USART4);

}
	void DEMO_USART5_IRQHandler(void)
{

		usart_readbyByte(DEMO_USART5);

}
	void DEMO_USART7_IRQHandler(void)
{

		usart_readbyByte(DEMO_USART7);

}




void usart_interrupt_init(void)
{
	  usart_config_t config;
	  usart_config_t RS485_config;
		USART_GetDefaultConfig(&config);
		USART_GetDefaultConfig(&RS485_config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;
	  RS485_config.baudRate_Bps = 9600;
    RS485_config.enableTx = true;
    RS485_config.enableRx = true;
	  USART_Init(DEMO_USART2, &config, DEMO_USART_CLK_FREQ2);
    USART_Init(DEMO_USART4, &config, DEMO_USART_CLK_FREQ4);
//		USART_Init(DEMO_USART5, &config, DEMO_USART_CLK_FREQ5);  //as gpio
		USART_Init(DEMO_USART7, &config, DEMO_USART_CLK_FREQ7);
    /* Enable RX interrupt. */
		USART_EnableInterrupts(DEMO_USART2, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    EnableIRQ(DEMO_USART_IRQn2);
		USART_EnableInterrupts(DEMO_USART4, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    EnableIRQ(DEMO_USART_IRQn4);
//		USART_EnableInterrupts(DEMO_USART5, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);  //as gpio
//    EnableIRQ(DEMO_USART_IRQn5);
    USART_EnableInterrupts(DEMO_USART7, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
    EnableIRQ(DEMO_USART_IRQn7);

}

/*GPIO INIT*/
	void gpio_init(void)
	{
		/* Define the init structure for the output LED pin*/
    gpio_pin_config_t gpio_config = {
        kGPIO_DigitalOutput, 0,
    };
		GPIO_PinInit(GPIO, LED_W_PORT, LED_W_PIN, &gpio_config);
		GPIO_PinInit(GPIO, LED_B_PORT, LED_B_PIN, &gpio_config);
		GPIO_PinInit(GPIO, LED_G_PORT, LED_G_PIN, &gpio_config);
		GPIO_PinInit(GPIO, LED_Y_PORT, LED_Y_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO0_21_PORT, PIO0_21_PIN, &gpio_config);
		GPIO_PinInit(GPIO, SW_PORT, SW_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO1_30_PORT, PIO1_30_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO0_28_PORT, PIO0_28_PIN, &gpio_config);		
		GPIO_PinInit(GPIO, RELAY_PORT, RELAY_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO1_22_PORT, PIO1_22_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO0_25_PORT, PIO0_25_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PIO1_0_PORT, PIO1_0_PIN, &gpio_config);
		GPIO_PinInit(GPIO, RS485_TX_PORT, RS485_TX_PIN, &gpio_config);
		GPIO_PinInit(GPIO, RS485_RX_PORT, RS485_RX_PIN, &gpio_config);
		GPIO_PinInit(GPIO, CP_PORT, CP_PIN, &gpio_config);
		GPIO_PinInit(GPIO, PWM_PORT, PWM_PIN, &gpio_config);
	}

/* ADC INIT*/
	void adc_interrupt_init(void)
	{
		 ADC_ClockPower_Configuration();
		    /* Calibration. */
    if (ADC_DoSelfCalibration(DEMO_ADC_BASE))
    {
//        PRINTF("ADC_DoSelfCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC_DoSelfCalibration() Failed.\r\n");
    }

    /* Configure the ADC as basic polling mode. */
    ADC_Configuration();

    /* Enable the interrupt. */
    /* Enable the interrupt the for sequence A done. */
    ADC_EnableInterrupts(DEMO_ADC_BASE, kADC_ConvSeqAInterruptEnable);
    NVIC_EnableIRQ(DEMO_ADC_IRQ_ID);

//    PRINTF("Configuration Done.\r\n");
	}
}

/***********************************check network connection***************************/
void checkNetworkAvailable(void)
{
    bool resendFlag = false;
    if(chargerException.serverConnectedFlag == true) 
			{						
			} 
		else 
			{
			  netconn_delete(tcpsocket);              //added by derek 2017.11.15

				tcpsocket = netconn_new(NETCONN_TCP);   // added by derek 2017.11.15 
        if(netconn_connect(tcpsocket,&SERVERADDRESS, ECHO_SERVER_PORT) != ERR_OK) 
				{
					printf("network unavailable!\r\n");
            //     printf("Unable to connect 2 to (%s) on port (%d)\r\n", ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);
//					NVIC_SystemReset();  //added by derek 2017.12.14
        } 
				else
				{	                                           
						printf("connected to server!\r\n");
            chargerException.serverConnectedFlag = true;
						printf("resetTimer = %d\r\n",resetTimer);
            if(eventHandle.firstConnectFlag != true)
            resendFlag = true;						
						else
							{
								// no action because download otaBin error has reported after recevie OTA BIN
							}
        }
			}
    if(resendFlag == true) 
		{
        notifyMsgSendHandle(notifyNewDevice);
        resendFlag = false;
    }
}




/*****************************************initEventHandle***********************************************/
void initEventHandle(void)
{
		eventHandle.firstConnectFlag = true;   
}
/***********************************1s timer thread***************************/
void timerOneSecondThread(void *arg)
{
		LWIP_UNUSED_ARG(arg);
    while (true) 
			{	
				int portIndex = 0;
        if(chargerException.serverConnectedFlag == false) 
					{
            resetTimer++;
					}
        vTaskDelay(1000);
        systemTimer++;  // retry msg parm
				for(portIndex = 0;portIndex < 16;portIndex ++)
					{
						if(chargerInfo.allPortStatus[portIndex] == 1)
						{
							chargerInfo.duration[portIndex]++;
						}
						else
						{
							chargerInfo.duration[portIndex] = 0;
						}
					}
    }
} 
/***********************************30s timer thread***************************/
void heartbeatThread(void *arg)
{
	 char heartbeatBuffer[64]; // send heartbeat package with json format,modified by orangeCai in 20170328 
		LWIP_UNUSED_ARG(arg);
    while (true) {
      	vTaskDelay(30000);           

            printf("send heartbeat packet!\r\n");
						chargerInfo.apiId = 2;
					  sprintf(heartbeatBuffer,NOTIFY_REQ_heartPackage,chargerInfo.apiId);
//						printf("heartbeatBuffer = %s\n\r",heartbeatBuffer);
						if(netconn_write(tcpsocket,heartbeatBuffer,strlen(heartbeatBuffer),NETCONN_COPY) != ERR_OK) 
						{      
                printf("send heartbeat packet error!\r\n");
                chargerException.serverConnectedFlag = false;
						} 
						else 
						{
                printf("send heartbeat packet successful!\r\n");
							  printf("send %d bytes,%s\r\n",strlen(heartbeatBuffer),heartbeatBuffer);
            }
					
        
			
    }
}
/*************************************respond  api*****************************************************/
void apiRespond()
{
			if(respond == 1)
		{
		respond = 0;
		switch(chargerInfo.apiId)
		{
			case 21:	
				cmdMsgRespHandle(notifyStartCharing);
				chargerInfo.apiId = 0;	 // api in idle	
				break;
			case 22:
				cmdMsgRespHandle(notifyEndCharging);
				chargerInfo.apiId = 0;	
				break;
			case 23:
				cmdMsgRespHandle(notifygetPortStatus);
				chargerInfo.apiId = 0;					
				break;
			case 24:
				cmdMsgRespHandle(notifygetPortStatus);
				chargerInfo.apiId = 0;
				break;
			case 51:
				cmdMsgRespHandle(notifysetAllPortStatus);
				chargerInfo.apiId = 0;					
				break;
			default:
				break;
		}
	
		}
		else
		{
		}
}

/*****************************Check whether finish charging*************************/
void checkChargingFinish(void)
{
	int portIndex;
	for(portIndex =0;portIndex<16;portIndex++)
	{
	if(chargerInfo.duration[portIndex]>chargerInfo.setDuration[portIndex]*60)
	{
		printf("time = %d\n\r",(chargerInfo.duration[portIndex]));
		
			switch(portIndex)
			{
				case 0: 		
						chargerInfo.index = 0;  
						chargerInfo.allPortStatus[chargerInfo.index] = CP_OFF();	
						chargerInfo.portStatus[0][0] = chargerInfo.index;
						chargerInfo.portStatus[0][1] = CP_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);						
					break;
				case 1:	
						chargerInfo.index = 1;
						chargerInfo.allPortStatus[chargerInfo.index] = PWM_OFF();
						chargerInfo.portStatus[1][0] = chargerInfo.index;
						chargerInfo.portStatus[1][1] = PWM_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 2: 
						chargerInfo.index = 2;
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_TX_OFF();
						chargerInfo.portStatus[2][0] = chargerInfo.index;
						chargerInfo.portStatus[2][1] = RS485_TX_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);			
					break;
				case 3:	
						chargerInfo.index = 3;
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_RX_OFF();
						chargerInfo.portStatus[3][0] = chargerInfo.index;
						chargerInfo.portStatus[3][1] = RS485_RX_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 4:	
						chargerInfo.index = 4;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_28_OFF();
						chargerInfo.portStatus[4][0] = chargerInfo.index;
						chargerInfo.portStatus[4][1] = PIO0_28_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 5:
						chargerInfo.index = 5;
						chargerInfo.allPortStatus[chargerInfo.index] = RELAY0_OFF();
						chargerInfo.portStatus[5][0] = chargerInfo.index;
						chargerInfo.portStatus[5][1] = RELAY0_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 6:	
						chargerInfo.index = 6;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_22_OFF();
						chargerInfo.portStatus[6][0] = chargerInfo.index;
						chargerInfo.portStatus[6][1] = PIO1_22_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 7:	
						chargerInfo.index = 7;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_25_OFF();
						chargerInfo.portStatus[7][0] = chargerInfo.index;
						chargerInfo.portStatus[7][1] = PIO0_25_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 8:	
						chargerInfo.index = 8;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_0_OFF();
						chargerInfo.portStatus[8][0] = chargerInfo.index;
						chargerInfo.portStatus[8][1] = PIO1_0_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 9:	
						chargerInfo.index = 9;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_21_OFF();
						chargerInfo.portStatus[9][0] = chargerInfo.index;
						chargerInfo.portStatus[9][1] = PIO0_21_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					break;
				case 10: 
						chargerInfo.index = 10;
						chargerInfo.allPortStatus[chargerInfo.index] = SW_OFF();
						chargerInfo.portStatus[10][0] = chargerInfo.index;
						chargerInfo.portStatus[10][1] = SW_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 11: 
						chargerInfo.index = 11;
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_30_OFF();
						chargerInfo.portStatus[11][0] = chargerInfo.index;
						chargerInfo.portStatus[11][1] = PIO1_30_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);			
					break;
				case 12: 					
						chargerInfo.index = 12;
						chargerInfo.allPortStatus[chargerInfo.index] = LED_Y_OFF();
						chargerInfo.portStatus[12][0] = chargerInfo.index;
						chargerInfo.portStatus[12][1] = LED_Y_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				case 13: 
						chargerInfo.index = 13;
						chargerInfo.allPortStatus[chargerInfo.index] = LED_G_OFF();
						chargerInfo.portStatus[13][0] = chargerInfo.index;
						chargerInfo.portStatus[13][1] = LED_G_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);			
					break;
				case 14: 
						chargerInfo.index = 14;
						chargerInfo.allPortStatus[chargerInfo.index] = LED_B_OFF();
						chargerInfo.portStatus[14][0] = chargerInfo.index;
						chargerInfo.portStatus[14][1] = LED_B_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);			
					break;
				case 15: 
						chargerInfo.index = 15;
						chargerInfo.allPortStatus[chargerInfo.index] = LED_W_OFF();
						chargerInfo.portStatus[15][0] = chargerInfo.index;
						chargerInfo.portStatus[15][1] = LED_W_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);		
					break;
				default:
					respond = 0;
					break;
			}
			chargerInfo.setDuration[portIndex]=0;  			
		}
	else
	{
		
	}

	}
}
/*****************************************init Server IP ADDRESS****************************************/
void initServer(void)
{
    char* cdata = (char*)SERVER_IP_ADDRESS;
    uint16_t crc16,tempcrc16 = 0;	
    crc16 = calculate_crc16(cdata, 6);

    tempcrc16 = (cdata[6] << 8) | cdata[7];
    if(crc16 == tempcrc16) {
			PRINTF(" match OK!\n");
        PRINTF(ECHO_SERVER_ADDRESS,"%d.%d.%d.%d",cdata[0],cdata[1],cdata[2],cdata[3]);
        ECHO_SERVER_PORT = (cdata[4]<<8)|cdata[5];
    } else {
        sprintf(ECHO_SERVER_ADDRESS,"%s",DEFAULT_SERVER_ADDRESS);
        ECHO_SERVER_PORT = DEFAULT_SERVER_PORT;
    }
    PRINTF("initServer: %s:%d\r\n",ECHO_SERVER_ADDRESS,ECHO_SERVER_PORT);
}

void openPortSwitch()
{
	switch(chargerInfo.index)
			{
				case 0: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{			
						chargerInfo.allPortStatus[chargerInfo.index] = CP_ON();			
						respcode = RESP_OK;
						apiRespond();	
						chargerInfo.portStatus[0][0] = chargerInfo.index;
						chargerInfo.portStatus[0][1] = CP_ON();	
						notifyMsgSendHandle(notifygetPortStatus);			
					}
					break;
				case 1:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PWM_ON();		
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[1][0] = chargerInfo.index;
						chargerInfo.portStatus[1][1] = PWM_ON();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					break;
				case 2: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_TX_ON();		
						respcode = RESP_OK;		
						apiRespond();
						chargerInfo.portStatus[2][0] = chargerInfo.index;
						chargerInfo.portStatus[2][1] = RS485_TX_ON();	
						notifyMsgSendHandle(notifygetPortStatus);			
					}
					break;
				case 3:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_RX_ON();	
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[3][0] = chargerInfo.index;
						chargerInfo.portStatus[3][1] = RS485_RX_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					}
					break;
				case 4:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_28_ON();	
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[4][0] = chargerInfo.index;
						chargerInfo.portStatus[4][1] = PIO0_28_ON();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 5:
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RELAY0_ON();	
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[5][0] = chargerInfo.index;
						chargerInfo.portStatus[5][1] = RELAY0_ON();	
						notifyMsgSendHandle(notifygetPortStatus);					
					}
					break;
				case 6:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_22_ON();	
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[6][0] = chargerInfo.index;
						chargerInfo.portStatus[6][1] = PIO1_22_ON();	
						notifyMsgSendHandle(notifygetPortStatus);					
					}
					break;
				case 7:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_25_ON();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[7][0] = chargerInfo.index;
						chargerInfo.portStatus[7][1] = PIO0_25_ON();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 8:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_0_ON();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[8][0] = chargerInfo.index;
						chargerInfo.portStatus[8][1] = PIO1_0_ON();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 9:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_21_ON();	
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[9][0] = chargerInfo.index;
						chargerInfo.portStatus[9][1] = PIO0_21_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					}						
					break;
				case 10: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = SW_ON();	
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[10][0] = chargerInfo.index;
						chargerInfo.portStatus[10][1] = SW_ON();	
						notifyMsgSendHandle(notifygetPortStatus);					
					}
					break;
				case 11: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_30_ON();			
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[11][0] = chargerInfo.index;
						chargerInfo.portStatus[11][1] = PIO1_30_ON();	
						notifyMsgSendHandle(notifygetPortStatus);			
					}
					break;
				case 12: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{						
						chargerInfo.allPortStatus[chargerInfo.index] = LED_Y_ON();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[12][0] = chargerInfo.index;
						chargerInfo.portStatus[12][1] = LED_Y_ON();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 13: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_G_ON();	
						respcode = RESP_OK;		
						apiRespond();
						chargerInfo.portStatus[13][0] = chargerInfo.index;
						chargerInfo.portStatus[13][1] = LED_G_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					}
					break;
				case 14: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_B_ON();
						respcode = RESP_OK;		
						apiRespond();
						chargerInfo.portStatus[14][0] = chargerInfo.index;
						chargerInfo.portStatus[14][1] = LED_B_ON();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 15: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 1)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_W_ON();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[15][0] = chargerInfo.index;
						chargerInfo.portStatus[15][1] = LED_W_ON();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				default:
					respond = 0;
					break;
			}
}
void closePortSwitch()
{
	printf("chargerIndex = %d\n\r",chargerInfo.index);
	switch(chargerInfo.index)
			{
				case 0: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{			
						chargerInfo.allPortStatus[chargerInfo.index] = CP_OFF();		
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[0][0] = chargerInfo.index;
						chargerInfo.portStatus[0][1] = CP_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);											
					}
					break;
				case 1:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PWM_OFF();	
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[1][0] = chargerInfo.index;
						chargerInfo.portStatus[1][1] = PWM_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 2: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_TX_OFF();	
						respcode = RESP_OK;		
						apiRespond();
						chargerInfo.portStatus[2][0] = chargerInfo.index;
						chargerInfo.portStatus[2][1] = RS485_TX_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 3:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_RX_OFF();	
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[3][0] = chargerInfo.index;
						chargerInfo.portStatus[3][1] = RS485_RX_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 4:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_28_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[4][0] = chargerInfo.index;
						chargerInfo.portStatus[4][1] = PIO0_28_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);									
					}
					break;
				case 5:
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = RELAY0_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[5][0] = chargerInfo.index;
						chargerInfo.portStatus[5][1] = RELAY0_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 6:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_22_OFF();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[6][0] = chargerInfo.index;
						chargerInfo.portStatus[6][1] = PIO1_22_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 7:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_25_OFF();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[7][0] = chargerInfo.index;
						chargerInfo.portStatus[7][1] = PIO0_25_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 8:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_0_OFF();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[8][0] = chargerInfo.index;
						chargerInfo.portStatus[8][1] = PIO1_0_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);							
					}
					break;
				case 9:	
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_21_OFF();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[9][0] = chargerInfo.index;
						chargerInfo.portStatus[9][1] = PIO0_21_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}						
					break;
				case 10: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = SW_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[10][0] = chargerInfo.index;
						chargerInfo.portStatus[10][1] = SW_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);						
					}
					break;
				case 11: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}	
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_30_OFF();
						respcode = RESP_OK;
						apiRespond();
						chargerInfo.portStatus[11][0] = chargerInfo.index;
						chargerInfo.portStatus[11][1] = PIO1_30_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);									
					}
					break;
				case 12: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{						
						chargerInfo.allPortStatus[chargerInfo.index] = LED_Y_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[12][0] = chargerInfo.index;
						chargerInfo.portStatus[12][1] = LED_Y_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);								
					}
					break;
				case 13: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_G_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[13][0] = chargerInfo.index;
						chargerInfo.portStatus[13][1] = LED_G_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);									
					}
					break;
				case 14: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_B_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[14][0] = chargerInfo.index;
						chargerInfo.portStatus[14][1] = LED_B_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);									
					}
					break;
				case 15: 
					if(chargerInfo.allPortStatus[chargerInfo.index] == 0)
					{
						respcode = RESP_ILLEGAL_REQUEST;
						apiRespond();	
						notifyMsgSendHandle(notifygetPortStatus);
					}
					else
					{
						chargerInfo.allPortStatus[chargerInfo.index] = LED_W_OFF();
						respcode = RESP_OK;	
						apiRespond();
						chargerInfo.portStatus[15][0] = chargerInfo.index;
						chargerInfo.portStatus[15][1] = LED_W_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);								
					}
					break;
				default:
					respond = 0;
					break;
			}
}
void respondPortSwitch()
{
	switch(chargerInfo.index)
			{
				case 0:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 1:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 2:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 3:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 4:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 5:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 6:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 7:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 8:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 9:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 10:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 11:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 12:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 13:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 14:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 15:
					respond = 1;
					respcode = RESP_OK;		
					apiRespond();
					break;
				case 100: 
					respond = 1;
					respcode = RESP_OK;	 
					apiRespond();
					break;
				default:
					respond = 0;
					break;
			}
}
void setAllPortStatusSwitch()
{
	if(switchAllPortStatusFlag == true)
	{
switch(chargerInfo.index)
			{
				case 0: 					
						chargerInfo.allPortStatus[chargerInfo.index] = CP_ON();		
						chargerInfo.portStatus[0][0] = chargerInfo.index;
						chargerInfo.portStatus[0][1] = CP_ON();		
						notifyMsgSendHandle(notifygetPortStatus);								
					break;
				case 1:						
						chargerInfo.allPortStatus[chargerInfo.index] = PWM_ON();		
						chargerInfo.portStatus[1][0] = chargerInfo.index;
						chargerInfo.portStatus[1][1] = PWM_ON();	
						notifyMsgSendHandle(notifygetPortStatus);
					break;
				case 2: 
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_TX_ON();		
						chargerInfo.portStatus[2][0] = chargerInfo.index;
						chargerInfo.portStatus[2][1] = RS485_TX_ON();		
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 3:	
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_RX_ON();		
						chargerInfo.portStatus[3][0] = chargerInfo.index;
						chargerInfo.portStatus[3][1] = RS485_RX_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 4:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_28_ON();		
						chargerInfo.portStatus[4][0] = chargerInfo.index;
						chargerInfo.portStatus[4][1] = PIO0_28_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 5:
						chargerInfo.allPortStatus[chargerInfo.index] = RELAY0_ON();		
						chargerInfo.portStatus[5][0] = chargerInfo.index;
						chargerInfo.portStatus[5][1] = RELAY0_ON();	
						notifyMsgSendHandle(notifygetPortStatus);
					break;
				case 6:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_22_ON();		
						chargerInfo.portStatus[6][0] = chargerInfo.index;
						chargerInfo.portStatus[6][1] = PIO1_22_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 7:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_25_ON();		
						chargerInfo.portStatus[7][0] = chargerInfo.index;
						chargerInfo.portStatus[7][1] = PIO0_25_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 8:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_0_ON();	
						chargerInfo.portStatus[8][0] = chargerInfo.index;
						chargerInfo.portStatus[8][1] = PIO1_0_ON();	
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 9:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_21_ON();	
						chargerInfo.portStatus[9][0] = chargerInfo.index;
						chargerInfo.portStatus[9][1] = PIO0_21_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 10: 
						chargerInfo.allPortStatus[chargerInfo.index] = SW_ON();	
						chargerInfo.portStatus[10][0] = chargerInfo.index;
						chargerInfo.portStatus[10][1] = SW_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 11: 
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_30_ON();	
						chargerInfo.portStatus[11][0] = chargerInfo.index;
						chargerInfo.portStatus[11][1] = PIO1_30_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 12: 			
						chargerInfo.allPortStatus[chargerInfo.index] = LED_Y_ON();	
						chargerInfo.portStatus[12][0] = chargerInfo.index;
						chargerInfo.portStatus[12][1] = LED_Y_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 13: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_G_ON();
						chargerInfo.portStatus[13][0] = chargerInfo.index;
						chargerInfo.portStatus[13][1] = LED_G_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 14: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_B_ON();	
						chargerInfo.portStatus[14][0] = chargerInfo.index;
						chargerInfo.portStatus[14][1] = LED_B_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 15: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_W_ON();
						chargerInfo.portStatus[15][0] = chargerInfo.index;
						chargerInfo.portStatus[15][1] = LED_W_ON();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				default:
					respond = 0;
					break;
			}
}
	else
	{
	switch(chargerInfo.index)
			{
				case 0: 	
						chargerInfo.allPortStatus[chargerInfo.index] = CP_OFF();		
						chargerInfo.portStatus[0][0] = chargerInfo.index;
						chargerInfo.portStatus[0][1] = CP_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 1:	
						chargerInfo.allPortStatus[chargerInfo.index] = PWM_OFF();	
						chargerInfo.portStatus[1][0] = chargerInfo.index;
						chargerInfo.portStatus[1][1] = PWM_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 2: 
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_TX_OFF();	
						chargerInfo.portStatus[2][0] = chargerInfo.index;
						chargerInfo.portStatus[2][1] = RS485_TX_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 3:	
						chargerInfo.allPortStatus[chargerInfo.index] = RS485_RX_OFF();	
						chargerInfo.portStatus[3][0] = chargerInfo.index;
						chargerInfo.portStatus[3][1] = RS485_RX_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 4:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_28_OFF();
						chargerInfo.portStatus[4][0] = chargerInfo.index;
						chargerInfo.portStatus[4][1] = PIO0_28_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 5:
						chargerInfo.allPortStatus[chargerInfo.index] = RELAY0_OFF();
						chargerInfo.portStatus[5][0] = chargerInfo.index;
						chargerInfo.portStatus[5][1] = RELAY0_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 6:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_22_OFF();
						chargerInfo.portStatus[6][0] = chargerInfo.index;
						chargerInfo.portStatus[6][1] = PIO1_22_OFF();	
						notifyMsgSendHandle(notifygetPortStatus);
					break;
				case 7:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_25_OFF();
						chargerInfo.portStatus[7][0] = chargerInfo.index;
						chargerInfo.portStatus[7][1] = PIO0_25_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 8:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_0_OFF();
						chargerInfo.portStatus[8][0] = chargerInfo.index;
						chargerInfo.portStatus[8][1] = PIO1_0_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 9:	
						chargerInfo.allPortStatus[chargerInfo.index] = PIO0_21_OFF();
						chargerInfo.portStatus[9][0] = chargerInfo.index;
						chargerInfo.portStatus[9][1] = PIO0_21_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 10: 
						chargerInfo.allPortStatus[chargerInfo.index] = SW_OFF();
						chargerInfo.portStatus[10][0] = chargerInfo.index;
						chargerInfo.portStatus[10][1] = SW_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 11: 
						chargerInfo.allPortStatus[chargerInfo.index] = PIO1_30_OFF();
						chargerInfo.portStatus[11][0] = chargerInfo.index;
						chargerInfo.portStatus[11][1] = PIO1_30_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 12: 				
						chargerInfo.allPortStatus[chargerInfo.index] = LED_Y_OFF();
						chargerInfo.portStatus[12][0] = chargerInfo.index;
						chargerInfo.portStatus[12][1] = LED_Y_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 13: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_G_OFF();
						chargerInfo.portStatus[13][0] = chargerInfo.index;
						chargerInfo.portStatus[13][1] = LED_G_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 14: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_B_OFF();
						chargerInfo.portStatus[14][0] = chargerInfo.index;
						chargerInfo.portStatus[14][1] = LED_B_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				case 15: 
						chargerInfo.allPortStatus[chargerInfo.index] = LED_W_OFF();
						chargerInfo.portStatus[15][0] = chargerInfo.index;
						chargerInfo.portStatus[15][1] = LED_W_OFF();
						notifyMsgSendHandle(notifygetPortStatus);				
					break;
				default:
					respond = 0;
					break;
			}
}
	
}
void apiHandle()
{
			if(chargerInfo.apiId == 21)
		{
			openPortSwitch();
		}
		else if(chargerInfo.apiId == 22)
		{
			closePortSwitch();
		}
		else if(chargerInfo.apiId == 23)
		{
			respondPortSwitch();
		}
		else if(chargerInfo.apiId == 51)
		{
			int portStatusIndex;
			for(portStatusIndex = 0 ; portStatusIndex<16 ; portStatusIndex++)
			{
				if(chargerInfo.allPortStatusBuffer[portStatusIndex]==1)
				{
				
					chargerInfo.setDuration[portStatusIndex] = 1;  // 1 minutes test 
					chargerInfo.index = portStatusIndex;
					switchAllPortStatusFlag = true;
					setAllPortStatusSwitch();
				}
				else
				{
					chargerInfo.index = portStatusIndex;
					switchAllPortStatusFlag = false;
					setAllPortStatusSwitch();
				}
				printf("relay0 = [%d][%d]\n\r",chargerInfo.portStatus[0][0],chargerInfo.portStatus[0][1]);		
			}
			chargerInfo.apiId = 51;
			respcode = RESP_OK;	
			apiRespond();
//			notifyMsgSendHandle(notifygetPortStatus);	
		}
		else
		{
		}
}

void workHandle(void *arg)
{
	LWIP_UNUSED_ARG(arg);	
	
	while(true)
	{
		apiHandle();
		checkChargingFinish();		
		vTaskDelay(1000);
	}
	
}

#define EXAMPLE_ENET ENET

/* IP address configuration. */
#define configIP_ADDR0 10
#define configIP_ADDR1 86
#define configIP_ADDR2 136  
#define configIP_ADDR3 200

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 10
#define configGW_ADDR1 86
#define configGW_ADDR2 136
#define configGW_ADDR3 1



/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 2048

/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO
void netWorkThread(void *arg)
{
	LWIP_UNUSED_ARG(arg);			
	while(1)
{				
        checkNetworkAvailable();  			
        if(chargerException.serverConnectedFlag == false) 
					{
            if(resetTimer > RESET_TIMER) 
							{ // 2 minutes
                printf("network error! system will be reset now, please wait...\r\n");
                NVIC_SystemReset();
							}
					} 
				else 
					{          		
            resetTimer = 0;
            if(notifySendID != invalidID)
							{
                if((systemTimer-notifySendCounter) >= SOCKET_RESEND_TIME)
                    notifyMsgSendHandle(notifySendID);  // >5s not receive resp msg, resend notify
							} 
						else 
							{
								if(eventHandle.firstConnectFlag == true)
									{
									  printf("first connetion!\r\n");
										eventHandle.firstConnectFlag = false;
									  notifyMsgSendHandle(notifyNewDevice);
									}									
									else if(eventHandle.getLatestFWFromServerFlag == true) 
										{
										//	printf("update OTA from server!\n\r");
											notifyMsgSendHandle(notifyUpdateVersion);
											eventHandle.getLatestFWFromServerFlag =false;
										}
										else
										{
										}  	
            }
						recvMsgHandle();
        }
						vTaskDelay(20);
			}
//				    netconn_disconnect(tcpsocket);   	
}

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/


void Internet_init()
{	
	 static struct netif fsl_netif0;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);
		IP4_ADDR(&SERVERADDRESS, configSERVER_ADDR0, configSERVER_ADDR1, configSERVER_ADDR2, configSERVER_ADDR3);
		tcpip_init(NULL, NULL);
    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" PING example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
    PRINTF("************************************************\r\n");
		char MAC_ADDRESS[] ={"001213101511"};  
		memcpy(chargerInfo.mac,MAC_ADDRESS,sizeof(MAC_ADDRESS)); 
		memcpy(otaInfo.versionSN,versionSN,sizeof(versionSN));
}

/*!
 * @brief Main function.
 */

int main(void)
{ 
	  CLOCK_EnableClock(kCLOCK_InputMux);
		CLOCK_EnableClock(kCLOCK_Iocon);   
	  CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH2);  //added by derek 2018.1.29 for NB-IOT
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH4);  //added by derek 2018.1.29 for bluetooth 
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH7);  //added by derek 2018.1.29 for debug uart
	    /* attach 12 MHz clock to FLEXCOMM0 (I2C in EEPROM) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    /* attach 12 MHz clock to FLEXCOMM1 (I2C in NFC) */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);
    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
		gpio_init();	
		usart_interrupt_init(); 		
		Internet_init();  
	  eeprom_init();		 
    initServer(); 
    initEventHandle(); 
    OTAInit();  
		if(sys_thread_new("timerOneSecondThread", timerOneSecondThread, NULL, /*1024 512*/512 , INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("timerOneSecondThread(): Task creation failed.", 0);
		if(sys_thread_new("heartbeatThread", heartbeatThread, NULL, /*1024 512*/512 , INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("heartbeatThread(): Task creation failed.", 0);		  		 
		if(sys_thread_new("netWorkThread", netWorkThread, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("netWorkThread(): Task creation failed.", 0);	
		if(sys_thread_new("workHandle", workHandle, NULL, 512, INIT_THREAD_PRIO) == NULL)
        LWIP_ASSERT("workHandle(): Task creation failed.", 0);			
    vTaskStartScheduler();


		
/*****************gpio test********************************/
//     LED_G_OFF();
//		 LED_B_OFF();
//		 LED_W_OFF();
//		 LED_Y_OFF();
//		 PIO1_30_OFF();
//		 SW_OFF();
//		 PIO0_21_OFF();
//		 PIO1_0_OFF();
//		 PIO0_28_OFF();
//		 RELAY0_OFF();
//		 PIO1_22_OFF();
//		 PIO0_25_OFF();
//		 RS485_TX_OFF();
//     RS485_RX_OFF();
//     CP_ON();
//     PWM_OFF();
//		 
/***********************************************************/
/******************eeprom test******************************/
//uint32_t data[32];
//    for (int i = 0; i <32; i++)
//    {
//        data[i] = i+1;
//    }
//char buffer[32];		
//write_EEPROM(EEPROM,1,data);
//read_EEPROM(FSL_FEATURE_EEPROM_BASE_ADDRESS,1,128,buffer);
/***********************************************************/
//uint8_t g_tipString[] =
//    "Usart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";
//while(1)
//{

	/****************************pwm test***********************/
//	disableCPPWM();   
//	enableCPPWM(99);	


	/***********************************************************/
/**********************RS485 USART  Test**********************/   
  
	
//	printf("ADC VALUE = %f\n",adc_read_result()*3.3);  // OK!

//	usart_writebyByte(DEMO_USART5); // OK!  STANDARD FUNC

	
//            USART_WriteByte(DEMO_USART7, regvalue[txIndex]);//no test  Charger Func

        
//				 USART_WriteBlocking(DEMO_USART5, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0])); 
				
	/***********************************************************/
//}

//#define FLASH_TEST
#ifdef FLASH_TEST
char a[256];
for(i = 0;i<256;i++)
{
	a[i] = i;
}
erase_sector(VERSION_STR_ADDRESS);
program_flash(VERSION_STR_ADDRESS,a,256);

while(1)
{
}
#endif
//#define JSONS_TEST
#ifdef JSONS_TEST
char buffer[256];

parseRecvMsgInfo(buffer);
notifyMsgSendHandle(notifyOTAResult);
while(1)
{
}
#endif



#define EEPROM_TEST
#ifdef EEPROM_ENABLE
#ifdef EEPROM_TEST

#if 1
		char dataTest[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		uint8_t addr = 0x20;
		PRINTF("addr:%x\r\n",addr);
		PRINTF("size of dataTest:%d\r\n",sizeof(dataTest));
		writeToEEPROM(addr,dataTest,sizeof(dataTest));
		readFromEEPROM(NULL,tempBuffer,sizeof(dataTest));  
		PRINTF("data:%s\r\n",tempBuffer);
#else
		uint8_t addr = 0x20;
		PRINTF("addr:%x\r\n",addr);		
		char meterNumber[9] = "12131415";
		ChargerInfo tmpChargerInfo;
		ChargerInfo tmpChargerInfo1;
		chargerInfo.connect = 1;
		chargerInfo.current = 14.5;
		chargerInfo.voltage = 233.3;
		chargerInfo.duration = 34;
		chargerInfo.energy = 13.5;
		chargerInfo.setDuration = 120;
		chargerInfo.power = 3133.3;
		chargerInfo.userId = 5;
		chargerInfo.status = idle;
		memcpy(chargerInfo.meterNumber,meterNumber,sizeof(chargerInfo.meterNumber));
#if 0	
		writeToEEPROM(addr,(char*)(&chargerInfo),sizeof(chargerInfo));
		readFromEEPROM(addr,(char*)(&tmpChargerInfo),sizeof(tmpChargerInfo));
		PRINTF("connet=%d\r\nuserid=%d\r\nstatus=%d\r\nmeterNumber=%s\r\nenergy=%f\n\rvoltage=%f\r\ncurrent=%f\r\npower=%f\r\n"
		,tmpChargerInfo.connect,tmpChargerInfo.userId,tmpChargerInfo.status,tmpChargerInfo.meterNumber,tmpChargerInfo.energy,tmpChargerInfo.voltage,
		tmpChargerInfo.current,tmpChargerInfo.power);
#else
		uint16_t checksum,crc16;
		char* temp = (char*)malloc(sizeof(chargerInfo)+2);
		char cdata[256];  // added by derek 2017.11.1
		crc16 = calculate_crc16((char*)&chargerInfo,sizeof(chargerInfo));
		printf("crc16=%x\r\n",crc16);
		temp[0] = crc16 >> 8;
		temp[1] = crc16 & 0xFF;
		memcpy(temp+2,(char*)&chargerInfo,sizeof(chargerInfo));
		writeToEEPROM(addr,temp,sizeof(chargerInfo)+2);    
		readFromEEPROM(addr,cdata,sizeof(chargerInfo)+2); 
		checksum = (cdata[0]<<8)|cdata[1];
		printf("checksum=%x\r\n",checksum);
#endif	
#endif
//		while(1){}
#endif
#endif
//		char* jsonString = getJsonString(chargerInfo.energy,chargerInfo.voltage,chargerInfo.current,chargerInfo.power,
//						chargerInfo.duration/60,chargerInfo.status,chargerInfo.connect,chargerInfo.setDuration);   //print json string
			

    }

#endif


