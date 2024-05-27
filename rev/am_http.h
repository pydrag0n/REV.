#ifndef __AM_HTTP_H__
#define __AM_HTTP_H__


#include <windows.h>
#include <winsock.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


SOCKET g_sock;
WSADATA g_wsData;


void HTTP_Init();
void HTTP_Send(char *buf, char *ip, int port);
void HTTP_Shutdown();

char *HTTP_PostRequest(char *inbuf, char *ip, int port);


#endif