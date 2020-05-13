
// Disable warnings about insecure CRT functions
//
#pragma warning(disable : 4996)

#include <Windows.h>
#include <Windowsx.h> // Macros to retrieve mouse coordinates
#include <WinBase.h>
#include <Wingdi.h>

#include <sys/stat.h>

#include <stdint.h>

#include <direct.h> // Chdir

#define BONSAI_FUNCTION_NAME __FUNCTION__

#define RED_TERMINAL ""
#define BLUE_TERMINAL ""
#define GREEN_TERMINAL ""
#define YELLOW_TERMINAL ""
#define WHITE_TERMINAL ""

#define GlDebugMessage(...)  PrintConsole(" * Gl Debug Message - "); \
                             VariadicOutputDebugString(__VA_ARGS__); \
                             PrintConsole("\n")

#define Info(...)  PrintConsole("   Info - ");             \
                   VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define Debug(...) VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define Error(...) PrintConsole(" ! Error - ");            \
                   VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define Warn(...)  PrintConsole(" * Warn - ");             \
                   VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define RuntimeBreak() __debugbreak()


#define GAME_LIB_PATH "bin/Debug/GameLoadable"

#define Newline "\r\n"

// In Cygwin printing to the console with printf doesn't work, so we wrap some
// Win32 crazyness that works
global_variable HANDLE Stdout = GetStdHandle(STD_OUTPUT_HANDLE);
#define PrintConsole(Message)                        \
        OutputDebugString(Message);                  \
        WriteFile(Stdout, Message, strlen(Message), 0, 0);

#define PLATFORM_OFFSET (sizeof(void*))

#define GAME_LIB (GAME_LIB_PATH".dll")

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
