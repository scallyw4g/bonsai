#define BONSAI_LINUX 1

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




#define THREAD_MAIN_RETURN void*

#define exported_function extern "C" __attribute__((visibility("default")))
#define function static

#define ReadBarrier  asm volatile("" ::: "memory"); _mm_lfence()
#define WriteBarrier asm volatile("" ::: "memory"); _mm_sfence()
#define FullBarrier  asm volatile("" ::: "memory"); _mm_sfence(); _mm_lfence()

//
// glX Business
//
#define bonsaiGlGetProcAddress(procName) glXGetProcAddress((GLubyte*)procName)

#define GlobalCwdBufferLength 2048

#define WindowEventMasks StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask

// FIXME(Jesse): Write own snprintf
#define Snprintf(...) snprintf(__VA_ARGS__)

// In Cygwin printing to the console with printf doesn't work, so we have a
// wrapper that does some additional crazyness on Win32
#define PrintConsole(Message) printf(Message)


//
// GLX
//
extern "C" {

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
    int screen;
    int depth;
#if defined(__cplusplus) || defined(c_plusplus)
    int c_class;
#else
    int class;
#endif
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
    int colormap_size;
    int bits_per_rgb;
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

  extern XVisualInfo* glXChooseVisual( Display *dpy, int screen, int *attribList );
  extern GLXFBConfig *glXChooseFBConfig( Display *dpy, int screen, const int *attribList, int *nitems );
  typedef GLXContext ( *PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);
  extern void (*glXGetProcAddress(const GLubyte *procname))( void );
  extern Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx);
  extern void glXSwapBuffers( Display *dpy, GLXDrawable drawable );

}

typedef void ( *PFNGLXSWAPINTERVALEXTPROC) (Display *dpy, GLXDrawable drawable, int interval);
typedef PFNGLXSWAPINTERVALEXTPROC PFNSWAPINTERVALPROC;

//
// GLX
//


typedef int thread_id;
typedef sem_t semaphore;

typedef void* shared_lib;

typedef Display* display;
typedef GLXContext gl_context;

typedef pthread_mutex_t native_mutex;
typedef Window window;

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


