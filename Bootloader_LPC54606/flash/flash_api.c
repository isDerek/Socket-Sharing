#include "LPC54608_features.h"
#include "stdint.h"
#include "fsl_flashiap.h"
#include "flash_api.h"
#include "fsl_debug_console.h"
uint32_t page_buf[FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES / sizeof(uint32_t)];


int program_flash(int address,char *data,unsigned int length)
{
		uint32_t SECTOR_SIZE = 32768;
		uint32_t i,status;
		uint32_t Sector_Num;
		Sector_Num = (address/SECTOR_SIZE);   // address/32K   added by derek 2017.12.25
    PRINTF("\r\nWriting flash sector \n Sector_Num = %d",Sector_Num);
	
    /* Generate data to be written to flash */
    for (i = 0; i < length; i++)
    {
        *(((uint8_t *)(&page_buf[0])) + i) = data[i];
    }
    /* Program sector */

        FLASHIAP_PrepareSectorForWrite(Sector_Num, Sector_Num);
        FLASHIAP_CopyRamToFlash(
            Sector_Num*FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES , &page_buf[0],
            length, SystemCoreClock);
    
    /* Verify sector contents */

        status = FLASHIAP_Compare(
            Sector_Num*FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES , &page_buf[0],
            length);

        if (status != kStatus_FLASHIAP_Success)
        {
            PRINTF("\r\nSector Verify failed\n");
            
        }
				else
				{
					PRINTF("\r\nSector Verify Success\n");
					return 0;
				}
    

   
}


void erase_sector(int address)
{
		uint32_t SECTOR_SIZE = 32768;
		uint32_t status;
	  uint32_t Sector_Num;
		Sector_Num = (address/SECTOR_SIZE);   // address/32K   added by derek 2017.12.25
    PRINTF("\r\nErasing flash sector \n\r Sector_Start_Num = %d",Sector_Num);
    FLASHIAP_PrepareSectorForWrite(Sector_Num, Sector_Num);
    FLASHIAP_EraseSector(Sector_Num, Sector_Num, SystemCoreClock);
    status = FLASHIAP_BlankCheckSector(Sector_Num, Sector_Num);
			
    if (status != kStatus_FLASHIAP_Success)
    {
        PRINTF("\r\nSector Erase failed");
    }
		else
		{
			PRINTF("\r\nErase flash sector success\n");
		}
}


