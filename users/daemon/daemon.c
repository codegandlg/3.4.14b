#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <time.h>
#include <errno.h>
#include "apmib.h"
#define MAP_UNCONFIGURED 0
#define MAP_CONTROLLER   1
#define MAP_AGENT        2

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

/* this is not standard function */
static char *_strrstr(const char *haystack, const char *needle)
{
    const char *p;
    int stackLen = strlen(haystack);
    int needleLen = strlen(needle);

    if (!needleLen || (needleLen > stackLen))
    {
        return NULL;
    }

    for (p = haystack + (stackLen - needleLen); p >= haystack; p --)
    {
        if ((*p == *needle) && !memcmp(p, needle, needleLen))
        {
            return (char *)p;
        }
    }
    return NULL;
}

char *trimRight(char *s)
{
	size_t len = strlen(s);

	/* trim trailing whitespace */
	while (len && isspace(s[len-1]))
    {
		--len;
    }   
    
	s[len] = '\0';
    
    return s;
}

int getProcessPidGroup(const char *name, int pidGroup[], int pidNum)
{
    char buffer[512];
    FILE *fp; 
    int cmdOffset;
    int fullMatch;
    int nameLen;
    int count;
    char *p, *pCommand;

    cmdOffset = 0;
    count = 0;    
    nameLen = strlen(name);
    
    if (strchr(name, '/'))
    {
        /* if name has '/', do not remove the folder name */
        fullMatch = 1;
    }
    else
    {
        fullMatch = 0;
    }
    
    fp = popen("ps -A", "r");

    if (fp == NULL)
    {
        printf("popen failed:%s(%d)\n", strerror(errno), errno);
        return -1;
    }

    while(!feof(fp) && (count < pidNum)) 
    {
        buffer[0] = 0;
        if (fgets(buffer, sizeof(buffer) - 1, fp))
        {
            buffer[sizeof(buffer) - 1] = '\0';
            
            trimRight(buffer);
            /* try to get the command offset */
            if (!cmdOffset)
            {
                p = _strrstr(buffer, "init");
                if (p != NULL)
                {
                    cmdOffset = p - buffer;
                }
            }
            else if (strlen(buffer) > cmdOffset)
            {
                pCommand = &buffer[cmdOffset];
            
                /* remove the string after the first space */
                p = strchr(pCommand, ' ');
                if (p)
                {
                    *p = '\0';
                }

                p = pCommand;
                
                if (!fullMatch)
                {
                    /* point to the name */
                    p = strrchr(pCommand, '/');
                    if (p == NULL)
                    {
                        /* not found, point to the first byte */
                        p = pCommand;
                    }
                    else 
                    {
                        p += 1;
                    }
                }
                /* compare it */
                if (!strncmp(p, name, nameLen))
                {
                    /* the first is the pid */
                    pidGroup[count] = strtoul(buffer, NULL, 10);
                    count ++;
                }
            }
        } 
    }    

    pclose(fp);
    
    return count;
}

int getProcessPid(const char *name)
{
    int pid;
    int ret;
    
    ret = getProcessPidGroup(name, &pid, 1);

    return (ret > 0) ? pid : ret; 
}

unsigned int upSecond(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);    
    return ts.tv_sec;
}

unsigned int g_lastCheckTIme = 0;

int main()
{
	printf("===>klinkDaemon\n");
    struct in_addr gwAddr;
	char ip[16]={0};
	int routeTableFlag=-1;
	g_lastCheckTIme = upSecond();
	int klinkMasterPid = 0,klinkSlavePid = 0;
	int ret = -1;
	int mapStatus=0;
	const char *killKlinkMaster = "killall -9 klinkM";
	const char *killKlinkSlave = "killall -9 klinkS";
	const char *startKlinkMaster = "klinkM &";
	const char *startKlinkSlave= "klinkS &";
	apmib_init();
	while(1)
	{
		if(upSecond() - g_lastCheckTIme > 5)
		{
		
		  if ( !apmib_get(MIB_MAP_CONTROLLER, (void *)&mapStatus) )
		   {
		    printf("=>map_controller=%d\n",mapStatus);
			continue;
		   }
		   if(access("/tmp/topology_json",F_OK)!=0)
		   	{
		   	 continue;
		   	}
		    if( MAP_UNCONFIGURED==mapStatus)
		    {
		     printf("=>mesh havn't config\n");
		    }

    		if(MAP_CONTROLLER==mapStatus)
			{
			 klinkMasterPid = getProcessPid("klinkM");
			 if(klinkMasterPid <= 0)
			 {
				printf("=>start klinkM\n");
				system(startKlinkMaster);
			 }
			}
			
			if(MAP_AGENT==mapStatus)
			{

			  klinkSlavePid = getProcessPid("klinkS");		 
			 if(klinkSlavePid <= 0)
			 {
				printf("=>start klinkS\n");
				system(startKlinkSlave);
			 }
			}
			g_lastCheckTIme = upSecond();
		}
		sleep(1);
	}
		
}

