
/*
Copyright (c) 2019, All rights reserved.

File         : slaveUpgrade.c
Status       : Current
Description  : 

Author       : lhw
Contact      : xxhanwen@163.com

Revision     : 2019-10 
Description  : Primary released

## Please log your description here for your modication ##

Revision     : 
Modifier     : 
Description  : 

*/

/* Standard headers */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>		/* gethostbyname, getnetbyname */
#include <net/ethernet.h>	/* struct ether_addr */
#include <sys/time.h>		/* struct timeval */
#include <unistd.h>

#include "apmib.h"
#include "trace.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/socket.h>


//#define MAX_EVERY_FRAME    10000

//int SendData(int sock_fd, char *buf, int len)
//{
//    int sent_len = 0;
//    int tmp;
//    int curr_send_len;
//    
//    while(len > sent_len)
//    {
//        curr_send_len = len - sent_len;
//        if(curr_send_len > MAX_EVERY_FRAME)
//        {
//            curr_send_len = MAX_EVERY_FRAME;
//        }
//        
//        tmp = send(sock_fd, &buf[sent_len], curr_send_len, 0);
//
//        if(tmp < 0)
//        {
//            DPrintf("send err, err_code = %d\n!",tmp);
//            return -1;
//        }
//
//        sent_len += tmp;
//    }
//
//    return 1;
//}

int SendData(int sock_fd, char *buf, int len)
{
    int sent_len = 0;
    int tmp;
    
    while(len > sent_len)
    {
        tmp = send(sock_fd, &buf[sent_len], len - sent_len, 0);

        if(tmp < 0)
        {
            DPrintf("send err, err_code = %d\n!",tmp);
            return -1;
        }

        sent_len += tmp;
    }

    return 1;
}

int RecvData(int sock_fd, char *buf, int len, int time_out_sec)
{
    fd_set rfds;
	int retval;
    struct timeval time_out;
    int recv_len;
    
    FD_ZERO(&rfds);
	FD_SET(sock_fd, &rfds);

    while(1)
    {
        time_out.tv_sec = time_out_sec;
        time_out.tv_usec = 0;
    	retval = select(sock_fd + 1,&rfds, NULL, NULL, &time_out);

        if(retval == -1 && errno == EINTR)
        {
            DTRACE(DTRACE_SLAVE_UPGRADE, "err, errno = %s.\n", strerror(errno));
            continue;
        }
    	if(retval <= 0 || !FD_ISSET(sock_fd, &rfds))
    	{
            DTRACE(DTRACE_SLAVE_UPGRADE, "select return, retval = %d, errno = %s.\n", retval, strerror(errno));
    		return -1;
    	}
        break;
    }

    recv_len = recv(sock_fd, buf, len, 0);
	
	return recv_len;
}

int SendFileToSlave(char *file_data, int file_len, char *slave_ip, int slave_port)
{
    char buf[1600];
    int len = 0;
    char part1_header[256];
    int part1_header_len = 0;
    char content_end[256];
    int content_end_len = 0;
    char *ptr;

    part1_header_len += sprintf(part1_header, "-----------------------------7e32db18201e4\r\n");
    part1_header_len += sprintf(part1_header+part1_header_len, "Content-Disposition: form-data; name=\"binary\"; filename=\"fw.bin\"\r\n");
    part1_header_len+= sprintf(part1_header+part1_header_len, "Content-Type: application/octet-stream\r\n\r\n");

    content_end_len += sprintf(content_end, "\r\n-----------------------------7e32db18201e4--\r\n");

    len += sprintf(buf, "POST /boafrm/formUpgradeSlave HTTP/1.1\r\n");
    len += sprintf(buf+len, "Referer: http://%s/softwareup.html\r\n", slave_ip);
    len += sprintf(buf+len, "Cache-Control: max-age=0\r\n");
    len += sprintf(buf+len, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    len += sprintf(buf+len, "Accept-Language: zh-CN\r\n");
    len += sprintf(buf+len, "Content-Type: multipart/form-data; boundary=---------------------------7e32db18201e4\r\n");
    len += sprintf(buf+len, "Upgrade-Insecure-Requests: 1\r\n");
    len += sprintf(buf+len, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763\r\n");
    len += sprintf(buf+len, "Accept-Encoding: gzip, deflate\r\n");
    len += sprintf(buf+len, "Host: %s\r\n", slave_ip);
    len += sprintf(buf+len, "Content-Length: %d\r\n", part1_header_len+file_len+content_end_len);
    len += sprintf(buf+len, "Connection: Keep-Alive\r\n");
    len += sprintf(buf+len, "Cookie: userLanguage=zh-CN\r\n\r\n");

    strcpy(buf+len, part1_header);
    len += part1_header_len;

    int sock_fd;
    struct sockaddr_in server_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port =  htons(slave_port);
    if(!inet_aton(slave_ip, &server_addr.sin_addr))
    {
        DPrintf("invalid slave_ip!\n");
        return -1;
    }

    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
       DPrintf("socket fail.\n");
       return -1;
    }

    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        DPrintf("connect fail.\n");
        close(sock_fd);
        return -1;
    }

    if(SendData(sock_fd, buf, len) < 0)
    {
        DPrintf("SendData fail!\n");
        close(sock_fd);
        return -1;
    }

    if(SendData(sock_fd, file_data, file_len) < 0)
    {
        DPrintf("SendData fail!\n");
        close(sock_fd);
        return -1;
    }

    if(SendData(sock_fd, content_end, content_end_len) < 0)
    {
        DPrintf("SendData fail!\n");
        close(sock_fd);
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    if(RecvData(sock_fd, buf, sizeof(buf), 10) > 0)
    {
        DTRACE(DTRACE_SLAVE_UPGRADE, "%s \n", buf);
        ptr = strstr(buf, "file send ok");
        if(ptr == NULL)
        {            
            DPrintf("SendData fail!\n");
            close(sock_fd);
            return -1;
        }
    } 
    else
    {
        DPrintf("recv out of time!\n");
        close(sock_fd);
        return -1;
    }
    
    close(sock_fd);
    
    return 1;
    
}

int SlaveStartRemoteUpgrade(char *slave_ip, int slave_port)
{
    char buf[1600];
    int len = 0;
    char content[256];
    int content_len = 0;
    char *ptr;
    char err_buf[10];
    int err_code;

    content_len += sprintf(content, "cmd=1\r\n");

    len += sprintf(buf, "POST /boafrm/formRemoteUpgrade HTTP/1.1\r\n");
    len += sprintf(buf+len, "Referer: http://%s/softwareup.html\r\n", slave_ip);
    len += sprintf(buf+len, "Cache-Control: max-age=0\r\n");
    len += sprintf(buf+len, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    len += sprintf(buf+len, "Accept-Language: zh-CN\r\n");
    len += sprintf(buf+len, "Content-Type: application/x-www-form-urlencoded\r\n");
    len += sprintf(buf+len, "Upgrade-Insecure-Requests: 1\r\n");
    len += sprintf(buf+len, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/18.17763\r\n");
    len += sprintf(buf+len, "Accept-Encoding: gzip, deflate\r\n");
    len += sprintf(buf+len, "Host: %s\r\n", slave_ip);
    len += sprintf(buf+len, "Content-Length: %d\r\n", content_len);
    len += sprintf(buf+len, "Connection: Keep-Alive\r\n");
    len += sprintf(buf+len, "Cookie: userLanguage=zh-CN\r\n\r\n");

    strcpy(buf+len, content);
    len += content_len;

    int sock_fd;
    struct sockaddr_in server_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port =  htons(slave_port);
    if(!inet_aton(slave_ip, &server_addr.sin_addr))
    {
        DPrintf("invalid slave_ip!\n");
        return 0;
    }

    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
       DPrintf("socket fail.\n");
       return 0;
    }

//    struct timeval time_out;
//    time_out.tv_sec = 8;
//    time_out.tv_usec = 0;
//    setsockopt(sock_fd,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out, sizeof(struct timeval)); 
    
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        DPrintf("connect fail.\n");
        close(sock_fd);
        return 0;
    }

    DTRACE(DTRACE_SLAVE_UPGRADE, "%s \n", buf);
    if(SendData(sock_fd, buf, len) < 0)
    {
        DPrintf("SendData fail!\n");
        close(sock_fd);
        return 0;
    }

    memset(buf, 0, sizeof(buf));
    int ret_val = RecvData(sock_fd, buf, sizeof(buf), 30);
    if(ret_val > 0)
    {        
        close(sock_fd); 
        DTRACE(DTRACE_SLAVE_UPGRADE, "%s \n", buf);
        ptr = strstr(buf, "err_code");
        if(ptr != NULL)
        {
            memset(err_buf, 0, sizeof(err_buf));
            memcpy(err_buf, ptr+9, 1);
            err_code = atoi(err_buf);
            if(err_code == 0)
            {                
                return 1;
            }
            else
            {
                DTRACE(DTRACE_SLAVE_UPGRADE, "upgrade fail, err_code = %d.\n", err_code);
                return -err_code;
            }
        }
        else
        {            
            DTRACE(DTRACE_SLAVE_UPGRADE, "upgrade fail, not find err_code.\n");
            return 0;
        }
    } 
    else
    {
        DTRACE(DTRACE_SLAVE_UPGRADE, "upgrade fail, recv nothing. ret_val = %d, errno = %s.\n", ret_val, strerror(errno));
    }
    
    close(sock_fd);    
    return 0;
}

