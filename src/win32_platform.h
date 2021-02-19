
// Disable warnings about insecure CRT functions
//
#pragma warning(disable : 4996)

#include <Windows.h>
#include <windowsx.h> // Macros to retrieve mouse coordinates
#include <WinBase.h>
#include <wingdi.h>
#include <bonsai_stdlib/headers/wgl.h>

#include <sys/stat.h>

#include <stdint.h>

#include <direct.h> // Chdir

#define BONSAI_FUNCTION_NAME __FUNCTION__

#define RED_TERMINAL ""
#define BLUE_TERMINAL ""
#define GREEN_TERMINAL ""
#define YELLOW_TERMINAL ""
#define WHITE_TERMINAL ""

#define RuntimeBreak() __debugbreak()

#define GAME_LIB_PATH "bin/Debug/GameLoadable"

#define Newline "\r\n"

// TODO(Jesse): Replace with our internal code to print to console?
/* global_variable HANDLE Stdout = ; */
/* #define PrintConsole(Message)                        \ */
/*         OutputDebugString(Message);                  \ */
/*         WriteFile(Stdout, Message, strlen(Message), 0, 0); */

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
typedef HANDLE native_mutex;


// ???
typedef HMODULE shared_lib;
typedef HWND window;
typedef HGLRC gl_context;
typedef HDC display;

struct native_file
{
  FILE* Handle;
  counted_string Path;
};

/* global_variable native_file Stdout = */
/* { */
/*   .Handle = GetStdHandle(STD_OUTPUT_HANDLE), */
/*   .Path = CSz("stdout") */
/* }; */

struct os
{
  HWND Window;
  HDC Display;
  HGLRC GlContext;

  b32 ContinueRunning = True;
};

bonsai_function u64
GetCycleCount()
{
  u64 Result = __rdtsc();
  return Result;
}

inline b32
AtomicCompareExchange( volatile u32 *Source, u32 Exchange, u32 Comparator )
{
  r64 Val = InterlockedCompareExchange( (LONG volatile *)Source, Exchange, Comparator);
  b32 Result = (Val == Comparator);
  return Result;
}

inline b32
AtomicCompareExchange( volatile void **Source, void *Exchange, void *Comparator )
{
  u64 Val = InterlockedCompareExchange( (u64 volatile *)Source, (u64)Exchange, (u64)Comparator);
  b32 Result = (Val == (u64)Comparator);
  return Result;
}

inline u32
AtomicIncrement( u32 volatile *Dest)
{
  u32 Result = InterlockedIncrement(Dest);
  return Result;
}

inline u64
AtomicIncrement( u64 volatile *Dest)
{
  u64 Result = InterlockedIncrement(Dest);
  return Result;
}

