
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


/*use for listen() func*/
#define PENDING_CONNECTION_NUM     3   

/*use for struct timeval,time unit is second*/
#define TIME_INTERVAL              60

extern int app_event_handler(int fd,char *buffer);





