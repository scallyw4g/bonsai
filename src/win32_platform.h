#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <Windows.h>
#include <WinBase.h>

// I know that unsafe shit is unsafe, thanks MSVC
#define _CRT_SECURE_NO_WARNINGS 1


#define THREAD_MAIN_RETURN DWORD WINAPI

typedef HANDLE thread_id;
typedef HANDLE semaphore;

// 1 Bit types
typedef unsigned char u8;

// 32 Bit types
typedef int s32;
typedef unsigned int u32;
typedef float r32;

// 64 Bit types
typedef long int s64;
typedef unsigned long int u64;
typedef u64 umm;

#endif
