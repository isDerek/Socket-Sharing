#include "LPC54608_features.h"
#include "stdint.h"
#include "fsl_flashiap.h"
#include "fsl_debug_console.h"
uint32_t page_buf[FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES / sizeof(uint32_t)];

int program_flash(int address,char *data,unsigned int length)
{
		uint32_t SECTOR_SIZE = 32768;
		uint32_t i,status;
		uint32_t Sector_Num;
		Sector_Num = (address/SECTOR_SIZE);   // address/32K   
    PRINTF("\r\nWriting flash sector \n Sector_Num = %d length = %d \n\r",Sector_Num, length);	
    /* Generate data to be written to flash */
    for (i = 0; i < length; i++)
    {
        *(((uint8_t *)(&page_buf[0])) + i) = data[i];
    }
    /* Program sector */
        FLASHIAP_PrepareSectorForWrite(Sector_Num, Sector_Num);
        FLASHIAP_CopyRamToFlash(
            address , &page_buf[0],
            length, SystemCoreClock);   
    /* Verify sector contents */
        status = FLASHIAP_Compare(
            address , &page_buf[0],
            length);
        if (status != kStatus_FLASHIAP_Success)
        {
            PRINTF("\r\nSector Verify failed\n");            
        }
				else
				{
//					PRINTF("\r\nSector Verify Success\n");
					return 0;
				}
  return 1; 
}
void erase_page(int startaddress,int endaddress)
{
		uint32_t SECTOR_SIZE = 32768;
		uint32_t PAGE_SIZE = 256;	
	  uint32_t Sector_Num;
		uint32_t Start_Page_Num;
	  uint32_t End_Page_Num;
		Sector_Num = (startaddress/SECTOR_SIZE);   // address/32K   
	  if((endaddress - startaddress)<=256){
			Start_Page_Num = startaddress/PAGE_SIZE;
			End_Page_Num = endaddress/PAGE_SIZE;
		}
		else{
		Start_Page_Num = startaddress/PAGE_SIZE;
	  End_Page_Num = endaddress/PAGE_SIZE-1;
		}
    PRINTF("\r\nErasing flash sector \n\r Sector_Start_Num = %d \n\r Page_Start_Num = %d \n\r Page_End_Num = %d",Sector_Num,Start_Page_Num,End_Page_Num);
    /* Erase page */
    FLASHIAP_PrepareSectorForWrite(Sector_Num, Sector_Num);
    FLASHIAP_ErasePage(Start_Page_Num,
                       End_Page_Num,
                       SystemCoreClock);
}

void erase_sector(int address)
{
		uint32_t SECTOR_SIZE = 32768;
		uint32_t status;
	  uint32_t Sector_Num;
		Sector_Num = (address/SECTOR_SIZE);   // address/32K   
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


