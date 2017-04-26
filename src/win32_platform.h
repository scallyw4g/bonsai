#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <Windows.h>
#include <Windowsx.h> // Macros to retrieve mouse coordinates
#include <WinBase.h>
#include <Wingdi.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#include <sys/stat.h>

#include <stdint.h>



#define Assert(condition) if (!(condition)) __debugbreak();

/*
 *  Cygwin GCC specific stuff
 */
#ifdef __CYGWIN__

#include <unistd.h> // Chdir
#define GAME_LIB_PATH "build/cygGame"

/*
 *  MSVC specific stuff
 */
#else

#include <direct.h> // Chdir

#define GAME_LIB_PATH "build/Debug/Game"
#define snprintf(...) _snprintf(__VA_ARGS__)

#endif // __CYGWIN__








#define PLATFORM_OFFSET (sizeof(void*))

#define GAME_LIB (GAME_LIB_PATH".dll")

#define EXPORT extern "C" __declspec( dllexport )

#define THREAD_MAIN_RETURN DWORD WINAPI
#define GAME_MAIN_PROC FARPROC GameMain

#define sleep(seconds) Sleep(seconds * 1000)

#define SWAP_BUFFERS SwapBuffers(hDC)

#define bonsaiGlGetProcAddress(procName) wglGetProcAddress(procName)

// #define Log(str) OutputDebugString(str)


typedef HANDLE thread_id;
typedef HANDLE semaphore;


// ???
typedef HMODULE shared_lib;
typedef HWND window;
typedef HGLRC gl_context;
typedef HDC display;

#endif
