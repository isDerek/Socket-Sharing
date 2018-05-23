#ifndef FLASHLAYOUT_H
#define FLASHLAYOUT_H
#include "UserConfig.h"
/*******************************************************
    Flash Layout
    bootloader(32K)     0x00000~0x07FFF
    application(128K)   0x08000~0x27FFF
    OTA (128K)          0x28000~0x47FFF
    Checksum (4B)       0x48000~0x48003   code checksum & ota code checksum: xx xx xx xx 
    OTA CODE VER(60B)   0x48004~0x4803F
    Param (4K)          0x7F000~0x7FFFF    
		ServerIP (4K)				0x7E000~0x7EFFF  // 0x7E000~0x7EFFF   IP1|IP2|IP2|IP3|PORT1|PORT2|CHECKSUM1|CHECKSUM2 Total 8 bytes
********************************************************/
#define SECTOR_SIZE  								32768
#define BOOTLOADER_START_ADDRESS    0
#define BOOTLOADER_SIZE             0x8000   // 32K  
#define CODE_SECTOR_NUM             4   // 4*32K Bytes    8*4K for bootloader total 160KB
#define MIN_CODE_SECTOR_NUM         2 // 2*32K Bytes minimal code size,  code size always > 90 KB
#define CODE_START_ADDRESS          (BOOTLOADER_SIZE)//0
#define CODE_SIZE                   (CODE_SECTOR_NUM * SECTOR_SIZE)
#define OTA_CODE_START_ADDRESS      (CODE_START_ADDRESS + CODE_SIZE)

#define VERSION_STR_ADDRESS         (OTA_CODE_START_ADDRESS + CODE_SIZE)
#define VERSION_STR_LEN             64  // version string 32 bytes  For ex: "WPI-DEMO VERSION 1.0"
#define VERSION_SN_LEN              64    //32 bytes string

#define PARAM_START_ADDRESS         (0x80000-0x1000)   //  last 4k flash for store param config

#if  defined(EEPROM_ENABLE)
	#define CHARGER_INFO_ADDRESS        (0x00)
#else
	#define CHARGER_INFO_ADDRESS        PARAM_START_ADDRESS
#endif

#define SERVER_IP_ADDRESS           (PARAM_START_ADDRESS - 0x1000)

#endif
