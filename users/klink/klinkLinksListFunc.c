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
KlinkNode_t* g_pKlinkHead = NULL;
KlinkNode_t klinkNodeData;


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
   printf("init head succed!\n");
   printf("#################\n");
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
	printf("%s_%d:\n ",__FUNCTION__,__LINE__);

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
		  printf("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveMac);
		  }
		   else if(type==KLINK_SLAVE_SOFT_VERSION)
		  {
		  printf("add list succed :data= %s\n",new_node->slaveVersionInfo.slaveSoftVer);	   
		  }
		 return phead;
	  }	   
   }
}
 
/*search node*/

KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,KlinkNode_t *pdata)
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

/*delete klink node*/
KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,KlinkNode_t *pdata)
{
	KlinkNode_t *phead=head;
	KlinkNode_t *q=head;
	if(phead==NULL)
	{
		printf("list is empty\n");
		return(0);
	}
	else
	{
		while(strcmp(phead->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac)&&phead->next!=NULL)
		{
			q=phead;
			phead=phead->next;
		}
		if(!strcmp(phead->slaveVersionInfo.slaveMac,pdata->slaveVersionInfo.slaveMac))
		{
			q->next=phead->next;
			printf("free %s succed\n",phead->slaveVersionInfo.slaveMac);
			free(phead);			
			printf("#################\n");
		}
		else
		{
			printf("NO NODE DELETE\n");
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
		printf("list is empty\n");
		//return 1;
	}
	else
	{
	   while(phead->next!=NULL)
		{
			printf("show list node date is:slaveMac     [%s]\n",phead->next->slaveVersionInfo.slaveMac);
			printf("show list node date is:slaveSoftVer [%s]\n",phead->next->slaveVersionInfo.slaveSoftVer);
			phead=phead->next;
		}
		printf("#################\n");
	}
	printf("show list node date is:slaveNum     [%d]\n",g_pKlinkHead->slaveMeshNum);
	
}

