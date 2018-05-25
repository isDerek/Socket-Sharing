#include "cJSON.h"
#include "ota.h"
#include "lib_crc16.h"
#include "UserConfig.h"
#include "api.h"
#include "Socket_Sharing.h"

extern "C"
{
	#include "lpc54608_api.h"
	#include "stdio.h"
	#include "stdlib.h"


}
#define SECTOR_SIZE 32768
MSGID msgID = invalidID;
MSGID notifySendID = invalidID;  // send notify to server flag
extern struct netconn  *tcpsocket;
int notifySendCounter = 0;
char msgIdstr[10];
int respcode ;
char jsonBuffer[256];
ChargerInfo chargerInfo;
ChargerException chargerException;
OTAInfo otaInfo;
u16_t len1;
int otaBinTotalSize;
/********************************************recvMsgHandle()*****************************/
void recvMsgHandle(void)   //designed by derek 2017.11.17
{
    int len;
		
	void *data;
	struct netbuf *buf ;
    // int n = tcpsocket.receive_all(socketInfo.inBuffer, len);
	if(( netconn_recv(tcpsocket, &buf)) == ERR_OK)
	{
		len = sizeof(socketInfo.inBuffer);
    memset(socketInfo.inBuffer,0x0,len);
			if((netbuf_data(buf, &data, &len1)) == ERR_OK)	  
			{
				memcpy(socketInfo.inBuffer,data,len1);
				socketInfo.inBuffer[len1] = '\0';    
        printf("receive %d bytes\r\n", len1);  
        printf("receive : %s \r\n",socketInfo.inBuffer);        		
				parseRecvMsgInfo(socketInfo.inBuffer);
				netbuf_delete(buf);   	
			}
	}
		else 
		{
			netbuf_delete(buf);
		}
         
	
	
}
void cmdMsgRespHandle(MSGID msgid)
{
    int len;

    if((msgid == invalidID)||(msgid >= unknownMsgID))
        return;
    memset(socketInfo.outBuffer,0,sizeof(socketInfo.outBuffer));
    if(msgid == notifyStartCharing) 
		{
			  chargerInfo.apiId = 21;			
				sprintf(socketInfo.outBuffer,CMD_RESP_startCharging,chargerInfo.msgId,chargerInfo.apiId,respcode);
    } 
		else if(msgid == notifyEndCharging) 
		{
			  chargerInfo.apiId = 22;			
				sprintf(socketInfo.outBuffer,CMD_RESP_endCharging ,chargerInfo.msgId,chargerInfo.apiId,respcode);
    }
		else if(msgid == notifygetPortStatus)
		{  
			  chargerInfo.apiId = 23;
				if(chargerInfo.index == 100)
				{
					sprintf(socketInfo.outBuffer,CMD_RESP_getAllPortStatus ,chargerInfo.msgId,chargerInfo.apiId,respcode,chargerInfo.allPortStatus[0],chargerInfo.allPortStatus[1],chargerInfo.allPortStatus[2],chargerInfo.allPortStatus[3],chargerInfo.allPortStatus[4],chargerInfo.allPortStatus[5],chargerInfo.allPortStatus[6],chargerInfo.allPortStatus[7],chargerInfo.allPortStatus[8],chargerInfo.allPortStatus[9],chargerInfo.allPortStatus[10],chargerInfo.allPortStatus[11],chargerInfo.allPortStatus[12],chargerInfo.allPortStatus[13],chargerInfo.allPortStatus[14],chargerInfo.allPortStatus[15]);
				}
				else
				{
					sprintf(socketInfo.outBuffer,CMD_RESP_getPortStatus ,chargerInfo.msgId,chargerInfo.apiId,respcode,chargerInfo.portStatus[chargerInfo.index][0],chargerInfo.portStatus[chargerInfo.index][1]);
				}
		} 
		else if(msgid == notifysetAllPortStatus) 
		{
				chargerInfo.apiId = 51;
				sprintf(socketInfo.outBuffer,CMD_RESP_setAllPortStatus ,chargerInfo.msgId,chargerInfo.apiId,respcode);
		}
		else if(msgid == notifyOTAUpdate)
		{
				chargerInfo.apiId = 24;
				sprintf(socketInfo.outBuffer,CMD_RESP_otaUpdate ,chargerInfo.msgId,chargerInfo.apiId,respcode);
		}
		else
		{
		}
    len = strlen(socketInfo.outBuffer);
    netconn_write(tcpsocket,socketInfo.outBuffer, len,NETCONN_COPY);   
		printf("cmdMsgRespHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
}
void notifyMsgSendHandle(MSGID msgid)
{
		#ifndef NOT_CHECK_NETWORK
    int len= 0;
    #ifdef DISABLE_NETWORK_CONMUNICATION_FUNC
    if(eventHandle.stopCommunicationFlag == true)
        return;
    #endif
    if((msgid == invalidID)||(msgid >= unknownMsgID))
        return;
    memset(socketInfo.outBuffer,0,sizeof(socketInfo.outBuffer));
    if(msgid == notifyNewDevice) 
		{
      chargerInfo.apiId = 1;		
//			printf(" MAC = %s\n\r VersionSN = %s\n\r",chargerInfo.mac,otaInfo.versionSN);
			sprintf(socketInfo.outBuffer,NOTIFY_REQ_newDevice,chargerInfo.apiId,otaInfo.versionSN,chargerInfo.mac,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false);
			len = strlen(socketInfo.outBuffer);
			printf("notifyMsgSendHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
			netconn_write(tcpsocket,socketInfo.outBuffer,len,NETCONN_COPY);  
    }
		else if(msgid == notifyUpdateVersion) 
		{
			uint16_t upDateBinCounter;
			int upDateCount;
			int otacrc16,codecrc16;			
			char* cdata = (char*)VERSION_STR_ADDRESS;
			int appBinTotalSize;
			upDateCount = (otaInfo.versionSize / 512) + 1;
			PRINTF("UPdateCount = %d ¡\\r\n",upDateCount);
		for(upDateBinCounter = 0;upDateBinCounter<upDateCount;upDateBinCounter++)
			{		
			chargerInfo.apiId = 4;
			otaInfo.blockSize = 512;	
			otaInfo.blockOffset = 512;				
			sprintf(socketInfo.outBuffer,NOTIFY_REQ_updateVersion,chargerInfo.apiId,otaInfo.versionSN,otaInfo.blockOffset*upDateBinCounter,otaInfo.blockSize);
      len = strlen(socketInfo.outBuffer);
			printf("notifyMsgSendHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
			netconn_write(tcpsocket,socketInfo.outBuffer,len,NETCONN_COPY); 
    	recvMsgHandle();	

			}
		  appBinTotalSize = (cdata[7]<<16)|(cdata[8]<<8)|(cdata[9]);
			codecrc16 = calculate_crc16((char*)(CODE_START_ADDRESS), appBinTotalSize);
			otacrc16 = calculate_crc16((char*)(OTA_CODE_START_ADDRESS), otaBinTotalSize);
			memset(tempBuffer,0x0,VERSION_STR_LEN);		
			tempBuffer[0] = (codecrc16 >> 8) & 0xff;  // appcodechecksum
			tempBuffer[1] = codecrc16 & 0xff; // appcodechecksum			
			tempBuffer[2] = (otacrc16 >> 8) & 0xff;  // otacodechecksum
			tempBuffer[3] = otacrc16 & 0xff; // otacodechecksum
			tempBuffer[4] = (otaBinTotalSize >> 16) & 0xff; // otatotal real size
			tempBuffer[5] = (otaBinTotalSize >> 8) & 0xff;// otatotal real size
			tempBuffer[6] = (otaBinTotalSize) & 0xff;// otatotal real size
			tempBuffer[7] = (appBinTotalSize >> 16) & 0xff; // otatotal real size
			tempBuffer[8] = (appBinTotalSize >> 8) & 0xff;// otatotal real size
			tempBuffer[9] = (appBinTotalSize) & 0xff;// otatotal real size		
			tempBuffer[10] = '1'; //1 means finish the first boot , 0 is not
			printf("otadownloadfinshedtotalsize = %d  appdownloadfinshedtotalsize = %d\n\r",otaBinTotalSize,appBinTotalSize);
			sprintf(tempBuffer+11,"%s",otaInfo.versionSN);
			__disable_irq();
			erase_sector(VERSION_STR_ADDRESS);
			program_flash(VERSION_STR_ADDRESS,tempBuffer, 256);
			msgid = invalidID;
			__enable_irq();
			if(otacrc16 == otaInfo.checkSum)
				{	
					chargerInfo.deviceStatus = 10;
					notifyMsgSendHandle(notifyOTAResult);
				}
			else
				{
					chargerInfo.deviceStatus = 11;
					notifyMsgSendHandle(notifyOTAResult);
				}
				
			updateCode();
    }  
		else if(msgid == notifygetPortStatus)
		{
        chargerInfo.apiId = 3;			
				chargerInfo.deviceStatus = 1;
				sprintf(socketInfo.outBuffer,NOTIFY_REQ_deviceStatus,chargerInfo.apiId,chargerInfo.deviceStatus,chargerInfo.portStatus[chargerInfo.index][0],chargerInfo.portStatus[chargerInfo.index][1]);
				len = strlen(socketInfo.outBuffer);
				printf("notifyMsgSendHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
				netconn_write(tcpsocket,socketInfo.outBuffer,len,NETCONN_COPY);  
		}
		else if(msgid == notifyOTAResult)
		{
				chargerInfo.apiId = 3;			
				sprintf(socketInfo.outBuffer,NOTIFY_REQ_otaDeviceStatus,chargerInfo.apiId,chargerInfo.deviceStatus);
				len = strlen(socketInfo.outBuffer);
				printf("notifyMsgSendHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
				netconn_write(tcpsocket,socketInfo.outBuffer,len,NETCONN_COPY);  
		}
		else 
		{
			
    } 
    notifySendID = msgid;
    notifySendCounter = systemTimer;
		#endif
}


/* Parse text to JSON, then render back to text, and print! */
void parseRecvMsgInfo(char *text)
{
	int i;
  cJSON *json;
   
	json=cJSON_Parse(text);
    if (!json)
		{
        printf("not json string, start to parse another way!\r\n");
        parseBincodeBuffer(text);
    } 
		else 
		{
        printf("start parse json!\r\n");
        respcode = 0 ; 
				chargerInfo.apiId = cJSON_GetObjectItem(json,"apiId")->valueint;
      	if(chargerInfo.apiId == 1)
				{
					respcode = cJSON_GetObjectItem(json,"respCode")->valueint;
					printf("apiId = %d, respCode = %d \r\n ",chargerInfo.apiId,respcode);
					if( respcode == 100)
					{
						notifySendID = invalidID;
					}
				}
				else if(chargerInfo.apiId == 2)
				{
					respcode = cJSON_GetObjectItem(json,"respCode")->valueint;
					printf("apiId = %d, respCode = %d \r\n ",chargerInfo.apiId,respcode);
					if( respcode == 100)
					{
						notifySendID = invalidID;
					}
				}
				else if(chargerInfo.apiId == 3)
				{
					respcode = cJSON_GetObjectItem(json,"respCode")->valueint;
					printf("apiId = %d, respCode = %d \r\n ",chargerInfo.apiId,respcode);
					if( respcode == 100)
					{
						notifySendID = invalidID;
					}
				}
				else if(chargerInfo.apiId == 21)
				{
					chargerInfo.index = cJSON_GetObjectItem(json,"index")->valueint;
					chargerInfo.setDuration[chargerInfo.index] = cJSON_GetObjectItem(json,"setDuration")->valueint;
					sprintf(chargerInfo.msgId,"%s",cJSON_GetObjectItem(json,"msgId")->valuestring);
					printf("msgId = %s, apiId = %d, index = %d, setDuration = %d \n\r",chargerInfo.msgId,chargerInfo.apiId,chargerInfo.index,chargerInfo.setDuration[chargerInfo.index]);
          					
				}
				else if(chargerInfo.apiId == 22)
				{
					chargerInfo.index = cJSON_GetObjectItem(json,"index")->valueint;
					sprintf(chargerInfo.msgId,"%s",cJSON_GetObjectItem(json,"msgId")->valuestring);
					printf("msgId = %s, apiId = %d, index = %d\n\r",chargerInfo.msgId,chargerInfo.apiId,chargerInfo.index);		
				}
				else if(chargerInfo.apiId == 23)
				{
					chargerInfo.index = cJSON_GetObjectItem(json,"index")->valueint;
					sprintf(chargerInfo.msgId,"%s",cJSON_GetObjectItem(json,"msgId")->valuestring);					
					printf("msgId = %s, apiId = %d, index = %d\n\r",chargerInfo.msgId,chargerInfo.apiId,chargerInfo.index);
					chargerInfo.portStatus[chargerInfo.index][0] = chargerInfo.index;  //first time to get port status
				}
				else if(chargerInfo.apiId == 24)
				{
					int otacodechecksum;
					sprintf(otaInfo.versionSN,"%s",cJSON_GetObjectItem(json,"versionSN")->valuestring);
					otaInfo.versionSize = cJSON_GetObjectItem(json,"versionSize")->valueint;
					sprintf(chargerInfo.msgId,"%s",cJSON_GetObjectItem(json,"msgId")->valuestring);
					otaInfo.checkSum = cJSON_GetObjectItem(json,"checkSum")->valueint;
					printf("versionSN = %s\n\r",otaInfo.versionSN);
					printf("msgId = %s, apiId = %d,  versionSN = %s,versionSize =%d, checkSum = %d \n\r",chargerInfo.msgId,chargerInfo.apiId,otaInfo.versionSN,otaInfo.versionSize,otaInfo.checkSum);		
					char* cdata = (char*)VERSION_STR_ADDRESS;
					otacodechecksum = (cdata[2]<<8)|cdata[3];

					if(otacodechecksum != otaInfo.checkSum)
					{					
						respcode = 100;
						cmdMsgRespHandle(notifyOTAUpdate);
						eventHandle.getLatestFWFromServerFlag = true; 
					}
					else
					{
						respcode = 101;
						cmdMsgRespHandle(notifyOTAUpdate);
						eventHandle.getLatestFWFromServerFlag = false; 
					}
					
				}
				else if(chargerInfo.apiId == 51)
				{
					chargerInfo.index = cJSON_GetObjectItem(json,"index")->valueint;
				  cJSON *array = cJSON_GetObjectItem(json,"portStatus");
					int size = cJSON_GetArraySize(array);
//					printf("size = %d\n\r",size);
					for(i=0;i<size;i++)
					{
					cJSON *object =  cJSON_GetArrayItem(array,i);
					jsonBuffer[i]= object->valueint;
//					printf("array %d = %d\n\r",i,jsonBuffer[i]);
					}
					memset(chargerInfo.allPortStatusBuffer,0,sizeof(chargerInfo.allPortStatusBuffer));
					memcpy(chargerInfo.allPortStatusBuffer,jsonBuffer,size);
					sprintf(chargerInfo.msgId,"%s",cJSON_GetObjectItem(json,"msgId")->valuestring);
					printf("msgId = %s, apiId = %d, portStatus = [%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\n\r",chargerInfo.msgId,chargerInfo.apiId,chargerInfo.allPortStatusBuffer[0],chargerInfo.allPortStatusBuffer[1],chargerInfo.allPortStatusBuffer[2],chargerInfo.allPortStatusBuffer[3],chargerInfo.allPortStatusBuffer[4],chargerInfo.allPortStatusBuffer[5],chargerInfo.allPortStatusBuffer[6],chargerInfo.allPortStatusBuffer[7],chargerInfo.allPortStatusBuffer[8],chargerInfo.allPortStatusBuffer[9],chargerInfo.allPortStatusBuffer[10],chargerInfo.allPortStatusBuffer[11],chargerInfo.allPortStatusBuffer[12],chargerInfo.allPortStatusBuffer[13],chargerInfo.allPortStatusBuffer[14],chargerInfo.allPortStatusBuffer[15]);		
				}
				else 
				{
				}
    } 
        cJSON_Delete(json);
    }
/*
        HEADER     BLOCK OFFSET       BLOCK SIZE  CHECKSUM    Bin data
       "OTABIN" 0x00 0x00 0x00 0x00    0x10 0x00  0xFF 0xFF
*/
#define BLOCKOFFSET_POS 6
#define BLOCKSIZE_POS 10
#define CHECKSUM_POS 12
#define BINDATA_POS 14
void parseBincodeBuffer(char *text)
{
    char* buf = NULL;
    int blockOffset = 0;
    int blockSize = 0;
    int checksum = 0;
    int crc16 = NULL;
	  int reWrite = 0;  //orangecai
	
    if(strncmp(text, SOCKET_OTA_HEADER, strlen(SOCKET_OTA_HEADER))== NULL) {
        blockOffset = ((text[BLOCKOFFSET_POS] << 24) | (text[BLOCKOFFSET_POS+1] << 16) |(text[BLOCKOFFSET_POS+2] << 8)|text[BLOCKOFFSET_POS+3]);
        blockSize = ((text[BLOCKSIZE_POS] << 8) | text[BLOCKSIZE_POS+1]);
        checksum = ((text[CHECKSUM_POS] << 8) | text[CHECKSUM_POS+1]);
        printf("read from server: blockoffset = %d blocksize = %d checksum = %x\r\n",blockOffset,blockSize,checksum);
        buf = (char*)malloc(blockSize);
        if(buf == NULL) 
				{
            printf("can't malloc enough memory!\r\n");
        } 
				else 
					{
            if(notifySendID == notifyUpdateVersion) 
							{
                notifySendID = invalidID;
							}
            memcpy(buf,text+BINDATA_POS,blockSize);
						crc16 = calculate_crc16(buf, len1-BINDATA_POS);
            printf("crc16 = %x checkSum = %x\r\n",crc16,checksum);
            if( crc16 == checksum) {
                // update current sector to flash ota partition;
								int iapCode;
							  strcpy(versionSN,otaInfo.versionSN);
								__disable_irq();
								erase_page(OTA_CODE_START_ADDRESS+blockOffset,OTA_CODE_START_ADDRESS+blockOffset+blockSize);
								iapCode = program_flash(OTA_CODE_START_ADDRESS + blockOffset,buf, blockSize);  							
								otaInfo.blockOffset = blockOffset;
								strcpy(otaInfo.versionSN,versionSN);
								crc16 = calculate_crc16((char*)(OTA_CODE_START_ADDRESS + blockOffset),len1-BINDATA_POS);
//								printf("crc16 rewrite before = %x\n\r",crc16);
								while(crc16 != checksum && reWrite < 10){   //try to rewrite data when verify failed
									printf("rewrite \r\n");
									erase_page(OTA_CODE_START_ADDRESS+blockOffset,OTA_CODE_START_ADDRESS+blockOffset+blockSize);
									iapCode = program_flash(OTA_CODE_START_ADDRESS + blockOffset,buf, len1-BINDATA_POS); 
									printf("iapCode=%d\r\n",iapCode);
									crc16 = calculate_crc16((char*)(OTA_CODE_START_ADDRESS + blockOffset),len1-BINDATA_POS);
									printf("crc16 rewrite later = %x\n\r",crc16);
									reWrite++;
								}
								otaBinTotalSize += (len1-BINDATA_POS);
								printf("rewrite times = %d\r\n",reWrite);
								if(crc16 == checksum)
									{
									printf("write and verify successfully\r\n");
									}
								else
									{
									printf("write failed!\r\n");
									eventHandle.getLatestFWFromServerFlag = false;   //stop update!
									NVIC_SystemReset();  //restart to recover ota
									}	
					
            }	
         
        }
			free(buf);
    }
		else
		{
		}
		__enable_irq();
}

void saveChargerInfoToEEPROM(void)
{
	uint16_t crc16;
	crc16 = calculate_crc16((char*)(&chargerInfo),sizeof(chargerInfo));
	tempBuffer[0] = crc16 >> 8;
	tempBuffer[1] = crc16 & 0xFF;
	memcpy(tempBuffer+2,(char*)(&chargerInfo),sizeof(chargerInfo));	
	if(writeToEEPROM((uint8_t)CHARGER_INFO_ADDRESS,tempBuffer,(uint8_t)(sizeof(chargerInfo)+2)))
	{
		printf("save chargerInfo successfully\r\n");
	}
	else
	{
		printf("fail to save chargerInfo\r\n");
	}
}

void setDefaultChargerInfoToEEPROM(void)
{
	for(int i =0;i<16;i++)
	{
  chargerInfo.setDuration[i] = 0;
	chargerInfo.duration[i] = 0;
	}
  saveChargerInfoToEEPROM(); 
}

void loadChargerInfoFromEEPROM(void)
{
	uint16_t crc16,checksum;
	char* cdata = (char*)malloc(sizeof(chargerInfo)+2);
	if(NULL == cdata){
		printf("not enough memory!\r\n");
		return;
	}
	readFromEEPROM((uint8_t)CHARGER_INFO_ADDRESS,cdata,(uint8_t)(sizeof(chargerInfo)+2));
	crc16 = calculate_crc16(cdata+2,sizeof(chargerInfo));
	checksum = (cdata[0]<<8) | cdata[1];
	if(crc16 == checksum){
		printf(" chargerinfo check ok! reload parm to ram!\r\n");
    memcpy(&chargerInfo, cdata+2, sizeof(ChargerInfo));
	}else{
		printf("eeprom param check fail, set default info to eeprom now!\r\n");
    setDefaultChargerInfoToEEPROM();
    return;
	}

}

