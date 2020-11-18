#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <pthread.h>

// Assert
#include <signal.h>

// chdir
#include <unistd.h>

// dlopen
#include <dlfcn.h>

#include <semaphore.h>

#include <sys/mman.h>   // mmap

#include <stdio.h>

#include <arpa/inet.h>  // inet_addr
#include <sys/socket.h>



#define BONSAI_FUNCTION_NAME __func__

#define RED_TERMINAL ""
#define BLUE_TERMINAL ""
#define GREEN_TERMINAL ""
#define YELLOW_TERMINAL ""
#define WHITE_TERMINAL ""

#define RuntimeBreak() raise(SIGTRAP)
#define TriggeredRuntimeBreak() if (GetDebugState) { GetDebugState()->TriggerRuntimeBreak ? RuntimeBreak() : 0 ; }

#define Newline "\n"

#define THREAD_MAIN_RETURN void*

#define exported_function extern "C"

#define FullBarrier  asm volatile("" ::: "memory"); _mm_sfence();

#define GlobalCwdBufferLength 2048

#define WindowEventMasks StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask


typedef s32 thread_id;
typedef sem_t semaphore;
typedef pthread_mutex_t native_mutex;

typedef void* shared_lib;

bonsai_function void
LogToConsole(counted_string Output);

inline void
WakeThread( semaphore *Semaphore )
{
  sem_post(Semaphore);
  return;
}

inline u64
GetCycleCount()
{
  u64 Result = 0;
  return Result;
}

void
PlatformDebugStacktrace()
{
  Crash();
  /* void *StackSymbols[32]; */
  /* s32 SymbolCount = backtrace(StackSymbols, 32); */
  /* backtrace_symbols_fd(StackSymbols, SymbolCount, STDERR_FILENO); */
  return;
}

inline void
AtomicDecrement( volatile u32 *Source )
{
  __sync_sub_and_fetch( Source, 1 );
  return;
}

inline u64
AtomicIncrement( volatile u64 *Source )
{
  u64 Result = __sync_fetch_and_add( Source, 1 );
  return Result;
}

inline u32
AtomicIncrement( volatile u32 *Source )
{
  u32 Result = __sync_fetch_and_add( Source, 1 );
  return Result;
}

inline void
AtomicDecrement( volatile s32 *Source )
{
  __sync_sub_and_fetch( Source, 1 );
  return;
}

inline void
AtomicIncrement( volatile s32 *Source )
{
  __sync_add_and_fetch( Source, 1 );
  return;
}

inline u32
AtomicExchange( volatile u32 *Source, const u32 Exchange )
{
  u32 Result = __sync_lock_test_and_set( Source, Exchange );
  return Result;
}

inline b32
AtomicCompareExchange( volatile char **Source, volatile char *Exchange, volatile char *Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline bool
AtomicCompareExchange( volatile void** Source, void* Exchange, void* Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline bool
AtomicCompareExchange( volatile u64 *Source, u64 Exchange, u64 Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline bool
AtomicCompareExchange( volatile u32 *Source, u32 Exchange, u32 Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

typedef umm gl_context;
struct os
{
  gl_context GlContext;
  b32 Window;
  b32 ContinueRunning = True;
};

struct native_file
{
  FILE* Handle;
  counted_string Path;
};

global_variable native_file Stdout =
{
  .Handle = stdout,
  .Path = CSz("stdout")
};
