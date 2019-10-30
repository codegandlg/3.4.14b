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
KlinkNode_t *addKlinkListNode(KlinkNode_t*head,char* date,int type)
{
     	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
   KlinkNode_t *phead,*new_node;
   phead=head;
   	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
   if(NULL==phead)
   {
   printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	return NULL;	
   }
   else
   {
   	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	while(phead->next!=NULL)   
	{   
	    /*slave already int the link list,exit*/
	    if((!strcmp(phead->next->slaveVersionInfo.slaveMac,date))&&(type==KLINK_CREATE_TOPOLOGY_LINK_LIST))
	    {
	     printf("==>%s_%d:slave already in the link list",__FUNCTION__,__LINE__);
		 return phead;
	    }
		phead=phead->next;		
	}
		printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	  if(NULL!=(new_node=(KlinkNode_t*)malloc(sizeof(KlinkNode_t))))
	  {
	  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	      if(type==KLINK_CREATE_TOPOLOGY_LINK_LIST)
	      {
	      printf("%s_%d:\n ",__FUNCTION__,__LINE__);
	       strcpy(new_node->slaveVersionInfo.slaveMac,date);
	      }	
		  else if(type==KLINK_SLAVE_SOFT_VERSION)
		  {
		  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
		   strcpy(new_node->slaveVersionInfo.slaveSoftVer,date);		   
		  }
		  phead->next=new_node;
		  new_node->next=NULL;
		  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
		  if(type==KLINK_CREATE_TOPOLOGY_LINK_LIST)
	      {
		  printf("add list succed :%s\n",new_node->slaveVersionInfo.slaveMac);
		  }
		   else if(type==KLINK_SLAVE_SOFT_VERSION)
		  {
		  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
		  printf("add list succed :%s\n",new_node->slaveVersionInfo.slaveSoftVer);	   
		  }
		 return phead;
	  }	   
   }
   	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
}
 
/*search node*/

KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,char* date)
{
	KlinkNode_t *phead=head;
	if(phead==NULL)
	{
		printf("head_node is empty\n");
		return NULL;
	}
	while(phead->date!=date&&phead->next!=NULL)
	{    
		phead=phead->next;
	}
	if(!(strcmp(phead->date,date)))
	{
		printf("serch succed phead->date=%s\n",phead->date);
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

//delete node
KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,char* date)
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
		while(strcmp(phead->date,date)&&phead->next!=NULL)
		{
			q=phead;
			phead=phead->next;
		}
		if(!strcmp(phead->date,date))
		{
			q->next=phead->next;
			printf("free %s succed\n",phead->date);
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
}
#if 0
int main(int argc,char*argv[])
{
	KlinkNode_t *head=NULL;
    int i;
	head=initKlinkListHead();
	for(i=0;i<10;i++)
	{
		addKlinkListNode(head,i);		
	}
	showKlinkNode(head);
	serchKlinkListNode(head,5);
	showKlinkNode(head);
	deletKlinkListNode(head,7);
	showKlinkNode(head);
return 0;
}
#endif

