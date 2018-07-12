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
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "lpc54608_api.h"
#include "fsl_eeprom.h"
#include "Socket_Sharing.h"
#include "pin_mux.h"
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
char dataBuffer[256];

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EEPROM_SOURCE_CLOCK kCLOCK_BusClk
#define EEPROM_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_EEPROM EEPROM
uint32_t eeprom_data = 0xFFFFFFFFU;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define FSL_FEATURE_EEPROM_PAGE_SIZE (FSL_FEATURE_EEPROM_SIZE/FSL_FEATURE_EEPROM_PAGE_COUNT)
/*******************************************************************************
 * Code
 ******************************************************************************/
 const uint16_t crc_tab_8005_reflected[] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,

    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

uint16_t update_crc16_reflected(const uint16_t *table, uint16_t crc, char c )
{
    uint16_t short_c;

    short_c  = 0x00ff & (uint16_t) c;

    /* Reflected form */
    return (crc >> 8) ^ table[(crc ^ short_c) & 0xff];
}


uint16_t update_crc16_A001( uint16_t crc, char c )
{
    return update_crc16_reflected(crc_tab_8005_reflected,crc,c);
}

uint16_t calculate_crc16(char *p, unsigned int length)
{
uint16_t crc;
unsigned int i;

    crc = 0;

    for (i=0; i < length; i++)
      {
        crc = update_crc16_A001(crc,*p++);
      }
    return crc;
}

uint16_t calculate_crc16_Modbus(char *p, unsigned int length)
{
    uint16_t crc;
    unsigned int i;

    crc = 0xFFFF;

    for (i=0; i < length; i++) {
        crc = update_crc16_A001(crc,*p++);
    }
    return crc;
}



uint8_t writeToEEPROM(uint8_t addr,char* data,uint8_t size)
{
	uint32_t Buffer[size];
	uint8_t ret = 0;
	uint16_t crc1,crc2;
	uint8_t pageNum;
//	uint8_t firstPageSize,lastPageSize;
	uint8_t ptr = 0;   //point to position of data to be writen
	
	if(addr + size > 256)
	{
		PRINTF("not enough storage!\r\n");
		return ret;
	}
	
//	if(addr%EEPROM_PAGE_SIZE == 0){
//		firstPageSize = EEPROM_PAGE_SIZE;
//  }else{
//		firstPageSize = EEPROM_PAGE_SIZE - addr%EEPROM_PAGE_SIZE;
//	}
//	
//	if(size <= firstPageSize){
//		pageNum = 1;
//    lastPageSize = firstPageSize;		
//	}
//	else{
//		if((size-firstPageSize)%EEPROM_PAGE_SIZE == 0){
//			pageNum = (size-firstPageSize)/EEPROM_PAGE_SIZE + 1;
//			lastPageSize = EEPROM_PAGE_SIZE;
//		}else{
//			pageNum = (size-firstPageSize)/EEPROM_PAGE_SIZE + 2;
//			lastPageSize = (size-firstPageSize)%EEPROM_PAGE_SIZE;
//		}
//	}
//	PRINTF("pageNum=%d\tfirstPageSize=%d\tlastPageSize=%d\r\n",pageNum,firstPageSize,lastPageSize);
/****************************designed by derek 2017.11.1*********************************************/
if(size<=EEPROM_PAGE_SIZE)
	{

	for(int i=0;i<size;i++,ptr++)
	{
		dataBuffer[i] = data[ptr];
		Buffer [i] = dataBuffer[i];  
//		PRINTF(" write data = %x\n",Buffer [i]);
	}
	EEPROM_WritePage(EXAMPLE_EEPROM, 0, Buffer);
	}
	else
	{
		PRINTF("size = %d\r\n",size);		
		pageNum = size/EEPROM_PAGE_SIZE;
//		PRINTF("pagenum = %d\n",pageNum);
	for(int i=0;i<pageNum+1;i++)
		{
			
		for(int j=0;j<EEPROM_PAGE_SIZE;j++)
			{
	
		  dataBuffer[j] = data[ptr++];
			Buffer [j] = dataBuffer[j];
					 if(ptr >= size)
				{
					j=32;
				}
//			PRINTF("write data =%c\n",Buffer [j]);
			}			
			EEPROM_WritePage(EXAMPLE_EEPROM, i, Buffer);	

		}
	}
/****************************************************************************************************/
  crc1 = calculate_crc16(data,size);
	readFromEEPROM(NULL,tempBuffer,size);
	crc2 = calculate_crc16(tempBuffer,size);
//	pc.printf("crc2 = %x\r\n",crc2);
	if(crc1 == crc2)
	{
		ret = 1;
		PRINTF("Write and verify data to eeprom successfully!\r\n");
	}else{
		PRINTF("Failed to write data to eeprom!\r\n");
	}
	return ret;
}

void readFromEEPROM(uint8_t addr,char* data,uint8_t size)
{
//	uint32_t tmp = addr;
//	EEPROM_WritePage(EXAMPLE_EEPROM, 1, &tmp);

	for (int j = 0; j < size; j++)
  {
	eeprom_data = *((uint32_t *)(FSL_FEATURE_EEPROM_BASE_ADDRESS + 0 * size*4 + j *4));
  data [j] = eeprom_data;

	}		

}


void write_EEPROM(EEPROM_Type *base, uint32_t pageNum, uint32_t *data)
{

	EEPROM_WritePage(base,pageNum,data);
}

void read_EEPROM(uint32_t base,uint32_t pageNum,uint8_t size,char *data)
{
		for (int j = 0; j < size/4; j++)
  {
	eeprom_data = *((uint32_t *)(base + pageNum * size + j * 4));
  data [j] = eeprom_data;
		PRINTF("read data = %d\n",data[j]);
	}
}

void eeprom_init(void)
{
	eeprom_config_t config;
  uint32_t sourceClock_Hz = 0;
	/* Init EEPROM */
  EEPROM_GetDefaultConfig(&config);
  sourceClock_Hz = EEPROM_CLK_FREQ;
  EEPROM_Init(EXAMPLE_EEPROM, &config, sourceClock_Hz);
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	PRINTF("task %s over flow!\n\r",pcTaskName);
}
