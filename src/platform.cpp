#define DEFAULT_GAME_LIB "./bin/world_gen_loadable" PLATFORM_RUNTIME_LIB_EXTENSION

#define PLATFORM_THREADING_IMPLEMENTATIONS 1
#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <engine/engine.h>


global_variable s64 LastGameLibTime;
global_variable bonsai_worker_thread_callback BONSAI_API_WORKER_THREAD_CALLBACK_NAME;

#include <sys/stat.h>

typedef struct stat bonsai_stat;

bonsai_function b32
LibIsNew(const char *LibPath, s64 *LastLibTime)
{
  b32 Result = False;
  bonsai_stat StatStruct;

  if (stat(LibPath, &StatStruct) == 0)
  {
    if (StatStruct.st_mtime > *LastLibTime)
    {
      *LastLibTime = StatStruct.st_mtime;
      Result = True;
    }
  }
  else
  {
    Error("Stat-ing Game library :( ");
  }

  return Result;
}

bonsai_function thread_local_state
DefaultThreadLocalState()
{
  thread_local_state Thread = {};

  Thread.TempMemory = AllocateArena();
  Thread.PermMemory = AllocateArena();

  // NOTE(Jesse): As it stands the debug system doesn't do any locking when
  // constructing the debug arena stats, so we can't ever free memory allocated
  // on debug registered arenas on threads outside the main one.
  //
  /* DEBUG_REGISTER_ARENA(Thread.TempMemory); */

  DEBUG_REGISTER_ARENA(Thread.PermMemory);

  return Thread;
}

bonsai_function void
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
      b32 Exchanged = AtomicCompareExchange( &Queue->DequeueIndex,
                                             (DequeueIndex+1) % WORK_QUEUE_SIZE,
                                             DequeueIndex );
      if ( Exchanged )
      {
        work_queue_entry* Entry = Queue->Entries + DequeueIndex;
        BONSAI_API_WORKER_THREAD_CALLBACK_NAME(Entry, Thread);
        Entry->Type = type_work_queue_entry_noop;
      }
    }
  }
}

bonsai_function THREAD_MAIN_RETURN
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

    // TODO(Jesse) call DrainQueue here?!  We should also escape this loop if
    // anything shows up on the high-priority queue.. so maybe that's why I
    // left this as-is.
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
        b32 Exchanged = AtomicCompareExchange( &LowPriority->DequeueIndex,
                                               (DequeueIndex+1) % WORK_QUEUE_SIZE,
                                               DequeueIndex );
        if ( Exchanged )
        {
          work_queue_entry* Entry = LowPriority->Entries + DequeueIndex;
          BONSAI_API_WORKER_THREAD_CALLBACK_NAME(Entry, &Thread);
          Entry->Type = type_work_queue_entry_noop;
        }
      }
    }

    Ensure(RewindArena(Thread.TempMemory));
  }
}

#if 0
// TODO(Jesse): Move to counted_string.h
bonsai_function b32
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

// TODO(Jesse): Move to counted_string.h
bonsai_function b32
StrStr(char *Str1, char *Str2)
{
  b32 Result = StrMatch(Str1, Str2);

  while(*Str1++)
  {
    Result |= StrMatch(Str1, Str2);
  }

  return Result;
}
#endif

bonsai_function void
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

    PlatformCreateThread( ThreadMain, Params );
  }

  return;
}

bonsai_function void
InitQueue(work_queue* Queue, memory_arena* Memory, semaphore* Semaphore)
{
  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;

  Queue->Entries = Allocate(work_queue_entry, Memory, WORK_QUEUE_SIZE);
  Queue->GlobalQueueSemaphore = Semaphore;

  return;
}

bonsai_function void
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->Memory = Memory;

  u32 LogicalCoreCount = PlatformGetLogicalCoreCount();
  u32 WorkerThreadCount = GetWorkerThreadCount();
  Info("Detected %u Logical cores, creating %u threads", LogicalCoreCount, WorkerThreadCount);

  Plat->QueueSemaphore = CreateSemaphore();

  InitQueue(&Plat->LowPriority, Plat->Memory, &Plat->QueueSemaphore);
  InitQueue(&Plat->HighPriority, Plat->Memory, &Plat->QueueSemaphore);

  Plat->Threads = Allocate(thread_startup_params, Plat->Memory, WorkerThreadCount);

  return;
}

#if 0
/*
 *  Poor mans vsync
 */
bonsai_function void
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

#if !BONSAI_WIN32
bonsai_function server_state*
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
#endif

s32
main()
{
  Info("Initializing Bonsai");

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return False; }
  Info("Found Bonsai Root : %S", CS(GetCwd()) );

  platform Plat = {};
  os Os         = {};

  if (!OpenAndInitializeWindow(&Os, &Plat)) { Error("Initializing Window :( "); return False; }
  Assert(Os.GlContext);

  if (!InitializeOpengl(&Os)) { Error("Initializing OpenGL :( "); return False; }

  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }
  init_debug_system_proc InitDebugSystem = (init_debug_system_proc)GetProcFromLib(DebugLib, "InitDebugSystem");
  GetDebugState = InitDebugSystem(&GL);

  AssertNoGlErrors;

  memory_arena *PlatMemory = AllocateArena();
  memory_arena *GameMemory = AllocateArena();

  DEBUG_REGISTER_ARENA(GameMemory);
  DEBUG_REGISTER_ARENA(PlatMemory);

  PlatformInit(&Plat, PlatMemory);

#if BONSAI_INTERNAL
  // debug_recording_state *Debug_RecordingState = Allocate(debug_recording_state, GameMemory, 1);
  // AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3));
#endif

  hotkeys Hotkeys = {};

#if EMCC ///////////////////////////////////// EMCC SHOULD COMPILE AND RUN CORRECTLY UP TO HERE
  return True;
#endif

  LibIsNew(DEFAULT_GAME_LIB, &LastGameLibTime);  // Hack to initialize the LastGameLibTime static

  shared_lib GameLib = OpenLibrary(DEFAULT_GAME_LIB);
  if (!GameLib) { Error("Loading GameLib :( "); return False; }

  bonsai_main_thread_init_callback GameInit = (bonsai_main_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  if (!GameInit) { Error("Retreiving " STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME) " from Game Lib :( "); return False; }

  bonsai_main_thread_callback BONSAI_API_MAIN_THREAD_CALLBACK_NAME = (bonsai_main_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  if (!BONSAI_API_MAIN_THREAD_CALLBACK_NAME) { Error("Retreiving " STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME) " from Game Lib :( "); return False; }

  BONSAI_API_WORKER_THREAD_CALLBACK_NAME = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );
  if (!BONSAI_API_WORKER_THREAD_CALLBACK_NAME) { Error("Retreiving " STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) " from Game Lib :( "); return False; }

  bonsai_worker_thread_init_callback WorkerThreadInitCallback = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));

  // NOTE(Jesse): This used to pass GetDebugState into the game library, but we
  // now have to use GetProcFromLib() to hook up the pointer.  I'm writing this
  // as I'm refactoring, so once you hit this there might be a better way, but
  // as of this writing that seems like the best option.  This change was made
  // such that GameInit() doesn't need to know about any debug system types,
  // and in fact the entire engine shouldn't need to know about anything in the
  // debug system.
  //
  void * SetDebugStateProc = GetProcFromLib(GameLib, "SetDebugState" );
  if (SetDebugStateProc)
  {
    SetDebugStateProc(GetDebugState);
  }
  else
  {
    // The game doesn't have to export that function if it doesn't want
    // debugging enabled
  }



  NotImplemented;

  game_state* GameState = GameInit(&Plat, GameMemory, &GL);
  if (!GameState) { Error("Initializing Game State :( "); return False; }

  PlatformLaunchWorkerThreads(&Plat, WorkerThreadInitCallback, GameState);

#if !BONSAI_WIN32
  server_state* ServerState = ServerInit(GameMemory);
  Assert(ServerState);
#endif

  /*
   *  Main Game loop
   */


  r64 LastMs = 0;
  r64 RealDt = 0;
  while ( Os.ContinueRunning )
  {
    ClearClickedFlags(&Plat.Input);
    DEBUG_FRAME_BEGIN(&Hotkeys);

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

#if !EMCC
    if ( LibIsNew(DEFAULT_GAME_LIB, &LastGameLibTime) )
    {
      SuspendWorkerThreads();

      CloseLibrary(GameLib);
      GameLib = OpenLibrary(DEFAULT_GAME_LIB);

      BONSAI_API_MAIN_THREAD_CALLBACK_NAME = (bonsai_main_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
      BONSAI_API_WORKER_THREAD_CALLBACK_NAME = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME));

      ResumeWorkerThreads();
    }
#endif

    // TODO(Jesse, id: 153, tags: hot_reload, debug_lib): Doing this properly requires some extra-credit work first.
    //
    // 1) We need the game code to be able to call GetDebugState() willy-nilly,
    // so when the debug lib gets refreshed it has to be passed to the game
    // code somehow.
    //
    // Possibly allow the game code to export the bonsai_function pointer it uses for
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

    /* TIMED_BLOCK("Network Ops"); */
    /*   if (IsDisconnected(&Plat.Network)) { ConnectToServer(&Plat.Network); } */
    /* END_BLOCK("Network Ops"); */

    BONSAI_API_MAIN_THREAD_CALLBACK_NAME(&Plat, GameState, &Hotkeys);


#if BONSAI_LINUX
    DEBUG_FRAME_END(&Plat, ServerState);
#else
    DEBUG_FRAME_END(&Plat, 0);
#endif

    BonsaiSwapBuffers(&Os);

    r64 CurrentMS = GetHighPrecisionClock();
    RealDt = (CurrentMS - LastMs)/1000.0;
    LastMs = CurrentMS;
    Plat.dt = (r32)RealDt;

    MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(RealDt);

    Ensure(RewindArena(TranArena));
  }

  Info("Shutting Down");
  Terminate(&Os);
  Info("Exiting");

  return True;
}
