#ifndef UNIX_PLATFORM_CPP
#define UNIX_PLATFORM_CPP

#include <stdio.h>

#include <unix_platform.h>
#include <platform.h>

#include <GL/glx.h>
#include <X11/keysymdef.h>

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
    printf(" !!! Error loading library: %s \n", error);
    Assert(False);
  }
  else
  {
    printf(" --- Game Lib loaded! \n");
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
  GLOBAL_VARIABLE char GlobalCwdBuffer[GlobalCwdBufferLength];
  getcwd(GlobalCwdBuffer, GlobalCwdBufferLength);
  return (GlobalCwdBuffer);
}

b32
IsFilesystemRoot(char *Filepath)
{
  b32 Result = ( Filepath[0] == '/' && Filepath[1] == 0 );
  return Result;
}

inline r64
GetHighPrecisionClock()
{
  struct timespec Time;
  clock_gettime(CLOCK_MONOTONIC, &Time);
  return(Time.tv_nsec);
}

r64
ComputeDtForFrame(r64 *lastTime)
{
  r64 cachedLastTime = *lastTime;
  r64 currentTime = GetHighPrecisionClock();

  if (cachedLastTime > currentTime)
    cachedLastTime -= 1000000000;

  r64 Result = ((currentTime - cachedLastTime) / 1000000000);

  *lastTime = currentTime;
  return Result;
}

b32
ProcessOsMessages(os *Os, platform *Plat)
{

  XEvent Event;
  b32 EventFound = XCheckWindowEvent(Os->Display, Os->Window, ExposureMask|KeyPressMask, &Event);

  if (EventFound)
  {
    if(Event.type == Expose)
    {
      XWindowAttributes WindowAttribs;
      XGetWindowAttributes(Os->Display, Os->Window, &WindowAttribs);

      /* s32 width = WindowAttribs.width; */
      /* s32 height = WindowAttribs.height; */
      s32 width = 300;
      s32 height = 300;

      glViewport(0, 0, width, height);

      printf("Exposed %d %d\n", width, height);
    }

    else if(Event.type == KeyPress)
    {
      int KeySym = XLookupKeysym(&Event.xkey, 0);
      switch (KeySym)
      {
        case XK_w:
        {
          Plat->Input.W = True;
        } break;

        case XK_s:
        {
          Plat->Input.S = True;
        } break;

        case XK_a:
        {
          Plat->Input.A = True;
        } break;

        case XK_d:
        {
          Plat->Input.D = True;
        } break;

        case XK_q:
        {
          Plat->Input.Q = True;
        } break;

        case XK_e:
        {
          Plat->Input.E = True;
        } break;

        case XK_F11:
        {
          Plat->Input.F11 = True;
        } break;

        default:
        {
        } break;
      }
    }
  }

  return EventFound;
}

inline void
BonsaiSwapBuffers(os *Os)
{
  glXSwapBuffers(Os->Display, Os->Window);
}

#endif
