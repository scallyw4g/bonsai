#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <pthread.h>

// Assert
#include <signal.h>

// dlopen
#include <dlfcn.h>

// Xlib
#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/glx.h>


#define Assert(condition) if (!(condition)) raise(SIGTRAP);

#define GAME_LIB_PATH "./libGame"
#define GAME_LIB (GAME_LIB_PATH".so")

#define THREAD_MAIN_RETURN void*

#define EXPORT extern "C" __attribute__((visibility("default")))

#define SWAP_BUFFERS glXSwapBuffers(dpy, Window)

typedef int thread_id;
typedef int semaphore;

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


typedef void* shared_lib;
typedef Window window;

#endif
