#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <Windows.h>
#include <WinBase.h>
#include <Wingdi.h>

#include <sys/stat.h>

#include <stdint.h>



#define Assert(condition) if (!(condition)) __debugbreak();

#ifdef __CYGWIN__
#define GAME_LIB_PATH "build/cygGame"
#else
#define GAME_LIB_PATH "Debug/Game"
#endif

#define GAME_LIB (GAME_LIB_PATH".dll")

#define EXPORT __declspec( dllexport )

#define THREAD_MAIN_RETURN DWORD WINAPI
#define GAME_MAIN_PROC FARPROC GameMain

#define sleep(seconds) Sleep(seconds * 1000)

#define SWAP_BUFFERS SwapBuffers()

typedef HANDLE thread_id;
typedef HANDLE semaphore;


// ???
typedef HMODULE shared_lib;
typedef HWND window;

#endif
