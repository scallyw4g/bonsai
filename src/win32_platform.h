#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <Windows.h>
#include <Windowsx.h> // Macros to retrieve mouse coordinates
#include <WinBase.h>
#include <Wingdi.h>

#include <sys/stat.h>

#include <stdint.h>




#include <direct.h> // Chdir

#define GAME_LIB_PATH "bin/Debug/GameLoadable"

// FIXME(Jesse): Just write our own snprintf
#define Snprintf(...) _snprintf(__VA_ARGS__)




// In Cygwin printing to the console with printf doesn't work, so we wrap some
// Win32 crazyness that works
global_variable HANDLE Stdout = GetStdHandle(STD_OUTPUT_HANDLE);
#define PrintConsole(Message)                        \
  OutputDebugString(Message);                        \
  WriteFile(Stdout, Message, strlen(Message), 0, 0);





#define PLATFORM_OFFSET (sizeof(void*))

#define GAME_LIB (GAME_LIB_PATH".dll")

#define exported_function extern "C" __declspec( dllexport )
#define function static

#define THREAD_MAIN_RETURN DWORD WINAPI
#define GAME_MAIN_PROC FARPROC GameMain

#define sleep(seconds) Sleep(seconds * 1000)

#define SWAP_BUFFERS SwapBuffers(hDC)

#define bonsaiGlGetProcAddress(procName) wglGetProcAddress(procName)
typedef PFNWGLSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;

// #define Log(str) OutputDebugString(str)


typedef HANDLE thread_id;
typedef HANDLE semaphore;


// ???
typedef HMODULE shared_lib;
typedef HWND window;
typedef HGLRC gl_context;
typedef HDC display;

#endif
