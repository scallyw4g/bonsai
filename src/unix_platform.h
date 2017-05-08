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

#include <semaphore.h>

// Valgrind
#include "/usr/include/valgrind/callgrind.h"



#define GAME_LIB "./bin/libGameLoadable.so"

#define THREAD_MAIN_RETURN void*

#define EXPORT extern "C" __attribute__((visibility("default")))

/*
 * glX Business
 */
#define bonsaiGlGetProcAddress(procName) glXGetProcAddress((GLubyte*)procName)
typedef PFNGLXSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;

#define GlobalCwdBufferLength 2048

#define WindowEventMasks ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask

// FIXME(Jesse): Just write our own snprintf
#define Snprintf(...) snprintf(__VA_ARGS__)

typedef int thread_id;
typedef sem_t semaphore;

typedef void* shared_lib;
typedef Window window;

typedef Display* display;
typedef GLXContext gl_context;

#endif
