
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

#define PLATFORM_RUNTIME_BREAK() __debugbreak()

#define runtime_lib_export __declspec(dllexport)

#define Newline "\n"

#define PLATFORM_OFFSET (sizeof(void*))

#define PLATFORM_RUNTIME_LIB_EXTENSION ".dll"

#define THREAD_MAIN_RETURN DWORD WINAPI
#define GAME_MAIN_PROC FARPROC GameMain

#define sleep(seconds) Sleep(seconds * 1000)

#define SWAP_BUFFERS SwapBuffers(hDC)

#define bonsaiGlGetProcAddress(procName) wglGetProcAddress(procName)
typedef PFNWGLSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;


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

inline u32
AtomicDecrement( u32 volatile *Dest)
{
  u32 Result = InterlockedDecrement(Dest);
  return Result;
}

inline u64
AtomicDecrement( u64 volatile *Dest)
{
  u64 Result = InterlockedDecrement(Dest);
  return Result;
}

link_internal void*
PlatformGetGlFunction(const char* Name)
{
  void *Result = 0;

  void *WglProc = (void*)wglGetProcAddress(Name);
  if ((s64)WglProc == -1 ||
      (s64)WglProc ==  0 ||
      (s64)WglProc ==  1 ||
      (s64)WglProc ==  2 ||
      (s64)WglProc ==  3 )
  {
    HMODULE OpenglDllHandle = LoadLibraryA("opengl32.dll");
    if (OpenglDllHandle)
    {
      Result = (void*)GetProcAddress(OpenglDllHandle, Name);
    }
    else
    {
      Error("Could not load opengl32.dll");
    }
  }
  else
  {
    Result = WglProc;
  }

  if (!Result)
  {
    Error("Couldn't load Opengl fucntion (%s)", Name);
  }

  return Result;
}
