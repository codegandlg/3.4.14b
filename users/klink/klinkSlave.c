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

#define _slave 

unsigned int g_lastCheckTIme = 0;


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

void _slave parseMessageFromMaster(int sd, char* masterMessage) 
{

    cJSON *json=NULL;
	cJSON *jason_obj=NULL; 

    json = cJSON_Parse(masterMessage);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
      /*{"slaveVersion":[{"slaveSoftVer":"WM V1.0.5","slaveMac":"00:11:22:33:44:55"}]}*/
     if(jason_obj = cJSON_GetObjectItem(json,"slaveVersion"))		
	 {
      //parseSlaveVersionConf(sd, jason_obj);
	 }
    
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

int _slave getSlaveVersionInfo(char *pSoftVersion, char *pWanHWAddr)
{
 	struct sockaddr hwaddr;
	unsigned char *pMacAddr;
	if ( getMacAddr(WAN_IF, (void *)&hwaddr ) ) 
	{
		pMacAddr = (unsigned char *)hwaddr.sa_data;
		sprintf(pWanHWAddr,"%02x:%02x:%02x:%02x:%02x:%02x",pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
	}
	else
	{
		sprintf(pWanHWAddr,"%s","00:00:00:00:00:00");
	}
	strcpy(pSoftVersion,"WM V1.0.6");
}


/*
 * version formate:
* {"slaveVersion":[{"slaveSoftVer":"WM V1.0.5","slaveMac":"00:11:22:33:44:55"}]}
*/

void _slave sendFwVersionMessageToMaster(int sd) 
{
	char macAddr[18]={0};
	char fwVersion[18]={0};	
	char* stringMessage=NULL;
    cJSON *topRoot=NULL;
	cJSON *root=NULL;
	cJSON *parameters=NULL;

    getSlaveVersionInfo(fwVersion, macAddr);	  
	topRoot = cJSON_CreateObject();
	cJSON_AddItemToObject(topRoot, "slaveVersion", root = cJSON_CreateArray());
	cJSON_AddItemToArray(root, parameters = cJSON_CreateObject());
	cJSON_AddStringToObject(parameters, "slaveSoftVer", fwVersion);
	cJSON_AddStringToObject(parameters, "slaveMac", macAddr);
	stringMessage = cJSON_Print(topRoot);  
	printf("%s_%d:sen message=%s \n",__FUNCTION__,__LINE__,stringMessage);
	send(sd, stringMessage,strlen(stringMessage), 0) ;
	cJSON_Delete(topRoot);	
}

int main(int argc,char **argv)
{
	char ip[16]={0};
	int routeTableFlag=-1;
    fd_set rset;
    FD_ZERO(&rset);
    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;   
    //struct hostent *h = gethostbyname(argv[1]);
    //server_addr.sin_addr = *((struct in_addr *)h->h_addr);
    routeTableFlag=getGetwayIp(ip);
	if(routeTableFlag!=0)
	{
	 printf("%s_%d:get gateway ip fail",__FUNCTION__,__LINE__);
	}
    printf("=slage get gateway ip=%s\n", ip );
	server_addr.sin_addr.s_addr = inet_addr(ip);  
    server_addr.sin_port = htons(KLINK_PORT);
    if(connect(sock,(struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0)
	{
        ERR_EXIT("connect to %s:%s failed", argv[1], argv[2]);
    }
    fprintf(stdout, "% 9d\n", getpid());
    fflush(stdout);
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
    char sendbuf[1024*4] = {0};  
    char recvbuf[1024*4] = {0};  
	apmib_init();
	
	g_lastCheckTIme = upSecond();
    while (1)
	{  
	 if(upSecond() - g_lastCheckTIme > 5)
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
           printf("---get info from maser:%s \n",recvbuf);
		   //send(sock, "hello", sizeof("hello"), 0) ;
		   sendFwVersionMessageToMaster(sock);
           memset(recvbuf, 0, sizeof(recvbuf));  
        }  
	  g_lastCheckTIme = upSecond();
	}
	sleep(1);
#if 0
		strcpy(sendbuf,"hello");
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
