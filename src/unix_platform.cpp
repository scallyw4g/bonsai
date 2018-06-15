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


debug_global __thread u64 ThreadLocal_ThreadIndex = 0;

inline b32
AtomicCompareExchange( volatile char **Source, const char *Exchange, const char *Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline bool
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

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
  pthread_mutex_init(Mutex, NULL);
  return;
}

void
PlatformUnlockMutex(mutex *Mutex)
{
  s32 Fail = pthread_mutex_unlock(Mutex);

  if (Fail)
  {
    Error("Failed to un-lock mutex");
    Assert(False);
  }

  return;
}

void
PlatformLockMutex(mutex *Mutex)
{
  s32 Fail = pthread_mutex_lock(Mutex);

  if (Fail)
  {
    Error("Failed to aquire lock");
    Assert(False);
  }

  return;
}

// TODO(Jesse): Have these initialize a static so we don't eat a syscall every time
u64
PlatformGetPageSize()
{
  // time this gets called
  u64 InvalidSysconfReturn = ((u64)-1);
  u64 PageSize = (u64)sysconf(_SC_PAGESIZE);
  Assert(PageSize != InvalidSysconfReturn);
  return PageSize;
}

s32
GetLogicalCoreCount()
{
  s32 Result = (s32)sysconf(_SC_NPROCESSORS_ONLN);
  return Result;
}
// TODO(Jesse): Have these initialize a static so we don't eat a syscall every time

u32
GetWorkerThreadCount()
{
  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1 + DEBUG_THREAD_COUNT_BIAS; // -1 because we already have a main thread
  return ThreadCount;
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
PlatformAllocateArena(umm RequestedBytes = Megabytes(1), b32 MemProtect = True)
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
  Result->MemProtect = MemProtect;

#if MEMPROTECT_OVERFLOW
  if (Result->MemProtect)
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
  sem_wait(Semaphore);
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

  Assert(Plat->WindowWidth && Plat->WindowHeight);

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
  global_variable char GlobalCwdBuffer[GlobalCwdBufferLength];
  getcwd(GlobalCwdBuffer, GlobalCwdBufferLength);
  return (GlobalCwdBuffer);
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

