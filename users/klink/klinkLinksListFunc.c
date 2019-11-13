/*
Copyright (c) 2019, All rights reserved.

File         : klinkLinkListFunc.c
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
#include <string.h>
#include<malloc.h>
#include <errno.h>
#include "klink.h"
#include "cJSON.h"


/*init head*/
KlinkNode_t* initKlinkListHead()
{
   KlinkNode_t*head=(KlinkNode_t*)malloc(sizeof(KlinkNode_t));
   if(NULL==head)
   {
	perror("malloc\n");
	return NULL; 
   } 
   head->slaveMeshNum=0;
   memset(&(head->slaveVersionInfo),0,sizeof(KlinkSlaveVersion_t));
   head->next=NULL;
   TRACE_DEBUG("init head succed!\n");
   TRACE_DEBUG("#################\n");
   return head;
} 

/*destroy link list*/
int destroyKlinkList( KlinkNode_t *head)  
{  
    KlinkNode_t *p;  
    if(head==NULL)  
        return 0;  
    while(head)  
    {  
        p=head->next;  
        free(head);  
        head=p;  
    }  
    return 1;  
}  
  
/*cleart link list node*/
int clearKlinkList( KlinkNode_t *head)  
{  
	TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);

    KlinkNode_t *p,*q;  
    if(head==NULL)  
        return 0;  
    p=head->next;  
    while(p!=NULL)  
    {  
        q=p->next;  
        free(p);  
        p=q;  
    }  
    head->next=NULL;  
    return 1;  
}  

/*add mode*/
KlinkNode_t *addKlinkListNode_1(KlinkNode_t*head,KlinkNode_t *pdata)
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
	{   TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
	    /*if found same slave node already in the link list,just update node data*/
	    if(!strcmp(phead->next->slaveDevideInfo.slaveMacAddr,pdata->slaveDevideInfo.slaveMacAddr))
	    {
	    TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
	      TRACE_DEBUG("==>%s_%d:found same mesh device in the link node list\n",__FUNCTION__,__LINE__);
		  switch(pdata->klinkMsgStaMachine)
		  {
		    case KLINK_SLAVE_REPORT_DEVICE_INFO:
		    {
		    TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
		     /*slave device info*/
		     strcpy(phead->next->slaveDevideInfo.slaveMacAddr,pdata->slaveDevideInfo.slaveMacAddr);
			 strcpy(phead->next->slaveDevideInfo.slaveFwVersion,pdata->slaveDevideInfo.slaveFwVersion);
			 strcpy(phead->next->slaveDevideInfo.sn,pdata->slaveDevideInfo.sn);
               TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
             /*slave led switch cfg*/
			 phead->next->syncCfg.ledSwitch=pdata->syncCfg.ledSwitch;
			 TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);

             /*slave uncrypt wifi cfg*/
			 phead->next->syncCfg.uncriptWifi.encryptMode_5g=pdata->syncCfg.uncriptWifi.encryptMode_5g;
			 strcpy(phead->next->syncCfg.uncriptWifi.uncryptSsid_5g,pdata->syncCfg.uncriptWifi.uncryptSsid_5g);
			 phead->next->syncCfg.uncriptWifi.encryptMode_2g=pdata->syncCfg.uncriptWifi.encryptMode_2g;
			 strcpy(phead->next->syncCfg.uncriptWifi.uncryptSsid_2g,pdata->syncCfg.uncriptWifi.uncryptSsid_2g);
			 TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);

			 /*guest wifi cfg*/
			 phead->next->syncCfg.guestWifi.guestWifiSwitch_5g= pdata->syncCfg.guestWifi.guestWifiSwitch_5g;
			 phead->next->syncCfg.guestWifi.guestWifiSwitch_2g= pdata->syncCfg.guestWifi.guestWifiSwitch_2g;
			 TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
			 break;
		    }
			case KLINK_HEARD_BEAD_SYNC_MESSAGE:
			{  
			 break;
			}
			case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
			{
			 if(phead->next->syncCfg.ledSwitch!=pdata->syncCfg.ledSwitch)
			 	 phead->next->syncCfg.ledSwitch=pdata->syncCfg.ledSwitch;			 	
			 break;
			}
		    case KLINK_SLAVE_SEND_UNCRYPT_WIFI_SETTING_ACK:
		    {
		      if((phead->next->syncCfg.uncriptWifi.encryptMode_5g!=ENCRYPT_DISABLED)&&
			  (phead->next->syncCfg.uncriptWifi.encryptMode_2g!=ENCRYPT_DISABLED))
		      {
		     	 break;
		      }
			  else
			  {
			     phead->next->syncCfg.uncriptWifi.encryptMode_5g=pdata->syncCfg.uncriptWifi.encryptMode_5g;
				 if(strcmp(phead->next->syncCfg.uncriptWifi.uncryptSsid_5g,pdata->syncCfg.uncriptWifi.uncryptSsid_5g))
				  {
				    memset(phead->next->syncCfg.uncriptWifi.uncryptSsid_5g,0,sizeof(phead->next->syncCfg.uncriptWifi.uncryptSsid_5g));
			        strcpy(phead->next->syncCfg.uncriptWifi.uncryptSsid_5g,pdata->syncCfg.uncriptWifi.uncryptSsid_5g);
				  }
			      phead->next->syncCfg.uncriptWifi.encryptMode_2g=pdata->syncCfg.uncriptWifi.encryptMode_2g;
				  if(strcmp(phead->next->syncCfg.uncriptWifi.uncryptSsid_2g,pdata->syncCfg.uncriptWifi.uncryptSsid_2g))
				  {
				    memset(phead->next->syncCfg.uncriptWifi.uncryptSsid_2g,0,sizeof(phead->next->syncCfg.uncriptWifi.uncryptSsid_2g));
			        strcpy(phead->next->syncCfg.uncriptWifi.uncryptSsid_2g,pdata->syncCfg.uncriptWifi.uncryptSsid_2g);
				  }		  	
			  }			 
		     break;
		    }
		    case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:
		    {
		     if(phead->next->syncCfg.guestWifi.guestWifiSwitch_5g != pdata->syncCfg.guestWifi.guestWifiSwitch_5g)
			 	phead->next->syncCfg.guestWifi.guestWifiSwitch_5g = pdata->syncCfg.guestWifi.guestWifiSwitch_5g;
			 if(phead->next->syncCfg.guestWifi.guestWifiSwitch_2g != pdata->syncCfg.guestWifi.guestWifiSwitch_2g)
			 	phead->next->syncCfg.guestWifi.guestWifiSwitch_2g = pdata->syncCfg.guestWifi.guestWifiSwitch_2g;
		     break;
		    }	   
		  }
		 return phead;
	    }
		phead=phead->next;		
	}
      /*else add new node to the link list tail*/
	  if(NULL!=(new_node=(KlinkNode_t*)malloc(sizeof(KlinkNode_t))))
	  {
		switch(pdata->klinkMsgStaMachine)
		{
		  case KLINK_SLAVE_REPORT_DEVICE_INFO:
		  {
		  TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
		   TRACE_DEBUG("%s_%d:--mac=%s\n ",__FUNCTION__,__LINE__,pdata->slaveDevideInfo.slaveMacAddr);
		     /*slave device info*/
		     strcpy(new_node->slaveDevideInfo.slaveMacAddr,pdata->slaveDevideInfo.slaveMacAddr);
			 	  TRACE_DEBUG("%s_%d:\n ",__FUNCTION__,__LINE__);
			 strcpy(new_node->slaveDevideInfo.slaveFwVersion,pdata->slaveDevideInfo.slaveFwVersion);
			  strcpy(new_node->slaveDevideInfo.sn,pdata->slaveDevideInfo.sn);
			 TRACE_DEBUG("%s_%d:=====>sn=%s===\n ",__FUNCTION__,__LINE__,new_node->slaveDevideInfo.sn);

             /*slave led switch cfg*/
			 new_node->syncCfg.ledSwitch=pdata->syncCfg.ledSwitch;


             /*slave uncrypt wifi cfg*/
			 new_node->syncCfg.uncriptWifi.encryptMode_5g=pdata->syncCfg.uncriptWifi.encryptMode_5g;
			 strcpy(new_node->syncCfg.uncriptWifi.uncryptSsid_5g,pdata->syncCfg.uncriptWifi.uncryptSsid_5g);
			 new_node->syncCfg.uncriptWifi.encryptMode_2g=pdata->syncCfg.uncriptWifi.encryptMode_2g;
			 strcpy(new_node->syncCfg.uncriptWifi.uncryptSsid_2g,pdata->syncCfg.uncriptWifi.uncryptSsid_2g);


			 /*guest wifi cfg*/
			 new_node->syncCfg.guestWifi.guestWifiSwitch_5g= pdata->syncCfg.guestWifi.guestWifiSwitch_5g;
			 new_node->syncCfg.guestWifi.guestWifiSwitch_2g= pdata->syncCfg.guestWifi.guestWifiSwitch_2g;

#if 0
		   /*
           set sync flag to 1 on slave report message,so that cfg can be sync
           once slave device add to mesh network
           */
		    new_node->syncCfg.ledSyncFlag=SYNC_FLAG_1;
			new_node->syncCfg.uncriptWifi.uncryptWifiSyncFlag=SYNC_FLAG_1;
			new_node->syncCfg.guestWifi.guestSyncFlag=SYNC_FLAG_1;	
#endif			 
		  }
		   break;
		}

		  phead->next=new_node;
		  new_node->next=NULL;
		 return phead;
	  }	   
   }
}


/*add mode*/
KlinkNode_t *addKlinkListNode(KlinkNode_t*head,KlinkNode_t *pdata,int type)
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
	     TRACE_DEBUG("==>%s_%d:found same mesh device in the link node list\n",__FUNCTION__,__LINE__);
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
	      if(type==KLINK_CREATE_TOPOLOGY_LINK_LIST)
	      {

	       strcpy(new_node->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac);
	      }	
		  else if(type==KLINK_SLAVE_SOFT_VERSION)
		  {
		   strcpy(new_node->slaveVersionInfo.slaveSoftVer,pdata->slaveVersionInfo.slaveSoftVer);		   
		  }
		  phead->next=new_node;
		  new_node->next=NULL;
		  if(type==KLINK_CREATE_TOPOLOGY_LINK_LIST)
	      {
		  TRACE_DEBUG("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveMac);
		  }
		   else if(type==KLINK_SLAVE_SOFT_VERSION)
		  {
		  TRACE_DEBUG("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveSoftVer);	   
		  }
		 return phead;
	  }	   
   }
}


/*update klink mode data*/
KlinkNode_t *updateKlinkListNodeData(KlinkNode_t*head,cJSON *messageBody)
{
    int messageType=-1;
	int value =-1;
	cJSON *jasonObj=NULL;
  	KlinkNode_t *phead=head;
	if(phead==NULL)
	{
		TRACE_DEBUG("head_node is empty\n");
		return NULL;
	}
	while(strcmp(phead->slaveDevideInfo.slaveMacAddr,cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring)&&phead->next!=NULL)
	{    
		phead=phead->next;
	}
	if(!(strcmp(phead->slaveDevideInfo.slaveMacAddr,cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring)))
	{
	    /*here we found the target node,just updata node data*/
		TRACE_DEBUG("serch succed klink node: [%s]\n",cJSON_GetObjectItem(messageBody,"sourceMac")->valuestring);
		messageType = atoi(cJSON_GetObjectItem(messageBody,"messageType")->valuestring);
		switch(messageType)
		{
		   case KLINK_HEARD_BEAD_SYNC_MESSAGE:
 			 break;
  		   case KLINK_SALAVE_SEND_LED_SWITCH_ACK:
  		   	{
  		   	  if(jasonObj = cJSON_GetObjectItem(messageBody,"ledSwitch"))
  		   	  {
  		   	    value=(strncmp(cJSON_GetObjectItem(jasonObj,"ledEnable")->valuestring, "0",1)?1:0);
			    if(phead->syncCfg.ledSwitch != value )
			   	phead->syncCfg.ledSwitch=value;
  		   	  } 		   	  
  			  phead->syncCfg.ledSyncFlag=SYNC_FLAG_0;
   			  TRACE_DEBUG("=>get slave led switch ack\n");
			  break;
  		   	}
    	   case KLINK_SLAVE_SEND_UNCRYPT_WIFI_SETTING_ACK:
    	   	{
    	   	  if(jasonObj = cJSON_GetObjectItem(messageBody,"uncrypWifi"))
  		   	  {
  		   	    value=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_5g")->valuestring);
				if(phead->syncCfg.uncriptWifi.encryptMode_5g!=value)
				   phead->syncCfg.uncriptWifi.encryptMode_5g=value;
				if(strcmp(phead->syncCfg.uncriptWifi.uncryptSsid_5g,cJSON_GetObjectItem(jasonObj,"ssid_5g")->valuestring))
				   {
				      memset(phead->syncCfg.uncriptWifi.uncryptSsid_5g,0,sizeof(phead->syncCfg.uncriptWifi.uncryptSsid_5g));
					  strcpy(phead->syncCfg.uncriptWifi.uncryptSsid_5g,phead->syncCfg.uncriptWifi.uncryptSsid_5g);
				   }

				value=atoi(cJSON_GetObjectItem(jasonObj,"encrypt_2g")->valuestring);
				if(phead->syncCfg.uncriptWifi.encryptMode_2g!=value)
				   phead->syncCfg.uncriptWifi.encryptMode_2g=value;
				if(strcmp(phead->syncCfg.uncriptWifi.uncryptSsid_2g,cJSON_GetObjectItem(jasonObj,"ssid_2g")->valuestring))
				   {
				      memset(phead->syncCfg.uncriptWifi.uncryptSsid_2g,0,sizeof(phead->syncCfg.uncriptWifi.uncryptSsid_2g));
					  strcpy(phead->syncCfg.uncriptWifi.uncryptSsid_2g,phead->syncCfg.uncriptWifi.uncryptSsid_2g);
				   }
  		   	  }
	 	      phead->syncCfg.uncriptWifi.uncryptWifiSyncFlag=SYNC_FLAG_0;
	  
    		  TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
			  break;
    	   	}
		   case KLINK_SLAVE_SEND_GUEST_WIFI_SETTING_ACK:
		   	{
		   	  if(jasonObj = cJSON_GetObjectItem(messageBody,"guestWifi"))
  		   	  {
  		   	    value=atoi(cJSON_GetObjectItem(jasonObj,"guestSwitch_5g")->valuestring);
				if(phead->syncCfg.guestWifi.guestWifiSwitch_5g!=value)
				   phead->syncCfg.guestWifi.guestWifiSwitch_5g=value;

				value=atoi(cJSON_GetObjectItem(jasonObj,"guestSwitch_2g")->valuestring);
				if(phead->syncCfg.guestWifi.guestWifiSwitch_2g!=value)
				   phead->syncCfg.guestWifi.guestWifiSwitch_2g=value;
  		   	  }
			  phead->syncCfg.guestWifi.guestSyncFlag=SYNC_FLAG_0;
    		  TRACE_DEBUG("=>get uncrypt wifi setting ack\n");
	 		 break;	
		   	}
  			default:
  			 break;			
		}
		return phead;
	}
	else
	{
		TRACE_DEBUG("%s_%d:can not find target updade node,fail...\n",__FUNCTION__,__LINE__);
		TRACE_DEBUG("#################\n");
	}
}

/*search node*/

KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,KlinkNode_t *pdata)
{
    TRACE_DEBUG("%s_%d: mac=%s\n ",__FUNCTION__,__LINE__,pdata->slaveVersionInfo.slaveMac);
	KlinkNode_t *phead=head;
	if(phead==NULL)
	{
		TRACE_DEBUG("head_node is empty\n");
		return NULL;
	}
	while(strcmp(phead->slaveDevideInfo.slaveMacAddr,pdata->slaveVersionInfo.slaveMac)&&phead->next!=NULL)
	{   
	    TRACE_DEBUG("%s_%d: mac=%s\n ",__FUNCTION__,__LINE__,phead->slaveVersionInfo.slaveMac);
		phead=phead->next;
	}
	if(!(strcmp(phead->slaveDevideInfo.slaveMacAddr,pdata->slaveVersionInfo.slaveMac)))
	{
		TRACE_DEBUG("serch succed klink node data=%s\n",phead->slaveVersionInfo.slaveMac);
		TRACE_DEBUG("#################\n");
		return phead;
	}
	else
	{
		TRACE_DEBUG("serch failed\n");
		TRACE_DEBUG("#################\n");
	}
}

/*delete klink node*/
KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,KlinkNode_t *pdata)
{
	KlinkNode_t *phead=head;
	KlinkNode_t *q=head;
	if(phead==NULL)
	{
		TRACE_DEBUG("list is empty\n");
		return(0);
	}
	else
	{
		while(strcmp(phead->slaveDevideInfo.slaveMacAddr,pdata->slaveDevideInfo.slaveMacAddr)&&phead->next!=NULL)
		{
			q=phead;
			phead=phead->next;
		}
		if(!strcmp(phead->slaveDevideInfo.slaveMacAddr,pdata->slaveDevideInfo.slaveMacAddr))
		{
			q->next=phead->next;
			TRACE_DEBUG("free %s succed\n",phead->slaveDevideInfo.slaveMacAddr);
			free(phead);			
			TRACE_DEBUG("#################\n");
		}
		else
		{
			TRACE_DEBUG("NO NODE DELETE\n");
			return(0);
		}
	}	
}

/*print klink node data*/
void showKlinkNode(KlinkNode_t*head)
{
	KlinkNode_t *phead=head;
	if(phead==NULL)
	{
		TRACE_DEBUG("list is empty\n");
		//return 1;
	}
	else
	{
	   while(phead->next!=NULL)
		{
			TRACE_DEBUG("show list node date is:slaveMac     [%s]\n",phead->next->slaveDevideInfo.slaveMacAddr);
			TRACE_DEBUG("show list node date is:slaveSoftVer [%s]\n",phead->next->slaveDevideInfo.slaveMacAddr);
			phead=phead->next;
		}
		TRACE_DEBUG("#################\n");
	}
	//TRACE_DEBUG("show list node date is:slaveNum     [%d]\n",g_pKlinkHead->slaveMeshNum);
	
}

