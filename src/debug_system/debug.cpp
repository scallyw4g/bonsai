#define DEBUG_LIB_INTERNAL_BUILD 1
#include <bonsai_types.h>

#include <debug_data_system.cpp>
#include <interactable.cpp>
#include <debug_render_system.cpp>

global_variable debug_state Internal_DebugState = {};

bonsai_function void
DebugFrameEnd(platform *Plat, server_state* ServerState = 0)
{
  TIMED_FUNCTION();

  debug_state *DebugState = GetDebugState();

  min_max_avg_dt Dt = {};

  debug_ui_render_group *UiGroup = &DebugState->UiGroup;

  UiGroup->GameGeo               = &DebugState->GameGeo;
  UiGroup->GameGeoShader         = &DebugState->GameGeoShader;
  UiGroup->Input                 = &Plat->Input;
  UiGroup->ScreenDim             = V2(Plat->WindowWidth, Plat->WindowHeight);
  UiGroup->MouseP                = &Plat->MouseP;
  UiGroup->MouseDP               = &Plat->MouseDP;

  if ( ! (Plat->Input.LMB.Pressed || Plat->Input.RMB.Pressed) )
  {
    UiGroup->PressedInteractionId = 0;
  }

  TIMED_BLOCK("Draw Status Bar");

  memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

  u32 TotalDrawCalls = 0;

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
    debug_draw_call *Call = &Global_DrawCalls[DrawCountIndex];
    if (Call->Caller)
    {
      TotalDrawCalls += Call->Calls;
    }
  }

  Dt = ComputeMinMaxAvgDt();

  v4 Padding = V4(0,0,50,0);
  ui_style Style = UiStyleFromLightestColor(V3(1));

  ui_element_reference DtTable = PushTableStart(UiGroup);
    StartColumn(UiGroup, &Style, Padding);
      Text(UiGroup, CS("+"));
      Text(UiGroup, CS(Dt.Max - Dt.Avg));
    EndColumn(UiGroup);

    PushNewRow(UiGroup);

    PushColumn(UiGroup, CS(Dt.Avg), &Style, Padding);
    PushColumn(UiGroup, CS(Plat->dt*1000.0f));

    PushNewRow(UiGroup);

    StartColumn(UiGroup, &Style, Padding);
      Text(UiGroup, CS("-"));
      Text(UiGroup, CS(Dt.Avg - Dt.Min));
    EndColumn(UiGroup);

  PushTableEnd(UiGroup);

  PushTableStart(UiGroup, Position_RightOf, DtTable);
    StartColumn(UiGroup, &Style, Padding);
      Text(UiGroup, CS("Allocations"));
    EndColumn(UiGroup);

    PushColumn(UiGroup, CS("Pushes"));
    PushColumn(UiGroup, CS("Draw Calls"));
    PushNewRow(UiGroup);

    PushColumn(UiGroup, CS(TotalStats.Allocations), &Style, Padding);
    PushColumn(UiGroup, CS(TotalStats.Pushes));
    PushColumn(UiGroup, CS(TotalDrawCalls));
    PushNewRow(UiGroup);
  PushTableEnd(UiGroup);

  END_BLOCK("Status Bar");


  if (DebugState->DisplayDebugMenu)
  {
    v4 Padding = V4(25);
    ui_style Style =  UiStyleFromLightestColor(V3(1));

    PushTableStart(UiGroup);
    if (Button(UiGroup, CS("PickedChunks"), (umm)"PickedChunks", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_PickedChunks);
    }

    if (Button(UiGroup, CS("Graphics"), (umm)"Graphics", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_Graphics);
    }

    if (Button(UiGroup, CS("Network"), (umm)"Network", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_Network);
    }

    if (Button(UiGroup, CS("Functions"), (umm)"Functions", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_CollatedFunctionCalls);
    }

    if (Button(UiGroup, CS("Callgraph"), (umm)"Callgraph", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_CallGraph);
    }

    if (Button(UiGroup, CS("Memory"), (umm)"Memory", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_Memory);
    }

    if (Button(UiGroup, CS("DrawCalls"), (umm)"DrawCalls", &Style, Padding))
    {
      ToggleBitfieldValue(DebugState->UIType, DebugUIType_DrawCalls);
    }
    PushTableEnd(UiGroup);




    if (DebugState->UIType & DebugUIType_PickedChunks)
    {
      DrawPickedChunks(UiGroup);
    }

    if (DebugState->UIType & DebugUIType_Graphics)
    {
      DebugDrawGraphicsHud(UiGroup, DebugState);
    }

    /* if (DebugState->UIType & DebugUIType_Network) */
    /* { */
    /*   DebugDrawNetworkHud(UiGroup, &Plat->Network, ServerState); */
    /* } */

    if (DebugState->UIType & DebugUIType_CollatedFunctionCalls)
    {
      DebugDrawCollatedFunctionCalls(UiGroup, DebugState);
    }

    if (DebugState->UIType & DebugUIType_CallGraph)
    {
      DebugDrawCallGraph(UiGroup, DebugState, Dt.Max);
    }

    if (DebugState->UIType & DebugUIType_Memory)
    {
      DebugDrawMemoryHud(UiGroup, DebugState);
    }

    if (DebugState->UIType & DebugUIType_DrawCalls)
    {
      DebugDrawDrawCalls(UiGroup);
    }

  }

  UiGroup->HighestWindow = GetHighestWindow(UiGroup, UiGroup->CommandBuffer);
  FlushCommandBuffer(UiGroup, UiGroup->CommandBuffer);

  FlushBuffers(UiGroup, UiGroup->ScreenDim);

  DebugState->BytesBufferedToCard = 0;

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     Global_DrawCalls[DrawCountIndex] = NullDrawCall;
  }

  for ( u32 FunctionIndex = 0;
      FunctionIndex < MAX_RECORDED_FUNCTION_CALLS;
      ++FunctionIndex)
  {
    ProgramFunctionCalls[FunctionIndex] = NullFunctionCall;
  }

  if (UiGroup->PressedInteractionId == 0 &&
      (Plat->Input.LMB.Pressed || Plat->Input.RMB.Pressed))
  {
    UiGroup->PressedInteractionId = StringHash("GameViewport");
  }

  if (DebugState->DoChunkPicking && Plat->Input.LMB.Clicked)
  {
    DebugState->DoChunkPicking = False;
  }

  return;
}

bonsai_function void
DebugFrameBegin(hotkeys *Hotkeys)
{
  debug_state *State = GetDebugState();

  if ( Hotkeys->Debug_RedrawEveryPush )
  {
     State->Debug_RedrawEveryPush = !State->Debug_RedrawEveryPush;
  }

  if ( Hotkeys->Debug_ToggleTriggeredRuntimeBreak )
  {
    Hotkeys->Debug_ToggleTriggeredRuntimeBreak = False;
    State->TriggerRuntimeBreak = !State->TriggerRuntimeBreak;
  }

  if ( Hotkeys->Debug_ToggleProfile )
  {
    Hotkeys->Debug_ToggleProfile = False;
    State->DebugDoScopeProfiling = !State->DebugDoScopeProfiling;
  }

  if ( Hotkeys->Debug_ToggleMenu )
  {
    Hotkeys->Debug_ToggleMenu = False;
    State->DisplayDebugMenu = !State->DisplayDebugMenu;
  }

  if (Hotkeys->Debug_MousePick)
  {
    State->DoChunkPicking = True;
  }

  return;
}

link_internal debug_state*
GetDebugState()
{
  debug_state *Result = 0;
  if (Internal_DebugState.Initialized)
  {
    Result = &Internal_DebugState;
  }
  else
  {
    // It's an error to call this without first initializing the debug_state.
    //
    // At the time of this writing we're unable to write to stdout here because
    // that path calls "FormatCountedString_", which is a TIMED_FUNCTION, which
    // interally calls GetDebugState.
    //
    // This path only happens when we compile the entire debug lib statically
    // inside another app (tests/ui_command_buffer.cpp), which should go away
    // eventaully.  When it does, this will no longer have to check Initialized
  }

  return Result;
}

dll_export get_debug_state_proc
InitDebugSystem(opengl *LoadedGLImpl)
{
  LastMs = GetHighPrecisionClock();
  Internal_DebugState.Frames[1].StartingCycle = GetCycleCount();

  Internal_DebugState.FrameEnd                        = DebugFrameEnd;
  Internal_DebugState.FrameBegin                      = DebugFrameBegin;
  Internal_DebugState.RegisterArena                   = RegisterArena;
  Internal_DebugState.WorkerThreadAdvanceDebugSystem  = WorkerThreadAdvanceDebugSystem;
  Internal_DebugState.MainThreadAdvanceDebugSystem    = MainThreadAdvanceDebugSystem;
  Internal_DebugState.MutexWait                       = MutexWait;
  Internal_DebugState.MutexAquired                    = MutexAquired;
  Internal_DebugState.MutexReleased                   = MutexReleased;
  Internal_DebugState.GetProfileScope                 = GetProfileScope;
  Internal_DebugState.Debug_Allocate                  = DEBUG_Allocate;
  Internal_DebugState.RegisterThread                  = RegisterThread;
  Internal_DebugState.ClearMetaRecordsFor             = ClearMetaRecordsFor;
  Internal_DebugState.TrackDrawCall                   = TrackDrawCall;
  Internal_DebugState.GetThreadLocalState             = GetThreadLocalState;
  Internal_DebugState.PickChunk                       = PickChunk;
  Internal_DebugState.ComputePickRay                  = ComputePickRay;
  Internal_DebugState.DebugValue                      = DebugValue;
  Internal_DebugState.DumpScopeTreeDataToConsole      = DumpScopeTreeDataToConsole;
  Internal_DebugState.OpenDebugWindowAndLetUsDoStuff  = OpenDebugWindowAndLetUsDoStuff;

  Internal_DebugState.Initialized = True;

  InitDebugDataSystem(&Internal_DebugState);

  if (LoadedGLImpl) {
    Assert(LoadedGLImpl->Initialized);
    heap_allocator Heap = InitHeap(Megabytes(128));
    InitDebugRenderSystem(&Internal_DebugState, &Heap, LoadedGLImpl);
  }

  return GetDebugState;
}
