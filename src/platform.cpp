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
      Result = True;
    }
  }
  else
  {
    printf("Error Stat-ing Game library :( \n");
  }

  return Result;
}

shared_lib
CheckAndReloadGameLibrary()
{
  shared_lib BonsaiLib = 0;

  if ( GameLibIsNew(GAME_LIB) )
  {
     BonsaiLib = LoadLibrary(GAME_LIB);
  }

  if (!BonsaiLib)
  {
    printf("Error Loading Game library :( \n");
  }

  return BonsaiLib;
}

int
main(s32 NumArgs, char ** Args)
{
  printf("\n -- Initializing Bonsai \n");

  /* char pwd[MAX_PATH]; */
  /* GetCurrentDirectory(MAX_PATH, pwd); */

  /* HINSTANCE AppHandle = GetModuleHandle(0); */

  shared_lib GameLib = CheckAndReloadGameLibrary();

  if (GameLib)
  {
    game_memory GameMemory = {};

    u32 WindowHeight = 256;
    u32 WindowWidth = 512;

    GAME_MAIN_PROC = (game_main_proc)GetProcFromLib(GameLib, "GameMain");
    if (!GameMain) { printf("Error retreiving GameMain from Game Lib :( \n"); return False; }

    window Window = OpenAndInitializeWindow(WindowWidth, WindowHeight ); // , AppHandle );
    if (!Window) { printf("Error Initializing Window :( \n"); return False; }

    for (;;)
    {
      XEvent xev;
      XNextEvent(dpy, &xev);

      if(xev.type == Expose) {
        XWindowAttributes gwa;
        XGetWindowAttributes(dpy, Window, &gwa);
        glViewport(0, 0, gwa.width, gwa.height);
        glXSwapBuffers(dpy, Window);
      }

      else if(xev.type == KeyPress) {
        glXMakeCurrent(dpy, None, NULL);
        /* glXDestroyContext(dpy, glc); */
        /* XDestroyWindow(dpy, win); */
        XCloseDisplay(dpy);
        exit(0);
      }

      GameMain(&GameMemory);
      GameLib = CheckAndReloadGameLibrary();
    }
  }

  return True;
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
      u32 EntryIndex = AtomicCompareExchange( &Queue->NextEntry,
                                                       (OriginalNext + 1) % WORK_QUEUE_SIZE,
                                                       OriginalNext);
      if ( EntryIndex == OriginalNext )
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
PlatformInit(platform *Platform)
{
  Assert(sizeof(u8)  == 1);

  Assert(sizeof(u32) == 4);
  Assert(sizeof(s32) == 4);
  Assert(sizeof(r32) == 4);

  Assert(sizeof(umm) == 8);
  Assert(sizeof(u64) == 8);
  Assert(sizeof(s64) == 8);

  u32 StackSize = 0;

  u32 LogicalCoreCount = GetLogicalCoreCount();
  u32 ThreadCount = LogicalCoreCount -1; // -1 because we already have a main thread

  Platform->Queue.Entries = (work_queue_entry *)calloc(sizeof(work_queue_entry), WORK_QUEUE_SIZE);
  Platform->Threads = (thread_startup_params *)calloc(sizeof(thread_startup_params), ThreadCount);
  work_queue *Queue = &Platform->Queue;

  Queue->Semaphore = CreateSemaphore(ThreadCount);

  for (u32 ThreadIndex = 0;
      ThreadIndex < ThreadCount;
      ++ ThreadIndex )
  {
    thread_startup_params *Params = &Platform->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex;
    Params->Queue = Queue;

    thread_id ThreadID = CreateThread( ThreadMain, Params );
  }

  return;
}

debug_state *
GetDebugState(void)
{
  return &DebugState;
}

#endif
