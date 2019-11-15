
/*
Copyright (c) 2019, All rights reserved.

File         : klink.h
Status       : Current
Description  : 

Author       : haopeng
Contact      : 376915244@qq.com

Revision     : 2019-10 
Description  : Primary released

## Please log your description here for your modication ##

Revision     : 
Modifier     : 
Description  : 

*/
#include<unistd.h>
#include "apmib.h"

#ifndef EXIT_FAILURE		
#define EXIT_FAILURE               1
#endif
#ifndef EXIT_SUCCESS		
#define EXIT_SUCCESS               0
#endif

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif

#define FALSE                      0
#define TRUE                       1
#define RETURN_SUCC                0
#define RETURN_FAIL                1
#define KLINK_PORT                 5001
#define MAX_CLIENT                 400
#define KLINK_IF               "wlan0"
#define MAX_SLAVE_NUM              15
#define ENUM_DEFAULT               0
#define HEART_BEAT_TIME_SCHEDULE   10    /*seconds*/
#define MESSAGE_BUFFER_SIZE        512
#define SYNC_FLAG_1                1   //prepare sync
#define SYNC_FLAG_0                0   //already sync 
#define DEFAULT_SN                 "000000000000000000000"


typedef enum  
{
  SLAVE0=ENUM_DEFAULT,SLAVE1,SLAVE2,SLAVE3,SLAVE4,SLAVE5,SLAVE6,SLAVE7,
  SLAVE8,SLAVE9,SLAVE10,SLAVE11,SLAVE12,SLAVE13,SLAVE14,SLAVE15,SLAVE16,   
}klinkMeshDeviceIndex;  

typedef enum  
{
  KLINK_START=ENUM_DEFAULT,
  	
  KLINK_SLAVE_REPORT_DEVICE_INFO=1,   
  KLINK_MASTER_REPORT_DEVICE_ACK=2,    

  KLINK_HEARD_BEAD_SYNC_MESSAGE=3,   
  
  KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE=4, 
  KLINK_SALAVE_SEND_LED_SWITCH_ACK=5, 

  KLINK_MASTER_SEND_WIFI_CFG_INFO_TO_SLAVE=6, 
  KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK=7, 

  KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE=8, 
  KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK=9, 
  
}klinkMsgStateMachine_t;  



typedef enum 
{
  KLINK_CREATE_TOPOLOGY_LINK_LIST=ENUM_DEFAULT,
  KLINK_SLAVE_SOFT_VERSION,
}klinkDataType_t;

/*klink slave version information */
typedef struct KlinkSlaveVersion
{
	char slaveSoftVer[18];
	char slaveMac[18];
}KlinkSlaveVersion_t;

typedef struct KlinkSlaveDeviceInfo
{
	char slaveFwVersion[18];
	char slaveMacAddr[18];
	char sn[32];
}KlinkSlaveDeviceInfo_t;


typedef struct uncryptWifiSetting
{
 ENCRYPT_T encrypt_2g;
 char ssid_2g[64]; 
 int auth_2g;
 int cipher_2g;
 char psk_2g[64];
 ENCRYPT_T encrypt_5g;
 char ssid_5g[64];
 int auth_5g;
 int cipher_5g;
 char psk_5g[64];
 int uncryptWifiSyncFlag;
}uncryptWifiSetting_t;

typedef struct guestWifiSeting
{
 int guestWifiSwitch_5g;
 int guestWifiSwitch_2g;
 int guestSyncFlag;
}guestWifiSetting_t;

typedef struct meshSeting
{
 int ledSwitch;
 int ledSyncFlag;
 uncryptWifiSetting_t wifiCfg;
 guestWifiSetting_t guestWifi;
}meshSetting_t;

//static meshSetting_t g_syncSettings;

/*klink node struct*/
typedef struct KlinkNode
{
    /*klink message header*/
    klinkMsgStateMachine_t klinkMsgStaMachine;
	char sourceMac[18];
	char destMac[18];

	/*klink message body*/
	KlinkSlaveDeviceInfo_t slaveDevideInfo;
	meshSetting_t syncCfg;	
	
    int slaveMeshNum;	
	KlinkSlaveVersion_t slaveVersionInfo; 
	klinkDataType_t dataType;
	struct KlinkNode *next;
}KlinkNode_t;



/*
 type 0: match any of belows
 type 1: match 001122334455
 type 2: match 00:11:22:33:44:55
 type 3: match 00-11-22-33-44-55
 type 4: match 0011:2233:4455
 type 5: match 0011-2233-4455
 type 6: match 001122:334455
 type 7: match 001122-334455
*/

enum{
    ETHER_TYPE_DEFAULT = 0,
    ETHER_TYPE_NO_SEPARTOR,
    ETHER_TYPE_ONE_COLON,
    ETHER_TYPE_ONE_DASH, 
    ETHER_TYPE_TWO_COLON,
    ETHER_TYPE_TWO_DASH, 
    ETHER_TYPE_FIVE_COLON,
    ETHER_TYPE_FIVE_DASH    
};
#define ETHER_ADDR_TYPE_DEFAULT  ETHER_TYPE_ONE_DASH 

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif 

typedef struct etherAddr 
{
    unsigned char octet[ETHER_ADDR_LEN];
}etherAddr_t;

/*use for listen() func*/
#define PENDING_CONNECTION_NUM     3   

/*use for struct timeval,time unit is second*/
#define TIME_INTERVAL              60

extern int app_event_handler(int fd,char *buffer);
extern KlinkNode_t* g_pKlinkHead;

#define DEBUG_ON_TAG_FILE	"/var/run/DEBUG_ON"

#define TRACE_DEBUG(fmt, args...)	 \
	{											\
		if(access( DEBUG_ON_TAG_FILE, F_OK ) >= 0) \
		{										\
			printf("[Debug]:"fmt"\n", ##args); \
		}										\
	}	








