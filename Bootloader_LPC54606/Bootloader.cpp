extern "C"
{
#include "flashLayout.h"
#include "string.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include <stdio.h>
#include "Bootloader.h"
#include "lpc54608.h"
}
#include "lib_crc16.h"
#include "md5Std.h"

//Could be nicer, but for now just erase all preceding sectors
#define NUM_SECTORS        15
#define TIMEOUT            10000000
#define BUFFER_SIZE        16

#define APPLICATION_START_SECTOR 2
#define APPLICATION_END_SECTOR 39

/*    Flash Layout 0~0xFFFFF
    bootloader(32K)     0x00000~0x07FFF
    application(128K)   0x08000~0x27FFF
    OTA (128K)          0x28000~0x47FFF
    OTA VER (64)        0x48000~0x4803F
    Param (4K)          0xFF000~0xFFFFF
*/
 
#define ApplicationAddress    CODE_START_ADDRESS//0x8000
typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
unsigned int JumpAddress;
extern void __set_MSP(unsigned int topOfMainStack);

asm void jump_app(uint32_t userSP,uint32_t userStartup)
{
	msr msp, r0;
	msr psp, r0;
//	jump to pc(r1);
	CPSIE I;
	BX  r1;
}
void houseClean(void)
{
SYSCON->MAINCLKSELB = 0;
}
void Jump_IAP(void)
{
	
  //NVIC_SETFAULTMASK();  //????
    //    NVIC_DisableIRQ;
    /* Test if user code is programmed starting from address "ApplicationAddress" */
//    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    { /* Jump to user application */
      JumpAddress = *(volatile unsigned int*) (ApplicationAddress + 4);
      Jump_To_Application = (pFunction) JumpAddress;
      /* Initialize user application's Stack Pointer */
      __set_MSP(*(volatile unsigned int*) ApplicationAddress);
      Jump_To_Application();
//			NVIC_SystemReset();
    }
}
int calculateBinSize(char* binStartAddress, int checkTotalSize)
{
		unsigned int binTotalSize;
		for(binTotalSize = 0 ; binTotalSize < CODE_SIZE ; binTotalSize ++)
	{
		int checkSize = 0 , validSize = 0;
		if(*(binStartAddress+binTotalSize) == 0xFF )
		{		
			for(checkSize = 0; checkSize<checkTotalSize;checkSize++)
			{
				if(*(binStartAddress+binTotalSize+checkSize) == 0xFF)
				{
					validSize++;
				}
			}		
		}
		if(validSize == checkTotalSize)
		{
//			printf("binTotalSize = %d\n\r",binTotalSize);
			return binTotalSize;
		}
	}		
		return 0;	
}

void setupserial();
void write(char *value);
char tempBuffer[128];
static void setupserial(void) 
		{
	    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
//		SystemCoreClockUpdate();
		CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH7);  //added by derek 2018.1.29 for debug uart
		BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
    }

void bootloader(void)
{
		char* cdata = (char*)VERSION_STR_ADDRESS;
		char* codePartition = (char*) CODE_START_ADDRESS;
		char* OTACodePartition = (char*) OTA_CODE_START_ADDRESS;
		int i = 0;
	  char versionBuffer[64], calculateOTAVersion[64], calculateAPPVersion[64];
		int codecrc16,otacodecrc16,codechecksum,otacodechecksum,otaTotalSize,appTotalSize;	
    setupserial();
    printf("\r\n*******Bootloader v1.02@20180102 by Eric.Yang && Derek.Zhu*****\r\n");
		if(strncmp("1",cdata+10,1) != NULL) // first time boot, write code to ota partition by app
		{			
			printf("first boot...\r\n");
		}
		else 
		{				
			codechecksum = (cdata[0]<<8)|cdata[1];
			otacodechecksum = (cdata[2]<<8)|cdata[3];			
			otaTotalSize = (cdata[4]<<16)|(cdata[5]<<8)|(cdata[6]);
			appTotalSize = (cdata[7]<<16)|(cdata[8]<<8)|(cdata[9]);
			//otaTotalSize = calculateBinSize(OTACodePartition,20);
			//appTotalSize = calculateBinSize(codePartition,20);
			md5Calculate((unsigned char*)OTACodePartition,otaTotalSize,(unsigned char*)calculateOTAVersion);
			md5Calculate((unsigned char*)codePartition,appTotalSize,(unsigned char*)calculateAPPVersion);
			sprintf(versionBuffer,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",calculateOTAVersion[0],calculateOTAVersion[1],calculateOTAVersion[2],calculateOTAVersion[3],calculateOTAVersion[4],calculateOTAVersion[5],calculateOTAVersion[6],calculateOTAVersion[7],calculateOTAVersion[8],calculateOTAVersion[9],calculateOTAVersion[10],calculateOTAVersion[11],calculateOTAVersion[12],calculateOTAVersion[13],calculateOTAVersion[14],calculateOTAVersion[15]);		
			codecrc16 = calculate_crc16(codePartition, appTotalSize);
			otacodecrc16 = calculate_crc16(OTACodePartition, otaTotalSize);			
			printf("otacodecrc16 = %x, otacodechecksum = %x, codechecksum= %x, codecrc16 = %x otaTotalSize = %d appTotalSize = %d OTAversionSN = %s\n\r",otacodecrc16,otacodechecksum,codechecksum,codecrc16,otaTotalSize,appTotalSize,versionBuffer);						
			if(strcmp(versionBuffer,cdata+11)==NULL)
			{	
				if(codechecksum != otacodechecksum ) // update;
				{
					printf("update...\r\n");
				
					for(i=0; i<CODE_SECTOR_NUM; i++) 
					{
							erase_sector(CODE_START_ADDRESS+i*SECTOR_SIZE);
							program_flash(CODE_START_ADDRESS+i*SECTOR_SIZE,OTACodePartition+i*SECTOR_SIZE, SECTOR_SIZE);
					}
					printf("code done!\r\n");
					codecrc16 = calculate_crc16(codePartition, otaTotalSize);
					printf("codecrc16 = %x\n\r",codecrc16);
					if(codecrc16 != otacodecrc16)
					{
						printf("checksum error,restart...\r\n");
						NVIC_SystemReset();
					}
					else
					{
					tempBuffer[0] = tempBuffer[2]= cdata[2];
					tempBuffer[1] = tempBuffer[3]= cdata[3];
					tempBuffer[4] = tempBuffer[7] = (otaTotalSize >> 16) & 0xff; // otatotal real size
					tempBuffer[5] = tempBuffer[8] =(otaTotalSize >> 8) & 0xff;// otatotal real size
					tempBuffer[6] = tempBuffer[9] =(otaTotalSize) & 0xff;// otatotal real size
					tempBuffer[10] = '1';
					for(i=11;i<VERSION_STR_LEN;i++)
					tempBuffer[i] = cdata[i];
					erase_sector(VERSION_STR_ADDRESS);
					program_flash(VERSION_STR_ADDRESS,tempBuffer, 256);
					printf("completed!\r\n");
					}
				}
				else
				{
					printf("Flash Code Check OK!\r\n");
				} 
			}
			else
			{
				if((codecrc16 == codechecksum)) // update ota;
				{
					printf("update ota...\r\n");
					for(i=0; i<CODE_SECTOR_NUM; i++) 
					{
						erase_sector(OTA_CODE_START_ADDRESS+i*SECTOR_SIZE);
						program_flash(OTA_CODE_START_ADDRESS+i*SECTOR_SIZE,codePartition+i*SECTOR_SIZE, SECTOR_SIZE);
					}
					otacodecrc16 = calculate_crc16(OTACodePartition, appTotalSize);
					if(codecrc16 != otacodecrc16)
					{
						printf("ota checksum error,restart...\r\n");
						NVIC_SystemReset();
					}
					sprintf(versionBuffer,"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",calculateAPPVersion[0],calculateAPPVersion[1],calculateAPPVersion[2],calculateAPPVersion[3],calculateAPPVersion[4],calculateAPPVersion[5],calculateAPPVersion[6],calculateAPPVersion[7],calculateAPPVersion[8],calculateAPPVersion[9],calculateAPPVersion[10],calculateAPPVersion[11],calculateAPPVersion[12],calculateAPPVersion[13],calculateAPPVersion[14],calculateAPPVersion[15]);
					tempBuffer[0] = tempBuffer[2]= cdata[0];
					tempBuffer[1] = tempBuffer[3]= cdata[1];
					tempBuffer[4] = tempBuffer[7] = (otaTotalSize >> 16) & 0xff; // otatotal real size
					tempBuffer[5] = tempBuffer[8] =(otaTotalSize >> 8) & 0xff;// otatotal real size
					tempBuffer[6] = tempBuffer[9] =(otaTotalSize) & 0xff;// otatotal real size
					tempBuffer[10] = '1';
					sprintf(tempBuffer+11,"%s",versionBuffer);
					erase_sector(VERSION_STR_ADDRESS);
					program_flash(VERSION_STR_ADDRESS,tempBuffer, 256);
					printf("ota completed!\r\n");
				}
				else  // code checksum and ota checksum error
				{
					printf("otherwise...\r\n");
				}
			}
		} 
    Jump_IAP();
}
    
