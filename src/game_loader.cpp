#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_LIB_LOADER_API 1

// TODO(Jesse): This is old/should go
#define BONSAI_DEBUG_SYSTEM_API 1

#define DEFAULT_DEBUG_LIB "./bin/lib_bonsai_debug_loadable" PLATFORM_RUNTIME_LIB_EXTENSION

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <engine/engine.h>

global_variable s64 LastGameLibTime;
global_variable s64 LastDebugLibTime;

#include <sys/stat.h>

typedef struct stat bonsai_stat;

link_internal b32
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

link_internal thread_local_state
DefaultThreadLocalState(mesh_freelist *MeshFreelist, u32 ThreadId)
{
  thread_local_state Thread = {};

  Thread.MeshFreelist = MeshFreelist;

  Thread.TempMemory = AllocateArena();
  Thread.PermMemory = AllocateArena(Megabytes(256));

  // TODO(Jesse)(safety): Given the below, how exactly is it safe to register
  // the PermMemory?  Seems to me like that's still just as liable to cause bad
  // behavior, but less likely.
  //
  // NOTE(Jesse): As it stands the debug system doesn't do any locking when
  // constructing the debug arena stats, so we can't ever free memory allocated
  // on debug registered arenas on threads outside the main one.
  //
  DEBUG_REGISTER_ARENA(Thread.TempMemory, ThreadId);
  DEBUG_REGISTER_ARENA(Thread.PermMemory, ThreadId);

  return Thread;
}

link_internal THREAD_MAIN_RETURN
ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;

  bonsai_worker_thread_callback GameWorkerThreadCallback = ThreadParams->GameWorkerThreadCallback;

  DEBUG_REGISTER_THREAD(ThreadParams->Self.ThreadIndex);

  thread_local_state Thread = DefaultThreadLocalState(&ThreadParams->EngineResources->MeshFreelist, ThreadParams->Self.ThreadIndex);

  if (ThreadParams->InitProc) { ThreadParams->InitProc(&Thread); }

  while (FutexNotSignaled(ThreadParams->WorkerThreadsExitFutex))
  {
#if 0
    // This is a pointer to a single semaphore for all queues, so only sleeping
    // on one is sufficient, and equal to sleeping on all, because they all
    // point to the same semaphore
    ThreadSleep( ThreadParams->HighPriority->GlobalQueueSemaphore );
#else
    for (;;)
    {
      WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

      /* TIMED_NAMED_BLOCK("CheckForWorkAndSleep"); */

      if (!QueueIsEmpty(ThreadParams->HighPriority)) break;

      if (!QueueIsEmpty(ThreadParams->LowPriority)) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex) ) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex) ) break;

      SleepMs(1);
    }
#endif

    WaitOnFutex(ThreadParams->WorkerThreadsSuspendFutex);

    AtomicIncrement(ThreadParams->HighPriorityWorkerCount);
    DrainQueue( ThreadParams->HighPriority, &Thread, GameWorkerThreadCallback );
    AtomicDecrement(ThreadParams->HighPriorityWorkerCount);

#if 1
    // TODO(Jesse): Vectorize the clear on this such that we can turn this back
    // on instead of allocating fresh pages every time.  In the end re-using
    // pages and zeroing them ourselves will (theoretically) be faster
    //
    // @turn_rewind_arena_back_on
    Ensure( RewindArena(Thread.TempMemory) );
#else
    // Can't do this anymore because the debug system needs a static handle to
    // the base address of the arena, which VaporizeArena unmaps
    //
    Ensure( VaporizeArena(Thread.TempMemory) );
    Ensure( Thread.TempMemory = AllocateArena() );
#endif

    work_queue* LowPriority = ThreadParams->LowPriority;
    for (;;)
    {
      WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

      if (!QueueIsEmpty(ThreadParams->HighPriority)) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex) ) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex) ) break;

      // NOTE(Jesse): Must read and comared DequeueIndex instead of calling QueueIsEmpty
      u32 DequeueIndex = LowPriority->DequeueIndex;
      if (DequeueIndex == LowPriority->EnqueueIndex)
      {
        break;
      }

      b32 Exchanged = AtomicCompareExchange( &LowPriority->DequeueIndex,
                                              GetNextQueueIndex(DequeueIndex),
                                              DequeueIndex );
      if ( Exchanged )
      {
        volatile work_queue_entry *Entry = LowPriority->Entries+DequeueIndex;
        GameWorkerThreadCallback(Entry, &Thread);
      }
    }
  }

  WaitOnFutex(ThreadParams->WorkerThreadsExitFutex);

  return 0;
}

link_internal void
PlatformLaunchWorkerThreads(platform *Plat, engine_resources *EngineResources, bonsai_worker_thread_init_callback WorkerThreadInit, bonsai_worker_thread_callback WorkerThreadCallback)
{
  u32 WorkerThreadCount = GetWorkerThreadCount();

  for ( u32 ThreadIndex = 0;
        ThreadIndex < WorkerThreadCount;
        ++ThreadIndex )
  {
    thread_startup_params *Params = &Plat->Threads[ThreadIndex];
    Params->Self.ThreadIndex = ThreadIndex + 1;
    Params->HighPriority = &Plat->HighPriority;
    Params->LowPriority = &Plat->LowPriority;
    Params->InitProc = WorkerThreadInit;
    Params->GameWorkerThreadCallback = WorkerThreadCallback;
    Params->EngineResources = EngineResources;

    Params->HighPriorityWorkerCount = &Plat->HighPriorityWorkerCount;
    Params->WorkerThreadsSuspendFutex = &Plat->WorkerThreadsSuspendFutex;
    Params->WorkerThreadsExitFutex = &Plat->WorkerThreadsExitFutex;

    PlatformCreateThread( ThreadMain, Params );
  }

  return;
}

link_internal void
InitQueue(work_queue* Queue, memory_arena* Memory) //, semaphore* Semaphore)
{
  Queue->EnqueueIndex = 0;
  Queue->DequeueIndex = 0;

  Queue->Entries = Allocate(work_queue_entry, Memory, WORK_QUEUE_SIZE);
  /* Queue->GlobalQueueSemaphore = Semaphore; */

  return;
}

link_internal void
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->Memory = Memory;

  u32 LogicalCoreCount = PlatformGetLogicalCoreCount();
  u32 WorkerThreadCount = GetWorkerThreadCount();
  Info("Detected %u Logical cores, creating %u threads", LogicalCoreCount, WorkerThreadCount);

  /* Plat->QueueSemaphore = CreateSemaphore(); */

  InitQueue(&Plat->LowPriority, Plat->Memory); //, &Plat->QueueSemaphore);
  InitQueue(&Plat->HighPriority, Plat->Memory); //, &Plat->QueueSemaphore);

  Plat->Threads = Allocate(thread_startup_params, Plat->Memory, WorkerThreadCount);

#if BONSAI_NETWORK_IMPLEMENTATION
  Plat->ServerState = ServerInit(GameMemory);
#endif

  return;
}

s32
main( s32 ArgCount, const char ** Args )
{
  Info("Initializing Bonsai");

  /* if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return 1; } */
  /* Info("Found Bonsai Root : %S", CS(GetCwd()) ); */

  platform Plat = {};
  os Os         = {};
  engine_resources EngineResources = {};
  hotkeys Hotkeys = {};

  EngineResources.Plat = &Plat;
  EngineResources.Os = &Os;
  EngineResources.Hotkeys = &Hotkeys;

  s32 VSyncFrames = 0;
  if (!OpenAndInitializeWindow(&Os, &Plat, VSyncFrames)) { Error("Initializing Window :( "); return 1; }
  Assert(Os.GlContext);


  shared_lib DebugLib = InitializeBonsaiDebug(DEFAULT_DEBUG_LIB);
  Assert(DebugLib);
  Assert(Global_DebugStatePointer);

  EngineResources.DebugState = Global_DebugStatePointer;

  heap_allocator DebugHeap = InitHeap(Megabytes(32));
  GetDebugState()->InitializeRenderSystem(GetDebugState(), &DebugHeap);


  memory_arena *PlatMemory = AllocateArena();
  memory_arena *GameMemory = AllocateArena();

  DEBUG_REGISTER_ARENA(GameMemory, 0);
  DEBUG_REGISTER_ARENA(PlatMemory, 0);

  DEBUG_REGISTER_NAMED_ARENA(TranArena, 0, "game_loader TranArena");

  PlatformInit(&Plat, PlatMemory);

#if BONSAI_INTERNAL
  // debug_recording_state *Debug_RecordingState = Allocate(debug_recording_state, GameMemory, 1);
  // AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3));
#endif

#if EMCC ///////////////////////////////////// EMCC SHOULD COMPILE AND RUN CORRECTLY UP TO HERE
  return True;
#endif

  const char* GameLibName = "./bin/asset_picker_loadable" PLATFORM_RUNTIME_LIB_EXTENSION;
  switch (ArgCount)
  {
    case 1: {} break;

    case 2:
    {
      GameLibName = Args[1];
    } break;

    default: { Error("Invalid number of arguments"); }
  }

  LibIsNew(GameLibName, &LastGameLibTime); // Hack to initialize the lib timer statics
  LibIsNew(DEFAULT_DEBUG_LIB, &LastDebugLibTime);

  shared_lib GameLib = OpenLibrary(GameLibName);
  if (!GameLib) { Error("Loading GameLib :( "); return 1; }

  game_api GameApi = {};
  if (!InitializeGameApi(&GameApi, GameLib)) { Error("Initializing GameApi :( "); return 1; }

  engine_api EngineApi = {};
  if (!InitializeEngineApi(&EngineApi, GameLib)) { Error("Initializing EngineApi :( "); return 1; }

  Ensure( EngineApi.OnLibraryLoad(&EngineResources) );
  Ensure( EngineApi.Init(&EngineResources) );

  if (GameApi.WorkerMain)
  {
    PlatformLaunchWorkerThreads(&Plat, &EngineResources, GameApi.WorkerInit, GameApi.WorkerMain);
  }

  thread_local_state MainThread = DefaultThreadLocalState(&EngineResources.MeshFreelist, 0);

  if (GameApi.GameInit)
  {
    EngineResources.GameState = GameApi.GameInit(&EngineResources, &MainThread);
    if (!EngineResources.GameState) { Error("Initializing Game :( "); return 1; }
  }


  /*
   *  Main Game loop
   */


  r64 LastMs = 0;
  r64 RealDt = 0;
  while ( Os.ContinueRunning )
  {
    ClearClickedFlags(&Plat.Input);
    Clear(&Hotkeys);

    if (Plat.dt > 0.1f)
    {
      Warn("DT exceeded 100ms, truncating to 33.33ms");
      Plat.dt = 0.03333f;
    }

    TIMED_BLOCK("Frame Preamble");

    v2 LastMouseP = Plat.MouseP;
    while ( ProcessOsMessages(&Os, &Plat) );
    Plat.MouseDP = LastMouseP - Plat.MouseP;

    BindHotkeysToInput(&Hotkeys, &Plat.Input);

    DEBUG_FRAME_BEGIN(Hotkeys.Debug_ToggleMenu, Hotkeys.Debug_ToggleProfiling);

#if !EMCC
    if ( LibIsNew(GameLibName, &LastGameLibTime) )
    {
      SignalAndWaitForWorkers(&Plat.WorkerThreadsSuspendFutex);

      CloseLibrary(GameLib);
      GameLib = OpenLibrary(GameLibName);

      Ensure(InitializeGameApi(&GameApi, GameLib));
      Ensure(InitializeEngineApi(&EngineApi, GameLib));

      Ensure( EngineApi.OnLibraryLoad(&EngineResources) );

      UnsignalFutex(&Plat.WorkerThreadsSuspendFutex);
    }

    if ( LibIsNew(DEFAULT_DEBUG_LIB, &LastDebugLibTime) )
    {
      SignalAndWaitForWorkers(&Plat.WorkerThreadsSuspendFutex);

      debug_state *Cached = Global_DebugStatePointer;
      Global_DebugStatePointer = 0;

      // NOTE(Jesse): We hold pointers to static strings in the first debug_lib
      // we allocate, so we can't unload.  TBD if we care about copying them.. but we might.
      //
      // We also allocate debug_profile_scopes from it, which we'd have to do
      // elsewhere .. somehow.. I think..
      //
      /* CloseLibrary(DebugLib); */

      DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
      bonsai_debug_api DebugApi = {};

      if (DebugLib)
      {
        if (InitializeBootstrapDebugApi(DebugLib, &DebugApi))
        {
          DebugApi.BonsaiDebug_OnLoad(Cached);
          Ensure( EngineApi.OnLibraryLoad(&EngineResources) );
        }
        else { Error("Initializing DebugLib API"); }
      }
      else { Error("Reloading DebugLib"); }

      Global_DebugStatePointer = Cached;

      UnsignalFutex(&Plat.WorkerThreadsSuspendFutex);
    }
#endif

    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    /* TIMED_BLOCK("Network Ops"); */
    /*   if (IsDisconnected(&Plat.Network)) { ConnectToServer(&Plat.Network); } */
    /* END_BLOCK("Network Ops"); */

    /* TIMED_BLOCK(" -- Frame --"); */
    Ensure( EngineApi.FrameBegin(&EngineResources) );

    TIMED_BLOCK("GameMain");
      GameApi.GameMain(&EngineResources, &MainThread);
    END_BLOCK("GameMain");

    Ensure( EngineApi.FrameEnd(&EngineResources) );

    DrainQueue(&Plat.HighPriority, &MainThread,GameApi.WorkerMain);
    /* WaitForWorkerThreads(&Plat.HighPriorityWorkerCount); */

    // Have to suspend workers to collate memory allocation records
    SignalAndWaitForWorkers(&Plat.WorkerThreadsSuspendFutex);

    DEBUG_FRAME_END(&Plat.MouseP, &Plat.MouseDP, V2(Plat.WindowWidth, Plat.WindowHeight), &Plat.Input, Plat.dt, &EngineResources.EngineDebug.PickedChunks);

    UnsignalFutex(&Plat.WorkerThreadsSuspendFutex);

    Ensure( EngineApi.Render(&EngineResources) );

    r64 CurrentMS = GetHighPrecisionClock();
    RealDt = (CurrentMS - LastMs)/1000.0;
    LastMs = CurrentMS;
    Plat.dt = (r32)RealDt;

    Ensure( RewindArena(TranArena) );

    MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(RealDt);
    /* END_BLOCK("-- Frame --"); */
  }

  Info("Shutting Down");

  Terminate(&Os, &Plat);

  Info("Exiting");

  return 0;
}
