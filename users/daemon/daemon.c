#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "apmib.h"
#define MAP_UNCONFIGURED 0
#define MAP_CONTROLLER   1
#define MAP_AGENT        2

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
			return 0;
		   }
		  printf("=>map_controller=%d\n",mapStatus);
			if(MAP_CONTROLLER==mapStatus)
			{
			 klinkMasterPid = getProcessPid("klinkM");
			 printf("=>klinkMasterPid[%d]\n",klinkMasterPid);
			 if(klinkMasterPid <= 0)
			 {
				printf("=>start klinkM\n");
				system(startKlinkMaster);
			 }
			}
			if(MAP_AGENT==mapStatus)
			{
			 klinkSlavePid = getProcessPid("klinkS");
			 //printf("=>klinkSlavePid[%d]\n",klinkSlavePid);
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

