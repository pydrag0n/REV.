@echo off
set CC="yourcc"
%CC% -o main.exe main.c am_http.c nxjson.c -lkernel32 -luser32 -lgdi32 -lws2_32
main.exe
echo.
echo.
echo.
echo [finish]
