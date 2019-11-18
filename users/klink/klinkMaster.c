/*
Copyright (c) 2019, All rights reserved.

File         : klinkMaster.c
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
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include<net/if_arp.h>
#include<sys/ioctl.h>
#include <net/route.h>
#include <net/if.h>
#include <time.h>
#include "cJSON.h"
#include "klink.h"



extern KlinkNode_t* initKlinkListHead();
extern KlinkNode_t *addKlinkListNode(KlinkNode_t*head,char* date,int type);
extern KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,char*  date);
extern KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,char* date);
extern void showKlinkNode(KlinkNode_t*head);
//extern KlinkNode_t* g_pKlinkHead;
//extern KlinkNode_t klinkNodeData;
static unsigned int g_lastMasterCheckTime = 0;
static unsigned int g_lastCheckTIme = 0;

static KlinkNode_t g_klinDataPkts;
KlinkNode_t* g_pKlinkHead;
KlinkNode_t klinkNodeData;
KlinkNode_t g_syncSettings;
static char masterMac[17]={0};
cJSON * masterGenerateMessageHeader(cJSON *root,int messageType,cJSON *messageBody);
const char* masterGenerateJsonMessageBody(int messageType,cJSON *messageBody,char** pMessage);

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


KlinkNode_t* createMeshTopologyLinkList(cJSON *root)
{
  cJSON *head;
  cJSON *pos;
  cJSON *parameters;
  int slaveNum=0;
  KlinkNode_t* pKlink=NULL; 
  KlinkNode_t* pKlinkHead=NULL;
  KlinkNode_t klinkNodeData;
  memset(&klinkNodeData,0,sizeof(KlinkNode_t));

  g_pKlinkHead=initKlinkListHead();
  pKlinkHead=g_pKlinkHead;
  
  /*before create mesh device link list ,clear link node first*/
  clearKlinkList(pKlinkHead);  
  head = cJSON_GetObjectItem(root, "child_devices");
  if(head != NULL)
   {
   for(pos = head->child; pos != NULL; pos = pos->next)
   {
    parameters = cJSON_GetObjectItem(pos, "mac_address");
    if(parameters != NULL)
    {
      slaveNum++;
	  strcpy(klinkNodeData.slaveVersionInfo.slaveMac,parameters->valuestring);
	  /*add slave mesh node to link list tail*/
	  addKlinkListNode(g_pKlinkHead,&klinkNodeData,KLINK_CREATE_TOPOLOGY_LINK_LIST);
    }
   }
   g_pKlinkHead->slaveMeshNum=slaveNum;
  }
  else
  {
     TRACE_DEBUG("%s:%d： can't find child_devices.", __FUNCTION__, __LINE__);
  }
  pKlinkHead->slaveMeshNum=slaveNum;
  return pKlinkHead;
}

KlinkNode_t* createKlinkLinkList()
{
 char* out=NULL;
 cJSON *root=NULL; 
 FILE *fp=NULL;
 KlinkNode_t* pKlinkHead=NULL;
 int read=0;
 int len  = 0;
 char*	line  = NULL;   
 fp = fopen("/tmp/topology_json", "r");
 if(fp == NULL)
 {
  TRACE_DEBUG("==>%s_%d:open /tmp/topology_json fail...",__FUNCTION__,__LINE__);
 }
 else
 {
  read = getline(&line, &len, fp);
  fclose(fp);
  if(line) 
  {
   root = cJSON_Parse(line);  
   free(line);    
   if(root==NULL)
   {
    TRACE_DEBUG("%s_%d: error...\n ",__FUNCTION__,__LINE__);
   }
   else
   {
    pKlinkHead=createMeshTopologyLinkList(root);
   }
  }
   else
   {
    TRACE_DEBUG("==>%s_%d:line value is null..",__FUNCTION__,__LINE__);
   }

 }
  out = cJSON_Print(root);
  TRACE_DEBUG("==>%s_%d:out=\n%s\n\n",__FUNCTION__,__LINE__,out);	
  cJSON_Delete(root);
  return pKlinkHead;
}


KlinkNode_t* setMeshLinklistDataToMib(KlinkNode_t*head)
{	
	KlinkNode_t *phead=head;
	char tmpBuf[64]={0};
	char tmpBuf2[64]={0};
	char tmpSn[32]={0};
	int i=0;
	if(phead==NULL)
	{
		TRACE_DEBUG("list is empty\n");
	}
	else
	{
	   while((phead->next!=NULL))
		{  
		   memset(tmpBuf,0,sizeof(tmpBuf));
		   memset(tmpBuf2,0,sizeof(tmpBuf2));
		   memset(tmpSn,0,sizeof(tmpSn));
		   apmib_get(MIB_KLINK_SLAVE16_SOFT_VERSION, (void *)tmpBuf2);
		   if(strlen(tmpBuf2)>12)
		   	{
		   	 apmib_set(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE2_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE3_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE4_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE5_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE6_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE7_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE8_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE9_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE10_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE11_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE12_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE13_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE14_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE15_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE16_SOFT_VERSION, (void *)tmpBuf);			 
		   	}

		   sprintf(tmpBuf,"%s;%s",phead->next->slaveDevideInfo.slaveMacAddr,phead->next->slaveDevideInfo.slaveFwVersion);

		   if(strlen(phead->next->slaveDevideInfo.sn)==0)
             sprintf(tmpSn,"%s",DEFAULT_SN);
		   else
		     sprintf(tmpSn,"%s",phead->next->slaveDevideInfo.sn);
		   
           i++;
		   phead=phead->next;
		   switch(i)
		   {
		   case SLAVE1:
		   	memset(tmpBuf2,0,sizeof(tmpBuf2));
		   	apmib_get(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave2;
			memset(tmpBuf2,0,sizeof(tmpBuf2));
	        apmib_set(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf);
			apmib_set(MIB_KLINK_SLAVE1_SN, (void *)tmpSn);
		    break;
		   case SLAVE2:
		   	slave2:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE2_SOFT_VERSION, (void *)tmpBuf2);
		    if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave3;
	
	        apmib_set(MIB_KLINK_SLAVE2_SOFT_VERSION, (void *)tmpBuf);
			apmib_set(MIB_KLINK_SLAVE2_SN, (void *)tmpSn);
		    break;
		   case SLAVE3:
		   	 slave3:
			 memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE3_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave4;
			 apmib_set(MIB_KLINK_SLAVE3_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE3_SN, (void *)tmpSn);
			 break;
		   case SLAVE4:
		   	slave4:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE4_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave5;
			 apmib_set(MIB_KLINK_SLAVE4_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE4_SN, (void *)tmpSn);
			 break;
		   case SLAVE5:
		   	 slave5:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE5_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave6;
			 apmib_set(MIB_KLINK_SLAVE5_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE5_SN, (void *)tmpSn);
			 break;
		   case SLAVE6:
		   	slave6:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE6_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave7;
			  apmib_set(MIB_KLINK_SLAVE6_SOFT_VERSION, (void *)tmpBuf);
			  apmib_set(MIB_KLINK_SLAVE6_SN, (void *)tmpSn);
			  break;
		   case SLAVE7:
		   	slave7:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE7_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave8;
			 apmib_set(MIB_KLINK_SLAVE7_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE7_SN, (void *)tmpSn);
			 break;
		   case SLAVE8:
		   	slave8:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE8_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave9;
			 apmib_set(MIB_KLINK_SLAVE8_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE8_SN, (void *)tmpSn);
			 break;
		   case SLAVE9:
		   	slave9:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE9_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave10;
			  apmib_set(MIB_KLINK_SLAVE9_SOFT_VERSION, (void *)tmpBuf);
			  apmib_set(MIB_KLINK_SLAVE9_SN, (void *)tmpSn);
			  break;
		   case SLAVE10:
		   	slave10:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE10_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave11;
			 apmib_set(MIB_KLINK_SLAVE10_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE10_SN, (void *)tmpSn);
			 break;
		   case SLAVE11:
		   	slave11:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE11_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave12;
			 apmib_set(MIB_KLINK_SLAVE11_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE11_SN, (void *)tmpSn);
			 break;
		   case SLAVE12:
		   	slave12:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE12_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave13;
			  apmib_set(MIB_KLINK_SLAVE12_SOFT_VERSION, (void *)tmpBuf);
			  apmib_set(MIB_KLINK_SLAVE12_SN, (void *)tmpSn);
			  break;
		   case SLAVE13:
		   slave13:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE13_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave14;
			 apmib_set(MIB_KLINK_SLAVE13_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE13_SN, (void *)tmpSn);
			 break;
		   case SLAVE14:
		   	slave14:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE14_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave15;
			 apmib_set(MIB_KLINK_SLAVE14_SOFT_VERSION, (void *)tmpBuf);
			 apmib_set(MIB_KLINK_SLAVE14_SN, (void *)tmpSn);
			 break;
		   case SLAVE15:
		   	slave15:
		   	memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE15_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			if(strncmp(tmpBuf,tmpBuf2,12))
				goto slave16;
			  apmib_set(MIB_KLINK_SLAVE15_SOFT_VERSION, (void *)tmpBuf);
			  apmib_set(MIB_KLINK_SLAVE15_SN, (void *)tmpSn);
		   case SLAVE16:
		   	slave16:
			memset(tmpBuf2,0,sizeof(tmpBuf2));
			apmib_get(MIB_KLINK_SLAVE16_SOFT_VERSION, (void *)tmpBuf2);
			if(strncmp(tmpBuf,tmpBuf2,sizeof(tmpBuf))==0)
			 break;
			  apmib_set(MIB_KLINK_SLAVE16_SOFT_VERSION, (void *)tmpBuf);
			  apmib_set(MIB_KLINK_SLAVE16_SN, (void *)tmpSn);
			  break;
		   default:
		   	  TRACE_DEBUG("%s_%d:index error,index numbre is %d \n",__FUNCTION__,__LINE__,i);	
		   	  break;
		   	}
		   apmib_update(CURRENT_SETTING);

		}
	}


}


/*
 * led is on :MIB_LED_ENABLE=1   
 * led is off:MIB_LED_ENABLE=0
 *
 *led switch data formate:
 *{"message":"3","ledSwitch":{"ledEnable":"0"}}
 *
*/
char* periodCheckSyncValueToSlave_1(int fd,int messageType,cJSON *messageBody)
{
   char *pSendMsg=NULL;
   cJSON *root=NULL;
   cJSON *parameters=NULL;
   int ledEnable;
   char buff[2]={0};
   int disableFlg_2g;
   int disableFlg_5g;
   int old_wlan_idx;
   int old_vwlan_idx;
   
    KlinkNode_t pKlinkNodeTmp;
    KlinkNode_t* pKlinkNode=NULL;
	memset(&pKlinkNode,0,sizeof(pKlinkNode));
	strcpy(pKlinkNodeTmp.slaveVersionInfo.slaveMac,cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring);
    pKlinkNode=serchKlinkListNode(g_pKlinkHead,&pKlinkNodeTmp);
	  
     /*led switch*/
     apmib_get(MIB_LED_ENABLE, (void *)&ledEnable);	 	 
     if(pKlinkNode->syncCfg.ledSwitch != ledEnable)
     pKlinkNode->syncCfg.ledSyncFlag=SYNC_FLAG_1;

     /*sync  wlan settings*/
	 ENCRYPT_T encrypt_5g;
	 char ssidBuf_5g[64]={0};
	 int authVal_5g;
	 int cipher_5g;
	 char psk_5g[64]={0};
	 
	 ENCRYPT_T encrypt_2g;
	 char ssidBuf_2g[64]={0}; 
	 int authVal_2g;
	 int cipher_2g;
	 char psk_2g[64]={0};
	 old_wlan_idx = wlan_idx;
	 old_vwlan_idx = vwlan_idx;
	 vwlan_idx = 0;
     wlan_idx = 0;
     apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_5g);
     apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_5g);
	 apmib_get(MIB_WLAN_WPA_AUTH, (void *)&authVal_5g);
	 apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher_5g);
	 apmib_get(MIB_WLAN_WPA_PSK, (void *)&psk_5g);
	 wlan_idx = 1;
	 apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_2g);
     apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_2g);
	 apmib_get(MIB_WLAN_WPA_AUTH, (void *)&authVal_2g);
	 apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher_2g);
	 apmib_get(MIB_WLAN_WPA_PSK, (void *)&psk_2g);
#if 0
	 if((encrypt_5g== ENCRYPT_DISABLED ) && ((pKlinkNode->syncCfg.wifiCfg.encrypt_5g!=ENCRYPT_DISABLED)||
	   (strcmp(pKlinkNode->syncCfg.wifiCfg.ssid_5g,ssidBuf_5g))))
	   {
	 	 pKlinkNode->syncCfg.wifiCfg.uncryptWifiSyncFlag=SYNC_FLAG_1;
	   }
	 if((encrypt_2g== ENCRYPT_DISABLED ) && ((pKlinkNode->syncCfg.wifiCfg.encrypt_2g!=ENCRYPT_DISABLED)||
	   (strcmp(pKlinkNode->syncCfg.wifiCfg.ssid_2g,ssidBuf_2g))))
	   {
	 	 pKlinkNode->syncCfg.wifiCfg.uncryptWifiSyncFlag=SYNC_FLAG_1;
	   }
#endif
     if((encrypt_5g!=pKlinkNode->syncCfg.wifiCfg.encrypt_5g)||(strcmp(ssidBuf_5g,pKlinkNode->syncCfg.wifiCfg.ssid_5g))||
	   (authVal_5g!=pKlinkNode->syncCfg.wifiCfg.auth_5g)||(cipher_5g!=pKlinkNode->syncCfg.wifiCfg.cipher_5g)||
	   (strcmp(psk_5g,pKlinkNode->syncCfg.wifiCfg.psk_5g))||(encrypt_2g!=pKlinkNode->syncCfg.wifiCfg.encrypt_2g)||
	   (strcmp(ssidBuf_2g,pKlinkNode->syncCfg.wifiCfg.ssid_2g))||(authVal_2g!=pKlinkNode->syncCfg.wifiCfg.auth_2g)||
	   (cipher_2g!=pKlinkNode->syncCfg.wifiCfg.cipher_2g)||(strcmp(psk_2g,pKlinkNode->syncCfg.wifiCfg.psk_2g)))
       {
        pKlinkNode->syncCfg.wifiCfg.uncryptWifiSyncFlag=SYNC_FLAG_1;
       }
	   	
	 wlan_idx=old_wlan_idx;
	 vwlan_idx=old_vwlan_idx;

	 /*guest*/
	 old_wlan_idx = wlan_idx;
	 old_vwlan_idx = vwlan_idx;
	 vwlan_idx = 2;
     wlan_idx = 0;
	 apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_5g);
	 wlan_idx = 1;
	 apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_2g);	
	 if((pKlinkNode->syncCfg.guestWifi.guestWifiSwitch_5g!=disableFlg_5g)||
	 (pKlinkNode->syncCfg.guestWifi.guestWifiSwitch_2g!=disableFlg_2g))
	 {	 
	  pKlinkNode->syncCfg.guestWifi.guestSyncFlag=SYNC_FLAG_1;
	 } 
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;	

	 /*led control*/
	 if(pKlinkNode->syncCfg.ledSyncFlag==SYNC_FLAG_1)
	 { 
       messageType=KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE;
	   pSendMsg=masterGenerateJsonMessageBody(messageType,messageBody,&pSendMsg);
       TRACE_DEBUG("%s_%d:send led sync message=\n%s \n",__FUNCTION__,__LINE__,pSendMsg);
       send(fd, pSendMsg,strlen(pSendMsg), 0) ;
	 } 

	 /*when wlan is disabled,sync wlan settings*/
	 else if (pKlinkNode->syncCfg.wifiCfg.uncryptWifiSyncFlag==SYNC_FLAG_1) 
	 {	
	   messageType=KLINK_MASTER_SEND_WIFI_CFG_INFO_TO_SLAVE;
	   pSendMsg=masterGenerateJsonMessageBody(messageType,messageBody,&pSendMsg);
       TRACE_DEBUG("%s_%d:send uncrypt sync message=\n%s\n",__FUNCTION__,__LINE__,pSendMsg);
       send(fd, pSendMsg,strlen(pSendMsg), 0) ;
	 } 
	 /*guest wifi settins*/
	 else if(pKlinkNode->syncCfg.guestWifi.guestSyncFlag==SYNC_FLAG_1)
	 {	   
	   messageType=KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE;
	   pSendMsg=masterGenerateJsonMessageBody(messageType,messageBody,&pSendMsg);
       TRACE_DEBUG("%s_%d:send guest sync message=\n%s\n",__FUNCTION__,__LINE__,pSendMsg);
       send(fd, pSendMsg,strlen(pSendMsg), 0);
	 }
    else
    {   
       sendHeartBeatMessage(fd,messageType,messageBody);       
    }
	
}

int parseSlaveVersionConf(int fd, cJSON *messageBody)
{
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   char *pSlaveFwVersion=NULL;
   char *pSlaveMac=NULL;
   KlinkNode_t* pKlinkHead=NULL;
   KlinkNode_t* pKlinkData=&klinkNodeData;

   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;

   if(jasonObj = cJSON_GetObjectItem(messageBody,"slaveVersion"))
   {
    strcpy(pKlinkData->slaveVersionInfo.slaveSoftVer,cJSON_GetObjectItem(jasonObj,"slaveSoftVer")->valuestring);
    strcpy(pKlinkData->slaveVersionInfo.slaveMac,cJSON_GetObjectItem(jasonObj,"slaveMac")->valuestring);
   }

   pKlinkHead=createKlinkLinkList();
   addKlinkListNode(pKlinkHead,pKlinkData,KLINK_SLAVE_SOFT_VERSION);
   showKlinkNode(pKlinkHead);
   setMeshLinklistDataToMib(pKlinkHead);
    /*send ack message to slave*/
	responseJSON = cJSON_CreateObject();
	cJSON_AddStringToObject(responseJSON, "messageType", "2"); //2==KLINK_MASTER_SEND_ACK_VERSION_INFO
//	cJSON_AddNumberToObject(responseJSON, "responseCode", fwVersion);
	pResponseMsg = cJSON_Print(responseJSON); 
	TRACE_DEBUG("%s_%d:send version ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);
	send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
	cJSON_Delete(responseJSON);	
}

/*
 *heard beat formate:
 *{"message":"10","heardBead":{"heardBeadState":"sync"}}
 *
*/
static int masterSendHeardBeatMessage(int fd)
{
  char* stringMessage=NULL;
  cJSON *topRoot=NULL;
  cJSON *root=NULL;
  cJSON *parameters=NULL;
  TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
  g_lastMasterCheckTime = upSecond();
  if(upSecond() - g_lastMasterCheckTime > 8)	
  {
      topRoot = cJSON_CreateObject();
      cJSON_AddStringToObject(topRoot, "messageType", "10");//1==KLINK_SLAVE_SEND_VERSION_INFO
      cJSON_AddItemToObject(topRoot, "heardBead", root = cJSON_CreateObject());
      cJSON_AddStringToObject(root, "heardBeadState", "sync");
      stringMessage = cJSON_Print(topRoot);  
      TRACE_DEBUG("%s_%d:sen message=%s \n",__FUNCTION__,__LINE__,stringMessage);
      send(fd, stringMessage,strlen(stringMessage), 0) ;
      cJSON_Delete(topRoot);
      g_lastMasterCheckTime = upSecond();
 }
}

/*backup config which already sync ,the config use for check config change */
int backupCurrentCfg(int messageType)
{
 meshSetting_t *pCfg = &g_syncSettings;
 int ledSwitch;
  ENCRYPT_T encrypt_5g;
  ENCRYPT_T encrypt_2g;
 char ssid_5g[64]={0};
 char ssid_2g[64]={0};
 int disableFlg_2g;
 int disableFlg_5g;
 int old_wlan_idx;
 int old_vwlan_idx;
 
 switch(messageType)
 {
  case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
  	 apmib_get(MIB_LED_ENABLE, (void *)&ledSwitch);
	 pCfg->ledSwitch=ledSwitch;
  	break;
  case KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK:
  	 old_wlan_idx = wlan_idx;
	 old_vwlan_idx = vwlan_idx;
	 vwlan_idx = 0;
     wlan_idx = 0;
	 apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_5g);
  	 pCfg->wifiCfg.encrypt_5g=encrypt_5g;
  	 apmib_get( MIB_WLAN_SSID,  (void *)&ssid_5g);
     memset(pCfg->wifiCfg.ssid_5g,0,sizeof(pCfg->wifiCfg.ssid_5g));
	 strcpy(pCfg->wifiCfg.ssid_5g,ssid_5g);
	  wlan_idx = 1;
	 apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_2g);
	 pCfg->wifiCfg.encrypt_2g=encrypt_2g;
  	 apmib_get( MIB_WLAN_SSID,  (void *)&ssid_2g);
     memset(pCfg->wifiCfg.ssid_2g,0,sizeof(pCfg->wifiCfg.ssid_2g));
	 strcpy(pCfg->wifiCfg.ssid_2g,ssid_2g);	 
	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;	
	  
  	break;
  case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:

  	 old_wlan_idx = wlan_idx;
	 old_vwlan_idx = vwlan_idx;
	 vwlan_idx = 2;
     wlan_idx = 0;
	 apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_5g);
	 pCfg->guestWifi.guestWifiSwitch_5g=disableFlg_5g;
	 wlan_idx = 1;
	 apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_2g);
	  pCfg->guestWifi.guestWifiSwitch_2g=disableFlg_2g;

	 wlan_idx = old_wlan_idx;
     vwlan_idx = old_vwlan_idx;	
  	break;
  default:
    break;
 }
  
}

/*add message to klink structer node*/
void cjsonToMessageNode(cJSON *messageBody,KlinkNode_t *messageNode)
{
 cJSON*jasonObj=NULL;
 int messageType;
 messageType=atoi(cJSON_GetObjectItem(messageBody,"messageType")->valuestring);
 messageNode->klinkMsgStaMachine=messageType;
  switch(messageType)
 {
  case KLINK_SLAVE_REPORT_DEVICE_INFO:
  	{
  	   strcpy(messageNode->slaveDevideInfo.slaveMacAddr,cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring);
	   if(jasonObj = cJSON_GetObjectItem(messageBody,"devInfo"))
	   {
         strcpy(messageNode->slaveDevideInfo.slaveFwVersion,cJSON_GetObjectItem(jasonObj,"slaveSoftVer")->valuestring);
		 strcpy(messageNode->slaveDevideInfo.sn,cJSON_GetObjectItem(jasonObj,"sn")->valuestring);
       } 
	   if(jasonObj = cJSON_GetObjectItem(messageBody,"ledSwitch"))
	   {
         messageNode->syncCfg.ledSwitch=atoi(cJSON_GetObjectItem(jasonObj,"ledEnable")->valuestring);
       }  
	   if(jasonObj = cJSON_GetObjectItem(messageBody,"wifiCfg"))
	   {
	     messageNode->syncCfg.wifiCfg.encrypt_5g=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_5g")->valuestring);
		 strcpy(messageNode->syncCfg.wifiCfg.ssid_5g,cJSON_GetObjectItem(jasonObj,"ssid_5g")->valuestring);
		 messageNode->syncCfg.wifiCfg.auth_5g=atoi(cJSON_GetObjectItem(jasonObj,"auth_5g")->valuestring);
		 messageNode->syncCfg.wifiCfg.cipher_5g=atoi(cJSON_GetObjectItem(jasonObj,"cipher_5g")->valuestring);
		 strcpy(messageNode->syncCfg.wifiCfg.psk_5g,cJSON_GetObjectItem(jasonObj,"psk_5g")->valuestring);
		 
		 messageNode->syncCfg.wifiCfg.encrypt_2g=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_2g")->valuestring);
		 strcpy(messageNode->syncCfg.wifiCfg.ssid_2g,cJSON_GetObjectItem(jasonObj,"ssid_2g")->valuestring);
		 messageNode->syncCfg.wifiCfg.auth_2g=atoi(cJSON_GetObjectItem(jasonObj,"auth_2g")->valuestring);
		 messageNode->syncCfg.wifiCfg.cipher_2g=atoi(cJSON_GetObjectItem(jasonObj,"cipher_2g")->valuestring);
		 strcpy(messageNode->syncCfg.wifiCfg.psk_2g,cJSON_GetObjectItem(jasonObj,"psk_2g")->valuestring);
	   } 

	   if(jasonObj = cJSON_GetObjectItem(messageBody,"guestWifi"))
	   {
         messageNode->syncCfg.guestWifi.guestWifiSwitch_5g=atoi(cJSON_GetObjectItem(jasonObj,"guestSwitch_5g")->valuestring);
		 messageNode->syncCfg.guestWifi.guestWifiSwitch_2g=atoi(cJSON_GetObjectItem(jasonObj,"guestSwitch_2g")->valuestring);
       }  
  	}
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
     TRACE_DEBUG("=>get slave heard bead \n");
	 break;
  case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
    TRACE_DEBUG("=>get slave led switch ack\n");
	 break;
    case KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK:
    TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;
	case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:	 
    TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;	
  default:
  	 break;
 }
}



cJSON * masterGenerateMessageHeader(cJSON *root,int messageType,cJSON *messageBody)
{   
	cJSON *pJson=root;
	char macAddr[18]={0};
	char fwVersion[18]={0};	

    switch (messageType)
    {
        case KLINK_START:
			cJSON_AddStringToObject(pJson, "messageType", "0");
			break;	
		case KLINK_SLAVE_REPORT_DEVICE_INFO: 					 
		   cJSON_AddStringToObject(pJson, "messageType", "2");
		   break;
		case KLINK_HEARD_BEAD_SYNC_MESSAGE:
		   cJSON_AddStringToObject(pJson, "messageType", "3");
		   TRACE_DEBUG("=>get slave heard bead \n");		  
		   break;
		   
		case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
		   cJSON_AddStringToObject(pJson, "messageType", "4");
		   break;
		case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
			   cJSON_AddStringToObject(pJson, "messageType", "3");
		  TRACE_DEBUG("=>get slave led switch ack\n");
		   break;
		
		case KLINK_MASTER_SEND_WIFI_CFG_INFO_TO_SLAVE:
		   cJSON_AddStringToObject(pJson, "messageType", "6");
		   break;
		  case KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK:
		  	   cJSON_AddStringToObject(pJson, "messageType", "3");
		  TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
		 
		   break;
		  case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:
		  	cJSON_AddStringToObject(pJson, "messageType", "8");
		   break;
		  case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:  
             cJSON_AddStringToObject(pJson, "messageType", "3");
		  TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
		   break; 
		default:
		   break;
    }
		cJSON_AddStringToObject(pJson, "sourceMac", masterMac);	
	    if(messageType!=KLINK_START)
	    cJSON_AddStringToObject(pJson, "destMac", cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring);	
	    return pJson;
}

/*add settings to message body*/
const char* masterGenerateJsonMessageBody(int messageType,cJSON *messageBody,char** pMessage)
{
	char* stringMessage=NULL;
    cJSON *topRoot=NULL;
	cJSON *root=NULL;
	char buff[2]={0};
	int old_wlan_idx;
    int old_vwlan_idx;
 		  
	topRoot = cJSON_CreateObject();
	 if (!topRoot)
    {
        TRACE_DEBUG("cJsonCreateObj failed!");
        return NULL;
    }
	 
    /* generate header */
    topRoot=masterGenerateMessageHeader(topRoot,messageType,messageBody);
 switch(messageType)
 {
  case KLINK_SLAVE_REPORT_DEVICE_INFO:                     	//messageType=0
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
  	{
     TRACE_DEBUG("=>get slave heard bead \n");
	 cJSON_AddItemToObject(topRoot, "heartbeat", root = cJSON_CreateObject());
	 cJSON_AddStringToObject(root, "heartbeatSync", "sync");
  	}
	 break;
  case KLINK_MASTER_SEND_LED_SWITCH_TO_SLAVE:
  	{
  	 int ledEnable;	 
	 apmib_get(MIB_LED_ENABLE, (void *)&ledEnable);	
     sprintf(buff,"%d",ledEnable);
	 cJSON_AddItemToObject(topRoot, "ledSwitch", root = cJSON_CreateObject());
	 cJSON_AddStringToObject(root, "ledEnable", buff);
  	}
	break;
  case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
    TRACE_DEBUG("=>get slave led switch ack\n");
	 break;
  case KLINK_MASTER_SEND_WIFI_CFG_INFO_TO_SLAVE:
  	{
  	   ENCRYPT_T encrypt_5g;
	   char ssidBuf_5g[64]={0};
	   int authVal_5g;
	   int cipher_5g;
	   char psk_5g[64]={0};
	   ENCRYPT_T encrypt_2g;
	   char ssidBuf_2g[64]={0}; 
	   int authVal_2g;
	   int cipher_2g;
	   char psk_2g[64]={0};
	   old_wlan_idx = wlan_idx;
	   old_vwlan_idx = vwlan_idx;
	   vwlan_idx = 0;
       wlan_idx = 0;
       apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_5g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_5g);
	   apmib_get(MIB_WLAN_WPA_AUTH, (void *)&authVal_5g);
	   apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher_5g);
	   apmib_get(MIB_WLAN_WPA_PSK, (void *)&psk_5g);
	   wlan_idx = 1;
	   apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt_2g);
       apmib_get( MIB_WLAN_SSID,  (void *)&ssidBuf_2g);
	   apmib_get(MIB_WLAN_WPA_AUTH, (void *)&authVal_2g);
	   apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&cipher_2g);
	   apmib_get(MIB_WLAN_WPA_PSK, (void *)&psk_2g);
	   wlan_idx=old_wlan_idx;
	   vwlan_idx=old_vwlan_idx;
  	   cJSON_AddItemToObject(topRoot, "wifiCfg", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_5g);
	   cJSON_AddStringToObject(root, "encrypt_5g", buff);
	   cJSON_AddStringToObject(root, "ssid_5g", ssidBuf_5g);
	   	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",authVal_5g);
	   cJSON_AddStringToObject(root, "auth_5g", buff);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",cipher_5g);
	   cJSON_AddStringToObject(root, "cipher_5g", buff);
	   cJSON_AddStringToObject(root, "psk_5g", psk_5g);
	   
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",encrypt_2g);
	   cJSON_AddStringToObject(root, "encrypt_2g", buff);
	   cJSON_AddStringToObject(root, "ssid_2g", ssidBuf_2g);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",authVal_2g);
	   cJSON_AddStringToObject(root, "auth_2g", buff);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",cipher_2g);
	   cJSON_AddStringToObject(root, "cipher_2g", buff);
	   cJSON_AddStringToObject(root, "psk_2g", psk_2g);
  	}
	 break;
    case KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK:
     TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;
	case KLINK_MASTER_SEND_GUEST_WIFI_INFO_TO_SLAVE:
	{
	   int disableFlg_2g;
       int disableFlg_5g;
	   old_wlan_idx = wlan_idx;
	   old_vwlan_idx = vwlan_idx;
	   vwlan_idx = 2;
       wlan_idx = 0;
       if (apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_5g) == 0)
       {
          TRACE_DEBUG("get guest wlan5g disable err\n");
       }
       wlan_idx = 1;
       if (apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disableFlg_2g) == 0)
       {
          TRACE_DEBUG("get guest wlan2g disable err\n");
       }
	   wlan_idx = old_wlan_idx;
       vwlan_idx = old_vwlan_idx;
	   cJSON_AddItemToObject(topRoot, "guestWifi", root = cJSON_CreateObject());
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_5g);
	   cJSON_AddStringToObject(root, "guestSwitch_5g", buff);
	   memset(buff,0,sizeof(buff));
	   sprintf(buff,"%d",disableFlg_2g);
	   cJSON_AddStringToObject(root, "guestSwitch_2g", buff);
	}
	 break;
	case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:
    TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;	
  default:
  	 break;
 }
 	*pMessage = cJSON_Print(topRoot);  
	cJSON_Delete(topRoot);	
	return *pMessage;
}

/*
 *heard beat formate:
 *{"message":"10","heardBead":{"heardBeadState":"sync"}}
 *
*/
int sendHeartBeatMessage(int fd,int messageType, cJSON *messageBody)
{
  char* stringMessage=NULL;
  cJSON *topRoot=NULL;
  cJSON *root=NULL;
  cJSON *parameters=NULL;

   stringMessage=masterGenerateJsonMessageBody(messageType,messageBody,&stringMessage);
   TRACE_DEBUG("%s_%d:send message=%s \n",__FUNCTION__,__LINE__,stringMessage);
   send(fd, stringMessage,strlen(stringMessage), 0) ;
}

int addSlaveDeviceInfoToLinkList(int fd, cJSON *messageBody,KlinkNode_t *messageNode)
{
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   char *pSlaveFwVersion=NULL;
   char *pSlaveMac=NULL;
   KlinkNode_t* pKlinkHead=NULL;
   KlinkNode_t* pKlinkData=&klinkNodeData;
   memset(&g_klinDataPkts, 0, sizeof(g_klinDataPkts));
   KlinkNode_t *messageDevice=messageNode;
   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;
   int messageType=0;

   cjsonToMessageNode(messageBody,messageNode);
   addKlinkListNode_1(g_pKlinkHead,messageNode);
   setMeshLinklistDataToMib(g_pKlinkHead);
   messageType = atoi(cJSON_GetObjectItem(messageBody,"messageType")->valuestring);
   pResponseMsg=masterGenerateJsonMessageBody(messageType,messageBody,&pResponseMsg);
   TRACE_DEBUG("%s_%d:send version ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);
   send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
}

int klinkMasterStateMaching(int sd,int messageType,cJSON *messageBody)
{
  memset(&g_klinDataPkts, 0, sizeof(g_klinDataPkts));
  //meshSetting_t *pCfg = &g_syncSettings;
 switch(messageType)
 {
  case KLINK_SLAVE_REPORT_DEVICE_INFO:
     addSlaveDeviceInfoToLinkList(sd,messageBody,&g_klinDataPkts); //parse version then sent ack messageType=2
	 break;
  case KLINK_HEARD_BEAD_SYNC_MESSAGE:
     TRACE_DEBUG("=>get slave heard bead \n");
	 break;
  case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
    updateKlinkListNodeData(g_pKlinkHead,messageBody);
    TRACE_DEBUG("=>get slave led switch ack\n");
	 break;
    case KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK:
	updateKlinkListNodeData(g_pKlinkHead,messageBody);
    TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;
	case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:
	 updateKlinkListNodeData(g_pKlinkHead,messageBody);
    TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 break;	
  default:
  	 break;
 }
 
 if((messageType==KLINK_SALAVE_SEND_LED_SWITCH_ACK)||(messageType==KLINK_SLAVE_SEND_WIFI_CFG_SETTING_ACK)||
 	(messageType==KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK)||(messageType==KLINK_HEARD_BEAD_SYNC_MESSAGE))
  {
   //periodCheckSyncValueToSlave(sd,messageType,messageBody);
   periodCheckSyncValueToSlave_1(sd,messageType,messageBody);
  }
 return 0;
}

int parseMessageFromSlave(int sd, char* slaveMessage) 
{

    cJSON *pJson=NULL;
	int messageType=-1;

    pJson = cJSON_Parse(slaveMessage);
    if (!pJson)
    {
        TRACE_DEBUG("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
      /*{"slaveVersion":["messageType":"0"{"slaveSoftVer":"WM V1.0.5","slaveMac":"00:11:22:33:44:55"}]}*/
     //if(jason_obj = cJSON_GetObjectItem(json,"slaveVersion"))	
    messageType = atoi(cJSON_GetObjectItem(pJson,"messageType")->valuestring);	
	if(messageType>=1)
	  {
	    /*if dest mac not equal,it means message not send to mine,exit*/
	    if(strcmp(cJSON_GetObjectItem(pJson,"destMac")->valuestring,masterMac))
		return 0;
	  }
	klinkMasterStateMaching(sd,messageType,pJson);
    }
	return 0;
}

const char* klinkStartMessageBody(int messageType,char** pMessage)
{
	char* stringMessage=NULL;
    cJSON *topRoot=NULL;
	cJSON *root=NULL;
 	root = cJSON_CreateObject();	  
	topRoot = cJSON_CreateObject();
	 if (!topRoot)
    {
        TRACE_DEBUG("cJsonCreateObj failed!");
        return NULL;
    }
	 
    topRoot=masterGenerateMessageHeader(topRoot,messageType,root);
 	*pMessage = cJSON_Print(topRoot);  
	cJSON_Delete(topRoot);	
	cJSON_Delete(root);	
	return *pMessage;
}

int main(int argc , char *argv[])
{
    int opt = TRUE;
	char *message=NULL;
    int masterSocket , addrlen , newSocket , clientSocket[30] , maxClients = 30 , activity, i , valread , sd;
    int maxSd;
    struct sockaddr_in address;
      
    char buffer[1025];  //data buffer of 1K
    /*set of socket descriptors*/
    fd_set readfds;
    memset(&g_syncSettings, 0, sizeof(g_syncSettings));
	getMacAddr(masterMac);     
    message = klinkStartMessageBody(KLINK_START,&message);

    /*initialise all clientSocket[] to 0 so not checked*/
    for (i = 0; i < maxClients; i++) 
    {
        clientSocket[i] = 0;
    }
      
    if( (masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {   
        close(masterSocket);
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    /*set master socket to allow multiple connections*/ 
    if( setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( KLINK_PORT );
      
    if (bind(masterSocket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    TRACE_DEBUG("Listener on port %d \n", KLINK_PORT);
     
    /*try to specify maximum of 16 pending connections for the master socket*/
    if (listen(masterSocket, 16) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    /*accept the incoming connection*/
    addrlen = sizeof(address);
    puts("klink master waiting for connections ...");
    apmib_init(); 
    int slaveNumber=0;

	//g_pKlinkHead=initKlinkListHead();
	 g_lastMasterCheckTime = upSecond();
	 g_pKlinkHead=initKlinkListHead(); 
     clearKlinkList(g_pKlinkHead);  
    while(TRUE) 
    {
        /*clear the socket set*/
        FD_ZERO(&readfds);
  
        /*add master socket to set*/
        FD_SET(masterSocket, &readfds);
        maxSd = masterSocket;
         
        /*add child sockets to set*/
        for ( i = 0 ; i < maxClients ; i++) 
        {
            /*socket descriptor*/
            sd = clientSocket[i];
             
            /*if valid socket descriptor then add to read list*/
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            /*highest file descriptor number, need it for the select function*/
            if(sd > maxSd)
                maxSd = sd;
        }
  
        /*wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely*/
        activity = select( maxSd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            TRACE_DEBUG("select error");
        }
          
        /*If something happened on the master socket , then its an incoming connection*/
        if (FD_ISSET(masterSocket, &readfds)) 
        {
            if ((newSocket = accept(masterSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            /*inform user of socket number - used in send and receive commands*/
            TRACE_DEBUG("new connection , socket fd is %d , ip is : %s  \n" , newSocket , inet_ntoa(address.sin_addr) );
        
            /*send new connection greeting message*/
            if( send(newSocket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            puts("Welcome message sent successfully");
              
            /*add new socket to array of sockets*/
            for (i = 0; i < maxClients; i++) 
            {
                /*if position is empty*/
                if( clientSocket[i] == 0 )
                {
                    clientSocket[i] = newSocket;
                    TRACE_DEBUG("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        /*else its some IO operation on some other socket :)*/
        for (i = 0; i < maxClients; i++) 
        {
            sd = clientSocket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                /*Check if it was for closing , and also read the incoming message*/
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    /*somebody disconnected , get his details and print*/
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    TRACE_DEBUG("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    /*Close the socket and mark as 0 in list for reuse*/
                    close( sd );
                    clientSocket[i] = 0;
                }
                  
                /*Echo back the message that came in*/
                else
                {
                    /*set the string terminating NULL byte on the end of the data read*/
                    buffer[valread] = '\0';
					TRACE_DEBUG("++++get info from slave :%s\n",buffer);
                    parseMessageFromSlave(sd, (char*)buffer);
					//periodCheckSyncValueToSlave(sd);
                }
            }
        }

    }
      
    return 0;
} 

