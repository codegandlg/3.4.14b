
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
#define KLINK_PORT                 5001
#define MAX_CLIENT                 400
#define WAN_IF                     "eth1"
#define MAX_SLAVE_NUM              15

#define NONE_CON_REQ             0
#define IDENTITY_CHECK           1
#define INT_CON_REQ              2
#define WLAN_BASIC_CON_REQ       3
#define WLAN_ENCRYP_CON_REQ      4
#define WLAN_SIMPLE_CON_REQ      5
#define FIREWLL_CON_REQ          6
#define SYS_TOOL_CON_REQ         7
#define CONFIGURE_ROUTER_EVENT   8
#define CLIENT_LIST_CON_REQ      9
enum MESSAGE_STATE_TYPE
{
  DEFAULT=0,
  KLINK_START=DEFAULT,
  KLINK_SLAVE_SEND_VERSION_INFO,
  KLINK_MASTER_SEND_ACK_VERSION_INFO,
  
};


struct klinkNode 
{ 
 int noteNum;
 char slaveMac[17];
 char slaveFwVersion[12];
 int data; 
struct Node *next; 
}; 

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





