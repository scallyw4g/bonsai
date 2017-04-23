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



struct platform;
struct game_state;



#define Assert(condition) if (!(condition)) raise(SIGTRAP);

#define GAME_LIB_PATH "./libGame"
#define GAME_LIB (GAME_LIB_PATH".so")

#define THREAD_MAIN_RETURN void*

typedef void (*GameCallback)(void*);
typedef game_state* (*game_init_proc)(platform*);
typedef bool (*game_main_proc)(platform*, game_state*);

#define EXPORT extern "C" __attribute__((visibility("default")))

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
