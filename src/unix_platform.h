#define BONSAI_LINUX 1

/* #include <linux/ftrace.h> */
/* #include <linux/getcpu.h> */

#include <pthread.h>

// Assert
#include <signal.h>

// chdir
#include <unistd.h>

// dlopen
#include <dlfcn.h>

// X11 typedefs Cursor to some internal type.. and I want it, so we're hijacking it here.
// https://stackoverflow.com/questions/25867905/how-could-i-temporarily-un-typedef-something
#define Cursor X11PleaseStop
#include <X11/X.h>
#include <X11/Xlib.h>
#undef Cursor

#define XK_LATIN1 1
#define XK_MISCELLANY 1
#include <X11/keysymdef.h>

// X11 defines this to 0, which is really annoying
#ifdef Success
#undef Success
#endif

#include <semaphore.h>

// Backtrace
#include <errno.h>
#include <execinfo.h>

#include <arpa/inet.h>  // inet_addr
#include <sys/socket.h>

#include <sys/mman.h>   // mmap

#include <stdio.h>




#define BONSAI_FUNCTION_NAME __func__

#define RED_TERMINAL "\x1b[31m"
#define BLUE_TERMINAL "\x1b[34m"
#define GREEN_TERMINAL "\x1b[32m"
#define YELLOW_TERMINAL "\x1b[33m"
#define WHITE_TERMINAL "\x1b[37m"

#define Info(...)                                                  \
  LogToConsole(CSz(BLUE_TERMINAL "   Info" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                             \
  LogToConsole(CSz("\n"))

#define Debug(...)     \
  printf(__VA_ARGS__); \
  LogToConsole(CSz("\n"))

#define Error(...)                                                 \
  LogToConsole(CSz(RED_TERMINAL " ! Error" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                             \
  LogToConsole(CSz("\n"))

#define Warn(...)                                                    \
  LogToConsole(CSz(YELLOW_TERMINAL " * Warn" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                               \
  LogToConsole(CSz("\n"))

#define OpenGlDebugMessage(...)                                                      \
  LogToConsole(CSz(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                                               \
  LogToConsole(CSz("\n"))

#define RuntimeBreak() raise(SIGTRAP)
#define TriggeredRuntimeBreak() if (GetDebugState) { GetDebugState()->TriggerRuntimeBreak ? RuntimeBreak() : 0 ; }

#define Newline "\n"

#define THREAD_MAIN_RETURN void*

#define exported_function extern "C"

#define ReadBarrier  asm volatile("" ::: "memory"); _mm_lfence()
#define WriteBarrier asm volatile("" ::: "memory"); _mm_sfence()
#define FullBarrier  asm volatile("" ::: "memory"); _mm_sfence(); _mm_lfence()

//
// glX Business
//
#define bonsaiGlGetProcAddress(procName) glXGetProcAddress((GLubyte*)procName)

#define GlobalCwdBufferLength 2048

#define WindowEventMasks StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask


//
// GLX
//

#define GLX_RGBA           4
#define GLX_DOUBLEBUFFER   5
#define GLX_DEPTH_SIZE     12

#define GLX_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GLX_CONTEXT_FLAGS_ARB             0x2094
#define GLX_CONTEXT_PROFILE_MASK_ARB      0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define GLX_CONTEXT_DEBUG_BIT_ARB         0x00000001

typedef struct {
  Visual *visual;
  VisualID visualid;
  s32 screen;
  s32 depth;
  s32 c_class;
  u64 red_mask;
  u64 green_mask;
  u64 blue_mask;
  s32 colormap_size;
  s32 bits_per_rgb;
} XVisualInfo;


typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXPixmap;
typedef XID GLXDrawable;
/* GLX 1.3 and later */
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXFBConfigID;
typedef XID GLXContextID;
typedef XID GLXWindow;
typedef XID GLXPbuffer;

exported_function XVisualInfo* glXChooseVisual( Display *dpy, s32 screen, s32 *attribList );
exported_function GLXFBConfig* glXChooseFBConfig( Display *dpy, s32 screen, const s32 *attribList, s32 *nitems );
exported_function void (*glXGetProcAddress(const u8 *procname))( void );
exported_function Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx);
exported_function void glXSwapBuffers( Display *dpy, GLXDrawable drawable );

typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const s32 *attrib_list);

typedef void ( *PFNGLXSWAPINTERVALEXTPROC) (Display *dpy, GLXDrawable drawable, s32 s32);
typedef PFNGLXSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;



//
// GLX
//



typedef s32 thread_id;
typedef sem_t semaphore;

typedef void* shared_lib;

typedef Display* display;
typedef GLXContext gl_context;

typedef pthread_mutex_t native_mutex;
typedef Window window;

function void
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
  u64 Result = __rdtsc();
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

void
ReadBytes(u8* Dest, u64 BytesToRead, FILE *Src)
{
  Assert(BytesToRead);
  u64 BytesRead = fread(Dest, 1, BytesToRead, Src);
  Assert(BytesRead != 0);
  return;
}

struct os
{
  window Window;
  display Display;
  gl_context GlContext;

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
