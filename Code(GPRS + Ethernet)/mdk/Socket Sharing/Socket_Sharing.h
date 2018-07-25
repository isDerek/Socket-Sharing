/*
* Note:last modification by orange cai in 20170720
*/

#ifndef SocketSharing_H
#define SocketSharing_H
#include "UserConfig.h"
#include "flashLayout.h"
#include "stdint.h"
typedef enum {
    RESP_OK = 100,
		RESP_PARAM_ERROR = 101,
		RESP_ILLEGAL_REQUEST = 102,
		RESP_ITEM_ERROR = 103
} RespCode;

typedef enum {
    invalidID = 0,
    setPortStart,   
    setPortEnd,    
	  getPortStatus,
	  notifyUpdateVersion,  
	  notifyNewDevice,       
		notifyStartCharing,    
	  notifyEndCharging,    
	  notifygetPortStatus,   
		notifysetAllPortStatus, 
		notifyOTAUpdate,
		notifyOTAResult,      
    unknownMsgID
		
} MSGID;


typedef struct {   
    int   setDuration[16]; 
    int   duration[16]; 
		int   apiId;           
		int   index;          
		int   deviceStatus;    
	  int		portStatus[16][2]; 
		char  allPortStatus[20];  
	  char  allPortStatusBuffer[20];
		char  msgId[10];       	
	  char  mac[12];
} ChargerInfo;


typedef struct{
    bool serverConnectedFlag;  // device can't connect to server
} ChargerException;

typedef struct _OTAInfo {
	  int blockOffset;      
		int blockSize;        
		int versionSize;      
		int checkSum ;   
		char versionSN[33];   
} OTAInfo;


#define SOCKET_OTA_HEADER  "OTABIN"

#ifdef  EEPROM_ENABLE
	#define  EEPROM_ADDRESS_WRITE      0xA0         
	#define  EEPROM_ADDRESS_READ       0xA1
	#define  EEPROM_PAGE_SIZE           32  
#endif

extern ChargerInfo chargerInfo;
extern ChargerException chargerException;
extern OTAInfo otaInfo;
extern MSGID notifySendID;
extern int notifySendCounter;

void parseRecvMsgInfo(char *text);
void parseBincodeBuffer(char *text);
void notifyMsgSendHandle(MSGID msgid);
void cmdMsgRespHandle(MSGID msgid);
void chargingExceptionHandle(void);
void recvMsgHandle(void);

#if defined(EEPROM_ENABLE)
void saveChargerInfoToEEPROM(void);
void loadChargerInfoFromEEPROM(void);
#else
void loadChargerInfoFromFlash(void);
void saveChargerInfoToFlash(void);
#endif
#endif

#define	NOTIFY_REQ_otaDeviceStatus	 "{\"apiId\":%d,\"deviceStatus\":%d}"		
#define NOTIFY_REQ_newDevice         "{\"apiId\":%d,\"versionSN\":\"%s\",\"mac\":\"%s\",\"reconnect\":%d,\"portStatus\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}"  
#define NOTIFY_REQ_heartPackage      "{\"apiId\":%d}"
#define NOTIFY_REQ_deviceStatus      "{\"apiId\":%d,\"deviceStatus\":%d,\"portStatus\":[%d,%d]}"
#define NOTIFY_REQ_updateVersion     "{\"apiId\":%d,\"versionSN\":\"%s\",\"blockOffset\":%d,\"blockSize\":%d}"
#define NOTIFY_REQ_startCharging     "{\"msgId\":\"%s\",\"apiId\":%d,\"index\":%d,\"setDuration\":%d}"
#define NOTIFY_REQ_endCharging       "{\"msgId\":\"%s\",\"apiId\":%d,\"index\":%d}"
#define NOTIFY_REQ_getPortStatus     "{\"msgId\":\"%s\",\"apiId\":%d,\"index\":%d}"
#define NOTIFY_REQ_otaResult         "{\"msgId\":\"%s\",\"apiId\":%d,\"versionSN\":\"%s\",\"versionSize\":%d,\"checkSum\":\"%s\"}"
#define NOTIFY_REQ_setAllPortStatus  "{\"msgId\":\"%s\",\"apiId\":%d,\"portStatus\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}"

#define CMD_RESP_newDevice           "{\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_heartPackage        "{\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_deviceStatus        "{\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_updateVersion       0   \\ none
#define CMD_RESP_startCharging       "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_endCharging         "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_getPortStatus       "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d,\"portStatus\":[%d,%d]}"
#define CMD_RESP_getAllPortStatus    "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d,\"portStatus\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}"
#define CMD_RESP_otaUpdate           "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d}"
#define CMD_RESP_setAllPortStatus    "{\"msgId\":\"%s\",\"apiId\":%d,\"respCode\":%d}"	

#define NOTIFY_REQ_newDevice_GPRS    "{\"apiId\":%d,\"versionSN\":\"%s\",\"mac\":\"%s\",\"reconnect\":%d,\"portStatus\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}\n\r"

