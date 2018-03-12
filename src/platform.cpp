#ifndef BONSAI_PLATFORM_CPP
#define BONSAI_PLATFORM_CPP


#include <iostream>

#include <bonsai_types.h>

#include <bonsai.h>
#include <constants.hpp>

#if BONSAI_WIN32
#include <win32_platform.cpp>
#else
#include <unix_platform.cpp>
#endif

#include <platform.h>
#include <bonsai_math.h>

// TODO(Jesse): Axe this!!
static gl_extensions *GL_Global = 0;

debug_global platform *Global_Plat = 0;
debug_global os *Global_Os = 0;

#include <texture.cpp>
#include <shader.cpp>
#include <bonsai_vertex.h>
#include <debug.cpp>
#include <render.cpp>

#include <sys/types.h>
#include <sys/stat.h>


global_variable s64 LastGameLibTime;
global_variable game_thread_callback_proc GameThreadCallback;

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

  work_queue *Queue = ThreadParams->Queue;

  for (;;)
  {
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
        GameThreadCallback(&Entry);
      }
    }

  }

  return 0;
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

s32
Length(char *Str)
{
  s32 Result = 0;
  while (Str)
  {
    Result ++;
    Str++;
  }

  return Result;
}

#define DefGlProc(ProcType, ProcName) Gl->ProcName = (ProcType)bonsaiGlGetProcAddress(#ProcName); Assert(Gl->ProcName)
void
InitializeOpenGlExtensions(gl_extensions *Gl, os *Os)
{
  Info("Initializing OpenGL Extensions");

#if 0
  const char* glxExtensionString = glXQueryExtensionsString(Os->Display, DefaultScreen(Os->Display));
  const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);
  Debug(glExtensionString);
  Debug(glxExtensionString);
#endif

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



  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  AssertNoGlErrors;

  // Platform specific (wgl / glX)
  u32 VsyncFrames = 1;
#if LINUX
  // TODO(Jesse): Not getting vsync on my arch laptop...
  Gl->glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("glXSwapIntervalEXT");
  Assert( Gl->glSwapInterval );
  Gl->glSwapInterval(Os->Display, Os->Window, VsyncFrames);
#elif WIN32
  Gl->glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("wglSwapIntervalEXT");
  Assert( Gl->glSwapInterval );
  Gl->glSwapInterval(VsyncFrames);
#endif

  return;
}

b32
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->PushStruct = PushStruct;
  Plat->PushStructChecked_ = PushStructChecked_;
  Plat->GetCycleCount = GetCycleCount;

  // Initialized from globals
  Plat->WindowHeight = SCR_HEIGHT;
  Plat->WindowWidth = SCR_WIDTH;

  Plat->Memory = Memory;

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1 + DEBUG_THREAD_COUNT_BIAS; // -1 because we already have a main thread

  Info("Detected %d Logical cores, creating %d threads", LogicalCoreCount, ThreadCount);

  Plat->Queue.EnqueueIndex = 0;
  Plat->Queue.DequeueIndex = 0;

  Plat->Queue.Entries = PUSH_STRUCT_CHECKED(work_queue_entry,  Plat->Memory, WORK_QUEUE_SIZE);
  Plat->Threads = PUSH_STRUCT_CHECKED(thread_startup_params,  Plat->Memory, ThreadCount);

  work_queue *Queue = &Plat->Queue;

  Queue->Semaphore = CreateSemaphore();

  for (u32 ThreadIndex = 0;
      ThreadIndex < ThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Plat->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex;
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
  Hotkeys->Debug_Pause                    = Input->F12.IsDown;
  Hotkeys->Debug_ToggleLoopedGamePlayback = Input->F11.WasPressed;

  if (Input->F10.WasPressed)
    Hotkeys->Debug_ToggleProfile = True;

  if (Input->F1.WasPressed)
    Hotkeys->Debug_NextUiState = True;

  Hotkeys->Left = Input->A.IsDown;
  Hotkeys->Right = Input->D.IsDown;
  Hotkeys->Forward = Input->W.IsDown;
  Hotkeys->Backward = Input->S.IsDown;

  Hotkeys->Player_Fire = Input->Space.WasPressed;
  Hotkeys->Player_Proton = Input->Shift.WasPressed;

  Hotkeys->Player_Spawn = Input->Space.WasPressed;

  Hotkeys->Debug_RedrawEveryPush = Input->F2.WasPressed;

  return;
}

void
FrameEnd(void)
{
  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     Global_DrawCalls[DrawCountIndex] = NullDrawCall;
  }
}

s32
main(s32 NumArgs, char ** Args)
{
  Info("Initializing Bonsai");

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return False; }
  Info("Found Bonsai Root : %s", GetCwd() );

  registered_memory_arena(DebugMemory);
  registered_memory_arena(PlatMemory);
  registered_memory_arena(GraphicsMemory);
  registered_memory_arena(GameMemory);

#if BONSAI_INTERNAL
  /* debug_recording_state *Debug_RecordingState = PUSH_STRUCT_CHECKED(debug_recording_state, GameMemory, 1); */
  /* AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3)); */
#endif

  platform Plat = {};
  PlatformInit(&Plat, PlatMemory);

  os Os = {};
  Os.ContinueRunning = True;

  Global_Plat = &Plat;
  Global_Os = &Os;

  GameLibIsNew(GAME_LIB);  // Hack to initialize the LastGameLibTime static

  shared_lib GameLib = OpenLibrary(GAME_LIB);
  if (!GameLib) { Error("Loading GameLib :( "); return False; }

  game_init_proc GameInit = (game_init_proc)GetProcFromLib(GameLib, "GameInit");
  if (!GameInit) { Error("Retreiving GameInit from Game Lib :( "); return False; }

  game_main_proc GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
  if (!GameUpdateAndRender) { Error("Retreiving GameUpdateAndRender from Game Lib :( "); return False; }

  game_init_globals_proc InitGlobals = (game_init_globals_proc)GetProcFromLib(GameLib, "InitGlobals");
  if (!InitGlobals) { Error("Retreiving InitGlobals from Game Lib :( "); return False; }

  GameThreadCallback = (game_thread_callback_proc)GetProcFromLib(GameLib, "GameThreadCallback");
  if (!GameThreadCallback) { Error("Retreiving GameThreadCallback from Game Lib :( "); return False; }

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }

  Assert(Os.Window);

  InitializeOpenGlExtensions(&Plat.GL, &Os);
  GL_Global = &Plat.GL;

#if BONSAI_INTERNAL
  InitDebugState(&Plat, DebugMemory);
#endif

  InitGlobals(&Plat);

  QueryAndSetGlslVersion(&Plat);

  hotkeys Hotkeys = {};

  Plat.Graphics = GraphicsInit(GraphicsMemory);
  if (!Plat.Graphics) { Error("Initializing Graphics"); return False; }

  game_state *GameState = GameInit(&Plat, GameMemory, &Os);
  if (!GameState) { Error("Initializing Game State :( "); return False; }

  /*
   *  Main Game loop
   */

  r64 LastMs = Plat.GetHighPrecisionClock();

#if BONSAI_INTERNAL
  u64 LastCycles = GetDebugState()->GetCycleCount();
#endif

  while ( Os.ContinueRunning )
  {
    r64 CurrentMS = GetHighPrecisionClock();
    Plat.dt = (CurrentMS - LastMs)/1000.0f;
    LastMs = CurrentMS;

#if BONSAI_INTERNAL
    u64 CurrentCycles = GetDebugState()->GetCycleCount();
    u64 FrameCycles = CurrentCycles - LastCycles;
    LastCycles = CurrentCycles;
#endif

    if (Plat.dt > 1000.0f)
    {
      Warn("DT exceeded 1s, truncating.");
      Plat.dt = 1000.0f;
    }

    ClearWasPressedFlags((input_event*)&Plat.Input);
    DEBUG_FRAME_BEGIN(&Hotkeys, Plat.dt, FrameCycles);

    TIMED_BLOCK("Frame Preamble");
    v2 LastMouseP = Plat.MouseP;
    while ( ProcessOsMessages(&Os, &Plat) );
    Plat.MouseDP = LastMouseP - Plat.MouseP;

    BindHotkeysToInput(&Hotkeys, &Plat.Input);

    if ( GameLibIsNew(GAME_LIB) )
    {
      CloseLibrary(GameLib);
      GameLib = OpenLibrary(GAME_LIB);

      GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
      InitGlobals = (game_init_globals_proc)GetProcFromLib(GameLib, "InitGlobals");
      GameThreadCallback = (game_thread_callback_proc)GetProcFromLib(GameLib, "GameThreadCallback");

      InitGlobals(&Plat);
    }


    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    GameUpdateAndRender(&Plat, GameState, &Hotkeys);

    TIMED_BLOCK("Frame End");
    DEBUG_FRAME_END(&Plat, FrameCycles);

    BonsaiSwapBuffers(&Os);

    /* WaitForFrameTime(LastMs, 30.0f); */

    FrameEnd();

    END_BLOCK("Frame End");
  }

  Info("Shutting Down");
  Terminate(&Os);
  Info("Exiting");

  return True;
}

#endif
