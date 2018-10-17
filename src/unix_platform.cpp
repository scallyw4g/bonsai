#include <stdio.h>

// network layer
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr

// mmap
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h> // get/set rlimits

#include <platform.h>

#include <GL/glx.h>
#include <X11/keysymdef.h>


#if 0
inline void
PrintSemValue( semaphore *Semaphore )
{
  s32 Value;

  s32 E = sem_getvalue(Semaphore, &Value);
  Assert(E==0);

  printf("Value: %d \n", Value);

  return;
}
#endif

void
PlatformInitializeMutex(mutex *Mutex)
{
  pthread_mutex_init(&Mutex->M, NULL);
  return;
}

void
PlatformUnlockMutex(mutex *Mutex, u32 ThreadIndex)
{
  s32 Fail = pthread_mutex_unlock(&Mutex->M);
  TIMED_MUTEX_RELEASED(Mutex);

  if (Fail)
  {
    Error("Failed to un-lock mutex");
    Assert(False);
  }

  return;
}

void
PlatformLockMutex(mutex *Mutex, u32 ThreadIndex)
{
  TIMED_MUTEX_WAITING(Mutex);

  s32 Fail = pthread_mutex_lock(&Mutex->M);

  TIMED_MUTEX_AQUIRED(Mutex);

  if (Fail)
  {
    Error("Failed to aquire lock");
    Assert(False);
  }

  return;
}

u64
PlatformGetPageSize()
{
  u64 InvalidSysconfReturn = ((u64)-1);
  local_persist u64 PageSize = (u64)sysconf(_SC_PAGESIZE);
  Assert(PageSize != InvalidSysconfReturn);
  return PageSize;
}

s32
GetLogicalCoreCount()
{
  local_persist s32 CoreCount = (s32)sysconf(_SC_NPROCESSORS_ONLN);
  return CoreCount;
}

u32
GetWorkerThreadCount()
{
  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1 + DEBUG_THREAD_COUNT_BIAS; // -1 because we already have a main thread
  return ThreadCount;
}

u32
GetTotalThreadCount()
{
  u32 Result = GetWorkerThreadCount() + 1;
  return Result;
}

u8*
PlatformProtectPage(u8* Mem)
{
  u64 PageSize = PlatformGetPageSize();

  Assert((u64)Mem % PageSize == 0);

  mprotect(Mem, PageSize, PROT_NONE);
  u8* Result = Mem + PageSize;
  return Result;
}

void
PlatformDeallocateArena(memory_arena *Arena)
{
  if (Arena->Start)
  {
    s32 Deallocated = (munmap(Arena->Start, TotalSize(Arena)) == 0);
    Assert(Deallocated);
  }

#if MEMPROTECT_OVERFLOW
  {
    umm PageSize = PlatformGetPageSize();
    u8 *ArenaBytes =  (u8*)Arena - ((umm)Arena % PageSize);
    s32 Deallocated = (munmap(ArenaBytes, PageSize*2) == 0);
    Assert(Deallocated);
  }
#else
  NotImplemented;
#endif

  return;
}

u8*
PlatformAllocateSize(umm AllocationSize)
{
  Assert(AllocationSize % PlatformGetPageSize() == 0);

  u32 Protection = PROT_READ|PROT_WRITE;
  u32 Flags = MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE;

  u8 *Bytes = (u8*)mmap(0, AllocationSize, Protection, Flags,  -1, 0);
  if (Bytes == MAP_FAILED)
  {
    Bytes = 0;
    s32 Error = errno;
    if (Error == ENOMEM)
    {
      Error("Out of memory, or exhausted virtual page table.");
      Assert(False);
    }
    else
    {
      Error("Unknown error allocating pages.");
      Assert(False);
    }
  }

  return Bytes;
}

memory_arena*
PlatformAllocateArena(umm RequestedBytes /* = Megabytes(1) */, b32 MemProtect /* = True */)
{
  // FIXME(Jesse): We shouldn't really be able to ask for < 1MB worth of space
  u64 PageSize = PlatformGetPageSize();
  u64 ToNextPage = PageSize - (RequestedBytes % PageSize);
  umm AllocationSize = RequestedBytes + ToNextPage;

  Assert(AllocationSize % PageSize == 0);

#if MEMPROTECT_OVERFLOW
  Assert(sizeof(memory_arena) < PageSize);
  u8 *ArenaBytes = PlatformAllocateSize(PageSize*2);
  ArenaBytes += (PageSize - sizeof(memory_arena));

#elif MEMPROTECT_UNDERFLOW
  NotImplemented;
#else
  u8 *ArenaBytes = PlatformAllocateSize(PageSize);
#endif

  memory_arena *Result = (memory_arena*)ArenaBytes;

  u8 *Bytes = PlatformAllocateSize(AllocationSize);
  Result->Start = Bytes;
  Result->At = Bytes;

  Result->End = Bytes + AllocationSize;
  Result->NextBlockSize = AllocationSize * 2;

#if MEMPROTECT_OVERFLOW
  if (MemProtect)
  {
    Assert(OnPageBoundary(Result, PageSize));
    PlatformProtectPage(ArenaBytes + sizeof(memory_arena));
  }

  Assert((umm)Result->Start % PageSize == 0);
  Assert(Remaining(Result) >= RequestedBytes);
#else
  NotImplemented
#endif

  return Result;
}

void
PlatformUnprotectArena(memory_arena *Arena)
{
  TIMED_FUNCTION();
  umm Size = Arena->End - Arena->Start;
  s32 Err = mprotect(Arena->Start, Size, PROT_READ|PROT_WRITE);
  if (Err == -1)
  {
    Error("Unprotecting arena failed");
    Assert(False);
  }

  return;
}

#if PLATFORM_THREADING_IMPLEMENTATIONS

inline void
ThreadSleep( semaphore *Semaphore )
{
  TIMED_FUNCTION();

  AtomicIncrement(&GetDebugState()->WorkerThreadsWaiting);
  sem_wait(Semaphore);
  AtomicDecrement(&GetDebugState()->WorkerThreadsWaiting);
  return;
}

semaphore
CreateSemaphore(void)
{
  semaphore Result;
  sem_init(&Result, 0, 0);

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

#endif // PLATFORM_THREADING_IMPLEMENTATIONS

#if PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS

void
CloseLibrary(shared_lib Lib)
{
  s32 Error = dlclose(Lib);
  if (Error != 0)
  {
    Error("Closing Shared Library");
  }

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
    Info("Library (%s) loaded!", filename);
  }

  return Result;
}

void
HandleGlDebugMessage(GLenum Source, GLenum Type, GLuint Id, GLenum Severity,
                     GLsizei MessageLength, const GLchar* Message, const void* UserData)
{
  if (Severity == GL_DEBUG_SEVERITY_NOTIFICATION)
  {
  }
  else
  {
    Debug("GL - %s", Message);
  }

  return;
}

b32
OpenAndInitializeWindow( os *Os, platform *Plat, s32 DebugFlags)
{
  GLint GlAttribs[] = {
    GLX_RGBA,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER,
    None };

  Os->Display = XOpenDisplay(0);
  if (!Os->Display) { Error("Cannot connect to X Server"); return False; }

  window RootWindow = DefaultRootWindow(Os->Display);
  if (!RootWindow) { Error("Unable to get RootWindow"); return False; }

  s32 Screen = DefaultScreen(Os->Display);

  XVisualInfo *VisualInfo = glXChooseVisual(Os->Display, Screen, GlAttribs);
  if (!VisualInfo) { Error("Unable to get Visual Info"); return False; }

  Assert(VisualInfo->screen == Screen);

  Colormap ColorInfo = XCreateColormap(Os->Display, RootWindow, VisualInfo->visual, AllocNone);

  XSetWindowAttributes WindowAttribs;
  WindowAttribs.colormap = ColorInfo;
  WindowAttribs.event_mask = WindowEventMasks;

  Assert(Plat->WindowWidth && Plat->WindowHeight);

  Window win = XCreateWindow(Os->Display, RootWindow,
      0, 0,
      Plat->WindowWidth, Plat->WindowHeight,
      0, VisualInfo->depth, InputOutput, VisualInfo->visual,
      CWColormap | CWEventMask, &WindowAttribs);

  if (!win) { Error("Unable to Create Window"); return False; }

  XMapWindow(Os->Display, win);
  XStoreName(Os->Display, win, "Bonsai");

  const s32 FramebufferAttributes[] = {None};

  s32 ValidConfigCount = 0;
  GLXFBConfig* ValidConfigs =
    glXChooseFBConfig( Os->Display,  VisualInfo->screen, FramebufferAttributes, &ValidConfigCount);

  Assert(ValidConfigCount);
  GLXFBConfig FramebufferConfig = ValidConfigs[0];

  const s32 OpenGlContextAttribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
    GLX_CONTEXT_FLAGS_ARB, DebugFlags,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

  GLXContext ShareContext = 0;
  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
    (PFNGLXCREATECONTEXTATTRIBSARBPROC)bonsaiGlGetProcAddress("glXCreateContextAttribsARB");
  Assert(glXCreateContextAttribsARB);

  Os->GlContext = glXCreateContextAttribsARB(Os->Display, FramebufferConfig, ShareContext, GL_TRUE, OpenGlContextAttribs);
  if (!Os->GlContext) { Error("Unable to Create GLXContext"); return False; }

  Assert(Os->Display != None && Os->GlContext);

  glXMakeCurrent(Os->Display, win, Os->GlContext);

  glDebugMessageCallback(HandleGlDebugMessage, 0);

  Os->Window = win;
  return True;
}

inline GameCallback
GetProcFromLib(shared_lib Lib, const char *Name)
{
  GameCallback Result = (GameCallback)dlsym(Lib, Name);
  return Result;
}

inline void
Terminate(os *Os)
{
  XDestroyWindow(Os->Display, Os->Window);
  XCloseDisplay(Os->Display);
  return;
}

b32
ProcessOsMessages(os *Os, platform *Plat)
{
  TIMED_FUNCTION();

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

      case ConfigureNotify:
      {
        Plat->WindowWidth = Event.xconfigure.width;
        Plat->WindowHeight = Event.xconfigure.height;
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
          Plat->Input.LMB.WasPressed = True;
          Plat->Input.LMB.IsDown = True;
        }
        if (Event.xbutton.button == Button2)
        {
          Plat->Input.MMB.WasPressed = True;
          Plat->Input.MMB.IsDown = True;
        }
        if (Event.xbutton.button == Button3)
        {
          Plat->Input.RMB.WasPressed = True;
          Plat->Input.RMB.IsDown = True;
        }
      } break;

      case ButtonRelease:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB.WasPressed = False;
          Plat->Input.LMB.IsDown = False;
        }
        if (Event.xbutton.button == Button2)
        {
          Plat->Input.MMB.WasPressed = False;
          Plat->Input.MMB.IsDown = False;
        }
        if (Event.xbutton.button == Button3)
        {
          Plat->Input.RMB.WasPressed = False;
          Plat->Input.RMB.IsDown = False;
        }
      } break;


#define BindToInput(Keysym, InputField, Boolean) \
  case Keysym: {                                 \
    Plat->Input.InputField.WasPressed = Boolean; \
    Plat->Input.InputField.IsDown = Boolean;     \
  } break;

#define BindKeydownToInput(Keysym, InputField) \
    BindToInput(Keysym, InputField, True)

#define BindKeyupToInput(Keysym, InputField) \
    BindToInput(Keysym, InputField, False)


      case KeyRelease:
      {
        u64 KeySym = XLookupKeysym(&Event.xkey, 0);
        switch (KeySym)
        {

          BindKeyupToInput(XK_w     , W)
          BindKeyupToInput(XK_s     , S)
          BindKeyupToInput(XK_a     , A);
          BindKeyupToInput(XK_d     , D);
          BindKeyupToInput(XK_q     , Q);
          BindKeyupToInput(XK_e     , E);

          BindKeyupToInput(XK_F1    , F1);
          BindKeyupToInput(XK_F2    , F2);
          BindKeyupToInput(XK_F3    , F3);
          BindKeyupToInput(XK_F4    , F4);
          BindKeyupToInput(XK_F5    , F5);
          BindKeyupToInput(XK_F6    , F6);
          BindKeyupToInput(XK_F7    , F7);
          BindKeyupToInput(XK_F8    , F8);
          BindKeyupToInput(XK_F9    , F9);
          BindKeyupToInput(XK_F10   , F10);
          BindKeyupToInput(XK_F11   , F11);
          BindKeyupToInput(XK_F12   , F12);

          BindKeyupToInput(XK_space , Space);

          default:
          {
          } break;
        }
      } break;

      case KeyPress:
      {
        u64 KeySym = XLookupKeysym(&Event.xkey, 0);
        switch (KeySym)
        {

          BindKeydownToInput(XK_w     , W)
          BindKeydownToInput(XK_s     , S)
          BindKeydownToInput(XK_a     , A);
          BindKeydownToInput(XK_d     , D);
          BindKeydownToInput(XK_q     , Q);
          BindKeydownToInput(XK_e     , E);

          BindKeydownToInput(XK_F1    , F1);
          BindKeydownToInput(XK_F2    , F2);
          BindKeydownToInput(XK_F3    , F3);
          BindKeydownToInput(XK_F4    , F4);
          BindKeydownToInput(XK_F5    , F5);
          BindKeydownToInput(XK_F6    , F6);
          BindKeydownToInput(XK_F7    , F7);
          BindKeydownToInput(XK_F8    , F8);
          BindKeydownToInput(XK_F9    , F9);
          BindKeydownToInput(XK_F10   , F10);
          BindKeydownToInput(XK_F11   , F11);
          BindKeydownToInput(XK_F12   , F12);

          BindKeydownToInput(XK_space , Space);

          case XK_Escape:
          {
            Os->ContinueRunning = False;
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

#endif // PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS

char*
GetCwd()
{
  // @memory-leak
  // FIXME(Jesse): get_current_dir_name mallocs interally .. do we care?
  global_variable char *Result = get_current_dir_name();
  return (Result);
}

b32
IsFilesystemRoot(char *Filepath)
{
  b32 Result = ( Filepath[0] == '/' && Filepath[1] == 0 );
  return Result;
}

#include <time.h>

inline r64
GetHighPrecisionClock()
{
  timespec Time;
  clock_gettime(CLOCK_MONOTONIC, &Time);

  r64 SecondsAsMs = (r64)Time.tv_sec*1000.0;
  r64 NsAsMs = Time.tv_nsec/1000000;

  r64 Ms = SecondsAsMs + NsAsMs;
  return Ms;
}

#if PLATFORM_GL_IMPLEMENTATIONS

inline void
BonsaiSwapBuffers(os *Os)
{
  TIMED_FUNCTION();
  glXSwapBuffers(Os->Display, Os->Window);
}

#endif // PLATFORM_GL_IMPLEMENTATIONS

inline void
ConnectToServer(network_connection *Connection)
{
  if (!Connection->Socket.Id)
  {
    Connection->Socket = CreateSocket(Socket_NonBlocking);
  }

  s32 ConnectStatus = connect(Connection->Socket.Id,
                              (sockaddr *)&Connection->Address,
                              sizeof(sockaddr_in));

  if (ConnectStatus == 0)
  {
      Debug("Connected");
      Connection->State = ConnectionState_AwaitingHandshake;
  }
  else if (ConnectStatus == -1)
  {
    switch (errno)
    {
      case 0:
      {
      } break;

      case EINPROGRESS:
      case EALREADY:
      {
        // Connection in progress
      } break;

      case ECONNREFUSED:
      {
        // Host is down
      } break;

      case EISCONN:
      {
        // Not sure if we should ever call connect on an already-connected connection
        Assert(False);
      } break;

      default :
      {
        Error("Connecting to remote host encountered an unexpected error : %s", strerror(errno));
        Assert(False);
      } break;

    }
  }
  else
  {
    InvalidCodePath();
  }

  return;
}

inline void
PlatformSetThreadPriority(s32 Priority)
{
  sched_param Param = {};
  Param.sched_priority = Priority;
  int Error = sched_setscheduler(0, SCHED_FIFO, &Param);
  if (Error)
  {
    Error("Setting Scheduler for main thread");

    switch (errno)
    {
      case EINVAL:
      {
        Error(" Invalid arguments: pid is negative or param is NULL.");
        Error(" (sched_setscheduler()) policy is not one of the recognized policies.");
        Error(" (sched_setscheduler()) param does not make sense for the specified policy.");
      }  break;

      case EPERM:  { Error("  The calling thread does not have appropriate privileges."); break; }
      case ESRCH:  { Error("  The thread whose ID is pid could not be found."); break; }
      InvalidDefaultCase;
    }
  }

  return;
}

// It seemed to me doing this actually made performance _worse_
#if 0
inline void
PlatformSetMainThreadPriority()
{
  cpu_set_t Cpu;
  CPU_ZERO(&Cpu);
  CPU_SET(0, &Cpu);

/*   int SetSuccessful = pthread_setaffinity_np(pthread_self(), sizeof(Cpu), &Cpu); */
/*   if (SetSuccessful == -1) */
/*   { */
/*     Error("Setting CPU affinity"); */
/*   } */

  /* PlatformSetThreadPriority(99); */
  return;
}

inline void
PlatformSetWorkerThreadPriority()
{
  cpu_set_t Cpu;
  CPU_ZERO(&Cpu);
  CPU_SET(1, &Cpu);

  /* int SetSuccessful = pthread_setaffinity_np(pthread_self(), sizeof(Cpu), &Cpu); */
  /* if (SetSuccessful == -1) */
  /* { */
  /*   Error("Setting CPU affinity"); */
  /* } */

  /* PlatformSetThreadPriority(90); */
  return;
}
#endif

inline void
RewindArena(memory_arena *Arena, umm RestartBlockSize = Megabytes(1) )
{
  if (Arena->Prev)
  {
    PlatformUnprotectArena(Arena->Prev);
    VaporizeArena(Arena->Prev);
    Arena->Prev = 0;
  }

  PlatformUnprotectArena(Arena);

  Arena->At = Arena->Start;
  Arena->NextBlockSize = RestartBlockSize;

#if BONSAI_INTERNAL
#ifndef BONSAI_NO_PUSH_METADATA
  Arena->Pushes = 0;
  DEBUG_CLEAR_META_RECORDS_FOR(Arena);
#endif
#endif

  return;
}

