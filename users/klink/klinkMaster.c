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
#include <time.h>
#include "cJSON.h"
#include "klink.h"
#include "apmib.h"

extern char *etherAddrToString(etherAddr_t *ether, int type);
extern KlinkNode_t* initKlinkListHead();
extern KlinkNode_t *addKlinkListNode(KlinkNode_t*head,char* date,int type);
extern KlinkNode_t* serchKlinkListNode(KlinkNode_t*head,char*  date);
extern KlinkNode_t* deletKlinkListNode(KlinkNode_t*head,char* date);
extern void showKlinkNode(KlinkNode_t*head);
KlinkNode_t* g_pKlinkHead = NULL;


KlinkNode_t* createMeshTopologyLinkList(cJSON *root)
{
  cJSON *head;
  cJSON *pos;
  cJSON *parameters;
  int slaveNum=0;
  KlinkNode_t* pKlink=NULL; g_pKlinkHead;
  KlinkNode_t* pKlinkHead=NULL; g_pKlinkHead;
    	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
  g_pKlinkHead=initKlinkListHead();
  pKlink=g_pKlinkHead;
  pKlinkHead=g_pKlinkHead;
  /*before create mesh device link list ,clear link node first*/
  clearKlinkList(pKlink);  
      	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
  head = cJSON_GetObjectItem(root, "child_devices");
  if(head != NULL)
   {
   for(pos = head->child; pos != NULL; pos = pos->next)
   {
    parameters = cJSON_GetObjectItem(pos, "mac_address");
    if(parameters != NULL)
    {
      slaveNum++;
	  /*add slave mesh node to link list tail*/
	  addKlinkListNode(g_pKlinkHead,parameters->valuestring,KLINK_CREATE_TOPOLOGY_LINK_LIST);
      printf("====>mac[%s]----slaveNum[%d]\n",parameters->valuestring,slaveNum);
    }
   }
  }
  else
  {
     printf("%s:%d： can't find child_devices.", __FUNCTION__, __LINE__);
  }
  pKlinkHead->slaveMeshNum=slaveNum;
  return pKlinkHead;
}

KlinkNode_t* createMeshDeviceLinkList()
{
 printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 char* out=NULL;
 cJSON *root=NULL; 
 FILE *fp=NULL;
 KlinkNode_t* pKlinkHead=NULL;
 int read=0;
 int len  = 0;
 char*	line  = NULL;   
  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 fp = fopen("/tmp/topology_json", "r");
   printf("%s_%d:\n ",__FUNCTION__,__LINE__);
 if(fp == NULL)
 {
  printf("==>%s_%d:open /tmp/topology_json fail...",__FUNCTION__,__LINE__);
  return RETURN_FAIL;
 }
 else
 {
  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
  read = getline(&line, &len, fp);
  fclose(fp);
  printf("%s_%d:\n ",__FUNCTION__,__LINE__);
  if(line) 
  {
    printf("%s_%d:\n ",__FUNCTION__,__LINE__);
   root = cJSON_Parse(line);  
   free(line);    
   if(root==NULL)
   {
    printf("%s_%d: error...\n ",__FUNCTION__,__LINE__);
   }
   else
   {
    printf("%s_%d:\n ",__FUNCTION__,__LINE__);
    pKlinkHead=createMeshTopologyLinkList(root);
   }
  }
   else
   {
    printf("==>%s_%d: topology_json error ",__FUNCTION__,__LINE__);
    return RETURN_FAIL;
   }

 }
  out = cJSON_Print(root);
 printf("==>%s_%d:out=[%s] open /tmp/topology_json fail...\n",__FUNCTION__,__LINE__,out);	
  cJSON_Delete(root);
  return pKlinkHead;
}

/*getMeshTopology();*/

//================add end
int parseSlaveVersionConf(int fd, cJSON *messageBody)
{
   cJSON *jasonObj=NULL;
   char *pMessageBody=NULL;
   char *pSlaveFwVersion=NULL;
   char *pSlaveMac=NULL;
   char tmpBuf[128]={0};
   KlinkNode_t* pKlinkHead=NULL;

   char *pResponseMsg=NULL;
   cJSON *responseJSON=NULL;

   if(jasonObj = cJSON_GetObjectItem(messageBody,"slaveVersion"))
   {
    pSlaveFwVersion=cJSON_GetObjectItem(jasonObj,"slaveSoftVer")->valuestring;
    pSlaveMac=cJSON_GetObjectItem(jasonObj,"slaveMac")->valuestring;
   }

   pKlinkHead=createMeshDeviceLinkList();
     	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
   addKlinkListNode(pKlinkHead,pSlaveFwVersion,KLINK_SLAVE_SOFT_VERSION);
     	printf("%s_%d:\n ",__FUNCTION__,__LINE__);
   showKlinkNode(pKlinkHead);
    sprintf(tmpBuf,"%d;%s;%s",pKlinkHead->slaveMeshNum,pSlaveMac,pSlaveFwVersion);
      	printf("%s_%d:tmpBuf=%s\n ",__FUNCTION__,__LINE__,tmpBuf);
	apmib_set(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf);
	memset(tmpBuf, 0, sizeof(tmpBuf));
	apmib_get(MIB_KLINK_SLAVE1_SOFT_VERSION, (void *)tmpBuf);
	printf("%s_%d:set slave version data [%s] ok \n",__FUNCTION__,__LINE__,tmpBuf);

	

    /*rend ack message to slave*/
	responseJSON = cJSON_CreateObject();
	cJSON_AddStringToObject(responseJSON, "messageType", "2"); //2==KLINK_MASTER_SEND_ACK_VERSION_INFO
//	cJSON_AddNumberToObject(responseJSON, "responseCode", fwVersion);
	pResponseMsg = cJSON_Print(responseJSON); 
	printf("%s_%d:send version ack data [%s]  \n",__FUNCTION__,__LINE__,pResponseMsg);

	send(fd , pResponseMsg, strlen(pResponseMsg) , 0 );
}

int klinkMasterStateMaching(int sd,int messageType,cJSON *messageBody)
{
 switch(messageType)
 {
  case KLINK_SLAVE_SEND_VERSION_INFO:
     parseSlaveVersionConf(sd,messageBody);
	 break;
  default:
  	 break;
 }
 return 0;
}

void parseMessageFromSlave(int sd, char* slaveMessage) 
{

    cJSON *pJson=NULL;
	int messageType=-1;

    printf("%s_%d:=>get slave message[%s]\n ",__FUNCTION__,__LINE__,slaveMessage);
    pJson = cJSON_Parse(slaveMessage);
    if (!pJson)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
      /*{"slaveVersion":["messageType":"0"{"slaveSoftVer":"WM V1.0.5","slaveMac":"00:11:22:33:44:55"}]}*/
     //if(jason_obj = cJSON_GetObjectItem(json,"slaveVersion"))	
    messageType = atoi(cJSON_GetObjectItem(pJson,"messageType")->valuestring);	
	klinkMasterStateMaching(sd,messageType,pJson);
	printf("%s_%d:messageType=%d\n ",__FUNCTION__,__LINE__,messageType);
    }
}


int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readfds;
      
    //a message
    char *message = "{\"messageType\":\"0\"}";

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {   
        close(master_socket);
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( KLINK_PORT );
      
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", KLINK_PORT);
     
    //try to specify maximum of 16 pending connections for the master socket
    if (listen(master_socket, 16) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("klink master waiting for connections ...");
    apmib_init(); 
    int slaveNumber=0;

	//g_pKlinkHead=initKlinkListHead();
    while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
              
            puts("Welcome message sent successfully");
              
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                  
                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
					printf("++++get info from slave :%s\n",buffer);
                   // send(sd , buffer , strlen(buffer) , 0 );
                   parseMessageFromSlave(sd, (char*)buffer);

                }
            }
        }
    }
      
    return 0;
} 

