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
ThreadSleep( semaphore *Semaphore )
{
  sem_wait(Semaphore);
  return;
}

inline void
WakeThread( semaphore *Semaphore )
{
  sem_post(Semaphore);
  return;
}

semaphore
CreateSemaphore( int ThreadCount )
{
  semaphore Result;
  sem_init(&Result, 0, 1);
  return Result;
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
    Error("Loading library: %s", error);
    Assert(False);
  }
  else
  {
    Info("Game Lib loaded!");
  }

  return Result;
}

b32
OpenAndInitializeWindow( os *Os, platform *Plat)
{
  GLint GlAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

  Os->Display = XOpenDisplay(0);
  if (!Os->Display) { Error("Cannot connect to X Server"); return False; }

  window RootWindow = DefaultRootWindow(Os->Display);
  if (!RootWindow) { Error("Unable to get RootWindow"); return False; }

  XVisualInfo *VisualInfo = glXChooseVisual(Os->Display, 0, GlAttribs);
  if (!VisualInfo) { Error("Unable to get Visual Info"); return False; }

  Colormap ColorInfo = XCreateColormap(Os->Display, RootWindow, VisualInfo->visual, AllocNone);

  XSetWindowAttributes WindowAttribs;
  WindowAttribs.colormap = ColorInfo;
  WindowAttribs.event_mask = WindowEventMasks;

  Window win = XCreateWindow(Os->Display, RootWindow,
      0, 0,
      Plat->WindowWidth, Plat->WindowHeight,
      0, VisualInfo->depth, InputOutput, VisualInfo->visual,
      CWColormap | CWEventMask, &WindowAttribs);

  if (!win) { Error("Unable to Create Window"); return False; }

  XMapWindow(Os->Display, win);
  XStoreName(Os->Display, win, "Bonsai");

  Os->GlContext = glXCreateContext(Os->Display, VisualInfo, NULL, GL_TRUE);
  if (!Os->GlContext) { Error("Unable to Create GLXContext"); return False; }

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
  b32 EventFound =
    XCheckWindowEvent(Os->Display,
                      Os->Window,
                      WindowEventMasks,
                      &Event);

  if (EventFound)
  {

    switch (Event.type)
    {

      case Expose:
      {
        Plat->WindowWidth = Event.xexpose.width;
        Plat->WindowHeight = Event.xexpose.height;
      } break;

      case MotionNotify:
      {
        Plat->MouseP.x = Event.xmotion.x;
        Plat->MouseP.y = Event.xmotion.y;
      } break;

      case ButtonPress:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB = True;
        }
      } break;

      case ButtonRelease:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB = False;
        }
      } break;

      case KeyRelease:
      {
        int KeySym = XLookupKeysym(&Event.xkey, 0);
        switch (KeySym)
        {
          case XK_w:
          {
            Plat->Input.W = False;
          } break;

          case XK_s:
          {
            Plat->Input.S = False;
          } break;

          case XK_a:
          {
            Plat->Input.A = False;
          } break;

          case XK_d:
          {
            Plat->Input.D = False;
          } break;

          case XK_q:
          {
            Plat->Input.Q = False;
          } break;

          case XK_e:
          {
            Plat->Input.E = False;
          } break;

          case XK_F11:
          {
            Plat->Input.F11 = False;
          } break;

          case XK_Escape:
          {
            Os->ContinueRunning = false;
          } break;

          default:
          {
          } break;
        }
      } break;

      case KeyPress:
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

      } break;
    }

  }

  return EventFound;
}

inline void
BonsaiSwapBuffers(os *Os)
{
  glXSwapBuffers(Os->Display, Os->Window);
}

inline void
Terminate(os *Os)
{
  XDestroyWindow(Os->Display, Os->Window);
  XCloseDisplay(Os->Display);
  return;
}

inline void
SetMouseP(v2 P)
{
  return;
}

#endif
