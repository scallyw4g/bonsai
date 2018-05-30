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

#include<arpa/inet.h> //inet_addr

// Valgrind
// #include "/usr/include/valgrind/callgrind.h"

// Backtrace
#include <errno.h>
#include <execinfo.h>



#define GAME_LIB "./bin/libGameLoadable.so"

#define THREAD_MAIN_RETURN void*

#define EXPORT extern "C" __attribute__((visibility("default")))

#define CompleteAllWrites  asm volatile("" ::: "memory"); _mm_sfence()

/*
 * glX Business
 */
#define bonsaiGlGetProcAddress(procName) glXGetProcAddress((GLubyte*)procName)
typedef PFNGLXSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;

#define GlobalCwdBufferLength 2048

#define WindowEventMasks StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask

// FIXME(Jesse): Write own snprintf
#define Snprintf(...) snprintf(__VA_ARGS__)

// In Cygwin printing to the console with printf doesn't work, so we have a
// wrapper that does some additional crazyness on Win32
#define PrintConsole(Message) printf(Message)

typedef int thread_id;
typedef sem_t semaphore;

typedef void* shared_lib;
typedef Window window;

typedef Display* display;
typedef GLXContext gl_context;

inline void
WakeThread( semaphore *Semaphore )
{
  sem_post(Semaphore);
  return;
}

__inline__ u64
GetCycleCount()
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  u64 Result = ( (u64)lo)|( ((u64)hi)<<32 );
  return Result;
}

void
PlatformDebugStacktrace()
{
  void *StackSymbols[32];
  s32 SymbolCount = backtrace(StackSymbols, 32);
  backtrace_symbols_fd(StackSymbols, SymbolCount, STDERR_FILENO);
  return;
}

