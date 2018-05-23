/*
* Note:last modification by Derek Zhu in 20180410
*/

#ifndef USERCONFIG_H
#define USERCONFIG_H


#define DEBUG_MODE  1       //when in debug mode,the device connect to another server port:22222 



#define EEPROM_ENABLE

	
/* ---------------------------------- Socket configure --------------------------------- */
#define SOCKET_OUT_BUFFER_SIZE 256
#define SOCKET_IN_BUFFER_SIZE (4096+256)
#define SOCKET_RESEND_TIME 5  // if msg send not success, msg will be resend after 5s 

/* ----------------------------------- Type definitin ------------------------------------*/
typedef struct _EventFlag {
    bool heatbeatFlag;
    bool getLatestFWFromServerFlag; 
	  bool firstConnectFlag ; 
		bool downLoadFinshFWFromServerFlag;

} SystemEventHandle;

typedef struct _SocketInfo {
    char inBuffer[SOCKET_IN_BUFFER_SIZE];
    char outBuffer[SOCKET_OUT_BUFFER_SIZE];
} SocketInfo;



/* ------------------------ Global variables and functions ------------------------ */

extern char versionSN [];
extern int systemTimer;
extern SystemEventHandle eventHandle;
extern SocketInfo socketInfo;

extern char tempBuffer[];  // crc16 Buffer

extern char dataBuffer[];  // EEPROM Buffer

extern int respond;   // api respond

extern int respcode;  // api respond Status Code

extern char allPortStatus[];  

extern int duration[];

extern int setDuration[];

extern char allPortStatusBuffer[];

extern bool getLatestFWFromServerFlag;

extern bool upDateBinFlag;

extern int otaBinTotalSize;


#endif
