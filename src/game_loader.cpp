#define PLATFORM_WINDOW_IMPLEMENTATIONS 1
#define BONSAI_STDLIB_WORK_QUEUE_IMPLEMENTATION 1

#define BONSAI_DEBUG_SYSTEM_API 1
#define BONSAI_DEBUG_SYSTEM_LOADER_API 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <engine/engine.h>
#include <engine/engine.cpp>

global_variable s64 LastGameLibTime;
global_variable s64 LastDebugLibTime;

/* #include <bonsai_debug/headers/win32_pmc.cpp> */


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

link_internal void
PrintFiles(file_traversal_node *Node)
{
  switch (Node->Type)
  {
    InvalidCase(FileTraversalType_None);

    case FileTraversalType_File:
    {
      DebugLine("File (%S)(%S)", Node->Dir, Node->Name);
    } break;

    case FileTraversalType_Dir:
    {
      DebugLine("Dir  (%S)(%S)", Node->Dir, Node->Name);
    } break;
  }
}

global_variable const char *
Global_ProjectSwitcherGameLibName = "./bin/game_libs/project_and_level_picker_loadable" PLATFORM_RUNTIME_LIB_EXTENSION;


s32
main( s32 ArgCount, const char ** Args )
{
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
  // pmc_counter pmc0{  L"ICacheMisses", 1, 9 };
  // pmc_counter pmc1{  L"ICacheIssues", 1, 20 };
  /* Session.start({}); */
  /* Session.start({pmc0}); */
  Session.start({pmc0, pmc1});
#endif

  if (!SearchForProjectRoot()) { Error("Couldn't find root dir, exiting."); return 1; }
  Info("Found Bonsai Root : %s", GetCwd() );

  engine_resources EngineResources_ = {};
  engine_resources *EngineResources = &EngineResources_;
  Global_EngineResources = EngineResources;

  const char* GameLibName = Global_ProjectSwitcherGameLibName;
  switch (ArgCount)
  {
    case 1:  {} break;
    case 2:  { GameLibName = Args[1]; } break;
    default: { Error("Invalid number of arguments"); } break;
  }


  // First we load DLLs and initialize API structs.  We have to do this first
  // because InitializeBonsaiStdlib requires that we pass the GameApi so it can
  // kick off the worker threads.
  //
  shared_lib       GameLib   = {};
  application_api *GameApi   = &EngineResources->GameApi;
  engine_api       EngineApi = {};
  {
    FileIsNew(GameLibName, &LastGameLibTime); // Hack to initialize the lib timer statics
    FileIsNew(DEFAULT_DEBUG_LIB, &LastDebugLibTime);

    GameLib = OpenLibrary(GameLibName);
    if (!GameLib) { Error("Loading GameLib :( "); return 1; }

    if (!InitializeGameApi(GameApi, GameLib)) { Error("Initializing GameApi :( "); return 1; }

    if (!InitializeEngineApi(&EngineApi, GameLib)) { Error("Initializing EngineApi :( "); return 1; }
  }


  memory_arena BootstrapArena = {};
  memory_arena *GameMemory = AllocateArena();


  {
    temp_memory_handle TempHandle = BeginTemporaryMemory(&BootstrapArena);
    EngineResources->Settings = ParseEngineSettings(CSz("settings.init"), &BootstrapArena);
  }

  EngineResources->Stdlib.Plat.ScreenDim = V2(SettingToValue(EngineResources->Settings.Graphics.WindowStartingSize));

  thread_main_callback_type Procs[2] = { RenderThread_Main, WorldUpdateThread_Main };
  thread_main_callback_type_buffer CustomWorkerProcs = {};
  CustomWorkerProcs.Start = Procs;
  CustomWorkerProcs.Count = 2;

  Ensure( InitializeBonsaiStdlib( bonsai_init_flags(BonsaiInit_OpenWindow|BonsaiInit_LaunchThreadPool|BonsaiInit_InitDebugSystem),
                                  GameApi,
                                  &EngineResources->Stdlib,
                                  &BootstrapArena,
                                  &CustomWorkerProcs ));

  while (EngineResources->Graphics.Initialized == False) { SleepMs(1); }

  /* EngineResources->DebugState = Global_DebugStatePointer; */

  Assert(EngineResources->Stdlib.ThreadStates);
  Assert(Global_ThreadStates);

  Ensure( EngineApi.OnLibraryLoad(EngineResources) );
  Ensure( Bonsai_Init(EngineResources) ); // <-- EngineResources now initialized

#if BONSAI_DEBUG_SYSTEM_API
  GetDebugState()->SetRenderer(&EngineResources->Ui);
#endif

  UNPACK_STDLIB(&EngineResources->Stdlib);

  memory_arena *WorkQueueMemory = AllocateArena();
  InitQueue(&Plat->HighPriority, WorkQueueMemory);
  InitQueue(&Plat->LowPriority,  WorkQueueMemory);
  InitQueue(&Plat->RenderQ,      WorkQueueMemory);
  InitQueue(&Plat->WorldUpdateQ, WorkQueueMemory);

  DEBUG_REGISTER_ARENA(GameMemory, 0);
  DEBUG_REGISTER_ARENA(WorkQueueMemory, 0);
  DEBUG_REGISTER_ARENA(&BootstrapArena, 0);

  thread_local_state *MainThread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  if (GameApi->GameInit)
  {
#if PLATFORM_WINDOW_IMPLEMENTATIONS
    // Block till RenderMain is initialized in case game wants to do rendering things in init..?
    // TODO(Jesse): Is there any reason to actually do this?
    while (EngineResources->Graphics.Initialized == False) { SleepMs(1); }
#endif
    EngineResources->GameState = GameApi->GameInit(EngineResources, MainThread);
    if (!EngineResources->GameState) { Error("Initializing Game :( "); return 1; }
  }


  /*
   *  Main Game loop
   */

  /* SleepMs(2000); */

  r32 LastMs = 0;
  while (Os->ContinueRunning)
  {
    /* u32 CSwitchEventsThisFrame = CSwitchEventsPerFrame; */
    /* CSwitchEventsPerFrame = 0; */
    /* DebugLine("%u", CSwitchEventsThisFrame); */

    TIMED_BLOCK("Frame Preamble");

    //
    // Input processing
    //
    {
      ResetInputForFrameStart(&Plat->Input, &EngineResources->Hotkeys);

      v2 LastMouseP = Plat->MouseP;
      while ( ProcessOsMessages(Os, Plat) );
      Plat->MouseDP = LastMouseP - Plat->MouseP;
      /* Assert(Plat->ScreenDim.x > 0); */
      /* Assert(Plat->ScreenDim.y > 0); */

      BindHotkeysToInput(&EngineResources->Hotkeys, &Plat->Input);

      /* if (Input->F12.Pressed) { EngineDebug->TriggerRuntimeBreak = True; } */
    }

    if (Plat->dt > 0.1f)
    {
      Warn("DT exceeded 100ms, truncating to 33.33ms");
      Plat->dt = 0.03333f;
    }

    if (Plat->Input.Escape.Clicked)
    {
      if (StringsMatch(GameLibName, Global_ProjectSwitcherGameLibName))
      {
        Os->ContinueRunning = False;
        break;
      }
      else
      {
        EngineResources->RequestedGameLibReloadBehavior = GameLibReloadBehavior_FullInitialize;
        GameLibName = Global_ProjectSwitcherGameLibName;
        LastGameLibTime = 0;
      }
    }

    DEBUG_FRAME_BEGIN(&EngineResources->Ui, Plat->dt, EngineResources->Hotkeys.Debug_ToggleMenu, EngineResources->Hotkeys.Debug_ToggleProfiling);

#if !EMCC
    if ( FileIsNew(GameLibName, &LastGameLibTime) )
    {
      Info("Reloading Game Lib");

      SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);

      DuplicateMetaTableNameStrings(EngineResources);

      CloseLibrary(GameLib);
      GameLib = OpenLibrary(GameLibName);

      Ensure(InitializeEngineApi(&EngineApi, GameLib));
      Ensure(InitializeGameApi(GameApi, GameLib));

      // Hook up global pointers
      Ensure( EngineApi.OnLibraryLoad(EngineResources) );

      if (EngineResources->RequestedGameLibReloadBehavior & GameLibReloadBehavior_FullInitialize)
      {
        EngineResources->RequestedGameLibReloadBehavior = game_lib_reload_behavior(EngineResources->RequestedGameLibReloadBehavior & ~GameLibReloadBehavior_FullInitialize);

        HardResetEngine(EngineResources);
        /* ApplyEditBufferToOctree(Engine, &Editor->WorldEdits); */

        EngineResources->GameState = GameApi->GameInit(EngineResources, MainThread);
        if (!EngineResources->GameState) { Error("Initializing Game :( "); return 1; }
      }

      // Do game-specific reload code
      if (GameApi->OnLibraryLoad) { GameApi->OnLibraryLoad(EngineResources, MainThread); }

      UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
      Info("Game Reload Success");
    }

#endif // EMCC

    /* DEBUG_FRAME_RECORD(Debug_RecordingState, &Hotkeys); */

    END_BLOCK("Frame Preamble");

    /* TIMED_BLOCK("Network Ops"); */
    /*   if (IsDisconnected(&Plat->Network)) { ConnectToServer(&Plat->Network); } */
    /* END_BLOCK("Network Ops"); */

    EngineApi.FrameBegin(EngineResources);

      TIMED_BLOCK("GameMain");
        GameApi->GameMain(EngineResources, MainThread);
      END_BLOCK("GameMain");

      EngineApi.Simulate(EngineResources);

      DrainQueue(&Plat->HighPriority, MainThread, GameApi);
      WaitForWorkerThreads(&Plat->HighPriorityWorkerCount);

      EngineApi.Render(EngineResources);

      DEBUG_FRAME_END(Plat->dt);

    EngineApi.FrameEnd(EngineResources);


    // NOTE(Jesse): We can't hold strings from PlatformTraverseDirectoryTreeUnordered
    // across frame boundaries because transient memory gets cleared, so this
    // has to happen before the end of the frame.
    if ( EngineResources->RequestedGameLibReloadNode.Name.Count )
    {
      LastGameLibTime = 0;
      // TODO(Jesse)(leak): We probably don't want to just leak these strings..
      GameLibName = ConcatZ( EngineResources->RequestedGameLibReloadNode.Dir, CSz("/"), EngineResources->RequestedGameLibReloadNode.Name, &BootstrapArena );

      EngineResources->RequestedGameLibReloadNode = {};
      EngineResources->RequestedGameLibReloadBehavior = GameLibReloadBehavior_FullInitialize;
    }


    Assert(ThreadLocal_ThreadIndex == 0);
    thread_local_state *TLS = GetThreadLocalState(ThreadLocal_ThreadIndex);
    Ensure( RewindArena(TLS->TempMemory) );

    r32 CurrentMS = (r32)GetHighPrecisionClock();
    r32 RealDt = (CurrentMS - LastMs)/1000.0f;
    LastMs = CurrentMS;
    Plat->dt = RealDt;
    Plat->GameTime += RealDt;

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

  SignalAndWaitForWorkers(&Plat->WorkerThreadsExitFutex);
  UnsignalFutex(&Plat->WorkerThreadsExitFutex);

  Terminate(Os, Plat);

  Info("Exiting");

  return 0;
}
