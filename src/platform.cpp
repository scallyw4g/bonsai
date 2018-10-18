#define BONSAI_NO_PUSH_METADATA
#define DEFAULT_GAME_LIB "./bin/AnimationTestLoadable.so"

#include <bonsai_types.h>

#if BONSAI_WIN32
#include <win32_platform.cpp>
#else
#define PLATFORM_THREADING_IMPLEMENTATIONS 1
#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#include <unix_platform.cpp>
#endif

global_variable s64 LastGameLibTime;
global_variable game_thread_callback_proc GameThreadCallback;



#include <sys/stat.h>
b32
GameLibIsNew(const char *LibPath)
{
  b32 Result = False;
  struct stat StatStruct;

  if (stat(LibPath, &StatStruct) == 0)
  {
    if (StatStruct.st_mtime > LastGameLibTime)
    {
      LastGameLibTime = (u64)StatStruct.st_mtime;
      Result = True;
    }
  }
  else
  {
    Error("Stat-ing Game library :( ");
  }

  return Result;
}

THREAD_MAIN_RETURN
ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;
  DEBUG_REGISTER_THREAD(ThreadParams->Self.ThreadIndex);

  work_queue *Queue = ThreadParams->Queue;

  memory_arena *ThreadArena = PlatformAllocateArena();
  DEBUG_REGISTER_ARENA(ThreadArena);

  for (;;)
  {
    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

    ThreadSleep( &Queue->Semaphore );

    b32 Exchanged = False;
    while (!Exchanged)
    {
      u32 DequeueIndex = Queue->DequeueIndex;
      Exchanged = AtomicCompareExchange(&Queue->DequeueIndex,
                                        (DequeueIndex+1)%WORK_QUEUE_SIZE,
                                        DequeueIndex);
      if ( Exchanged )
      {
        work_queue_entry Entry = Queue->Entries[DequeueIndex];
        GameThreadCallback(&Entry, ThreadArena);
      }
    }

    PlatformUnprotectArena(ThreadArena);
    RewindArena(ThreadArena);
  }
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EnqueueIndex] = *Entry;

  CompleteAllWrites;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( &Queue->Semaphore );

  return;
}

b32
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

b32
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
void
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

b32
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->Memory = Memory;

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 WorkerThreadCount = GetWorkerThreadCount();
  Info("Detected %d Logical cores, creating %d threads", LogicalCoreCount, WorkerThreadCount);

  Plat->Queue.EnqueueIndex = 0;
  Plat->Queue.DequeueIndex = 0;

  Plat->Queue.Entries = Allocate(work_queue_entry,  Plat->Memory, WORK_QUEUE_SIZE);
  Plat->Threads = Allocate(thread_startup_params,  Plat->Memory, WorkerThreadCount);

  work_queue *Queue = &Plat->Queue;
  Queue->Semaphore = CreateSemaphore();

  for (u32 ThreadIndex = 0;
      ThreadIndex < WorkerThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Plat->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex + 1;
    Params->Queue = Queue;

    CreateThread( ThreadMain, Params );
  }

  return True;
}

/*
 *  Poor mans vsync
 */
void
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

inline b32
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

b32
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

void
QueryAndSetGlslVersion(platform *Plat)
{
  char *GL_Version = (char*)glGetString ( GL_VERSION );
  Info("GL verison : %s", GL_Version );

  r64 GLSL_Version = atof((char*)glGetString ( GL_SHADING_LANGUAGE_VERSION ));
  Info("GLSL verison : %f", GLSL_Version );


  if (GLSL_Version >= 3.3)
    Plat->GlslVersion = "330";
  else
    Plat->GlslVersion = "310ES";

  return;
}

void
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

void
BindHotkeysToInput(hotkeys *Hotkeys, input *Input)
{

#if BONSAI_INTERNAL
  Hotkeys->Debug_Pause                    = Input->F12.IsDown;
  Hotkeys->Debug_ToggleLoopedGamePlayback = Input->F11.WasPressed;

  if (Input->F10.WasPressed)
    Hotkeys->Debug_ToggleProfile = True;

  if (Input->F1.WasPressed)
    Hotkeys->Debug_NextUiState = True;

  if (Input->F2.WasPressed)
    Hotkeys->Debug_ToggleGlobalDebugBreak = True;

  Hotkeys->Debug_RedrawEveryPush = Input->F2.WasPressed;
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

server_state*
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

  shared_lib DebugLib = OpenLibrary("./bin/libDebugSystem.so");
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, DebugFlags);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }

  Assert(Os.Window);
  InitializeOpenGlExtensions(&Os);

  AssertNoGlErrors;


  memory_arena *PlatMemory     = PlatformAllocateArena();
  memory_arena *GameMemory     = PlatformAllocateArena();

  DEBUG_REGISTER_ARENA(GameMemory);
  DEBUG_REGISTER_ARENA(PlatMemory);

  PlatformInit(&Plat, PlatMemory);

#if BONSAI_INTERNAL
  /* debug_recording_state *Debug_RecordingState = Allocate(debug_recording_state, GameMemory, 1); */
  /* AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3)); */
#endif

  hotkeys Hotkeys = {};

  GameLibIsNew(DEFAULT_GAME_LIB);  // Hack to initialize the LastGameLibTime static


  shared_lib GameLib = OpenLibrary(DEFAULT_GAME_LIB);
  if (!GameLib) { Error("Loading GameLib :( "); return False; }

  game_init_proc GameInit = (game_init_proc)GetProcFromLib(GameLib, "GameInit");
  if (!GameInit) { Error("Retreiving GameInit from Game Lib :( "); return False; }

  game_main_proc GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
  if (!GameUpdateAndRender) { Error("Retreiving GameUpdateAndRender from Game Lib :( "); return False; }

  GameThreadCallback = (game_thread_callback_proc)GetProcFromLib(GameLib, "GameThreadCallback");
  if (!GameThreadCallback) { Error("Retreiving GameThreadCallback from Game Lib :( "); return False; }

  QueryAndSetGlslVersion(&Plat);

  game_state* GameState = GameInit(&Plat, GameMemory );
  if (!GameState) { Error("Initializing Game State :( "); return False; }

  server_state* ServerState = ServerInit(GameMemory);
  Assert(ServerState);


  /*
   *  Main Game loop
   */


  r32 RealDt = 0;
  while ( Os.ContinueRunning )
  {
    ClearWasPressedFlags((input_event*)&Plat.Input);
    DEBUG_FRAME_BEGIN(&Hotkeys, RealDt);

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

    if ( GameLibIsNew(DEFAULT_GAME_LIB) )
    {
      CloseLibrary(GameLib);
      GameLib = OpenLibrary(DEFAULT_GAME_LIB);

      GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
      GameThreadCallback = (game_thread_callback_proc)GetProcFromLib(GameLib, "GameThreadCallback");
    }

    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    TIMED_BLOCK("Network Ops");
      if (IsDisconnected(&Plat.Network)) { ConnectToServer(&Plat.Network); }
    END_BLOCK("Network Ops");

    GameUpdateAndRender(&Plat, GameState, &Hotkeys);

    DEBUG_FRAME_END(&Plat, ServerState);

    BonsaiSwapBuffers(&Os);
    RealDt = MAIN_THREAD_ADVANCE_DEBUG_SYSTEM();
  }

  Info("Shutting Down");
  Terminate(&Os);
  Info("Exiting");

  return True;
}
