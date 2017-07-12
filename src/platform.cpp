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
        Entry.Callback(&Entry);
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


#define DefGlProc(ProcType, ProcName) Gl->ProcName = (ProcType)bonsaiGlGetProcAddress(#ProcName); Assert(Gl->ProcName)
void
InitializeOpenGlExtensions(gl_extensions *Gl)
{
  Info("Initializing OpenGL Extensions");

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
  DefGlProc(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
  DefGlProc(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);

  // TODO(Jesse): This function appears to not work on ES 3.1 ..??
  // DefGlProc(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture);

  DefGlProc(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
  DefGlProc(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
  DefGlProc(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
  DefGlProc(PFNGLGENBUFFERSPROC, glGenBuffers);
  DefGlProc(PFNGLBINDBUFFERPROC, glBindBuffer);
  DefGlProc(PFNGLBUFFERDATAPROC, glBufferData);
  DefGlProc(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
  DefGlProc(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
  DefGlProc(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
  DefGlProc(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
  DefGlProc(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
  DefGlProc(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
  DefGlProc(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
  DefGlProc(PFNGLUNIFORM3FVPROC, glUniform3fv);
  DefGlProc(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  DefGlProc(PFNGLUNIFORM1IPROC, glUniform1i);
  DefGlProc(PFNGLACTIVETEXTUREPROC, glActiveTexture);


  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  AssertNoGlErrors;

  // Platform specific (wgl / glX)
  Gl->glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("wglSwapIntervalEXT");
  Assert( Gl->glSwapInterval );
  Gl->glSwapInterval(1); // vsync

  return;
}

b32
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->PushWorkQueueEntry = PushWorkQueueEntry;
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
  Plat->Threads = PUSH_STRUCT_CHECKED(thread_startup_params,  Plat->Memory, WORK_QUEUE_SIZE);

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
DoDebugFrameRecord(debug_recording_state *State, input *Input, memory_arena *MainMemory)
{
  {
    GLOBAL_VARIABLE b32 Toggled = False;
    if (Input->F1 && !Toggled)
    {
      Toggled = True;
      State->Mode = (debug_recording_mode)((State->Mode + 1) % RecordingMode_Count);

      switch (State->Mode)
      {
        case RecordingMode_Clear:
        {
          State->FramesRecorded = 0;
          State->FramesPlayedBack = 0;
        } break;

        case RecordingMode_Record:
        {
          CopyArena(MainMemory, &State->RecordedMainMemory);
        } break;

        case RecordingMode_Playback:
        {
          CopyArena(&State->RecordedMainMemory, MainMemory);
        } break;

        InvalidDefaultCase;
      }

    }
    else if (!Input->F1)
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
      Input->F1 = False;
      State->Inputs[State->FramesRecorded++] = *Input;
    } break;

    case RecordingMode_Playback:
    {
      *Input = State->Inputs[State->FramesPlayedBack++];

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

int
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

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }

  Assert(Os.Window);

  InitializeOpenGlExtensions(&Plat.GL);

  QueryAndSetGlslVersion(&Plat);

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

    if (Plat.Input.F11)
      Plat.dt = 0;

    if (Plat.dt > 1.0f)
      Plat.dt = 1.0f;

    DoDebugFrameRecord(Debug_RecordingState, &Plat.Input, &MainMemory);

    GameUpdateAndRender(&Plat, GameState);
    BonsaiSwapBuffers(&Os);
  }

  Terminate(&Os);

  return True;
}

#endif
