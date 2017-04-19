#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <Windows.h>
#include <WinBase.h>

#include <sys/stat.h>

#define Assert(condition) if (!(condition)) __debugbreak();

#ifdef __CYGWIN__
#define GAME_LIB_PATH "build/cygGame"
#else
#define GAME_LIB_PATH "Debug/Game"
#endif

#define GAME_LIB (GAME_LIB_PATH".dll")

#define EXPORT __declspec( dllexport )

#define THREAD_MAIN_RETURN DWORD WINAPI

typedef HANDLE thread_id;
typedef HANDLE semaphore;

// 1 Bit types
typedef unsigned char u8;

// 32 Bit types
typedef int s32;
typedef unsigned int u32;
typedef float r32;
typedef u32 b32;
typedef float real32;

// 64 Bit types
typedef long int s64;
typedef unsigned long int u64;
typedef u64 umm;
typedef double real64;

// ???
typedef HMODULE shared_lib;
typedef HWND window;

#endif
