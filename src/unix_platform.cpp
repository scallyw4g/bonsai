#include <stdio.h>

// network layer
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr

// mmap
#include <sys/mman.h>
#include <sys/time.h>

#include <platform.h>

#include <GL/glx.h>
#include <X11/keysymdef.h>

#include <net/server.h>

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

u64 InvalidSysconfReturn = ((u64)-1);

u64
PlatformGetPageSize()
{
  u64 PageSize = sysconf(_SC_PAGESIZE);
  Assert(PageSize != InvalidSysconfReturn);
  return PageSize;
}

u8*
PlatformProtectPage(u8* Mem)
{
  u64 PageSize = PlatformGetPageSize();

  Assert((s64)Mem % PageSize == 0);

  mprotect(Mem, PageSize, PROT_NONE);
  u8* Result = Mem + PageSize;
  return Result;
}

memory_arena*
PlatformAllocateArena(umm RequestedBytes = Megabytes(1))
{
  u64 PageSize = PlatformGetPageSize();
  u64 BytePagePad = PageSize - (RequestedBytes % PageSize);
  umm AllocationSize = RequestedBytes + BytePagePad;
  Assert(AllocationSize % PageSize == 0);

  u8 *Bytes = (u8*)mmap(0, AllocationSize, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  if (Bytes == MAP_FAILED)
  {
    Bytes = 0;
    s32 Error = errno;
    if (Error == ENOMEM)
    {
      Assert(!"Out of memory, or something..");
    }
    else
    {
      Assert(!"Unknown error allocating virtual memory!");
    }
  }

  memory_arena *NewArena = (memory_arena*)Bytes;

  NewArena->FirstFreeByte = (u8*)(Bytes + PageSize);
  NewArena->Remaining = AllocationSize - sizeof(memory_arena);
  NewArena->TotalSize = AllocationSize;
  NewArena->NextBlockSize = AllocationSize * 2;
#if MEMPROTECT
  NewArena->MemProtect = True;
#endif

  Assert((umm)NewArena->FirstFreeByte % PageSize == 0);

  return NewArena;
}

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

__inline__ u64
GetCycleCount()
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  u64 Result = ( (u64)lo)|( ((u64)hi)<<32 );
  return Result;
}

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
      } break;

      case ButtonRelease:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB.WasPressed = False;
          Plat->Input.LMB.IsDown = False;
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
        int KeySym = XLookupKeysym(&Event.xkey, 0);
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
        int KeySym = XLookupKeysym(&Event.xkey, 0);
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

inline void
BonsaiSwapBuffers(os *Os)
{
  TIMED_FUNCTION();
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

inline void
ConnectToServer(network_connection *Connection, server *Server)
{
  if (!Connection->Socket) 
  {
    Connection->Socket = CreateSocket();
  }

  s32 ConnectStatus = connect(Connection->Socket,
                              (sockaddr *)&Server->Address,
                              sizeof(sockaddr_in));

  if (ConnectStatus == 0)
  {
    Debug("Connected");
    Connection->Connected = True;
  }
  else
  {
    Error("Connecting to remote host failed : %s", strerror(errno));
  }

  return;
}

inline void
PingServer(network_connection *Connection)
{
  server_message Message = {};

  Send(Connection, &Message);
  Read(Connection, &Message);

  return;
}
