
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

/* define exit() codes if not provided */
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
#define WAN_IF                     "eth1"
#define MAX_SLAVE_NUM              15
#define ENUM_DEFAULT              0

typedef enum  
{
  KLINK_START=ENUM_DEFAULT,
  KLINK_SLAVE_SEND_VERSION_INFO,    
  KLINK_MASTER_SEND_ACK_VERSION_INFO,    
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


/*klink node struct*/
typedef struct KlinkNode
{
    int date;
    int slaveMeshNum;
	KlinkSlaveVersion_t slaveVersionInfo;
	klinkDataType_t dataType;
	klinkMsgStateMachine_t stateMachine;
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





