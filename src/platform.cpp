#define BONSAI_NO_PUSH_METADATA
#define DEFAULT_GAME_LIB "./bin/WorldGenLoadable.so"
#define DEFAULT_DEBUG_LIB "./bin/libDebugSystem.so"

#include <bonsai_types.h>
#include <heap_memory_types.cpp>

#if BONSAI_WIN32
#include <win32_platform.cpp>
#else
#define PLATFORM_THREADING_IMPLEMENTATIONS 1
#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#include <unix_platform.cpp>
#endif


global_variable s64 LastGameLibTime;
global_variable bonsai_worker_thread_callback BONSAI_API_WORKER_THREAD_CALLBACK_NAME;

#include <work_queue.cpp>


#include <sys/stat.h>
function b32
LibIsNew(const char *LibPath, s64 *LastLibTime)
{
  b32 Result = False;
  struct stat StatStruct;

  if (stat(LibPath, &StatStruct) == 0)
  {
    if (StatStruct.st_mtime > *LastLibTime)
    {
      *LastLibTime = (u64)StatStruct.st_mtime;
      Result = True;
    }
  }
  else
  {
    Error("Stat-ing Game library :( ");
  }

  return Result;
}

function thread_local_state
DefaultThreadLocalState()
{
  thread_local_state Thread = {};

  Thread.TempMemory = PlatformAllocateArena();
  Thread.PermMemory = PlatformAllocateArena();

  // NOTE(Jesse): As it stands the debug system doesn't do any locking when
  // constructing the debug arena stats, so we can't ever free memory allocated
  // on debug registered arenas on threads outside the main one.
  //
  /* DEBUG_REGISTER_ARENA(Thread.TempMemory); */

  DEBUG_REGISTER_ARENA(Thread.PermMemory);

  return Thread;
}

function void
DrainQueue(work_queue* Queue, thread_local_state* Thread)
{
  for (;;)
  {
    u32 DequeueIndex = Queue->DequeueIndex;
    if ( DequeueIndex == Queue->EnqueueIndex)
    {
      break;
    }
    else
    {
      b32 Exchanged = AtomicCompareExchange(&Queue->DequeueIndex,
                                        (DequeueIndex+1)% WORK_QUEUE_SIZE,
                                        DequeueIndex);
      if ( Exchanged )
      {
        work_queue_entry* Entry = Queue->Entries + DequeueIndex;
        BONSAI_API_WORKER_THREAD_CALLBACK_NAME(Entry, Thread);
        Entry->Type = WorkEntryType_None;
      }
    }
  }
}

function THREAD_MAIN_RETURN
ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;
  DEBUG_REGISTER_THREAD(ThreadParams->Self.ThreadIndex);

  thread_local_state Thread = DefaultThreadLocalState();
  if (ThreadParams->InitProc) { ThreadParams->InitProc(&Thread, ThreadParams->GameState); }

  for (;;)
  {
    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

    if (MainThreadBlocksWorkerThreads)
    {
      AtomicIncrement(&WorkerThreadsWaiting);
      while(MainThreadBlocksWorkerThreads);
      AtomicDecrement(&WorkerThreadsWaiting);
    }

    // This is a pointer to a single semaphore for all queues, so only sleeping
    // on one is sufficient, and equal to sleeping on all, because they all
    // point to the same semaphore
    ThreadSleep( ThreadParams->HighPriority->GlobalQueueSemaphore );

    DrainQueue(ThreadParams->HighPriority, &Thread);

    work_queue* LowPriority = ThreadParams->LowPriority;
    for (;;)
    {
      u32 DequeueIndex = LowPriority->DequeueIndex;
      if ( DequeueIndex == LowPriority->EnqueueIndex ||
           !QueueIsEmpty(ThreadParams->HighPriority) )
      {
        break;
      }
      else
      {
        b32 Exchanged = AtomicCompareExchange(&LowPriority->DequeueIndex,
                                          (DequeueIndex+1)% WORK_QUEUE_SIZE,
                                          DequeueIndex);
        if ( Exchanged )
        {
          work_queue_entry* Entry = LowPriority->Entries + DequeueIndex;
          BONSAI_API_WORKER_THREAD_CALLBACK_NAME(Entry, &Thread);
          Entry->Type = WorkEntryType_None;
        }
      }
    }

    PlatformUnprotectArena(Thread.TempMemory);
    RewindArena(Thread.TempMemory);
  }
}

function b32
StrMatch(char *Str1, char *Str2)
{
  char *Haystack = Str1;
  char *Needle = Str2;

  b32 Matched = True;
  while( Matched && *Haystack && *Needle )
  {
    Matched = *(Haystack++) == *(Needle++);
  }

  b32 Result = (*Needle == 0 && Matched);
  return Result;
}

function b32
StrStr(char *Str1, char *Str2)
{
  b32 Result = StrMatch(Str1, Str2);

  while(*Str1++)
  {
    Result |= StrMatch(Str1, Str2);
  }

  return Result;
}

#define DefGlProc(ProcType, ProcName) \
  ProcType ProcName = (ProcType)bonsaiGlGetProcAddress(#ProcName); Assert(ProcName)
function void
InitializeOpenGlExtensions(os *Os)
{
  Info("Initializing OpenGL Extensions");

#if 0
  const char* glxExtensionString = glXQueryExtensionsString(Os->Display, DefaultScreen(Os->Display));
  const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);
  Debug(glExtensionString);
  Debug(glxExtensionString);
#endif

  // Somehow on linux this is irrelevant..
#if 1

  /*
   * 1.3
   */
  DefGlProc(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
  DefGlProc(PFNGLACTIVETEXTUREPROC, glActiveTexture);

  /*
   * 1.5
   */
  DefGlProc(PFNGLGENBUFFERSPROC, glGenBuffers);
  DefGlProc(PFNGLBINDBUFFERPROC, glBindBuffer);
  DefGlProc(PFNGLBUFFERDATAPROC, glBufferData);
  DefGlProc(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);

  /*
   * 2.0
   */
  DefGlProc(PFNGLCREATESHADERPROC, glCreateShader);
  DefGlProc(PFNGLSHADERSOURCEPROC, glShaderSource);
  DefGlProc(PFNGLCOMPILESHADERPROC, glCompileShader);
  DefGlProc(PFNGLGETSHADERIVPROC, glGetShaderiv);
  DefGlProc(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
  DefGlProc(PFNGLATTACHSHADERPROC, glAttachShader);
  DefGlProc(PFNGLDETACHSHADERPROC, glDetachShader);
  DefGlProc(PFNGLDELETESHADERPROC, glDeleteShader);
  DefGlProc(PFNGLCREATEPROGRAMPROC, glCreateProgram);
  DefGlProc(PFNGLLINKPROGRAMPROC, glLinkProgram);
  DefGlProc(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
  DefGlProc(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
  DefGlProc(PFNGLUSEPROGRAMPROC, glUseProgram);
  DefGlProc(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
  DefGlProc(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
  DefGlProc(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
  DefGlProc(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
  DefGlProc(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
  DefGlProc(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
  DefGlProc(PFNGLUNIFORM3FVPROC, glUniform3fv);
  DefGlProc(PFNGLUNIFORM2FVPROC, glUniform2fv);
  DefGlProc(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  DefGlProc(PFNGLUNIFORM1IPROC, glUniform1i);
  DefGlProc(PFNGLUNIFORM1FPROC, glUniform1f);
  DefGlProc(PFNGLUNIFORM1UIPROC, glUniform1ui);

  /*
   * 3.0
   */
  DefGlProc(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
  DefGlProc(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
  DefGlProc(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
  DefGlProc(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
  DefGlProc(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
  DefGlProc(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);


  /*
   * 3.2
   */
  // My laptop is running 3.1ES, but this worked with GLEW, so presumably
  // it can be loaded somehow..  Maybe the _EXT or _ARB one?
  // DefGlProc(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture);

  /*
   * 4.3
   */
  DefGlProc(PFNGLXCREATECONTEXTATTRIBSARBPROC, glXCreateContextAttribsARB);



#endif

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  AssertNoGlErrors;

  // Platform specific (wgl / glX)
  u32 VsyncFrames = 1;
#if LINUX
  // TODO(Jesse): Not getting vsync on my arch laptop...
  PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("glXSwapIntervalEXT");
  if ( glSwapInterval )
  {
    glSwapInterval(Os->Display, Os->Window, VsyncFrames);
  }
  else
  {
    Info("No Vsync");
  }
#elif WIN32
  PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("wglSwapIntervalEXT");
  Assert( glSwapInterval );
  glSwapInterval(VsyncFrames);
#endif

  return;
}

function void
PlatformLaunchWorkerThreads(platform *Plat, bonsai_worker_thread_init_callback WorkerThreadInit, game_state* GameState)
{
  u32 WorkerThreadCount = GetWorkerThreadCount();

  for (u32 ThreadIndex = 0;
      ThreadIndex < WorkerThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Plat->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex + 1;
    Params->HighPriority = &Plat->HighPriority;
    Params->LowPriority = &Plat->LowPriority;
    Params->InitProc = WorkerThreadInit;
    Params->GameState = GameState;

    CreateThread( ThreadMain, Params );
  }

  return;
}

function void
InitQueue(work_queue* Queue, memory_arena* Memory, semaphore* Semaphore)
{
  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;

  Queue->Entries = Allocate(work_queue_entry, Memory, WORK_QUEUE_SIZE);
  Queue->GlobalQueueSemaphore = Semaphore;

  return;
}

function void
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->Memory = Memory;

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 WorkerThreadCount = GetWorkerThreadCount();
  Info("Detected %d Logical cores, creating %d threads", LogicalCoreCount, WorkerThreadCount);

  Plat->QueueSemaphore = CreateSemaphore();

  InitQueue(&Plat->LowPriority, Plat->Memory, &Plat->QueueSemaphore);
  InitQueue(&Plat->HighPriority, Plat->Memory, &Plat->QueueSemaphore);

  Plat->Threads = Allocate(thread_startup_params,  Plat->Memory, WorkerThreadCount);

  return;
}

#if 0
/*
 *  Poor mans vsync
 */
function void
WaitForFrameTime(r64 FrameStartMs, float FPS)
{
  TIMED_FUNCTION();
  r64 frameTarget = (1.0/(r64)FPS)*1000.0f;
  r64 FrameTime = GetHighPrecisionClock() - FrameStartMs;

  while (FrameTime < frameTarget)
  {
    FrameTime = GetHighPrecisionClock() - FrameStartMs;
  }

  return;
}
#endif

function inline b32
FileExists(const char *Path)
{
  b32 Result = False;

  if (FILE *file = fopen(Path, "r"))
  {
    fclose(file);
    Result = True;
  }

  return Result;
}

function b32
SearchForProjectRoot(void)
{
  b32 Result = False;

  if (FileExists(".root_marker"))
  {
    Result = True;
  }
  else
  {
    b32 ChdirSuceeded = (chdir("..") == 0);
    b32 NotAtFilesystemRoot = (!IsFilesystemRoot(GetCwd()));

    if (ChdirSuceeded && NotAtFilesystemRoot)
      Result = SearchForProjectRoot();

    else
      Result = False;

  }

  return Result;
}

function b32
CheckShadingLanguageVersion()
{
  char *OpenGlVersion = (char*)glGetString ( GL_VERSION );
  r32 ShadingLanguageVersion = (r32)atof((char*)glGetString ( GL_SHADING_LANGUAGE_VERSION ));

  Info("OpenGl Verison : %s", OpenGlVersion );
  Info("Shading Language Verison : %f", ShadingLanguageVersion );

  r32 RequiredShadingLanguageVersion = 3.3f;
  if (ShadingLanguageVersion < RequiredShadingLanguageVersion)
  {
    Error("Unsupported Version of GLSL :: Got %f, Needed: %f", ShadingLanguageVersion, RequiredShadingLanguageVersion);
  }

  b32 Result = (ShadingLanguageVersion > RequiredShadingLanguageVersion);
  return Result;
}

function void
ClearWasPressedFlags(input_event *Input)
{
  TIMED_FUNCTION();

  u32 TotalEvents = sizeof(input)/sizeof(input_event);

  for ( u32 EventIndex = 0;
        EventIndex < TotalEvents;
        ++EventIndex)
  {
    input_event *Event = Input + EventIndex;

    // This is some super-janky insurance that we're overwriting boolean values
    Assert(Event->IsDown == False || Event->IsDown == True);
    Assert(Event->WasPressed == False || Event->WasPressed == True);

    Event->WasPressed = False;
  }
}

function void
BindHotkeysToInput(hotkeys *Hotkeys, input *Input)
{

#if BONSAI_INTERNAL
  Hotkeys->Debug_Pause                    = Input->F12.IsDown;
  Hotkeys->Debug_ToggleLoopedGamePlayback = Input->F11.WasPressed;

  if (Input->F10.WasPressed)
  {
    Hotkeys->Debug_ToggleProfile = True;
  }

  if (Input->F1.WasPressed)
  {
    Hotkeys->Debug_NextUiState = True;
  }

  if (Input->F2.WasPressed)
  {
    Hotkeys->Debug_ToggleTriggeredRuntimeBreak = True;
  }

  /* Hotkeys->Debug_RedrawEveryPush = Input->F2.WasPressed; */
#endif

  Hotkeys->Left = Input->A.IsDown;
  Hotkeys->Right = Input->D.IsDown;
  Hotkeys->Forward = Input->W.IsDown;
  Hotkeys->Backward = Input->S.IsDown;

  Hotkeys->Player_Fire = Input->Space.WasPressed;
  Hotkeys->Player_Proton = Input->Shift.WasPressed;

  Hotkeys->Player_Spawn = Input->Space.WasPressed;

  return;
}

function server_state*
ServerInit(memory_arena* Memory)
{
  server_state* ServerState = Allocate(server_state, Memory, 1);
  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    ServerState->Clients[ClientIndex].Id = -1;
  }

  return ServerState;
}

s32
main()
{
  Info("Initializing Bonsai");

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return False; }
  Info("Found Bonsai Root : %s", GetCwd() );

  platform Plat = {};

  os Os = {};

#if BONSAI_INTERNAL
  s32 DebugFlags = GLX_CONTEXT_DEBUG_BIT_ARB;
#else
  s32 DebugFlags = 0;
#endif

  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, DebugFlags);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }

  Assert(Os.Window);

  AssertNoGlErrors;

  InitializeOpenGlExtensions(&Os);

  b32 ShadingLanguageIsRecentEnough = CheckShadingLanguageVersion();
  if (!ShadingLanguageIsRecentEnough) {  return False; }

  AssertNoGlErrors;


  memory_arena *PlatMemory = PlatformAllocateArena();
  memory_arena *GameMemory = PlatformAllocateArena();

  DEBUG_REGISTER_ARENA(GameMemory);
  DEBUG_REGISTER_ARENA(PlatMemory);

  PlatformInit(&Plat, PlatMemory);

#if BONSAI_INTERNAL
  /* debug_recording_state *Debug_RecordingState = Allocate(debug_recording_state, GameMemory, 1); */
  /* AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3)); */
#endif

  hotkeys Hotkeys = {};

  LibIsNew(DEFAULT_GAME_LIB, &LastGameLibTime);  // Hack to initialize the LastGameLibTime static

  shared_lib GameLib = OpenLibrary(DEFAULT_GAME_LIB);
  if (!GameLib) { Error("Loading GameLib :( "); return False; }

  bonsai_main_thread_init_callback GameInit = (bonsai_main_thread_init_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  if (!GameInit) { Error("Retreiving " TO_STRING(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME) " from Game Lib :( "); return False; }

  bonsai_main_thread_callback BONSAI_API_MAIN_THREAD_CALLBACK_NAME = (bonsai_main_thread_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  if (!BONSAI_API_MAIN_THREAD_CALLBACK_NAME) { Error("Retreiving " TO_STRING(BONSAI_API_MAIN_THREAD_CALLBACK_NAME) " from Game Lib :( "); return False; }

  BONSAI_API_WORKER_THREAD_CALLBACK_NAME = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );
  if (!BONSAI_API_WORKER_THREAD_CALLBACK_NAME) { Error("Retreiving " TO_STRING(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) " from Game Lib :( "); return False; }

  bonsai_worker_thread_init_callback WorkerThreadInitCallback = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));

  game_state* GameState = GameInit(&Plat, GameMemory, GetDebugState);
  if (!GameState) { Error("Initializing Game State :( "); return False; }

  server_state* ServerState = ServerInit(GameMemory);
  Assert(ServerState);

  PlatformLaunchWorkerThreads(&Plat, WorkerThreadInitCallback, GameState);

  /*
   *  Main Game loop
   */


  r32 RealDt = 0;
  while ( Os.ContinueRunning )
  {
    ClearWasPressedFlags((input_event*)&Plat.Input);
    DEBUG_FRAME_BEGIN(&Hotkeys);

    Plat.dt = RealDt;
    if (Plat.dt > 0.1f)
    {
      Warn("DT exceeded 100ms, truncating.");
      Plat.dt = 0.1f;
    }

    TIMED_BLOCK("Frame Preamble");

    v2 LastMouseP = Plat.MouseP;
    while ( ProcessOsMessages(&Os, &Plat) );
    Plat.MouseDP = LastMouseP - Plat.MouseP;

    BindHotkeysToInput(&Hotkeys, &Plat.Input);

    if ( LibIsNew(DEFAULT_GAME_LIB, &LastGameLibTime) )
    {
      SuspendWorkerThreads();

      CloseLibrary(GameLib);
      GameLib = OpenLibrary(DEFAULT_GAME_LIB);

      BONSAI_API_MAIN_THREAD_CALLBACK_NAME = (bonsai_main_thread_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
      BONSAI_API_WORKER_THREAD_CALLBACK_NAME = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, TO_STRING(BONSAI_API_WORKER_THREAD_CALLBACK_NAME));

      ResumeWorkerThreads();
    }

    // TODO(Jesse): Doing this properly requires some extra-credit work first.
    //
    // 1) We need the game code to be able to call GetDebugState() willy-nilly,
    // so when the debug lib gets refreshed it has to be passed to the game
    // code somehow.
    //
    // Possibly allow the game code to export the function pointer it uses for
    // GetDebugState, then we can just overwrite it from here..?  Or have the
    // game code store a pointer to the actual debug_state that we can
    // overwrite.  That kinda seems like a step backwards though.
    //
    // 2) Reloading the debug lib as it stands will wipe out any data in the
    // debug_state, so if that wants to be preserved it's gotta get copied
    // somewhere, or live outside the debug lib.  Might make sense to have the
    // platform layer allocate and pass the debug_state around and the
    // debug_lib is just a pile of functions to operate on it ..?

#if 0
    local_persist s64 LastDebugLibTime;
    if ( LibIsNew(DEFAULT_DEBUG_LIB, &LastDebugLibTime) )
    {
      CloseLibrary(DebugLib);
      DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
      if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

      GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
      if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }
    }
#endif

    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    TIMED_BLOCK("Network Ops");
      if (IsDisconnected(&Plat.Network)) { ConnectToServer(&Plat.Network); }
    END_BLOCK("Network Ops");

    BONSAI_API_MAIN_THREAD_CALLBACK_NAME(&Plat, GameState, &Hotkeys);

    DEBUG_FRAME_END(&Plat, ServerState);

    BonsaiSwapBuffers(&Os);
    RealDt = MAIN_THREAD_ADVANCE_DEBUG_SYSTEM();
  }

  Info("Shutting Down");
  Terminate(&Os);
  Info("Exiting");

  return True;
}
