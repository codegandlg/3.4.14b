/*
 * Copyright (c) 2019, All rights reserved.
 *
 * File         : deviceProcIf.c
 * Status       : Current
 * Description  :
 *
 * Author       : haopeng
 * Contact      : 376915244@qq.com
 *
 * Revision     : 2019-07
 * Description  : Primary released
 *
 * ## Please log your description here for your modication ##
 *
 * Revision     :
 * Modifier     :
 * Description  :
 *
 * */

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
 
#include "deviceProcIf.h" 
#define MBPS_TO_KBPS(a) (a*1024)
/*reset flag info*/
void ifstatResetInterfaces(struct ifstatList *ifs) 
{
    struct ifstatData *ptr;
    int hasindex = 1;
 
    for (ptr = ifs->first; ptr != NULL; ptr = ptr->next) 
    {
        if (!(ptr->flags & IFSTAT_HASINDEX))
            hasindex = 0;
        ptr->flags &= ~(IFSTAT_HASSTATS|IFSTAT_HASINDEX);
    }
    if (hasindex)
        ifs->flags |= IFSTAT_HASINDEX;
    else
        ifs->flags &= ~IFSTAT_HASINDEX;
}
 
void ifstatAddInterface(struct ifstatList *ifs, char *ifname, int flags) 
{
    struct ifstatData *cur, *last;
    int len;
 
    /*check nic name*/
    if (*ifname == '\0')
        return;
    len = strlen(ifname);
 
    last = NULL;
    for (cur = ifs->first; cur != NULL; cur = cur->next)
    {
        if (len == cur->namelen && cur->name[0] == ifname[0] &&
            !strncmp(cur->name + 1, ifname + 1, len - 1) &&
            !(flags & IFSTAT_TOTAL) && !(cur->flags & IFSTAT_TOTAL))
            return;
        last = cur;
    }
 
    if ((cur = calloc(1, sizeof(struct ifstatData))) == NULL) 
    {
        printf("[%s %d]malloc error\n",__FUNCTION__,__LINE__);
        exit(EXIT_FAILURE);
    }
    cur->name = strdup(ifname);
    cur->namelen = len;
    cur->flags = flags;
    if (last != NULL)
        last->next = cur;
    if (ifs->first == NULL)
        ifs->first = cur;
}
 
 
void ifstatFreeInterface(struct ifstatData *data) 
{
    free(data->name);
    free(data);
}
 
 
struct ifstatData *ifstatGetInterface(struct ifstatList *ifs, char *ifname)
{
    struct ifstatData *ptr;
    int len = strlen(ifname);
 
    for (ptr = ifs->first; ptr != NULL; ptr = ptr->next)
    {
        if (len == ptr->namelen && ptr->name[0] == ifname[0] &&
            !strncmp(ptr->name + 1, ifname + 1, len - 1) &&
            !(ptr->flags & IFSTAT_TOTAL))
        {
            return ptr;
        }
    }
    return NULL;
}
 
void ifstatSetInterfaceStats(struct ifstatData *data,unsigned long bytesin,unsigned long bytesout)
{
    if (data->bout > bytesout || data->bin > bytesin) 
    {
        printf("[%s %d]warning: rollover for interface %s, reinitialising\n",__FUNCTION__,__LINE__, data->name);
        data->obout = bytesout;
        data->obin = bytesin;
    } 
    else
    {
        data->obout = data->bout;
        data->obin = data->bin;
    }
    data->bout = bytesout;
    data->bin = bytesin;
    data->flags |= IFSTAT_HASSTATS;
}
 
 
 void examineInterface(struct ifstatList *ifs, char *name,int ifflags) 
{
    if ((ifflags & IFF_LOOPBACK) && !(ifs->flags & IFSTAT_LOOPBACK))
        return;
    if (!(ifflags & IFF_UP) && !(ifs->flags & IFSTAT_DOWN))
        return;
    ifstatAddInterface(ifs, name, 0);
}
 
 int ioctlMapIfs(int sd,int (*mapf)(int sd, struct ifreq *ifr, void *data),void *mdata) 
{
    struct if_nameindex *iflist, *cur;
    struct ifreq ifr;
    if ((iflist = if_nameindex()) == NULL) 
    {
        printf("[%s %d]if_nameindex error\n",__FUNCTION__,__LINE__);
        return 0;
    }
 
    for(cur = iflist; cur->if_index != 0 && cur->if_name != NULL; cur++) 
    {
        memcpy(ifr.ifr_name, cur->if_name, sizeof(ifr.ifr_name));
        ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';
        if (!mapf(sd, &ifr, mdata))
            return 0;
    }
    if_freenameindex(iflist);
    return 1;
}
 
 
 int ioctlMapScan(int sd, struct ifreq *ifr, void *data) 
{
    if (ioctl(sd, SIOCGIFFLAGS, (char *)ifr) != 0)
        return 1;
    examineInterface((struct ifstat_list *) data, ifr->ifr_name,ifr->ifr_flags);
    return 1;
}
 
 int ioctlScanInterfaces(struct ifstatDriver *driver,struct ifstatList *ifs) 
{
    int sd;
 
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("[%s %d]socket error\n",__FUNCTION__,__LINE__);
        return 0;
    }
    ioctlMapIfs(sd, &ioctlMapScan, (void *) ifs);
    close(sd);
    return 1;
} 
 
int procOpenDriver(struct ifstatDriver *driver) 
{
    struct procDriverData *data;
 
    if ((data = malloc(sizeof(struct procDriverData))) == NULL) 
    {
        printf("[%s %d]malloc error\n",__FUNCTION__,__LINE__);
        return 0;
    }
 
    data->file = NULL;
    data->checked = 0;
    driver->data = (void *) data;
 
    return 1;
}
 
 void procCloseDriver(struct ifstatDriver *driver) 
{
    struct procDriverData *data = driver->data;
 
    if (data->file != NULL)
        free(data->file);
    free(data);
}
 
 int procGetStats(struct ifstatDriver *driver,struct ifstatList *ifs) 
{
    char buf[1024];
    FILE *f;
    char *iface, *stats;
    unsigned long bytesin, bytesout;
    struct ifstatData *cur;
    struct procDriverData *data = driver->data;
    char *file;
    if (data->file != NULL)
        file = data->file;
    else
        file = PROC_FILE_PATH;
 
    if ((f = fopen(file, "r")) == NULL)
    {
        printf("[%s %d]can't open %s: %s\n",__FUNCTION__,__LINE__, file, strerror(errno));
        return 0;
    }
 
    /*check first line*/
    if (fgets(buf, sizeof(buf), f) == NULL)
        goto badproc;
    if (!data->checked && strncmp(buf, "Inter-|", 7))
        goto badproc;
    if (fgets(buf, sizeof(buf), f) == NULL)
        goto badproc;
    if (!data->checked)
    {
        if (strncmp(buf, " face |by", 9))
            goto badproc;
        data->checked = 1;
    }
 
    while (fgets(buf, sizeof(buf), f) != NULL) 
    {
        if ((stats = strchr(buf, ':')) == NULL)
            continue;
        *stats++ = '\0';
        iface = buf;
        while (*iface == ' ')
            iface++;
        if (*iface == '\0')
            continue;
        if (sscanf(stats, "%lu %*u %*u %*u %*u %*u %*u %*u %lu %*u", &bytesin, &bytesout) != 2)
            continue;
 
        if ((cur = ifstatGetInterface(ifs, iface)) != NULL)
            ifstatSetInterfaceStats(cur, bytesin, bytesout);
    }
    fclose(f);
    return 1;
 
badproc:
    fclose(f);
    printf("[%s %d]%s: unsupported format\n",__FUNCTION__,__LINE__, file);
    return 0;
}
 
 
int ifstatGetDriver(struct ifstatDriver *driver) 
{
     struct ifstatDriver drivers[] = 
    {
        {
			"proc", 
			&procOpenDriver,
			&ioctlScanInterfaces,
			&procGetStats,
			&procCloseDriver
		},
        {NULL} 
    };
    
    if (drivers[0].name == NULL)
        return 0;
    memcpy(driver, &(drivers[0]), sizeof(struct ifstatDriver));
    driver->data = NULL;
    return 1;
}
 
  
/*
        eth0                  lo
  KB/s in  KB/s out    KB/s in  KB/s out
 14562.23  12345.25       0.00      0.00
*/			 
  struct ifstatRate* getNicRate(struct ifstatList *ifs,struct ifstatRate *nic,struct timeval *start,struct timeval *end)
{
    struct ifstatData *ptr;
    int hasindex = 1;
    double delay, kbin, kbout, tkbin, tkbout, scale;
 
    delay = end->tv_sec - start->tv_sec + ((double) (end->tv_usec - start->tv_usec))/ (double) 1000000;
    scale = delay * 1024;
 
    tkbin = tkbout = 0;
    for (ptr = ifs->first; ptr != NULL; ptr = ptr->next) 
    {
        if(!(strncmp(ptr->name,nic->ifname,strlen(nic->ifname))))
       {
        if (ptr->flags & IFSTAT_HASSTATS)
        {
            kbin = (double) (ptr->bin - ptr->obin) / (double) scale;
            tkbin += kbin;
            kbout = (double) (ptr->bout - ptr->obout) / (double) scale;
            tkbout += kbout;
            ptr->flags &= ~IFSTAT_HASSTATS;
        } 
        else if (ptr->flags & IFSTAT_TOTAL)
        {
            kbin = tkbin;
            kbout = tkbout;
        } 
        else
        {
            kbin = -1;
            kbout = -1;
        }
 
        if (ptr->flags & IFSTAT_HASINDEX)
            ptr->flags &= ~IFSTAT_HASINDEX;
        else
            hasindex = 0;
 
        if (kbin < 0)
            kbin = 0;
 
        if (kbout < 0)
            kbout = 0;
	 //printf("name:%s      bin:%0.5lfMpbs     out:%0.5lfMpbs\n",ptr->name,kbin,kbout);
	 nic->rxRate = kbin;  //KB
	 nic->txRate =kbout;  //KB
        }      
    }
 
    if (hasindex)
        ifs->flags |= IFSTAT_HASINDEX;
    else
        ifs->flags &= ~IFSTAT_HASINDEX;
   return nic;
}
 
 void filterInterfaces(struct ifstatList *ifs)
{
    struct ifstatData *cur, *next, *parent;
    cur = ifs->first;
    parent = NULL;
    while (cur != NULL) 
    {
        if (!(cur->flags & IFSTAT_HASSTATS)) 
        {
            next = cur->next;
            if (parent != NULL)
                parent->next = next;
            else
                ifs->first = next;
            ifstatFreeInterface(cur);
            cur = next;
        }
        else
        {
            parent = cur;
            cur = cur->next;
        }
    }
}
 
 
 struct ifstatRate* getProcIfData( struct ifstatRate*nic)
{
    struct ifstatList ifs;
    struct ifstatDriver driver;
    struct timeval start, tv_delay, tv;
 
    ifs.flags = 0;
    ifs.first = NULL;
  
    if (!ifstatGetDriver(&driver)) 
    {
        return EXIT_FAILURE;
    }
 
    /*init nic device*/
    if (driver.openDriver != NULL &&!driver.openDriver(&driver))
        return EXIT_FAILURE;
 
    if (ifs.first == NULL)
        driver.scanInterfaces(&driver, &ifs);
 
    /* scan nic data*/
    if (ifs.first != NULL) 
    {
        if (driver.getStats != NULL && !driver.getStats(&driver, &ifs))
            return EXIT_FAILURE;
        gettimeofday(&start, NULL);
        filterInterfaces(&ifs);
        ifstatResetInterfaces(&ifs);
    }
    
    if (ifs.first == NULL) 
    {
        if (driver.closeDriver != NULL)
            driver.closeDriver(&driver);
        return EXIT_FAILURE;
    }
  
    if (ifs.flags & IFSTAT_TOTAL)
    {
        ifstatAddInterface(&ifs, "Total", IFSTAT_TOTAL);
    }
  
    tv_delay.tv_sec = 1;
    tv_delay.tv_usec = (int) ((1 - tv_delay.tv_sec) * 1000000);
	
        tv = tv_delay;
        select(0, NULL, NULL, NULL, &tv);
        if (driver.getStats != NULL && !driver.getStats(&driver, &ifs))
            return EXIT_FAILURE;
        gettimeofday(&tv, NULL);
        getNicRate(&ifs, nic,&start, &tv);
        start = tv;
        fflush(stdout);
 
    if (driver.closeDriver != NULL)
        driver.closeDriver(&driver);
    //	 printf("++++     in:%0.5lfMpbs     out:%0.5lfMpbs+++\n",nic->rxRate,nic->txRate);

    return nic;
}


