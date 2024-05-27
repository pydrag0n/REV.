#include "am_http.h"


void HTTP_Init()
{
    if(WSAStartup(MAKEWORD(2, 0), &g_wsData) < 0)
    {
        MessageBoxW(NULL, L"Не удаётся инициальзировать WinSock.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }

    g_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(g_sock == INVALID_SOCKET)
    {
        MessageBoxW(NULL, L"Не удаётся инициальзировать WinSock.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }
}


void HTTP_Send(char *buf, char *ip, int port)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if((connect(g_sock, (struct ockaddr *)&addr, sizeof(addr))) == SOCKET_ERROR)
    {
        MessageBoxW(NULL, L"Не удаётся подключиться к серверу: Timeout.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }

    static char sendbuf[1024];
    strcat(sendbuf, "POST /validate_data HTTP/1.1\r\n");
	char temp[64];
	sprintf(temp, "Content-Length: %d\r\n", strlen(buf));
	strcat(sendbuf, temp);
    strcat(sendbuf, "Content-Type: application/json\r\n\r\n");
    strcat(sendbuf, buf);

    if((send(g_sock, sendbuf, strlen(sendbuf), 0)) == SOCKET_ERROR)
    {
        MessageBoxW(NULL, L"Не удаётся отправить данные.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }

    closesocket(g_sock);
}


char *HTTP_PostRequest(char *inbuf, char *ip, int port)
{
	static char outbuf[2048];
	struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if((connect(g_sock, (struct ockaddr *)&addr, sizeof(addr))) == SOCKET_ERROR)
    {
        MessageBoxW(NULL, L"Не удаётся подключиться к серверу: Timeout.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }

    static char sendbuf[2048];
    strcat(sendbuf, "POST /validate_data HTTP/1.1\r\n");
	static char temp[128];
	sprintf(temp, "Content-Length: %d\r\n", strlen(inbuf));
	strcat(sendbuf, temp);
    strcat(sendbuf, "Content-Type: application/json\r\n\r\n");
    strcat(sendbuf, inbuf);

	//MessageBoxA(NULL, sendbuf, "Request to serv", MB_OK);

    if((send(g_sock, sendbuf, strlen(sendbuf), 0)) == SOCKET_ERROR)
    {
        MessageBoxW(NULL, L"Не удаётся отправить данные.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
    }

	if((recv(g_sock, outbuf, 2048, 0)) == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"Не удаётся получить данные.", L"Ошибка", MB_ICONERROR | MB_OK);
        exit(-1);
	}

    closesocket(g_sock);

	return outbuf;
}


void HTTP_Shutdown()
{
    WSACleanup();
}