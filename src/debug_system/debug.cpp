#if BONSAI_INTERNAL

#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <heap_memory_types.cpp>

#include <debug_data_system.cpp>
#include <interactable.cpp>

// FIXME(Jesse): we now have camera code in here that needs to know this value
// for realz!
global_variable chunk_dimension WORLD_CHUNK_DIM = Chunk_Dimension(32,32,16);
//

#include <canonical_position.cpp>
#include <render_position.cpp>
#include <camera.cpp>

global_variable memory_arena* TranArena = AllocateArena();
#include <debug_render_system.cpp>

global_variable debug_state Internal_DebugState = {};
global_variable interactable NullInteraction = {};

function void
DebugFrameEnd(platform *Plat, server_state* ServerState)
{
  TIMED_FUNCTION();

  debug_state *DebugState            = GetDebugState();

  min_max_avg_dt Dt           = {};

  debug_ui_render_group *UiGroup = &DebugState->UiGroup;

  UiGroup->GameGeo               = &DebugState->GameGeo;
  UiGroup->GameGeoShader         = &DebugState->GameGeoShader;
  UiGroup->Input                 = &Plat->Input;
  UiGroup->ScreenDim             = V2(Plat->WindowWidth, Plat->WindowHeight);
  UiGroup->MouseP                = &Plat->MouseP;
  UiGroup->MouseDP               = &Plat->MouseDP;

  if ( ! (Plat->Input.LMB.Pressed || Plat->Input.RMB.Pressed))
  {
    UiGroup->PressedInteraction = NullInteraction;
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

  /* ui_style Style =  UiStyleFromLightestColor(V3(1), V4(0,0,35,0)); */

  ui_element_reference DtTable = PushTableStart(UiGroup);
    StartColumn(UiGroup);
      Text(UiGroup,  CS("+"));
      Text(UiGroup, CS(Dt.Max - Dt.Avg));
    PushNewRow(UiGroup);

    StartColumn(UiGroup);
      Text(UiGroup, CS(" "));
      Text(UiGroup, CS(Dt.Avg));

    PushColumn(UiGroup, CS(Plat->dt*1000.0f));
    PushNewRow(UiGroup);

    StartColumn(UiGroup);
      Text(UiGroup, CS("-"));
      Text(UiGroup, CS(Dt.Avg - Dt.Min));
  PushTableEnd(UiGroup);

  PushTableStart(UiGroup, Position_RightOf, DtTable);
    PushColumn(UiGroup, CS("Allocations"));
    PushColumn(UiGroup, CS("Pushes"));
    PushColumn(UiGroup, CS("Draw Calls"));
    PushNewRow(UiGroup);
    PushColumn(UiGroup, CS(TotalStats.Allocations));
    PushColumn(UiGroup, CS(TotalStats.Pushes));
    PushColumn(UiGroup, CS(TotalDrawCalls));
    PushNewRow(UiGroup);
  PushTableEnd(UiGroup);

  END_BLOCK("Status Bar");


  if (DebugState->DisplayDebugMenu)
  {
    ui_style Style =  UiStyleFromLightestColor(V3(1), V2(50));
    PushTableStart(UiGroup);

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("PickedChunks"), (umm)"PickedChunks", &Style))
    {
      DebugState->UIType = DebugUIType_PickedChunks;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("Graphics"), (umm)"Graphics", &Style))
    {
      DebugState->UIType = DebugUIType_Graphics;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("Network"), (umm)"Network", &Style))
    {
      DebugState->UIType = DebugUIType_Network;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("Functions"), (umm)"Functions", &Style))
    {
      DebugState->UIType = DebugUIType_CollatedFunctionCalls;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("Callgraph"), (umm)"Callgraph", &Style))
    {
      DebugState->UIType = DebugUIType_CallGraph;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("Memory"), (umm)"Memory", &Style))
    {
      DebugState->UIType = DebugUIType_Memory;
    }

    StartColumn(UiGroup);
    if (Button(UiGroup, CS("DrawCalls"), (umm)"DrawCalls", &Style))
    {
      DebugState->UIType = DebugUIType_Memory;
    }

    PushTableEnd(UiGroup);

    switch (DebugState->UIType)
    {
      case DebugUIType_None:
      {
      } break;

      case DebugUIType_PickedChunks:
      {
        DrawPickedChunks(UiGroup);
      } break;

      case DebugUIType_Graphics:
      {
        DebugDrawGraphicsHud(UiGroup, DebugState);
      } break;

      case DebugUIType_Network:
      {
        DebugDrawNetworkHud(UiGroup, &Plat->Network, ServerState);
      } break;

      case DebugUIType_CollatedFunctionCalls:
      {
        DebugDrawCollatedFunctionCalls(UiGroup, DebugState);
      } break;

      case DebugUIType_CallGraph:
      {
        DebugDrawCallGraph(UiGroup, DebugState, Dt.Max);
      } break;

      case DebugUIType_Memory:
      {
        DebugDrawMemoryHud(UiGroup, DebugState);
      } break;

      case DebugUIType_DrawCalls:
      {
        DebugDrawDrawCalls(UiGroup);
      } break;

      InvalidDefaultCase;
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

  if (UiGroup->PressedInteraction.ID == 0 &&
      (Plat->Input.LMB.Pressed || Plat->Input.RMB.Pressed))
  {
    UiGroup->PressedInteraction = Interactable(InvertedInfinityRectangle(), StringHash("GameViewport"), 0);
  }

  if (DebugState->DoChunkPicking && Plat->Input.LMB.Clicked)
  {
    DebugState->DoChunkPicking = False;
  }

  RewindArena(TranArena);

  return;
}

function void
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

exported_function debug_state*
GetDebugState_Internal();

exported_function void
InitDebugSystem(b32 DoInitDebugRenderSystem = True)
{
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

  Internal_DebugState.Initialized = True;

  GetDebugState = GetDebugState_Internal;

  heap_allocator Heap = InitHeap(Megabytes(128));
  InitDebugDataSystem(&Internal_DebugState);
  if (DoInitDebugRenderSystem) { InitDebugRenderSystem(&Internal_DebugState, &Heap); }
  return;
}

exported_function debug_state*
GetDebugState_Internal()
{
  if (!Internal_DebugState.Initialized)
  {
    GetDebugState = GetDebugState_Internal;
    InitDebugSystem();
  }

  return &Internal_DebugState;
}

#endif
