#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <pthread.h>

// Assert
#include <signal.h>

// chdir
#include <unistd.h>

// dlopen
#include <dlfcn.h>

// Xlib
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>

// Valgrind
#include "/usr/include/valgrind/callgrind.h"



#define Assert(condition) if (!(condition)) raise(SIGTRAP);

#define GAME_LIB_PATH "./libGame"
#define GAME_LIB (GAME_LIB_PATH".so")

#define THREAD_MAIN_RETURN void*

#define EXPORT extern "C" __attribute__((visibility("default")))

#define bonsaiGlGetProcAddress(procName) glXGetProcAddress((GLubyte*)procName)

#define GlobalCwdBufferLength 2048

typedef int thread_id;
typedef int semaphore;

typedef void* shared_lib;
typedef Window window;

typedef Display* display;
typedef GLXContext gl_context;

#endif
