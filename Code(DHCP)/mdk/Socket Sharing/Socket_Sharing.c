#include "cJSON.h"
#include "otaCharger.h"
#include "lib_crc16.h"
#include "UserConfig.h"
#include "FreescaleIAP.h"
#include "api.h"

extern "C"
{
	#include "lpc54608_api.h"
	#include "stdio.h"
	#include "stdlib.h"


}
int notifySendCounter = 0;

void notifyMsgSendHandle(MSGID msgid)
{
		#ifndef NOT_CHECK_NETWORK
    int i,len= 0;
    int crc16;
//    if(notifySendID != invalidID) // can't send msg before prev msg send ok(receive resp msg)
//        return;
    #ifdef DISABLE_NETWORK_CONMUNICATION_FUNC
    if(eventHandle.stopCommunicationFlag == true)
        return;
    #endif
    if((msgid == invalidID)||(msgid >= unknownMsgID))
        return;

    memset(socketInfo.outBuffer,0,sizeof(socketInfo.outBuffer));

    if(msgid == notifyNewDevice) {
        if(strcmp(chargerInfo.meterNumber, NULL) == NULL) {  //20160918 ericyang
            sprintf(socketInfo.outBuffer,NOTIFY_REQ_NewDevice,EVChargerModelNO,false);
        } else {
            sprintf(socketInfo.outBuffer,NOTIFY_REQ_NewDevice,EVChargerModelNO,true);
        }
    }
		/*
		  else if(msgid == notifyCheckVersion) {
        sprintf(socketInfo.outBuffer,NOTIFY_REQ_CheckVersion);
    }*/
		else if(msgid == notifyUpdateVersion) {

        printf("fwSectorNum=%d,otaInfo.currentFWSector=%d\r\n",otaInfo.FWSectorNum,otaInfo.currentFWSector);
        if(otaInfo.currentFWSector < otaInfo.FWSectorNum) {
            if(otaInfo.currentFWSector + 1 == otaInfo.FWSectorNum) {
              sprintf(socketInfo.outBuffer,NOTIFY_REQ_UpdateVersion,otaInfo.latestVersionSNFromServer,otaInfo.currentFWSector * SECTOR_SIZE,otaInfo.lastSectorSize); //orangecai 20170410
							printf("otaInfo.lastFWSectorSize=%d\r\n",otaInfo.lastSectorSize);
							printf("otaInfo.latestVersionSNFromServer:%s!\r\n",otaInfo.latestVersionSNFromServer);
                //eventHandle.getLatestFWFromServerFlag = false;  // be careful!
            } else {
              sprintf(socketInfo.outBuffer,NOTIFY_REQ_UpdateVersion,otaInfo.latestVersionSNFromServer,otaInfo.currentFWSector * SECTOR_SIZE,SECTOR_SIZE); //orangecai 20170410
							printf("otaInfo.currentFWSectorSize=%d\r\n",SECTOR_SIZE);
							printf("otaInfo.latestVersionSNFromServer:%s!\r\n",otaInfo.latestVersionSNFromServer);
            }
        } else {//receive all sectors successful, then write version info to flash
            printf("erase reset ota partition!\r\n");
            if(otaInfo.FWSectorNum < CODE_SECTOR_NUM) {
                for(i=otaInfo.FWSectorNum; i<CODE_SECTOR_NUM; i++) {
                    erase_sector(OTA_CODE_START_ADDRESS+i*SECTOR_SIZE);  
                }
            }

            eventHandle.getLatestFWFromServerFlag = false;  // be careful!
            crc16 = calculate_crc16((char*) OTA_CODE_START_ADDRESS, otaInfo.FWSizeFromServer);
            printf("code crc: %X  checksum %X\r\n ",crc16,otaInfo.FWCheckSumFromServer);
            if(otaInfo.FWCheckSumFromServer == crc16) {
							if(strncmp("Test",otaInfo.latestVersionFromServer+14,4)!=NULL){
                char* cdata = (char*)VERSION_STR_ADDRESS;
                crc16 = calculate_crc16((char*) OTA_CODE_START_ADDRESS, CODE_SIZE);
                printf("get right checksum of bin code, reset new checksum:%4X\r\n",crc16);
                memset(tempBuffer,0x0,VERSION_STR_LEN);
                tempBuffer[0] = cdata[0];
                tempBuffer[1] = cdata[1];
                tempBuffer[2] = (crc16 >> 8) & 0xff;
                tempBuffer[3] = crc16 & 0xff;
                sprintf(tempBuffer+4,"%s",otaInfo.latestVersionFromServer);
                erase_sector(VERSION_STR_ADDRESS);                    
                program_flash(VERSION_STR_ADDRESS,tempBuffer, SECTOR_SIZE);
                eventHandle.updateVersionDoneFlag = true;  // add by orangeCai 20170328
                otaSuccessFlag = true;  //orangecai 20170720								
							}else{
								printf("This version is only for testing flash!no update!\r\n");
							}
            }
						else
							printf("ota checksum error!\r\n");
            return ;
        }
    } else if(msgid == notifyChargerStatus) {
        sprintf(socketInfo.outBuffer,NOTIFY_REQ_ChargerStatus,chargerInfo.status,chargerInfo.connect);
//    } else if(msgid == notifyConnectStatus) {
//        sprintf(socketInfo.outBuffer,NOTIFY_REQ_ConnectStatus,chargerInfo.connect);
    } else if(msgid == notifyChargingInfo) {
        sprintf(socketInfo.outBuffer,NOTIFY_REQ_ChargingInfo,chargerInfo.chargingType,chargerInfo.energy,chargerInfo.voltage,
			  chargerInfo.current,chargerInfo.power,chargerInfo.duration/60,chargerInfo.status,chargerInfo.connect,chargerInfo.setDuration,chargerInfo.setEnergy);
    } else if(msgid == notifyEndCharging) {
        //sprintf(socketInfo.outBuffer,NOTIFY_REQ_EndCharging,chargerInfo.energy,chargerInfo.duration/60,chargerInfo.status,chargerInfo.connect);
			  sprintf(socketInfo.outBuffer,NOTIFY_REQ_EndCharging,chargerInfo.userId,curChargingEnergy,curChargingDuration/60,chargerInfo.status,chargerInfo.connect,setChargingDuration,setChargingEnergy,chargingEndType);// orangecai 20170608
    } else if(msgid == notifyOTAResult){  //modify 20170720
				sprintf(socketInfo.outBuffer,NOTIFY_REQ_OTAResult,otaSuccessFlag,otaInfo.oldVersionId,otaInfo.newVersionId);  
		} else {
    }
    len = strlen(socketInfo.outBuffer);
    printf("notifyMsgSendHandle,send %d bytes: %s\r\n",len,socketInfo.outBuffer);
    netconn_write(tcpsocket,socketInfo.outBuffer,len,NETCONN_COPY);  
    notifySendID = msgid;
    notifySendCounter = systemTimer;
		#endif
}

/* Parse text to JSON, then render back to text, and print! */
void parseRecvMsgInfo(char *text)
{
    cJSON *json;
    //  int respcode;
    json=cJSON_Parse(text);
    if (!json) {
        printf("not json string, start to parse another way!\r\n");
        parseBincodeBuffer(text);
        //printf("Error before: [%s]\r\n",cJSON_GetErrorPtr());
    } else {
        printf("start parse json!\r\n");
        cJSON *data = cJSON_GetObjectItem(json,"data");
        respcode = 0 ;
        if(cJSON_GetObjectItem(json,"reqType") != NULL) {
            msgType = req;
            sprintf(jsonBuffer,"%s",cJSON_GetObjectItem(json,"reqType")->valuestring);
            printf("reqType: %s\r\n",jsonBuffer);
        } else if(cJSON_GetObjectItem(json,"respType") != NULL) {
            msgType = resp;
            sprintf(jsonBuffer,"%s",cJSON_GetObjectItem(json,"respType")->valuestring);
            printf("respType: %s\r\n",jsonBuffer);
        } else {
            msgType = invalidType;
        }
        if(msgType == req) {
            //if(strcmp(jsonBuffer,SETChargingTime) == NULL) {
            //    msgID = setChargingTime;
            //    // chargerInfo.setDuration =  cJSON_GetObjectItem(data,"time")->valueint;
            //    sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
            //    printf("recv msg %s, msgid = %s\r\n",SETChargingTime,msgIdstr);
            //    respcode = 100;
            //    cmdMsgRespHandle(msgID);
            //    // chargerInfo.status = charging;// test only
            //} else  
            if(strcmp(jsonBuffer,SETChargingStart) == NULL) {
                msgID = setChargingStart;
							  if(cJSON_HasObjectItem(data,"type")==1 && cJSON_HasObjectItem(data,"time")==1 
									&& cJSON_HasObjectItem(data,"energy")==1 && cJSON_HasObjectItem(data,"userId")==1){
										sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
										chargerInfo.chargingType = cJSON_GetObjectItem(data,"type")->valueint;
										chargerInfo.userId = cJSON_GetObjectItem(data,"userId")->valueint;
										respcode = RESP_ERROR;
										printf("recv msg %s = %d, msgid = %s\r\n",SETChargingStart, chargerInfo.setDuration,msgIdstr);
                    if(chargerInfo.status == connected){
											if(chargerInfo.chargingType == CHARGING_TYPE_MONEY){
												chargerInfo.setEnergy = cJSON_GetObjectItem(data,"energy")->valuedouble;
												chargerInfo.setDuration = 0;
												printf("charging type:money!\tsetEnergy=%f\r\n",chargerInfo.setEnergy);
												if(chargerInfo.setEnergy > 0){
													#ifdef GB18487_1_2015_AUTH_FUNC
														preStartCharging();
													#else
														startCharging();
													#endif
													respcode = RESP_OK;
													startS2SwitchWaitCounterFlag = true;
													waitS2SwitchOnCounter = 0;
												}
											}else if(chargerInfo.chargingType == CHARGING_TYPE_TIME){
												chargerInfo.setDuration = cJSON_GetObjectItem(data,"time")->valueint;
												chargerInfo.setEnergy = 0;
												printf("charing type:time\tsetDuration=%d\r\n",chargerInfo.setDuration);
												if(chargerInfo.setDuration > 0){
													#ifdef GB18487_1_2015_AUTH_FUNC
														preStartCharging();
													#else
														startCharging();
													#endif
													respcode = RESP_OK;
													startS2SwitchWaitCounterFlag = true;
													waitS2SwitchOnCounter = 0;
												}
											}else{
												printf("charging type error!\r\n");
											}
									}
									cmdMsgRespHandle(msgID);
								}else{
									printf("json item error!\r\n");
								}			
            } else  if(strcmp(jsonBuffer,SETChargingEnd) == NULL) {
                msgID = setChargingEnd;
                // chargerInfo.setDuration =  cJSON_GetObjectItem(data,"time")->valueint;
                sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
                printf("recv msg %s , msgid = %s\r\n",SETChargingEnd,msgIdstr);
							if((chargerInfo.status & 0x03)==charging){
                respcode = RESP_OK;
							#ifdef GB18487_1_2015_AUTH_FUNC
								preStopCharging(); // ericyang 20170111
							#else
							  stopCharging();// ericyang 20161216 move before cmdMsgRespHandle
							#endif
							  chargingEndType = END_IN_ADVANCE;  //20170719
							}else{
								respcode = RESP_ERROR;
							}
                cmdMsgRespHandle(msgID);
            }
						else  if(strcmp(jsonBuffer,SETUpdateVersion) == NULL){ //add by orangeCai , 20170328
							msgID = setUpdateVersion;						
							sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
							sprintf(otaInfo.latestVersionFromServer,cJSON_GetObjectItem(data,"versionNumber")->valuestring);
							sprintf(otaInfo.latestVersionSNFromServer,cJSON_GetObjectItem(data,"versionSN")->valuestring); //orangecai 20170410
							printf("latest versionSN from server:%s\r\n",otaInfo.latestVersionSNFromServer); //orangecai 20170410
							otaInfo.FWSizeFromServer = cJSON_GetObjectItem(data,"versionSize")->valueint;
              otaInfo.FWCheckSumFromServer = cJSON_GetObjectItem(data,"checkSum")->valueint;
							otaInfo.oldVersionId = cJSON_GetObjectItem(data,"oldVersionId")->valueint;   //orangecai 20170414
					    otaInfo.newVersionId = cJSON_GetObjectItem(data,"newVersionId")->valueint;   //orangecai 20170414
							respcode = RESP_OK;
							
							printf("recv msg %s , msgid = %s\r\n",SETUpdateVersion,msgIdstr);
							printf("lastest version : %s , size : %d Bytes , checkSum : %x\r\n",
							          otaInfo.latestVersionFromServer,otaInfo.FWSizeFromServer,otaInfo.FWCheckSumFromServer);
							printf("oldVersionId:%d\tnewVersionId:%d\r\n",otaInfo.oldVersionId,otaInfo.newVersionId);  //orangecai 20170414
							if(otaInfo.FWSizeFromServer % SECTOR_SIZE == 0) {
                    otaInfo.FWSectorNum = otaInfo.FWSizeFromServer / SECTOR_SIZE;
                    otaInfo.lastSectorSize = SECTOR_SIZE;
                } else {
                    otaInfo.FWSectorNum = otaInfo.FWSizeFromServer / SECTOR_SIZE + 1;
                    otaInfo.lastSectorSize = otaInfo.FWSizeFromServer % SECTOR_SIZE;
                }
								
							if((otaInfo.FWSectorNum > CODE_SECTOR_NUM) || (otaInfo.FWSectorNum < MIN_CODE_SECTOR_NUM)) {
                    printf("fw size %d is too large or too small, please check...\r\n",otaInfo.FWSizeFromServer);
                } else {
                    if(strcmp(VERSION_INFO,otaInfo.latestVersionFromServer)) { // fw version is not the same, update flash ota code;
                        eventHandle.getLatestFWFromServerFlag = true; //  get fw code from server;
                        otaInfo.currentFWSector = 0;
                        printf("has new FW version On Server,start to update!\r\n"); // orangecai 20170407
                    } else {
											//  eventHandle.updateVersionDoneFlag = true;   //orangeCai 20170329
                        printf("the fw version is the newest!\r\n");
                    }
                }
							cmdMsgRespHandle(msgID);
						}
						#ifdef RTC_ENABLE
						else if(strcmp(jsonBuffer,SETCalibrateTime) == NULL){
							  uint32_t timeSeconds;
								msgID = setCalibrateTime;
								sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
							  timeSeconds = (uint32_t)cJSON_GetObjectItem(data,"timestamp")->valueint;
							  printf("recv msg %s , msgid = %s\r\n",SETCalibrateTime,msgIdstr);
							  printf("timeSeconds = %d\r\n",timeSeconds);
							  respcode = 100;
							  rtcInit(timeSeconds);  //calibrate rtc
							  cmdMsgRespHandle(msgID);
						}
						#endif
						else  if(strcmp(jsonBuffer,GETChargerStatus) == NULL) {
                msgID = getChargerStatus;
                sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
                printf("recv msg %s , msgid = %s\r\n",GETChargerStatus, msgIdstr);
                respcode = RESP_OK;
                cmdMsgRespHandle(msgID);
            } else  if(strcmp(jsonBuffer,GETChargingInfo) == NULL) {
                msgID = getChargingInfo;
                sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
                printf("recv msg %s , msgid = %s\r\n",GETChargingInfo, msgIdstr);
							  if((chargerInfo.status & 0x03)==charging)
									respcode = RESP_OK;
								else
									respcode = RESP_ERROR;
                cmdMsgRespHandle(msgID);
            }else if(strcmp(jsonBuffer,GETCurVersion) == NULL){
								msgID = getCurVersion;
							  sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
							  printf("recv msg %s , msgid = %s\r\n",GETCurVersion,msgIdstr);
							  respcode = RESP_OK;
							  cmdMsgRespHandle(msgID);
						}
						#ifdef RTC_ENABLE
						else if(strcmp(jsonBuffer,GETDate) == NULL){
								msgID = getDate;
							  sprintf(msgIdstr,cJSON_GetObjectItem(data,"msgId")->valuestring);
							  printf("recv msg %s , msgid = %s\r\n",GETDate,msgIdstr);
							  respcode = RESP_OK;
						}
						#endif
        } else if(msgType == resp) {
            respcode = cJSON_GetObjectItem(data,"respCode")->valueint;
            if(strcmp(jsonBuffer,NOTIFYNewDevice) == NULL) {
                msgID = notifyNewDevice;
                //if(cJSON_GetObjectItem(data,"meterNumber") != NULL) {
                if(cJSON_HasObjectItem(data,"meterNumber") == 1) {
                    sprintf(chargerInfo.meterNumber,cJSON_GetObjectItem(data,"meterNumber")->valuestring);
                    //sprintf(chargerInfo.meterNumber,"15500544");//for test
                    printf("recv msg %s , meterNumber = %s\r\n",NOTIFYNewDevice, chargerInfo.meterNumber);
									  #if defined(EEPROM_ENABLE)
                    saveChargerInfoToEEPROM();    
								    #else
									  saveChargerInfoToFlash();
                    #endif									
									
                } else {
                    #ifdef DISABLE_NETWORK_CONMUNICATION_FUNC
                    eventHandle.stopCommunicationFlag = true;
                    #endif
                    printf("NotifyNewDevice has no meterNumber param, stop Conmunication!\r\n");
                }
            }/* else  if(strcmp(jsonBuffer,NOTIFYCheckVersion) == NULL) {
                msgID = notifyCheckVersion;
                sprintf(otaInfo.latestVersionFromServer,cJSON_GetObjectItem(data,"latestVersion")->valuestring);
                otaInfo.FWSizeFromServer = cJSON_GetObjectItem(data,"versionSize")->valueint;
                otaInfo.FWCheckSumFromServer = cJSON_GetObjectItem(data,"checkSum")->valueint;
                printf("recv msg %s , latestVersion = %s size = %d, checksum= %x\r\n",NOTIFYCheckVersion, otaInfo.latestVersionFromServer,otaInfo.FWSizeFromServer,otaInfo.FWCheckSumFromServer);

                if(otaInfo.FWSizeFromServer % SECTOR_SIZE == 0) {
                    otaInfo.FWSectorNum = otaInfo.FWSizeFromServer / SECTOR_SIZE;
                    otaInfo.lastSectorSize = SECTOR_SIZE;
                } else {
                    otaInfo.FWSectorNum = otaInfo.FWSizeFromServer / SECTOR_SIZE + 1;
                    otaInfo.lastSectorSize = otaInfo.FWSizeFromServer % SECTOR_SIZE;
                }

                if((otaInfo.FWSectorNum > CODE_SECTOR_NUM) || (otaInfo.FWSectorNum < MIN_CODE_SECTOR_NUM)) {
                    printf("fw size %d is too larger or too small, please check...\r\n",otaInfo.FWSizeFromServer);
                } else {
                    if(strcmp(VERSION_INFO,otaInfo.latestVersionFromServer)) { // fw version is not the same, update flash ota code;
                        eventHandle.getLatestFWFromServerFlag = true; //  get fw code from server;
                        otaInfo.currentFWSector = 0;
                        printf("has new FW version On Server\r\n");
                    } else {
                        printf("the fw version is the newest!\r\n");
                    }
                }
                //eventHandle.getLatestFWFromServerFlag = true; //  for test!!!!!!!
            } */else  if(strcmp(jsonBuffer,NOTIFYUpdateVersion) == NULL) {
                msgID = notifyUpdateVersion;
            } else  if(strcmp(jsonBuffer,NOTIFYChargerStatus) == NULL) {
                msgID = notifyChargerStatus;
 //           } else  if(strcmp(jsonBuffer,NOTIFYConnectStatus) == NULL) {
 //               msgID = notifyConnectStatus;
            } else  if(strcmp(jsonBuffer,NOTIFYChargingInfo) == NULL) {
                msgID = notifyChargingInfo;
            } else  if(strcmp(jsonBuffer,NOTIFYEndCharging) == NULL) {
                msgID = notifyEndCharging;
						}	else if(strcmp(jsonBuffer,NOTIFYOTAResult) == NULL){  // orangeCai 20170328
							  msgID = notifyOTAResult;
							} else {
                msgID = invalidID;
            }
            if((notifySendID == msgID)&&(respcode == RESP_OK)) {
                printf("send msg %s successful!\r\n",jsonBuffer);
							  if(notifySendID == notifyOTAResult && otaSuccessFlag == true){
									updateCodeFlag = true;  //start to update code when charger is not charging
									otaSuccessFlag = false;  //orangecai 20170720
								}
                notifySendID = invalidID;
            } else {
                //  notifyMsgSendHandle(notifySendID);  //if notify not get right resp, resend notify
            }
        } else {
        }
        cJSON_Delete(json);
    }
}