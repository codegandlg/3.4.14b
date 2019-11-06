
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


int getMacAddr( char *interface, void *pAddr )
{
    struct ifreq ifr;
    int skfd, found=0;
	struct sockaddr_in *addr;
    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    strcpy(ifr.ifr_name, interface);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0){
    		close( skfd );
		return (0);
	}
    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0) {
		memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
		found = 1;
	}
    else {    	
    	if (ioctl(skfd, SIOCGIFDSTADDR, &ifr) >= 0) {
		addr = ((struct sockaddr_in *)&ifr.ifr_addr);
		*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
		found = 1;
	}
    	
    }
    close( skfd );
    return found;
}

char *etherAddrToString(etherAddr_t *ether, int type)
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

int getNetifHwAddr(const char *ifname, etherAddr_t *hwaddr)
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

int _slave getSlaveVersionInfo(char *pSoftVersion, char *pWanHWAddr)
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
	strcpy(pSoftVersion,fwVersion);
}


/*
 *heard beat formate:
 *{"message":"10","heardBead":{"heardBeadState":"sync"}}
 *
*/
int sendHeardBeatMessage(int fd)
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
  topRoot = cJSON_CreateObject();
  cJSON_AddStringToObject(topRoot, "messageType", "3");//3==KLINK_SLAVE_SEND_VERSION_INFO
  cJSON_AddItemToObject(topRoot, "heardBead", root = cJSON_CreateObject());
  cJSON_AddStringToObject(root, "heardBeadState", "sync");
  stringMessage = cJSON_Print(topRoot);  
  printf("%s_%d:sen message=%s \n",__FUNCTION__,__LINE__,stringMessage);
  send(fd, stringMessage,strlen(stringMessage), 0) ;
  cJSON_Delete(topRoot);
  g_lastSlaveCheckTime = upSecond();
  break;
  }
 }
}


/*
 * version formate:
* {"messageType": "1","slaveVersion":{"slaveSoftVer":"WM V1.0.5","slaveMac":"001122334455"}}
*/

void _slave sendFwVersionMessageToMaster(int sd) 
{
	char macAddr[18]={0};
	char fwVersion[18]={0};	
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
	
    getSlaveVersionInfo(fwVersion, macAddr);	  
	topRoot = cJSON_CreateObject();
	cJSON_AddStringToObject(topRoot, "messageType", "1");//1==KLINK_SLAVE_SEND_VERSION_INFO
	cJSON_AddItemToObject(topRoot, "slaveVersion", root = cJSON_CreateObject());
	cJSON_AddStringToObject(root, "slaveSoftVer", fwVersion);
	cJSON_AddStringToObject(root, "slaveMac", macAddr);
	stringMessage = cJSON_Print(topRoot);  
	printf("%s_%d:sen message=%s \n",__FUNCTION__,__LINE__,stringMessage);
	send(sd, stringMessage,strlen(stringMessage), 0) ;
	cJSON_Delete(topRoot);	
}


/*
 *led switch formate:
 *{"message":"3","ledSwitch":{"ledEnable":"0"}}
 *
*/
int syncMasterLedSwitch(int fd, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
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
	  printf("%s_%d: \n",__FUNCTION__,__LINE__); 
   }
	printf("%s_%d: \n",__FUNCTION__,__LINE__);

    /*response ack message to master*/
	responseJSON = cJSON_CreateObject();
	cJSON_AddStringToObject(responseJSON, "messageType", "5"); //4==KLINK_SALAVE_SEND_ACK_RESPONSE
	pResponseMsg = cJSON_Print(responseJSON); 
	printf("%s_%d:send led ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
	cJSON_Delete(responseJSON);	
}



/*
 *guest wifi settings formate:
 *{"message":"8","ledSwitch":{"ledEnable":"0"}}
 *
*/
int syncGuestWifiSettings(int fd, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   int value=-1;
   int localValue=1;
   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;
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
      localValue=value;
	  apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&value);	
     }
    }

	 wlan_idx = 1;
	 value=(strncmp(cJSON_GetObjectItem(jasonObj,"guestSwitch_2g")->valuestring, "0",1)?1:0);
	 printf("%s_%d: value=%d\n",__FUNCTION__,__LINE__,value);	   
	  if(apmib_get(MIB_WLAN_WLAN_DISABLED,(void *)&localValue))
	{
	   if(localValue!=value)
	   {
		localValue=value;
		apmib_set(MIB_WLAN_WLAN_DISABLED,(void *)&value); 
	   }
	}
	if(apmib_update(CURRENT_SETTING) <= 0)
	{
	 printf("apmib_update CURRENT_SETTING fail.\n");
	}    	 
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;
   }
	printf("%s_%d: \n",__FUNCTION__,__LINE__);

    /*response ack message to master*/
	responseJSON = cJSON_CreateObject();
	cJSON_AddStringToObject(responseJSON, "messageType", "9"); //9==KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK
	pResponseMsg = cJSON_Print(responseJSON); 
	printf("%s_%d:send guest wifi ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
	cJSON_Delete(responseJSON);	
    system("init.sh gw bridge");
}


/*
 *uncrypt wifi settings formate:
 *{"message":"6","uncrypWifiSetting":{"encrypt":"0","ssid":"ssidName"}}
 *
*/
int syncUncrypWifiSettings(int fd, cJSON *messageBody)
{
  printf("%s_%d: \n",__FUNCTION__,__LINE__);
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   char ssidBuf[64]={0};  
   ENCRYPT_T encrypt_5g; 
   char ssid_5g[64]={0};
   ENCRYPT_T encrypt_2g; 
   char ssid_2g[64]={0};
   int value=-1;
   int localValue=1;
   int ret=0;
   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;
    int old_wlan_idx;
    int old_vwlan_idx;
	int settingFlag=0;

   if(jasonObj = cJSON_GetObjectItem(messageBody,"uncrypWifiSetting"))
   {
    printf("%s_%d: \n",__FUNCTION__,__LINE__);


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
	}
    
	wlan_idx = 1;
	encrypt_2g=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_2g")->valuestring);
	strcpy(ssid_2g,cJSON_GetObjectItem(jasonObj,"ssid_2g")->valuestring);
	if(encrypt_2g==ENCRYPT_DISABLED)
	{
	apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt_2g);
	apmib_set(MIB_WLAN_SSID, (void *)&ssid_2g);	
	}

	if(apmib_update(CURRENT_SETTING) <= 0)
    {
      printf("apmib_update CURRENT_SETTING fail.\n");
    }
 
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;
   }
	printf("%s_%d: \n",__FUNCTION__,__LINE__);

    /*response ack message to master*/
	responseJSON = cJSON_CreateObject();
	cJSON_AddStringToObject(responseJSON, "messageType", "7"); //4==KLINK_SALAVE_SEND_ACK_RESPONSE
	pResponseMsg = cJSON_Print(responseJSON); 
	printf("%s_%d:send version ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
	cJSON_Delete(responseJSON);	
	system("init.sh ap bridge");
}

int _slave klinkSlaveStateMaching(int sd,int messageType,cJSON *messageBody)
{

 switch(messageType)
 {
  case KLINK_START:                     	//messageType=0
     sendFwVersionMessageToMaster(sd); 		//messageType=1
	 break;
  case KLINK_MASTER_SEND_VERSION_ACK:  //messageType=2
     printf("=>get_master_version_ack\n");
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
     sendHeardBeatMessage(sd);  
	 break;
   case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
     syncMasterLedSwitch(sd,messageBody); 
	 break;
   case KLINK_MASTER_SEND_UNENCRYP_WIFI_INFO_TO_SLAVE:
     syncUncrypWifiSettings(sd,messageBody); 
	 break; 
   case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:    
     syncGuestWifiSettings(sd,messageBody); 
	 break;
  default:
  	/*if havn't incomming message,must period send beartbead*/ 
  	 messageType=KLINK_HEARD_BEAD_SYNC_MESSAGE; 
  	 break;
 }

  if(messageType==KLINK_MASTER_SEND_VERSION_ACK)
   {
     printf("%s_%d: slave prepare send heartBeat sync...\n",__FUNCTION__,__LINE__);
     sendHeardBeatMessage(sd);   
   }
}

void _slave parseMessageFromMaster(int sd, char* masterMessage) 
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
	 printf("%s_%d:messageType=%d\n ",__FUNCTION__,__LINE__,messageType);
	 klinkSlaveStateMaching(sd,messageType,json);
     
    }
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
