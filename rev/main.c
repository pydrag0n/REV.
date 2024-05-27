#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "nxjson.h"
#include "am_http.h"

#define PASSWORD_FIELD_ID 1
#define IP_FIELD_ID 2
#define PORT_FIELD_ID 3

#define SUBMIT_BUTTON_ID 4
#define MYID_FIELD_ID 5
#define SELFIP_FIELD_ID 6

HWND password_field;
HWND selfip_field;
HWND ip_field;
HWND port_field;
HWND myid_field;
HWND submit_button;

HWND window;
HDC dc;

#define NO_PASSWORD RGB(255, 255, 255)
#define WRONG_PASSWORD  RGB(255, 0, 0)
#define CORRECT_PASSWORD RGB(0, 255, 0)

int server_port = 5000;
static char server_ip[128];
static char self_ip[64];

COLORREF password_status = NO_PASSWORD;

void Paint()
{
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCBrushColor(dc, password_status);    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, password_status);
    Rectangle(dc, 0, 0, 250, 250);
    Sleep(20);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_COMMAND:
            if(LOWORD(wparam) == SUBMIT_BUTTON_ID)
            {
                GetWindowTextA(ip_field, server_ip, sizeof(server_ip));
				char buf[6];
                GetWindowTextA(port_field, buf, sizeof(buf));
				server_port = strtol(buf, NULL, 10);
				static char password_buf[256];
				static char temp_buf[128];
				GetWindowTextA(password_field, temp_buf, sizeof(temp_buf));
				int password = strtol(temp_buf, NULL, 10);
				static char idbuf[16];
				GetWindowTextA(myid_field, idbuf, sizeof(idbuf));
				int myid = strtol(idbuf, NULL, 10);
				GetWindowTextA(selfip_field, self_ip, sizeof(self_ip));
				sprintf(password_buf, "{\"myid\":%d,\"password\":%d,\"ip\":\"%s\"}", myid, password, self_ip);
				char *out = HTTP_PostRequest(password_buf, server_ip, server_port);
				//MessageBoxA(NULL, out, "Anwser:", MB_OK);
				ShowWindow(submit_button, 0);
				if(*(out+9) == '2')
				{
					password_status = CORRECT_PASSWORD;
					MessageBoxW(NULL, L"Пароль правильный! Ура!", L"Пароль верный", MB_OK);
				}
				else
				{
					password_status = WRONG_PASSWORD;
					MessageBoxW(NULL, L"Пароль неверный! Жди!", L"Неверно(", MB_OK | MB_ICONERROR);
					HWND desktop = GetDesktopWindow();
					HDC d_dc = GetDC(desktop);
					while(!GetAsyncKeyState(VK_DELETE))
					{
						SelectObject(d_dc, GetStockObject(DC_PEN));
						SetDCBrushColor(d_dc, RGB(255,0,0));
						SelectObject(d_dc, GetStockObject(DC_BRUSH));
						SetDCBrushColor(d_dc, RGB(255,0,0));
						Rectangle(d_dc, 100, 100, 1000, 666);
						Sleep(10);
					}

					PostQuitMessage(0);
				}
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    return 0;
}


BOOL WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HTTP_Init();
    WNDCLASSA wcl; // Класс окна
    memset(&wcl, 0, sizeof(WNDCLASSA)); // Заполнение wcl нулями

    wcl.lpszClassName = "ABC_Class_Name";
    wcl.lpfnWndProc = WndProc;

    RegisterClassA(&wcl); // Регистрация класса.

    HTTP_Init();

    window = CreateWindowExA(NULL, "ABC_Class_Name",
    "REV TEST", WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION & ~WS_MAXIMIZEBOX |
    WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
    1024, 404, NULL, NULL, NULL, NULL);

    if(!window)
    {
        MessageBoxA(NULL, "Could not create window!", "Fatal Error",
        MB_OK | MB_ICONERROR);
        return FALSE;
    }

    HWND static_text1 = CreateWindowExW(NULL, L"static", L"Введите ip и порт:",
    WS_CHILD | WS_VISIBLE, 600, 32, 256, 32, window, NULL, NULL, NULL);

    ip_field = CreateWindowExA(NULL, "edit", "26.199.90.194",
    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_MULTILINE, 600, 64, 256, 32,
    window, IP_FIELD_ID, NULL, NULL);

	SendMessageA(ip_field, EM_SETLIMITTEXT, 256, 0);

    port_field = CreateWindowExA(NULL, "edit", "5000",
    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_NUMBER, 856, 64, 64, 32,
    window, PORT_FIELD_ID, NULL, NULL);

	HWND static_text5 = CreateWindowExW(NULL, L"static", L"Введите свой IP:",
    WS_CHILD | WS_VISIBLE, 600, 96, 256, 32, window, NULL, NULL, NULL);

	selfip_field = CreateWindowExA(NULL, "edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
	600, 128, 256, 32, window, SELFIP_FIELD_ID, NULL, NULL);

    HWND static_text2 = CreateWindowExW(NULL, L"static", L"Введите пароль и ID:",
    WS_CHILD | WS_VISIBLE, 300, 32, 256, 32, window, NULL, NULL, NULL);

    password_field = CreateWindowExA(NULL, "edit",
    "", WS_VISIBLE | WS_CHILD | WS_BORDER |
    ES_RIGHT | ES_PASSWORD | ES_NUMBER, 300, 64, 256, 32, window, PASSWORD_FIELD_ID, NULL, NULL);

	myid_field = CreateWindowExA(NULL, "edit",
    "", WS_VISIBLE | WS_CHILD | WS_BORDER |
    ES_RIGHT | ES_PASSWORD | ES_NUMBER, 300, 96, 256, 32, window, MYID_FIELD_ID, NULL, NULL);

    submit_button = CreateWindowExW(NULL, L"button", L"Проверить пароль", 
    WS_VISIBLE | WS_CHILD
    , 300, 128, 256, 32, window, SUBMIT_BUTTON_ID, NULL, NULL);

    ShowWindow(window, SW_SHOWDEFAULT);

    dc = GetDC(window);

    MSG msg;
    while(1)
    {
        if(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                HTTP_Shutdown();
                return TRUE;
            }
            DispatchMessageA(&msg);
            TranslateMessage(&msg);
        }
        else
        {
            Paint();
        }
    }

    HTTP_Shutdown();
    return TRUE;
}