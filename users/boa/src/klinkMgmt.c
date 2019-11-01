/*
Copyright (c) 2019, All rights reserved.

File         : klinkMgmt.c
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

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include "apmib.h"
#include "cJSON.h"



#include "./../../klink/klink.h"
extern convertIntToString(char* str, int intnum);
extern KlinkNode_t* initKlinkListHead();
extern KlinkNode_t *addKlinkListNode(KlinkNode_t*head,char* date,int type);
extern KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,char*  date);
extern KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,char* date);
extern void showKlinkNode(KlinkNode_t*head);
//static KlinkNode_t  klinkNodeData;
 KlinkNode_t* g_pKlinkHeadNode;

int getSlaveMacNum(cJSON *root)
{
  cJSON *head;
  cJSON *pos;
  cJSON *parameters;
  int slaveNum=0;
  head = cJSON_GetObjectItem(root, "child_devices");
  if(head != NULL)
   {
   for(pos = head->child; pos != NULL; pos = pos->next)
   {
    parameters = cJSON_GetObjectItem(pos, "mac_address");
    if(parameters != NULL)
    {
      slaveNum++;
    }
   }
  }
  else
  {
     printf("%s:%d： can't find child_devices.", __FUNCTION__, __LINE__);
  }
  return slaveNum;
}

int getMeshNodeNumber()
{
 int slaveNum=0;
 char* out=NULL;
 cJSON *root=NULL; 
 FILE *fp=NULL;
 int read=0;
 int len  = 0;
 char*	line  = NULL;   
 fp = fopen("/tmp/topology_json", "r");
 if(fp == NULL)
 {
  printf("==>%s_%d:open /tmp/topology_json fail...",__FUNCTION__,__LINE__);
  return 0;
 }
 else
 {
  read = getline(&line, &len, fp);
  fclose(fp);
  if(line) 
  {
   root = cJSON_Parse(line);  
   free(line);    
   if(root==0)
   {
    printf("%s_%d: error...\n ",__FUNCTION__,__LINE__);
   }
   else
   {
    slaveNum=getSlaveMacNum(root);
   }
  }
   else
   {
    return RETURN_FAIL;
   }

 }
  out = cJSON_Print(root);
  printf("==>%s_%d:out=\n%s\n\n",__FUNCTION__,__LINE__,out);	
  cJSON_Delete(root);
  return slaveNum;
}

KlinkNode_t getMibInfo(char* sourceValue)
 {
    char *p;
    char *buff;
	int ret=0;
    int i=0;
	KlinkNode_t klinkNodeData;
	memset(&klinkNodeData,0,sizeof(KlinkNode_t));
	
    buff=sourceValue;  
    p = strsep(&buff, ";");
    while(p)
    {
     if(i==1)
	 strcpy(klinkNodeData.slaveVersionInfo.slaveMac,p);
	 if(i==2)
     strcpy(klinkNodeData.slaveVersionInfo.slaveSoftVer,p);
     p = strsep(&buff, ";");
     i++;
    }
    return klinkNodeData;
}


/*add mode*/
KlinkNode_t* addKlinkNodeData(KlinkNode_t*head,KlinkNode_t *pdata)
{
   KlinkNode_t *phead,*new_node;
   phead=head;

   if(NULL==phead)
   {
	return NULL;	
   }
   else
   { 
	while(phead->next!=NULL)   
	{   
	    /*if found same slave node already in the link list,just update node data*/
	    if(!strcmp(phead->next->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac))
	    {
	     printf("==>%s_%d:found same mesh device in the link node list\n",__FUNCTION__,__LINE__);
		 if(strcmp(phead->next->slaveVersionInfo.slaveSoftVer,pdata->slaveVersionInfo.slaveSoftVer))
		 {
		  memset(phead->next->slaveVersionInfo.slaveSoftVer,0,sizeof(phead->next->slaveVersionInfo.slaveSoftVer));
		  strcpy(phead->next->slaveVersionInfo.slaveSoftVer,pdata->slaveVersionInfo.slaveSoftVer);
		 }
		 return phead;
	    }
		phead=phead->next;		
	}
      /*else add new node to the link list tail*/
	  if(NULL!=(new_node=(KlinkNode_t*)malloc(sizeof(KlinkNode_t))))
	  {
	      strcpy(new_node->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac);
		  strcpy(new_node->slaveVersionInfo.slaveSoftVer,pdata->slaveVersionInfo.slaveSoftVer);		   
		  phead->next=new_node;
		  new_node->next=NULL;
		  printf("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveMac);
		  printf("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveSoftVer);	   
		 return phead;
	  }	   
   }
}

int createMeshLinkList()
{
  char tmpBuf[128]={0};
  int slaveNum=0;
  int i=0;
  KlinkNode_t klinkNodeData;
  memset(&klinkNodeData,0x00,sizeof(KlinkNode_t));
  memset(tmpBuf,0x00,sizeof(tmpBuf));
  if (!apmib_get( MIB_KLINK_SLAVE1_SOFT_VERSION,(void *)tmpBuf)) 
  slaveNum=getMeshNodeNumber();	
  if(slaveNum==0)
  {
  	return 0;
  }
  g_pKlinkHeadNode=initKlinkListHead();
  clearKlinkList(g_pKlinkHeadNode);  
   
  for(i; i< slaveNum; i++)
  {
   switch(i)
   {
	case SLAVE1:
	 apmib_get(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE2:
	 apmib_get(MIB_KLINK_SLAVE2_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE3:
	 apmib_get(MIB_KLINK_SLAVE3_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE4:
	 apmib_get(MIB_KLINK_SLAVE4_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE5:
	 apmib_get(MIB_KLINK_SLAVE5_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE6:
	 apmib_get(MIB_KLINK_SLAVE6_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE7:
	 apmib_get(MIB_KLINK_SLAVE7_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE8:
	 apmib_get(MIB_KLINK_SLAVE8_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE9:
	 apmib_get(MIB_KLINK_SLAVE9_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE10:
	 apmib_get(MIB_KLINK_SLAVE10_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE11:
	 apmib_get(MIB_KLINK_SLAVE11_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE12:
	 apmib_get(MIB_KLINK_SLAVE12_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE13:
	 apmib_get(MIB_KLINK_SLAVE13_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE14:
	 apmib_get(MIB_KLINK_SLAVE14_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE15:
	 apmib_get(MIB_KLINK_SLAVE15_SOFT_VERSION, (void *)tmpBuf);
	 break;
	case SLAVE16:
	 apmib_get(MIB_KLINK_SLAVE16_SOFT_VERSION, (void *)tmpBuf);
	 break;
	default:
	 printf("%s_%d:index error,index numbre is %d \n",__FUNCTION__,__LINE__,i);    
	 break;
	}
    klinkNodeData=getMibInfo(tmpBuf);
	addKlinkNodeData(g_pKlinkHeadNode,&klinkNodeData);
	memset(tmpBuf,0,sizeof(tmpBuf));
   
  }
}

 
/*search target node*/

 KlinkNode_t* serchTargetNode(KlinkNode_t*head,KlinkNode_t *pdata)
{
	KlinkNode_t *phead=head;
	if(phead==NULL)
	{
		printf("head_node is empty\n");
		return NULL;
	}
	while(strcmp(phead->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac)&&phead->next!=NULL)
	{    
		phead=phead->next;
	}
	if(!(strcmp(phead->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac)))
	{
		printf("serch succed klink node data=%s\n",phead->slaveVersionInfo.slaveMac);
		printf("#################\n");
		return phead;
	}
	else
	{
		printf("serch failed\n");
		printf("#################\n");
		return NULL;
	}
}

void getSlaveVersion(char targetVersion[], char* mac)
{
 char *pMac=mac;
 KlinkNode_t *pTargetVersion=NULL;
 KlinkNode_t data;
 memset(&data,0x00,sizeof(KlinkNode_t));
 strcpy(data.slaveVersionInfo.slaveMac,pMac);
 createMeshLinkList();
 pTargetVersion=serchTargetNode(g_pKlinkHeadNode,&data);
 strcpy(targetVersion,pTargetVersion->slaveVersionInfo.slaveSoftVer);
}



