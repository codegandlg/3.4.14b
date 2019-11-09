
/*
Copyright (c) 2019, All rights reserved.

File         : klinkSlave.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include<net/if_arp.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include <netinet/in.h>
#include <net/route.h>
#include <net/if.h>

#include<errno.h>
#include "klink.h"
#include <time.h>
#include "cJSON.h"
#include "apmib.h"


#define _slave 

static unsigned int g_lastSlaveCheckTime = 0;
extern char *fwVersion;
static char slaveMac[17]={0};
cJSON * slaveGenerateMessageHeader(cJSON *root,int messageType,cJSON *messageBody);
const char* slaveGenerateJsonMessageBody(int messageType,cJSON *messageBody,char** pMessage);

#define ERR_EXIT(m, ...)\
    do{\
        fprintf(stderr, m"\n", ##__VA_ARGS__);\
        log(m"\n", ##__VA_ARGS__);\
        exit(EXIT_FAILURE);\
    } while (0) 
#define MAX_BUFF_LEN 1024
void log(const char *fmt, ...)
{
    char buffer[MAX_BUFF_LEN] = {0};
    va_list args;
    time_t timep;
    time(&timep);
    struct tm *p = gmtime(&timep);
    FILE* fp = fopen("klink.log", "a+");
    if(fp){
        snprintf(buffer, MAX_BUFF_LEN, "[%.4d-%.2d-%.2d %.2d:%.2d:%.2d][%d]", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, getpid());
        va_start(args, fmt);
        vsnprintf(buffer + strlen(buffer), MAX_BUFF_LEN - strlen(buffer) - 1, fmt, args);
        va_end(args);
        fwrite(buffer, strlen(buffer), 1, fp);
        fclose(fp);
    }
}
ssize_t      /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n){
    size_t  nleft;
    ssize_t  nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) 
	{
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) 
		{
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;  /* and call write() again */
            else
                return(-1);   /* error */
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n);
}

unsigned int upSecond(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);	
	return ts.tv_sec;
}

 
 static *etherAddrToString(etherAddr_t *ether, int type)
 {
	 static char buffer[8][64];
	 static int buffer_index = 0;
 
	 if (buffer_index >= sizeof(buffer)/sizeof(buffer[0]) - 1)
	 {
		 buffer_index = 0;
	 }
	 else 
	 {
		 buffer_index ++;
	 }
 
	 if (type == ETHER_TYPE_DEFAULT)
	 {
		 type = ETHER_ADDR_TYPE_DEFAULT;
	 }
	 
	 switch(type)
	 {
		 case ETHER_TYPE_NO_SEPARTOR:
			 sprintf(buffer[buffer_index], "%02x%02x%02x%02x%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);
			 break;
		 case ETHER_TYPE_ONE_COLON:
			 sprintf(buffer[buffer_index], "%02x%02x%02x:%02x%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break;
		 case ETHER_TYPE_ONE_DASH:
			 sprintf(buffer[buffer_index], "%02x%02x%02x-%02x%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break; 		   
		 case ETHER_TYPE_TWO_COLON:
			 sprintf(buffer[buffer_index], "%02x%02x:%02x%02x:%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break; 		   
		 case ETHER_TYPE_TWO_DASH:
			 sprintf(buffer[buffer_index], "%02x%02x-%02x%02x-%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break; 		   
		 case ETHER_TYPE_FIVE_COLON:
			 sprintf(buffer[buffer_index], "%02x:%02x:%02x:%02x:%02x:%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break; 		   
		 case ETHER_TYPE_FIVE_DASH:
			 sprintf(buffer[buffer_index], "%02x%02x%02x%02x%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break;
		 default:
			 sprintf(buffer[buffer_index], "%02x%02x%02x%02x%02x%02x", 
				 ether->octet[0], ether->octet[1], ether->octet[2],
				 ether->octet[3], ether->octet[4], ether->octet[5]);  
			 break; 		   
	 }
	 
	 return buffer[buffer_index];
 }
 
 static int getNetifHwAddr(const char *ifname, etherAddr_t *hwaddr)
 {
	 int sock, ret;
	 struct ifreq ifr;
 
	 sock = socket(AF_INET, SOCK_STREAM, 0);
	 if(sock < 0) 
	 {	 
		 return -1;
	 }
 
	 memset(&ifr, 0, sizeof(struct ifreq));
	 strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
	 
	 ret = ioctl(sock, SIOCGIFHWADDR, &ifr);
 
	 if(!ret && hwaddr)
	 {
		 memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, sizeof(etherAddr_t));
	 }
 
	 close(sock);
	 
	 return (ret < 0) ? -1 : 0;
 }
 
 static int  getMacAddr( char *pWanHWAddr)
 {
	 struct sockaddr hwaddr;
	 etherAddr_t addr;
	 unsigned char *pMacAddr;
	 if (!(getNetifHwAddr(KLINK_IF, &addr)) ) 
	 {
	  sprintf(pWanHWAddr, "%s",etherAddrToString(&addr, ETHER_TYPE_NO_SEPARTOR));
	 }
	 else
	 {
		 sprintf(pWanHWAddr,"%s","00:00:00:00:00:00");
	 }
	 return 0;
 }


#define zeroIp(ip) ((ip)->s_addr == 0)

int validHostIp(struct in_addr *ip)
{
    int fa;
    fa = ip ? (ntohl(ip->s_addr) >> 24) & 0xff : 0;

    if ((fa > 223) || (fa == 0) || (fa == 127))
    {
        return 0;
    }

    return 1;
}

int validIpAddr(struct in_addr *ip)
{
    if (!zeroIp(ip) && !validHostIp(ip))
    {
        return 0;
    }

    return 1;
}

int getGetwayIp(char gatewayIp[])
{
	FILE *fp=NULL;
	int i=0;
	char *bufPtr=NULL;
	char buff[128]={0};
	int lineNum = 0;

	system("route > /tmp/defautgw");
	if(fp=fopen("/tmp/defautgw","r"))
	{
	    while(fgets(buff,sizeof(buff)-1,fp))
	   { 
		 lineNum++;
		 if(lineNum <3)
			 continue;
		 if(strstr(buff,"default"))		 
		 {
		   bufPtr = buff;
		   while(!((*bufPtr >= '0')&&(*bufPtr <= '9')))
		   	bufPtr++;
		    while((*bufPtr != '\0') && (*bufPtr != ' '))
             {
               gatewayIp[i] = *bufPtr;
               bufPtr++;
			   i++;
             }
		     break;
		 }
		 else
		 {
			fclose(fp);
			return 1;
		 }
	   }
	} 	
	fclose(fp);
	return 0;
		
}


int getGetwayIp2(char gatewayIp[])
{
   FILE *fp=NULL;
   int i=0;
   char *bufPtr1=NULL;
   char *bufPtr2=NULL;
   char buff[64]={0};
   int lineNum = 0;
  
   fp=fopen("/etc/resolv.conf","r");
   if(fp == NULL)
        {
          printf("open /etc/resolv.conf failed\n");     
        return 1;
        }
   while(fgets(buff,64,fp))
   {
        lineNum++;
        if(lineNum > 1)
            break;
        bufPtr1=strstr(buff,"nameserver");
        if(bufPtr1)
        {
           bufPtr2 = bufPtr1 + 11;
           i = 0;
           while(*bufPtr2 != '\r' && *bufPtr2 != '\n')
           {
              gatewayIp[i] = *bufPtr2;
              bufPtr2++;
              i++;
           }
           gatewayIp[i] = '\0';
	  //sysLog(LOG_NOTICE,"gatewayIp:%s\n",gatewayIp);   
	 break;
        }
        else
        {
           fclose(fp);
           return 1;
        }
   }
   fclose(fp);
   return 0;
}

cJSON * slaveGenerateMessageHeader(cJSON *root,int messageType,cJSON *messageBody)
{   
	cJSON *pJson=root;
	char macAddr[18]={0};
	char fwVersion[18]={0};	

    switch (messageType)
    {
		  case KLINK_START: 					 
		   cJSON_AddStringToObject(pJson, "messageType", "1");
		   break;
		case KLINK_MASTER_REPORT_DEVICE_ACK:  //messageType=2
		   cJSON_AddStringToObject(pJson, "messageType", "3");
		   printf("=>get_master_version_ack\n");
		   break;
		case KLINK_HEARD_BEAD_SYNC_MESSAGE:
		   cJSON_AddStringToObject(pJson, "messageType", "3");
		   break;
		 case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
		   cJSON_AddStringToObject(pJson, "messageType", "5");
		   break;
		 case KLINK_MASTER_SEND_UNENCRYP_WIFI_INFO_TO_SLAVE:
		   cJSON_AddStringToObject(pJson, "messageType", "7");
		   break; 
		 case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:	 
		   cJSON_AddStringToObject(pJson, "messageType", "9");
		   break;
		default:
		  /*if havn't incomming message,must period send beartbead*/ 
		   messageType=KLINK_HEARD_BEAD_SYNC_MESSAGE; 
		   break;
    }
		printf("%s_%d:\n ",__FUNCTION__,__LINE__);
		cJSON_AddStringToObject(pJson, "sourceMac", slaveMac);
	    cJSON_AddStringToObject(pJson, "destMac", cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring);
	   	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	  //  cJSON_AddStringToObject(pJson, "slaveMac", macAddr);
	    return pJson;
}

const char* slaveGenerateJsonMessageBody(int messageType,cJSON *messageBody,char** pMessage)
{
	char* stringMessage=NULL;
    cJSON *topRoot=NULL;
	cJSON *root=NULL;

	int ledEnable;
    char buff[2]={0};
    /*for guest network*/
    int disableFlg_2g;
    int disableFlg_5g;
    int old_wlan_idx;
    int old_vwlan_idx;
	ENCRYPT_T encrypt_5g;
	ENCRYPT_T encrypt_2g;
	char ssidBuf_5g[64]={0};   
    char ssidBuf_2g[64]={0};
 	printf("%s_%d:\n ",__FUNCTION__,__LINE__);		  
	topRoot = cJSON_CreateObject();
	 if (!topRoot)
    {
        printf("cJsonCreateObj failed!");
        return NULL;
    }
	 
    /* generate header */
    topRoot=slaveGenerateMessageHeader(topRoot,messageType,messageBody);
	 	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 switch(messageType)
 {
    case KLINK_START:                     	//messageType=0
     {
       cJSON_AddItemToObject(topRoot, "slaveVersion", root = cJSON_CreateObject());
	   cJSON_AddStringToObject(root, "slaveSoftVer", fwVersion);
		printf("%s_%d:\n ",__FUNCTION__,__LINE__);

	    /*for led switch*/
	   apmib_get(MIB_LED_ENABLE, (void *)&ledEnable);
	   sprintf(buff,"%d",ledEnable);
	   cJSON_AddItemToObject(topRoot, "ledSwitch", root = cJSON_CreateObject());
	   cJSON_AddStringToObject(root, "ledEnable", buff);

       old_wlan_idx = wlan_idx;
	   old_vwlan_idx = vwlan_idx;
	   
	   /*for uncrypt wifi cfg*/
	   vwlan_idx = 0;
       wlan_idx = 0;
	   apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_5g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_5g);
	   wlan_idx = 1;
	   apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_2g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_2g);

	   /*for guest wifi cfg*/
	   vwlan_idx = 2;
       wlan_idx = 0;
	   apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_5g);
	   wlan_idx = 1;
	   apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_2g);	
	   
	   wlan_idx = old_wlan_idx;
       vwlan_idx = old_vwlan_idx;	   
	   printf("%s_%d:\n ",__FUNCTION__,__LINE__);

	   /*for uncrypt wifi Cfg*/
	   cJSON_AddItemToObject(topRoot, "uncrypWifi", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_5g);
	   cJSON_AddStringToObject(root, "encrypt_5g", buff);
	   cJSON_AddStringToObject(root, "uncryptSsid_5g", ssidBuf_5g);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_2g);
	   cJSON_AddStringToObject(root, "encrypt_2g", buff);
	   cJSON_AddStringToObject(root, "uncryptSsid_2g", ssidBuf_2g);
		printf("%s_%d:\n ",__FUNCTION__,__LINE__);

	    /*for guest wifi Cfg*/
	   cJSON_AddItemToObject(topRoot,"guestWifi", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_5g);
	   cJSON_AddStringToObject(root, "guestSwitch_5g", buff);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_2g);
	   cJSON_AddStringToObject(root, "guestSwitch_2g", buff); 
	   printf("%s_%d:\n ",__FUNCTION__,__LINE__);
     }

	 
	 break;
  case KLINK_MASTER_REPORT_DEVICE_ACK:  //messageType=2
     printf("=>get_master_version_ack\n");
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
  	  cJSON_AddItemToObject(topRoot, "heartbeat", root = cJSON_CreateObject());
	  cJSON_AddStringToObject(root, "heartbeatSync", "sync");
   //  sendHeardBeatMessage(sd);  
	 break;
   case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
      {
	    apmib_get(MIB_LED_ENABLE, (void *)&ledEnable);
	    memset(buff,0,sizeof(buff));
	    sprintf(buff,"%d",ledEnable);
	    cJSON_AddItemToObject(topRoot, "ledSwitch", root = cJSON_CreateObject());
	    cJSON_AddStringToObject(root, "ledEnable", buff);
		break;
   	  }	
   case KLINK_MASTER_SEND_UNENCRYP_WIFI_INFO_TO_SLAVE:
   	  {
   	   old_wlan_idx = wlan_idx;
	   old_vwlan_idx = vwlan_idx;
	   vwlan_idx = 0;
       wlan_idx = 0;
	   apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_5g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_5g);
	   wlan_idx = 1;
	   apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_2g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_2g);   
	   wlan_idx = old_wlan_idx;
       vwlan_idx = old_vwlan_idx;

	   cJSON_AddItemToObject(topRoot, "uncrypWifi", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_5g);
	   cJSON_AddStringToObject(root, "encrypt_5g", buff);
	   cJSON_AddStringToObject(root, "uncryptSsid_5g", ssidBuf_5g);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_2g);
	   cJSON_AddStringToObject(root, "encrypt_2g", buff);
	   cJSON_AddStringToObject(root, "uncryptSsid_2g", ssidBuf_2g);
	   break; 
   	  }
   case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:    
      {
       old_wlan_idx = wlan_idx;
	   old_vwlan_idx = vwlan_idx;
	   vwlan_idx = 2;
       wlan_idx = 0;
	   apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_5g);
	   wlan_idx = 1;
	   apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_2g);	   
	   wlan_idx = old_wlan_idx;
       vwlan_idx = old_vwlan_idx;

	   cJSON_AddItemToObject(topRoot,"guestWifi", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_5g);
	   cJSON_AddStringToObject(root, "guestSwitch_5g", disableFlg_5g);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_2g);
	   cJSON_AddStringToObject(root, "guestSwitch_2g", disableFlg_2g); 
	   break;
   	 }
  default:
  	/*if havn't incomming message,must period send beartbead*/ 
  	 messageType=KLINK_HEARD_BEAD_SYNC_MESSAGE; 
  	 break;
 }
 printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 	*pMessage = cJSON_Print(topRoot);  
	cJSON_Delete(topRoot);	
	return *pMessage;
}

int _slave parseMasterConf(int fd, cJSON *jason_obj)
{
   cJSON *tasklist=jason_obj->child;
   while(tasklist!=NULL)
   {
    cJSON_GetObjectItem(tasklist,"slaveSoftVer")->valuestring;
    cJSON_GetObjectItem(tasklist,"slaveMac")->valuestring;
    tasklist=tasklist->next;
   }
		
}


/*
 *led switch formate:
 *sync:
 *{"message":"4","sourceMac":"xxxxxx","destMac":"yyyyyy","ledSwitch":{"ledEnable":"0"}}
 *
 *ack:
 *{"message":"5","sourceMac":"xxxxxx","destMac":"yyyyyy","ledSwitch":{"ledEnable":"0"}}
*/
int syncMasterLedSwitch(int fd, int messageType, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   char tmp[2]={0};
   int value=-1;
   int localValue=1;
   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;

   if(jasonObj = cJSON_GetObjectItem(messageBody,"ledSwitch"))
   {
    printf("%s_%d: \n",__FUNCTION__,__LINE__);
	 value=(strncmp(cJSON_GetObjectItem(jasonObj,"ledEnable")->valuestring, "0",1)?1:0);
	  printf("%s_%d: value=%d\n",__FUNCTION__,__LINE__,value);

     if(apmib_get(MIB_LED_ENABLE,(void *)&localValue))
    {
     if(localValue!=value)
     {
      localValue=value;
	  apmib_set(MIB_LED_ENABLE,(void *)&value);	
	  if(apmib_update(CURRENT_SETTING) <= 0)
         {
           printf("apmib_update CURRENT_SETTING fail.\n");
         }
     }
    }
   }
	pResponseMsg=slaveGenerateJsonMessageBody(messageType,messageBody,&pResponseMsg);
	printf("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd, pResponseMsg,strlen(pResponseMsg), 0) ;

}

/*
 *guest wifi settings formate:
 *sync:
 *{"message":"8","sourceMac":"xxxxxx","destMac":"yyyyyy","guestWifi":{"guestSwitch_5g":"0","guestSwitch_2g":"0"}}
 *
 *ack:
 *{"message":"9","sourceMac":"yyyyyy","destMac":"xxxxxx","guestWifi":{"guestSwitch_5g":"0","guestSwitch_2g":"0"}}
 *
*/
int syncGuestWifiSettings(int fd, int messageType, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   int value=-1;
   int localValue=1;
   char *pResponseMsg=NULL;
  /*for guest network*/
  int disableFlg_2g;
  int disableFlg_5g;
  int old_wlan_idx;
  int old_vwlan_idx;
   if(jasonObj = cJSON_GetObjectItem(messageBody,"gustWifi"))
   {
   	  old_wlan_idx = wlan_idx;
	  old_vwlan_idx = vwlan_idx;
	  vwlan_idx = 2;
      wlan_idx = 0;
    printf("%s_%d: \n",__FUNCTION__,__LINE__);
	 value=(strncmp(cJSON_GetObjectItem(jasonObj,"guestSwitch_5g")->valuestring, "0",1)?1:0);
	  printf("%s_%d: value=%d\n",__FUNCTION__,__LINE__,value);
     
     if(apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&localValue))
    {
     if(localValue!=value)
     {
	  apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&value);	
	  if(value==1)
	  	system("ifconfig wlan0-va1 down");
	  else if(value==0)
	  	system("ifconfig wlan0-va1 up");  
     }
    }

	 wlan_idx = 1;
	 value=(strncmp(cJSON_GetObjectItem(jasonObj,"guestSwitch_2g")->valuestring, "0",1)?1:0);
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;
	 printf("%s_%d: value=%d\n",__FUNCTION__,__LINE__,value);	   
	  if(apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&localValue))
	{
	   if(localValue!=value)
	   {
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&value); 
	    if(value==1)
	  	    system("ifconfig wlan1-va1 down");
	    else if(value==0)
	  	    system("ifconfig wlan1-va1 up");                
	   }
	}
	if(apmib_update(CURRENT_SETTING) <= 0)
	{
	 printf("apmib_update CURRENT_SETTING fail.\n");
	}    	 
   }
	pResponseMsg=slaveGenerateJsonMessageBody(messageType,messageBody,&pResponseMsg);
	printf("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd, pResponseMsg,strlen(pResponseMsg), 0) ;
		
}


/*
 *uncrypt wifi settings formate:
 *sync:
 *{"message":"6","sourceMac":"xxxxxx","destMac":"yyyyyy","uncrypWifiSetting":{"encrypt_5g":"0","ssid_5g":"ssidName","encrypt_2g":"0","ssid_2g":"ssidName"}}
 *ack:
 *{"message":"7","sourceMac":"yyyyyy","destMac":"xxxxxx","uncrypWifiSetting":{"encrypt_5g":"0","ssid_5g":"ssidName","encrypt_2g":"0","ssid_2g":"ssidName"}}
 *
*/
int syncUncrypWifiSettings(int fd, int messageType, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   char ssidBuf[64]={0};  
   ENCRYPT_T encrypt_5g; 
   char ssid_5g[64]={0};
   ENCRYPT_T encrypt_2g; 
   char ssid_2g[64]={0};
   int value=-1;
   int setFlag=0;
   int localValue=1;
   int ret=0;
   char *pResponseMsg=NULL;
    int old_wlan_idx;
    int old_vwlan_idx;
	int settingFlag=0;

   if(jasonObj = cJSON_GetObjectItem(messageBody,"uncrypWifi"))
   {
	old_wlan_idx = wlan_idx;
	old_vwlan_idx = vwlan_idx;
	vwlan_idx = 0;
    wlan_idx = 0;
	encrypt_5g=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_5g")->valuestring);
	strcpy(ssid_5g,cJSON_GetObjectItem(jasonObj,"ssid_5g")->valuestring);
	if(encrypt_5g==ENCRYPT_DISABLED)
	{
	 apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt_5g);
	 apmib_set(MIB_WLAN_SSID, (void *)&ssid_5g);
	 setFlag=1;
	}
    
	wlan_idx = 1;
	encrypt_2g=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_2g")->valuestring);
	strcpy(ssid_2g,cJSON_GetObjectItem(jasonObj,"ssid_2g")->valuestring);
	if(encrypt_2g==ENCRYPT_DISABLED)
	{
	apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt_2g);
	apmib_set(MIB_WLAN_SSID, (void *)&ssid_2g);	
	setFlag=1;
	}

	if(apmib_update(CURRENT_SETTING) <= 0)
    {
      printf("apmib_update CURRENT_SETTING fail.\n");
    }
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;
   }

	pResponseMsg=slaveGenerateJsonMessageBody(messageType,messageBody,&pResponseMsg);
	printf("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd, pResponseMsg,strlen(pResponseMsg), 0) ;
	if(setFlag==1)
		system("init.sh gw bridge");

}


/*
 * report device info formate:
 *sync:
 {"messageType": "1","sourceMac":"xxxxxx","destMac":"yyyyyy","slaveVersion":{"slaveSoftVer":"WM V1.0.5"},
  "ledSwitch":{"ledEnable":"0"},
  "uncrypWifiSetting":{"encrypt_5g":"0","ssid_5g":"ssidName","encrypt_2g":"0","ssid_2g":"ssidName"},
  "guestWifi":{"guestSwitch_5g":"0","guestSwitch_2g":"0"}}
 * ack:
  {"messageType": "1","sourceMac":"yyyyyy","destMac":"xxxxxx","slaveVersion":{"slaveSoftVer":"WM V1.0.5"},
  "ledSwitch":{"ledEnable":"0"},
  "uncrypWifiSetting":{"encrypt_5g":"0","ssid_5g":"ssidName","encrypt_2g":"0","ssid_2g":"ssidName"},
  "guestWifi":{"guestSwitch_5g":"0","guestSwitch_2g":"0"}}
*/

void _slave slaveReportDeviceInfoToMaster(int sd, int messageType, cJSON *messageBody) 
{
	char *messageBuff;
	char* stringMessage=NULL;
    cJSON *topRoot=NULL;
	cJSON *root=NULL;
	cJSON *parameters=NULL;
	int localValue=1;
	
	printf("%s_%d:\n ",__FUNCTION__,__LINE__);

    if(apmib_get(MIB_FIRST_LOGIN,(void *)&localValue))
    {
     if(localValue!=1)
     {
      localValue=1;
	  apmib_set(MIB_FIRST_LOGIN,(void *)&localValue);	
	  if(apmib_update(CURRENT_SETTING) <= 0)
         {
           printf("apmib_update CURRENT_SETTING fail.\n");
         }
     }
    }
	
    stringMessage=slaveGenerateJsonMessageBody(messageType,messageBody,&stringMessage);
	printf("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,stringMessage);
	send(sd, stringMessage,strlen(stringMessage), 0) ;
}

/*
 *heard beat formate:
 *{"message":"10","heardBead":{"heardBeadState":"sync"}}
 *
*/
int sendHeardBeatMessage(int fd,int messageType, cJSON *messageBody)
{
  char* stringMessage=NULL;
  cJSON *topRoot=NULL;
  cJSON *root=NULL;
  cJSON *parameters=NULL;
  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 while(1) 
 {
 if(upSecond() - g_lastSlaveCheckTime > HEART_BEAT_TIME_SCHEDULE)	
  {
   stringMessage=slaveGenerateJsonMessageBody(messageType,messageBody,&stringMessage);
   printf("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,stringMessage);
   send(fd, stringMessage,strlen(stringMessage), 0) ;
   g_lastSlaveCheckTime = upSecond();
  break;
  }
 }
}


int _slave klinkSlaveStateMaching(int sd,int messageType,cJSON *messageBody)
{
 switch(messageType)
 {
  case KLINK_START:                     	          //messageType=0
     slaveReportDeviceInfoToMaster(sd,messageType,messageBody);   //messageType=1
	 break;
  case KLINK_MASTER_REPORT_DEVICE_ACK:                 //messageType=2
     printf("=>get_master_version_ack\n");
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
     sendHeardBeatMessage(sd,messageType,messageBody);  
	 break;
   case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
     syncMasterLedSwitch(sd,messageType,messageBody); 
	 break;
   case KLINK_MASTER_SEND_UNENCRYP_WIFI_INFO_TO_SLAVE:
     syncUncrypWifiSettings(sd,messageType,messageBody);  
	 break; 
   case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:    
     syncGuestWifiSettings(sd,messageType,messageBody); 
	 break;
  default:
  	/*if havn't incomming message,must period send beartbead*/ 
  	 messageType=KLINK_HEARD_BEAD_SYNC_MESSAGE; 
  	 break;
 }

  if(messageType==KLINK_MASTER_REPORT_DEVICE_ACK)
   {
     printf("%s_%d: slave prepare send heartBeat sync...\n",__FUNCTION__,__LINE__);
     sendHeardBeatMessage(sd,messageType,messageBody);   
   }
}

int _slave parseMessageFromMaster(int sd, char* masterMessage) 
{

    cJSON *json=NULL;
	int messageType=-1;

    printf("%s_%d:get master message[%s] \n",__FUNCTION__,__LINE__,masterMessage);
    json = cJSON_Parse(masterMessage);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
      /*{"slaveVersion":["messageType":"0"{"slaveSoftVer":"WM V1.0.5","slaveMac":"00:11:22:33:44:55"}]}*/
     //if(jason_obj = cJSON_GetObjectItem(json,"slaveVersion"))	
     messageType = atoi(cJSON_GetObjectItem(json,"messageType")->valuestring);
	 if(messageType>=1)
	  {
	    /*if dest mac not equal,it means message not send to mine,exit*/
	    if(strcmp(cJSON_GetObjectItem(json,"destMac")->valuestring,slaveMac))
		return 0;
	  }
	 printf("%s_%d:messageType=%d\n ",__FUNCTION__,__LINE__,messageType);
	 klinkSlaveStateMaching(sd,messageType,json);
     
    }
	return 0;
}

int main(int argc,char **argv)
{
	char ip[16]={0};
	static int messagetype=0;
	int routeTableFlag=-1;
	int routeTableFlag2=-1;
	struct in_addr gwAddr;
    fd_set rset;
    FD_ZERO(&rset);
    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;   
    //struct hostent *h = gethostbyname(argv[1]);
    //server_addr.sin_addr = *((struct in_addr *)h->h_addr);
    routeTableFlag=getGetwayIp(ip);
	routeTableFlag2=getGetwayIp2(ip);
	if((routeTableFlag!=0)&&(routeTableFlag2!=0))
	{
	 printf("%s_%d:get gateway ip fail\n",__FUNCTION__,__LINE__);
	 return 0;
	}
	
    printf("=slave gateway ip=%s\n", ip );
	server_addr.sin_addr.s_addr = inet_addr(ip);  
    server_addr.sin_port = htons(KLINK_PORT);
    if(connect(sock,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0)
	{
        ERR_EXIT("connect to %s:%s failed\n", argv[1], argv[2]);
    }

    int ready;  
    int maxfd;  
    int fd_stdin = fileno(stdin);
    int fd_stdout = fileno(stdout);
    if (fd_stdin > sock) 
    {
      maxfd = fd_stdin;  
    }
    else 
    {
      maxfd = sock; 
    }
    char sendbuf[1024] = {0};  
    char recvbuf[1024] = {0};  
	apmib_init();
	
	g_lastSlaveCheckTime = upSecond();
	getMacAddr(slaveMac);
    while (1)
	{  

        FD_SET(fd_stdin, &rset);  
        FD_SET(sock, &rset);  
	   /*select return value:there is something readble message */
        ready = select(maxfd + 1, &rset, NULL, NULL, NULL); 
        if (ready == -1) ERR_EXIT("select error");  
        if (ready == 0) 
			continue;  
        if (FD_ISSET(sock, &rset))
		{  
            int ret = read(sock, recvbuf, sizeof(recvbuf)); 
            if (ret == -1) 
            {
			  ERR_EXIT("read from socket error");  
            }
            else if (ret  == 0)
            {
			  ERR_EXIT("server close"); 
            }
           // writen(fd_stdout, recvbuf, ret);
           printf("==>received messagerom maser:\n%s \n",recvbuf);
		   //send(sock, "hello", sizeof("hello"), 0) ;
		   
		   parseMessageFromMaster(sock, (char*)recvbuf);
           memset(recvbuf, 0, sizeof(recvbuf));  
        }  
	sleep(1);
#if 0
		strcpy(sendbuf,"test");
        if (FD_ISSET(fd_stdin, &rset))
		{  
            int ret = read(fd_stdin, sendbuf, sizeof(sendbuf));
            if(ret > 0) 
			{
                writen(sock, sendbuf, ret);  
                memset(sendbuf, 0, sizeof(sendbuf));
            }else
			{
                ERR_EXIT("read from stdin error");                
            }
        }  
#endif
    }  
    close(sock);  
    exit(0);
}
