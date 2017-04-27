#ifndef BONSAI_PLATFORM_CPP
#define BONSAI_PLATFORM_CPP

#include <iostream>

#include <platform_constants.h>
#include <bonsai_types.h>
#include <constants.hpp>

#if _WIN32
#include <win32_platform.cpp>
#else
#include <unix_platform.cpp>
#endif

#include <platform.h>

#include <sys/types.h>
#include <sys/stat.h>

GLOBAL_VARIABLE s64 LastGameLibTime = 0;
GLOBAL_VARIABLE input NullInput = {};

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
    printf("Error Stat-ing Game library :( \n");
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
    u32 OriginalNext = Queue->NextEntry;

    if (OriginalNext != Queue->EntryCount)
    {
      if ( AtomicCompareExchange(&Queue->NextEntry, (OriginalNext+1)%WORK_QUEUE_SIZE, OriginalNext) )
      {
        work_queue_entry Entry = Queue->Entries[OriginalNext];
        Entry.Callback(&Entry);
      }

    }
    else
    {
      ThreadSleep( Queue->Semaphore );
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

  WakeThread( Queue->Semaphore );

  return;
}

void
InitializeOpenGlExtensions(gl_extensions *GL)
{
  GL->glCreateShader = (PFNGLCREATESHADERPROC)bonsaiGlGetProcAddress("glCreateShader");;
  GL->glShaderSource = (PFNGLSHADERSOURCEPROC)bonsaiGlGetProcAddress("glShaderSource");
  GL->glCompileShader = (PFNGLCOMPILESHADERPROC)bonsaiGlGetProcAddress("glCompileShader");
  GL->glGetShaderiv = (PFNGLGETSHADERIVPROC)bonsaiGlGetProcAddress("glGetShaderiv");
  GL->glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)bonsaiGlGetProcAddress("glGetShaderInfoLog");
  GL->glAttachShader = (PFNGLATTACHSHADERPROC)bonsaiGlGetProcAddress("glAttachShader");
  GL->glDetachShader = (PFNGLDETACHSHADERPROC)bonsaiGlGetProcAddress("glDetachShader");
  GL->glDeleteShader = (PFNGLDELETESHADERPROC)bonsaiGlGetProcAddress("glDeleteShader");
  GL->glCreateProgram = (PFNGLCREATEPROGRAMPROC)bonsaiGlGetProcAddress("glCreateProgram");
  GL->glLinkProgram = (PFNGLLINKPROGRAMPROC)bonsaiGlGetProcAddress("glLinkProgram");
  GL->glGetProgramiv = (PFNGLGETPROGRAMIVPROC)bonsaiGlGetProcAddress("glGetProgramiv");
  GL->glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)bonsaiGlGetProcAddress("glGetProgramInfoLog");
  GL->glUseProgram = (PFNGLUSEPROGRAMPROC)bonsaiGlGetProcAddress("glUseProgram");
  GL->glDeleteProgram = (PFNGLDELETEPROGRAMPROC)bonsaiGlGetProcAddress("glDeleteProgram");
  GL->glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)bonsaiGlGetProcAddress("glGetUniformLocation");
  GL->glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)bonsaiGlGetProcAddress("glGenFramebuffers");
  GL->glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)bonsaiGlGetProcAddress("glBindFramebuffer");

  // TODO(Jesse): This function appears to not work on ES 3.1 ..??
  // GL->glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)bonsaiGlGetProcAddress("glFramebufferTexture");

  GL->glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)bonsaiGlGetProcAddress("glFramebufferTexture2D");
  GL->glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)bonsaiGlGetProcAddress("glCheckFramebufferStatus");
  GL->glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)bonsaiGlGetProcAddress("glCompressedTexImage2D");
  GL->glGenBuffers = (PFNGLGENBUFFERSPROC)bonsaiGlGetProcAddress("glGenBuffers");
  GL->glBindBuffer = (PFNGLBINDBUFFERPROC)bonsaiGlGetProcAddress("glBindBuffer");
  GL->glBufferData = (PFNGLBUFFERDATAPROC)bonsaiGlGetProcAddress("glBufferData");
  GL->glDrawBuffers = (PFNGLDRAWBUFFERSPROC)bonsaiGlGetProcAddress("glDrawBuffers");
  GL->glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)bonsaiGlGetProcAddress("glDeleteBuffers");
  GL->glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)bonsaiGlGetProcAddress("glVertexAttribPointer");
  GL->glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)bonsaiGlGetProcAddress("glEnableVertexAttribArray");
  GL->glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)bonsaiGlGetProcAddress("glDisableVertexAttribArray");
  GL->glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)bonsaiGlGetProcAddress("glGenVertexArrays");
  GL->glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)bonsaiGlGetProcAddress("glBindVertexArray");
  GL->glUniform3fv = (PFNGLUNIFORM3FVPROC)bonsaiGlGetProcAddress("glUniform3fv");
  GL->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)bonsaiGlGetProcAddress("glUniformMatrix4fv");
  GL->glUniform1i = (PFNGLUNIFORM1IPROC)bonsaiGlGetProcAddress("glUniform1i");
  GL->glActiveTexture = (PFNGLACTIVETEXTUREPROC)bonsaiGlGetProcAddress("glActiveTexture");

  return;
}

void
PlatformInit(platform *Plat)
{
  Assert(sizeof(u8)  == 1);

  Assert(sizeof(u32) == 4);
  Assert(sizeof(s32) == 4);
  Assert(sizeof(r32) == 4);

  Assert(sizeof(umm) == 8);
  Assert(sizeof(u64) == 8);
  Assert(sizeof(s64) == 8);

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1; // -1 because we already have a main thread

  Plat->Queue.EntryCount = 0;
  Plat->Queue.NextEntry = 0;

  Plat->Queue.Entries = (work_queue_entry *)calloc(sizeof(work_queue_entry), WORK_QUEUE_SIZE);
  Plat->Threads = (thread_startup_params *)calloc(sizeof(thread_startup_params), ThreadCount);
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

  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->PushWorkQueueEntry = PushWorkQueueEntry;

  return;
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


int
main(s32 NumArgs, char ** Args)
{
  printf("\n -- Initializing Bonsai \n");

  if (!SearchForProjectRoot())
  {
    printf(" -- Error -- Couldn't find root dir, exiting. \n");
    return False;
  }

  printf(" -- Running out of : %s \n", GetCwd() );

  platform Plat = {};
  PlatformInit(&Plat);

  os Os = {};

  Plat.WindowHeight = SCR_HEIGHT;
  Plat.WindowWidth = SCR_WIDTH;

  GameLibIsNew(GAME_LIB);  // Hack to initialize the LastGameLibTime static

  shared_lib GameLib = OpenLibrary(GAME_LIB);
  if (!GameLib) { printf("Error Loading GameLib :( \n"); return False; }

  game_init_proc GameInit = (game_init_proc)GetProcFromLib(GameLib, "GameInit");
  if (!GameInit) { printf("Error retreiving GameInit from Game Lib :( \n"); return False; }

  game_main_proc GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
  if (!GameUpdateAndRender) { printf("Error retreiving GameUpdateAndRender from Game Lib :( \n"); return False; }

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, Plat.WindowWidth, Plat.WindowHeight);
  if (!WindowSuccess) { printf("Error Initializing Window :( \n"); return False; }

  Assert(Os.Window);

  InitializeOpenGlExtensions(&Plat.GL);

  r64 GLSL_Version = atof((char*)glGetString ( GL_SHADING_LANGUAGE_VERSION ));
  printf(" GLSL verison : %f \n", GLSL_Version );

  if (GLSL_Version >= 3.3)
    Plat.GlslVersion = "330";

  else
    Plat.GlslVersion = "310ES";


  game_state *GameState = GameInit(&Plat);
  if (!GameState) { printf("Error Initializing Game State :( \n"); return False; }

  /*
   *  Main Game loop
   */

  r64 lastTime = Plat.GetHighPrecisionClock();

  for (;;)
  {
    Plat.dt = (r32)ComputeDtForFrame(&lastTime);

    // printf("%f \n", Plat.dt);

    // Zero out inputs from last frame
    /* Plat.Input = NullInput; */

    // Flush Message Queue
    while ( ProcessOsMessages(&Os, &Plat) );

    if ( GameLibIsNew(GAME_LIB) )
    {
      CloseLibrary(GameLib);

      // FIXME(Jesse): Do something smart instead of blocking and hoping the
      // compiler is finished
      sleep(1);

      GameLib = OpenLibrary(GAME_LIB);
      GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
    }

    GameUpdateAndRender(&Plat, GameState);
    BonsaiSwapBuffers(&Os);

    /* float FPS = 60.0f; */
    /* WaitForFrameTime(lastTime, FPS); */
  }

  return True;
}

#endif
