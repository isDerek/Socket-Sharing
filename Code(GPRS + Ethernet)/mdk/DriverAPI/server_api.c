#include "Socket_Sharing.h"
#include "cJSON.h"
#include "ota.h"
#include "lib_crc16.h"
#include "UserConfig.h"
#include "api.h"
#include "stdio.h"
	#include "lpc54608_api.h"
	#include "stdio.h"
	#include "stdlib.h"
	#include "fsl_usart.h"



int respond = 0; 

/*************************************respond  api*****************************************************/
void apiRespond()
{
			if(respond == 1)  // device handle success
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
				cmdMsgRespHandle(notifyOTAUpdate);
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
