#ifndef BONSAI_PLATFORM_CPP
#define BONSAI_PLATFORM_CPP

#include <iostream>

#include <platform_constants.h>
#include <bonsai_types.h>
#include <constants.hpp>

#if BONSAI_WIN32
#include <win32_platform.cpp>
#else
#include <unix_platform.cpp>
#endif

#include <platform.h>
#include <debug.h>

#include <sys/types.h>
#include <sys/stat.h>

GLOBAL_VARIABLE s64 LastGameLibTime = 0;
GLOBAL_VARIABLE game_thread_callback_proc GameThreadCallback;


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

    u32 OriginalNext = Queue->NextEntry;

    if (OriginalNext != Queue->EntryCount)
    {
      if ( AtomicCompareExchange(&Queue->NextEntry,
                                 (OriginalNext+1)%WORK_QUEUE_SIZE,
                                 OriginalNext) )
      {
        work_queue_entry Entry = Queue->Entries[OriginalNext];
        GameThreadCallback(&Entry);
      }

    }
  }

  return 0;
}

void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  Queue->Entries[Queue->EntryCount] = *Entry;

  CompleteAllWrites;

  Queue->EntryCount = ++Queue->EntryCount % WORK_QUEUE_SIZE;

  // TODO(Jesse): Is this check correct?
  Assert(Queue->NextEntry != Queue->EntryCount);

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
  DefGlProc(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  DefGlProc(PFNGLUNIFORM1IPROC, glUniform1i);

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



  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  AssertNoGlErrors;

#if 0
  // Platform specific (wgl / glX)
  Gl->glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("wglSwapIntervalEXT");
  Assert( Gl->glSwapInterval );
  Gl->glSwapInterval(1); // vsync
 #endif

  return;
}

b32
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->PushStruct = PushStruct;
  Plat->PushStructChecked_ = PushStructChecked_;
  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->GetCycleCount = GetCycleCount;

  // Initialized from globals
  Plat->WindowHeight = SCR_HEIGHT;
  Plat->WindowWidth = SCR_WIDTH;

  Plat->Memory = Memory;

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1 + DEBUG_THREAD_COUNT_BIAS; // -1 because we already have a main thread

  Info("Detected %d Logical cores, creating %d threads", LogicalCoreCount, ThreadCount);

  Plat->Queue.EntryCount = 0;
  Plat->Queue.NextEntry = 0;

  Plat->Queue.Entries = PUSH_STRUCT_CHECKED(work_queue_entry,  Plat->Memory, WORK_QUEUE_SIZE);
  Plat->Threads = PUSH_STRUCT_CHECKED(thread_startup_params,  Plat->Memory, ThreadCount);

  work_queue *Queue = &Plat->Queue;

  Queue->Semaphore = CreateSemaphore(ThreadCount);

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
WaitForFrameTime(r64 frameStart, float FPS)
{
  r64 frameTime = GetHighPrecisionClock() - frameStart;

  while (frameTime < (1.0f/FPS))
  {
    frameTime = GetHighPrecisionClock() - frameStart;
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

inline void
DoDebugFrameRecord(
    debug_recording_state *State,
    hotkeys *Hotkeys,
    memory_arena *MainMemory)
{
  {
    GLOBAL_VARIABLE b32 Toggled = False;
    if (Hotkeys->Debug_ToggleLoopedGamePlayback  && !Toggled)
    {
      Toggled = True;
      State->Mode = (debug_recording_mode)((State->Mode + 1) % RecordingMode_Count);

      switch (State->Mode)
      {
        case RecordingMode_Clear:
        {
          Log("Clear");
          State->FramesRecorded = 0;
          State->FramesPlayedBack = 0;
        } break;

        case RecordingMode_Record:
        {
          Log("Recording");
          CopyArena(MainMemory, &State->RecordedMainMemory);
        } break;

        case RecordingMode_Playback:
        {
          Log("Playback");
          CopyArena(&State->RecordedMainMemory, MainMemory);
        } break;

        InvalidDefaultCase;
      }

    }
    else if (!Hotkeys->Debug_ToggleLoopedGamePlayback)
    {
      Toggled = False;
    }
  }

  switch (State->Mode)
  {
    case RecordingMode_Clear:
    {
    } break;

    case RecordingMode_Record:
    {
      Assert(State->FramesRecorded < DEBUG_RECORD_INPUT_SIZE);
      Hotkeys->Debug_ToggleLoopedGamePlayback = False;
      State->Inputs[State->FramesRecorded++] = *Hotkeys;
    } break;

    case RecordingMode_Playback:
    {
      *Hotkeys = State->Inputs[State->FramesPlayedBack++];

      if (State->FramesPlayedBack == State->FramesRecorded)
      {
        State->FramesPlayedBack = 0;
        CopyArena(&State->RecordedMainMemory, MainMemory);
      }

    } break;

    InvalidDefaultCase;
  }

  return;
}

void
BindHotkeysToInput(hotkeys *Hotkeys, input *Input)
{
  Hotkeys->Debug_ToggleLoopedGamePlayback = Input->F11;
  Hotkeys->Debug_Pause = Input->F12;

  Hotkeys->Left = Input->A;
  Hotkeys->Right = Input->D;
  Hotkeys->Forward = Input->W;
  Hotkeys->Backward = Input->S;

  Hotkeys->Player_Fire = Input->Space;
  Hotkeys->Player_Proton = Input->Shift;

  return;
}

s32
main(s32 NumArgs, char ** Args)
{
  Info("Initializing Bonsai");

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return False; }
  Info("Found Bonsai Root : %s", GetCwd() );

  memory_arena MainMemory = {};
  memory_arena DebugMemory = {};

  AllocateAndInitializeArena(&MainMemory, MAIN_STORAGE_SIZE);
  AllocateAndInitializeArena(&DebugMemory, Megabytes(8) );


  memory_arena PlatMemory = {};
  memory_arena GameMemory = {};

  SubArena(&MainMemory, &PlatMemory, PLATFORM_STORAGE_SIZE);
  SubArena(&MainMemory, &GameMemory, GAME_STORAGE_SIZE);


  debug_recording_state *Debug_RecordingState =
    PUSH_STRUCT_CHECKED(debug_recording_state, &DebugMemory, 1);


  AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, MAIN_STORAGE_SIZE);

  platform Plat = {};
  PlatformInit(&Plat, &PlatMemory);

  os Os = {};
  Os.ContinueRunning = True;

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

  QueryAndSetGlslVersion(&Plat);

  hotkeys Hotkeys;

  game_state *GameState = GameInit(&Plat, &GameMemory);
  if (!GameState) { Error("Initializing Game State :( "); return False; }

  InitGlobals(&Plat);

  /*
   *  Main Game loop
   */

  r64 lastTime = Plat.GetHighPrecisionClock();

  while ( Os.ContinueRunning )
  {
    Plat.dt = (r32)ComputeDtForFrame(&lastTime);

    v2 LastMouseP = Plat.MouseP;
    while ( ProcessOsMessages(&Os, &Plat) );
    Plat.MouseDP = LastMouseP - Plat.MouseP;

    if ( GameLibIsNew(GAME_LIB) )
    {
      CloseLibrary(GameLib);
      GameLib = OpenLibrary(GAME_LIB);
      GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
      InitGlobals = (game_init_globals_proc)GetProcFromLib(GameLib, "InitGlobals");

      InitGlobals(&Plat);
    }

    if (Plat.dt > 1.0f)
      Plat.dt = 1.0f;

    BindHotkeysToInput(&Hotkeys, &Plat.Input);

    DoDebugFrameRecord(Debug_RecordingState, &Hotkeys, &MainMemory);

    GameUpdateAndRender(&Plat, GameState, &Hotkeys);
    BonsaiSwapBuffers(&Os);
  }

  Terminate(&Os);

  return True;
}

#endif
