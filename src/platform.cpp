#ifndef BONSAI_PLATFORM_CPP
#define BONSAI_PLATFORM_CPP

#include <iostream>

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

GLOBAL_VARIABLE u32 LastGameLibTime = 0;

b32
GameLibIsNew(const char *LibPath)
{
  b32 Result = False;
  struct stat StatStruct;

  if (stat(LibPath, &StatStruct) == 0)
  {
    if (StatStruct.st_mtime > LastGameLibTime)
    {
      LastGameLibTime = StatStruct.st_mtime;
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
  thread *Self = &ThreadParams->Self;

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
PlatformInit(platform *Plat)
{
  Assert(sizeof(u8)  == 1);

  Assert(sizeof(u32) == 4);
  Assert(sizeof(s32) == 4);
  Assert(sizeof(r32) == 4);

  Assert(sizeof(umm) == 8);
  Assert(sizeof(u64) == 8);
  Assert(sizeof(s64) == 8);

  u32 StackSize = 0;

  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  InitializeOpenGlExtensions(&Plat->GL);

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1; // -1 because we already have a main thread

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

    thread_id ThreadID = CreateThread( ThreadMain, Params );
  }

  Plat->GetHighPrecisionClock = GetHighPrecisionClock;
  Plat->PushWorkQueueEntry = PushWorkQueueEntry;

  return;
}

/*
 *  Poor mans vsync
 */
void
WaitForFrameTime(real64 frameStart, float FPS)
{
  real64 frameTime = GetHighPrecisionClock() - frameStart;

  while (frameTime < (1.0f/FPS))
  {
    frameTime = GetHighPrecisionClock() - frameStart;
  }

  return;
}

int
main(s32 NumArgs, char ** Args)
{
  printf("\n -- Initializing Bonsai \n");

  GameLibIsNew(GAME_LIB);  // Hack to initialize the LastGameLibTime static

  shared_lib GameLib = LoadLibrary(GAME_LIB);

  if (GameLib)
  {
    platform Plat = {};
    PlatformInit(&Plat);

    Plat.WindowHeight = 256;
    Plat.WindowWidth = 512;

    game_init_proc GameInit = (game_init_proc)GetProcFromLib(GameLib, "GameInit");
    if (!GameInit) { printf("Error retreiving GameInit from Game Lib :( \n"); return False; }

    game_main_proc GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
    if (!GameUpdateAndRender) { printf("Error retreiving GameUpdateAndRender from Game Lib :( \n"); return False; }

    window Window = OpenAndInitializeWindow(Plat.WindowWidth, Plat.WindowHeight );
    if (!Window) { printf("Error Initializing Window :( \n"); return False; }

    double lastTime = Plat.GetHighPrecisionClock();

    game_state *GameState = GameInit(&Plat);
    if (!GameState) { printf("Error Initializing Game State :( \n"); return False; }

    for (;;)
    {

      double currentTime = Plat.GetHighPrecisionClock();
      Plat.dt = (real32)((currentTime - lastTime) / 1000000.0f);
      lastTime = currentTime;

      printf("%f\n", Plat.dt);


      /* XEvent xev; */
      /* XNextEvent(dpy, &xev); */
      /* if(xev.type == Expose) { */
      /*   XWindowAttributes gwa; */
      /*   XGetWindowAttributes(dpy, Window, &gwa); */
      /*   glViewport(0, 0, gwa.width, gwa.height); */
      /*   glXSwapBuffers(dpy, Window); */
      /* } */
      /* else if(xev.type == KeyPress) { */
      /*   glXMakeCurrent(dpy, None, NULL); */
      /*   glXDestroyContext(dpy, glc); */
      /*   XDestroyWindow(dpy, win); */
      /*   XCloseDisplay(dpy); */
      /*   exit(0); */
      /* } */


      GameUpdateAndRender(&Plat, GameState);

      if ( GameLibIsNew(GAME_LIB) )
      {
        CloseLibrary(GameLib);

        sleep(1); // FIXME(Jesse): Do a copy on the library or something instead

        GameLib = LoadLibrary(GAME_LIB);

        GameUpdateAndRender = (game_main_proc)GetProcFromLib(GameLib, "GameUpdateAndRender");
      }

      glXSwapBuffers(dpy, Window);

      /* float FPS = 60.0f; */
      /* WaitForFrameTime(lastTime, FPS); */
    }
  }

  return True;
}

debug_state *
GetDebugState(void)
{
  return &DebugState;
}

#endif
