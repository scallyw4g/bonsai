#ifndef UNIX_PLATFORM_CPP
#define UNIX_PLATFORM_CPP

#include <stdio.h>

#include <unix_platform.h>
#include <platform.h>

inline bool
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline void
ThreadSleep( semaphore Semaphore )
{
  /* WaitForSingleObject( Semaphore, INFINITE ); */
  return;
}

inline void
WakeThread( semaphore Semaphore )
{
  /* ReleaseSemaphore( Semaphore, 1, 0 ); */
  return;
}

semaphore
CreateSemaphore( int ThreadCount )
{
  /* semaphore Result = CreateSemaphore( 0, 0, ThreadCount, 0); */
  return 0;
}

int
GetLogicalCoreCount()
{
  int Result = sysconf(_SC_NPROCESSORS_ONLN);
  return Result;
}

thread_id
CreateThread( void* (*ThreadMain)(void*), thread_startup_params *Params)
{
  pthread_t Thread;

  pthread_attr_t Attribs;
  pthread_attr_init(&Attribs);

  Params->Self.ID = pthread_create(&Thread, &Attribs, ThreadMain, Params);

  return Params->Self.ID;
}

#define CompleteAllWrites  asm volatile("" ::: "memory"); _mm_sfence()

__inline__ unsigned long long
GetCycleCount()
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));

  unsigned long long Result = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );

  return Result;
}

void
CloseLibrary(shared_lib Lib)
{
  int e = dlclose(Lib);
  Assert(!e);

  return;
}

inline void*
OpenLibrary(const char *filename)
{
  void* Result = dlopen(filename, RTLD_NOW);

  if (!Result)
  {
    char *error = dlerror();
    printf("Error loading library: %s \n", error);
    Assert(False);
  }
  else
  {
    printf("Game Lib loaded! \n");
  }

  return Result;
}

b32
OpenAndInitializeWindow( os *Os, platform *Plat, int WindowWidth, int WindowHeight )
{
  GLint GlAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

  Os->Display = XOpenDisplay(0);
  if (!Os->Display) { printf(" Cannot connect to X Server \n"); return False; }

  window RootWindow = DefaultRootWindow(Os->Display);
  if (!RootWindow) { printf(" Unable to get RootWindow \n"); return False; }

  XVisualInfo *VisualInfo = glXChooseVisual(Os->Display, 0, GlAttribs);
  if (!VisualInfo) { printf(" Unable to get Visual Info \n"); return False; }

  Colormap ColorInfo = XCreateColormap(Os->Display, RootWindow, VisualInfo->visual, AllocNone);

  XSetWindowAttributes WindowAttribs;
  WindowAttribs.colormap = ColorInfo;
  WindowAttribs.event_mask = ExposureMask | KeyPressMask;

  Window win = XCreateWindow(Os->Display, RootWindow, 0, 0, 600, 600, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &WindowAttribs);
  if (!win) { printf(" Unable to Create Window \n"); return False; }

  XMapWindow(Os->Display, win);
  XStoreName(Os->Display, win, "Bonsai");

  Os->GlContext = glXCreateContext(Os->Display, VisualInfo, NULL, GL_TRUE);
  if (!Os->GlContext) { printf(" Unable to Create GLXContext \n"); return False; }

  glXMakeCurrent(Os->Display, win, Os->GlContext);

  Os->Window = win;

  return True;
}

inline GameCallback
GetProcFromLib(shared_lib Lib, const char *Name)
{
  GameCallback Result = (GameCallback)dlsym(Lib, Name);
  return Result;
}

char*
GetCwd()
{
  getcwd(GlobalCwdBuffer, GlobalCwdBufferLength);
  return (GlobalCwdBuffer);
}

b32
IsFilesystemRoot(char *Filepath)
{
  b32 Result = ( Filepath[0] == '/' && Filepath[1] == 0 );
  return Result;
}

inline u64
GetHighPrecisionClock()
{
  struct timespec Time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &Time);
  return(Time.tv_nsec);
}

b32
ProcessOsMessages(os *Os)
{
  return False;
}

inline void
BonsaiSwapBuffers(os *Os)
{
  glXSwapBuffers(Os->Display, Os->Window);
}

#endif
