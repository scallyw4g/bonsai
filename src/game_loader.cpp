#define PLATFORM_WINDOW_IMPLEMENTATIONS 1

#define DEBUG_SYSTEM_API 1
#define DEBUG_SYSTEM_LOADER_API 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <engine/engine.h>
#include <engine/engine.cpp>

global_variable s64 LastGameLibTime;
global_variable s64 LastDebugLibTime;

#include <sys/stat.h>

/* #include <bonsai_debug/headers/win32_pmc.cpp> */

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

  return Result;
}

link_internal THREAD_MAIN_RETURN
ThreadMain(void *Input)
{
  thread_startup_params *ThreadParams = (thread_startup_params *)Input;

  SetThreadLocal_ThreadIndex(ThreadParams->ThreadIndex);
  ThreadParams->EngineApi->WorkerInit(ThreadParams->EngineResources, ThreadParams);

  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
  Thread->Index = ThreadParams->ThreadIndex;


  if (ThreadParams->GameApi->WorkerInit) { ThreadParams->GameApi->WorkerInit(Global_ThreadStates, ThreadParams->ThreadIndex); }

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

      if ( ! FutexIsSignaled(ThreadParams->HighPriorityModeFutex) &&
           ! QueueIsEmpty(ThreadParams->LowPriority) ) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex) ) break;

      if ( FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex) ) break;

      SleepMs(1);
    }
#endif

    WaitOnFutex(ThreadParams->WorkerThreadsSuspendFutex);

    // NOTE(Jesse): This is here to ensure the game lib (and, by extesion, the debug lib)
    // has ThreadLocal_ThreadIndex set.  This is super annoying and I want a better solution.
    ThreadParams->EngineApi->WorkerInit(ThreadParams->EngineResources, ThreadParams);

    AtomicIncrement(ThreadParams->HighPriorityWorkerCount);
    DrainQueue( ThreadParams->HighPriority, Thread, ThreadParams->GameApi );
    AtomicDecrement(ThreadParams->HighPriorityWorkerCount);

#if 1
    if ( ! FutexIsSignaled(ThreadParams->HighPriorityModeFutex) )
    {
      Ensure( RewindArena(Thread->TempMemory) );
    }
#else
    // Can't do this because the debug system needs a static handle to the base
    // address of the arena, which VaporizeArena unmaps
    //
    Ensure( VaporizeArena(Thread.TempMemory) );
    Ensure( Thread.TempMemory = AllocateArena() );
#endif

    work_queue* LowPriority = ThreadParams->LowPriority;
    for (;;)
    {
      WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

      if ( ! QueueIsEmpty(ThreadParams->HighPriority)) break;

      if ( FutexIsSignaled(ThreadParams->HighPriorityModeFutex) ) break;

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

        HandleJob(Entry, Thread, ThreadParams->GameApi);

        Ensure( RewindArena(Thread->TempMemory) );
      }
    }
  }

  WaitOnFutex(ThreadParams->WorkerThreadsExitFutex);

  return 0;
}

link_internal void
LaunchWorkerThreads(platform *Plat, engine_resources *EngineResources, engine_api *EngineApi, game_api *GameApi)
{
  s32 TotalThreadCount  = (s32)GetTotalThreadCount();

#if 0
  Global_ThreadStates = AllocateAligned(thread_local_state, EngineResources->Plat->Memory, TotalThreadCount, CACHE_LINE_SIZE);

  for ( s32 ThreadIndex = 0; ThreadIndex < TotalThreadCount; ++ThreadIndex )
  {
    Global_ThreadStates[ThreadIndex] = DefaultThreadLocalState(EngineResources, ThreadIndex);
  }
#endif

  // This loop is for worker threads; it's skipping thread index 0, the main thread
  for ( s32 ThreadIndex = 1; ThreadIndex < TotalThreadCount; ++ThreadIndex )
  {
    /* thread_local_state *TLS = GetThreadLocalState(ThreadIndex); */
    /* Tls->Index = ThreadIndex; */

    thread_startup_params *Params = &Plat->Threads[ThreadIndex];
    Params->ThreadIndex = ThreadIndex;
    Params->HighPriority = &Plat->HighPriority;
    Params->LowPriority = &Plat->LowPriority;

    Params->EngineResources = EngineResources;
    Params->GameApi = GameApi;
    Params->EngineApi = EngineApi;

    Params->HighPriorityWorkerCount = &Plat->HighPriorityWorkerCount;

    Params->HighPriorityModeFutex = &Plat->HighPriorityModeFutex;
    Params->WorkerThreadsSuspendFutex = &Plat->WorkerThreadsSuspendFutex;
    Params->WorkerThreadsExitFutex = &Plat->WorkerThreadsExitFutex;

    PlatformCreateThread( ThreadMain, Params, ThreadIndex );
  }

  return;
}

link_internal void
PlatformInit(platform *Plat, memory_arena *Memory)
{
  Plat->Memory = Memory;

  u32 LogicalCoreCount = PlatformGetLogicalCoreCount();
  u32 WorkerThreadCount = GetWorkerThreadCount();
  s32 TotalThreadCount  = (s32)GetTotalThreadCount();
  Info("Detected %u Logical cores, creating %u threads", LogicalCoreCount, WorkerThreadCount);

  /* Plat->QueueSemaphore = CreateSemaphore(); */

  InitQueue(&Plat->LowPriority, Plat->Memory); //, &Plat->QueueSemaphore);
  InitQueue(&Plat->HighPriority, Plat->Memory); //, &Plat->QueueSemaphore);

  Plat->Threads = Allocate(thread_startup_params, Plat->Memory, TotalThreadCount);

#if BONSAI_NETWORK_IMPLEMENTATION
  Plat->ServerState = ServerInit(GameMemory);
#endif

  return;
}


#if 0
void
Debug_BeginPMCMonitoring(pmc_kernel_session *Session)
{
  pmc_counter pmc0{  L"DCMiss", 1, 50 };
  pmc_counter pmc1{  L"DCAccess", 1, 49 };

  /* pmc_counter pmc0{  L"DCacheMisses", 1, 8 }; */
  /* pmc_counter pmc1{  L"ICacheMisses", 1, 9 }; */

  /* pmc_counter pmc0{  L"ICMiss ", 1, 95 }; */
  /* pmc_counter pmc1{  L"ICFetch", 1, 94 }; */

  /* pmc_counter pmc0{  L"DCacheMisses", 1, 8 }; */
  /* pmc_counter pmc1{  L"DCacheAccesses", 1, 21 }; */

/*     pmc_counter pmc0{  L"ICacheMisses", 1, 9 }; */
/*     pmc_counter pmc1{  L"ICacheIssues", 1, 20 }; */

  Session->start({});
  /* Session->start({pmc0}); */
  /* Session->start({pmc0, pmc1}); */
}

void Debug_EndAndReportPMCMonitoring(pmc_kernel_session *Session)
{
  Session->stop();

  std::cout << "++++++RESULTS+++++++" << std::endl;

  auto results = Session->results();

  auto t0 = results[pmc0];
  auto t1 = results[pmc1];

  /* DebugLine("Total %s : %llu", pmc0.Name, t0); */
  /* DebugLine("Total %s : %llu", pmc1.Name, t1); */

  std::wcout << "Total " << pmc0.Name << "(" << t0 << ")" << std::endl;
  std::wcout << "Total " << pmc1.Name << "(" << t1 << ")" << std::endl;

  std::cout << "Ratio: " << SafeDivide0((double)t0, (double)t1) << std::endl;
}
#endif

s32
main( s32 ArgCount, const char ** Args )
{
  SetThreadLocal_ThreadIndex(0);

  Info("Initializing Bonsai");

#if 0
  /* for (const auto& pmc : query_available_pmc()) { */
  /*     std::wcout << pmc.Name << "(" << pmc.native_source << ")" << std::endl; */
  /* } */

  AMD_CheckSupportedCounters();

  pmc_kernel_session Session;
  /* Debug_BeginPMCMonitoring(&PMCSession); */

  /* pmc_counter pmc0{  L"DCMiss", 1, 50 }; */
  /* pmc_counter pmc1{  L"DCAccess", 1, 49 }; */

  pmc_counter pmc0{  L"DCacheMisses", 1, 8 };
  pmc_counter pmc1{  L"ICacheMisses", 1, 9 };

  /* pmc_counter pmc0{  L"ICMiss ", 1, 95 }; */
  /* pmc_counter pmc1{  L"ICFetch", 1, 94 }; */

  /* pmc_counter pmc0{  L"DCacheMisses", 1, 8 }; */
  /* pmc_counter pmc1{  L"DCacheAccesses", 1, 21 }; */

/*     pmc_counter pmc0{  L"ICacheMisses", 1, 9 }; */
/*     pmc_counter pmc1{  L"ICacheIssues", 1, 20 }; */

  /* Session.start({}); */
  /* Session.start({pmc0}); */
  Session.start({pmc0, pmc1});

#endif


  /* if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return 1; } */
  /* Info("Found Bonsai Root : %S", CS(GetCwd()) ); */

#if 1
  platform Plat = {};
  os Os         = {};
  engine_resources EngineResources = {};
  hotkeys Hotkeys = {};

  memory_arena BootstrapArena = {};

  EngineResources.Plat = &Plat;
  EngineResources.Os = &Os;
  EngineResources.Hotkeys = &Hotkeys;
  EngineResources.ThreadStates = Initialize_ThreadLocal_ThreadStates((s32)GetTotalThreadCount(), &EngineResources, &BootstrapArena);

  Global_ThreadStates = EngineResources.ThreadStates;

  s32 VSyncFrames = 0;
  if (!OpenAndInitializeWindow(&Os, &Plat, VSyncFrames)) { Error("Initializing Window :( "); return 1; }
  Assert(Os.GlContext);

  Ensure( InitializeOpenglFunctions() );

#if DEBUG_SYSTEM_API
  shared_lib DebugLib = InitializeBonsaiDebug("./bin/lib_debug_system_loadable" PLATFORM_RUNTIME_LIB_EXTENSION, Global_ThreadStates);
  Assert(DebugLib);
  Assert(Global_DebugStatePointer);
  EngineResources.DebugState = Global_DebugStatePointer;
#endif

  memory_arena *PlatMemory = AllocateArena();
  memory_arena *GameMemory = AllocateArena();

  DEBUG_REGISTER_ARENA(GameMemory, 0);
  DEBUG_REGISTER_ARENA(PlatMemory, 0);
  DEBUG_REGISTER_ARENA(&BootstrapArena, 0);

  PlatformInit(&Plat, PlatMemory);

#if BONSAI_INTERNAL
  // debug_recording_state *Debug_RecordingState = Allocate(debug_recording_state, GameMemory, 1);
  // AllocateAndInitializeArena(&Debug_RecordingState->RecordedMainMemory, Gigabytes(3));
#endif

#if EMCC ///////////////////////////////////// EMCC SHOULD COMPILE AND RUN CORRECTLY UP TO HERE
  return True;
#endif

  const char* GameLibName = "./bin/blank_project_loadable" PLATFORM_RUNTIME_LIB_EXTENSION;
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


#if DEBUG_SYSTEM_API
  GetDebugState()->SetRenderer(&EngineResources.Ui);
#endif

  LaunchWorkerThreads(&Plat, &EngineResources, &EngineApi, &GameApi);

  thread_local_state MainThread = DefaultThreadLocalState(&EngineResources, 0);

  if (GameApi.GameInit)
  {
    EngineResources.GameState = GameApi.GameInit(&EngineResources, &MainThread);
    if (!EngineResources.GameState) { Error("Initializing Game :( "); return 1; }
  }


  /*
   *  Main Game loop
   */

  r32 LastMs = 0;
  r32 RealDt = 0;
  while ( Os.ContinueRunning )
  {
    /* u32 CSwitchEventsThisFrame = CSwitchEventsPerFrame; */
    /* CSwitchEventsPerFrame = 0; */
    /* DebugLine("%u", CSwitchEventsThisFrame); */

    TIMED_BLOCK("Frame Preamble");

    ResetInputForFrameStart(&Plat.Input, &Hotkeys);

    if (Plat.dt > 0.1f)
    {
      Warn("DT exceeded 100ms, truncating to 33.33ms");
      Plat.dt = 0.03333f;
    }

    v2 LastMouseP = Plat.MouseP;
    while ( ProcessOsMessages(&Os, &Plat) );
    Plat.MouseDP = LastMouseP - Plat.MouseP;
    Plat.ScreenDim = V2(Plat.WindowWidth, Plat.WindowHeight);

    BindHotkeysToInput(&Hotkeys, &Plat.Input);

    // NOTE(Jesse): Must come after input has been processed
    UiFrameBegin(&EngineResources.Ui);

    DEBUG_FRAME_BEGIN(Hotkeys.Debug_ToggleMenu, Hotkeys.Debug_ToggleProfiling);

#if !EMCC
    if ( LibIsNew(GameLibName, &LastGameLibTime) )
    {
      Info("Reloading Game Lib");

      SignalAndWaitForWorkers(&Plat.WorkerThreadsSuspendFutex);

      CloseLibrary(GameLib);
      GameLib = OpenLibrary(GameLibName);

      Ensure(InitializeEngineApi(&EngineApi, GameLib));
      Ensure(InitializeGameApi(&GameApi, GameLib));

      Ensure( EngineApi.OnLibraryLoad(&EngineResources) );

      UnsignalFutex(&Plat.WorkerThreadsSuspendFutex);
      Info("Game Reload Success");
    }

#if DEBUG_SYSTEM_API
    if ( LibIsNew(DEFAULT_DEBUG_LIB, &LastDebugLibTime) )
    {
      SignalAndWaitForWorkers(&Plat.WorkerThreadsSuspendFutex);

      debug_state *Cached = Global_DebugStatePointer;
      Global_DebugStatePointer = 0;

      CloseLibrary(DebugLib);
      DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);

      bonsai_debug_api DebugApi = {};
      if (DebugLib)
      {
        if (InitializeBootstrapDebugApi(DebugLib, &DebugApi))
        {
          DebugApi.BonsaiDebug_OnLoad(Cached, Global_ThreadStates);
          Ensure( EngineApi.OnLibraryLoad(&EngineResources) );
        }
        else { Error("Initializing DebugLib API"); }
      }
      else { Error("Reloading DebugLib"); }

      Global_DebugStatePointer = Cached;

      UnsignalFutex(&Plat.WorkerThreadsSuspendFutex);
      Info("Debug lib Reload Success");
    }
#endif // DEBUG_SYSTEM_API

#endif // EMCC

    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    /* TIMED_BLOCK("Network Ops"); */
    /*   if (IsDisconnected(&Plat.Network)) { ConnectToServer(&Plat.Network); } */
    /* END_BLOCK("Network Ops"); */

    EngineApi.FrameBegin(&EngineResources);

    TIMED_BLOCK("GameMain");
      GameApi.GameMain(&EngineResources, &MainThread);
    END_BLOCK("GameMain");

    EngineApi.SimulateAndBufferGeometry(&EngineResources);

    DrainQueue(&Plat.HighPriority, &MainThread, &GameApi);
    WaitForWorkerThreads(&Plat.HighPriorityWorkerCount);

    EngineApi.Render(&EngineResources);

    DEBUG_FRAME_END(Plat.dt);

    // NOTE(Jesse): FrameEnd must come after the game geometry has rendered so
    // the alpha-blended text works properly
    EngineApi.FrameEnd(&EngineResources);

    BonsaiSwapBuffers(EngineResources.Os);

    thread_local_state *TLS = GetThreadLocalState(ThreadLocal_ThreadIndex);
    Ensure( RewindArena(TLS->TempMemory) );

    r32 CurrentMS = (r32)GetHighPrecisionClock();
    RealDt = (CurrentMS - LastMs)/1000.0f;
    LastMs = CurrentMS;
    Plat.dt = RealDt;
    Plat.GameTime += RealDt;

    MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(RealDt);
  }

#if 0
  Debug_EndAndReportPMCMonitoring(&PMCSession);
#endif

#if 0
  /* Session.stop(); */
  /* std::cout << "++++++RESULTS+++++++" << std::endl; */
  /* auto results = Session.results(); */

  auto t0 = results[pmc0];
  auto t1 = results[pmc1];

  /* DebugLine("Total %s : %llu", pmc0.Name, t0); */
  /* DebugLine("Total %s : %llu", pmc1.Name, t1); */

  std::wcout << "Total " << pmc0.Name << "(" << t0 << ")" << std::endl;
  std::wcout << "Total " << pmc1.Name << "(" << t1 << ")" << std::endl;

  std::cout << "Ratio: " << SafeDivide0((double)t0, (double)t1) << std::endl;

#endif

  Info("Shutting Down");

  SignalAndWaitForWorkers(&Plat.WorkerThreadsExitFutex);
  UnsignalFutex(&Plat.WorkerThreadsExitFutex);

  Terminate(&Os, &Plat);

  Info("Exiting");

  return 0;
#endif
}
