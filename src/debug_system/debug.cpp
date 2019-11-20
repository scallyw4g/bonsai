#if BONSAI_INTERNAL

#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <heap_memory_types.cpp>

#include <debug_data_system.cpp>

// FIXME(Jesse): we now have camera code in here that needs to know this value
// for realz!
global_variable chunk_dimension WORLD_CHUNK_DIM = Chunk_Dimension(32,32,16);
//

#include <canonical_position.cpp>
#include <render_position.cpp>
#include <camera.cpp>

global_variable memory_arena* TranArena = AllocateArena();
#include <interactable.cpp>
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

#if 1
  TIMED_BLOCK("Draw Status Bar");
    PushTableStart(UiGroup);

    /* buffer_value_params StatusBarBufferParams = BufferValueParams(0, &Layout); */

    Dt = ComputeMinMaxAvgDt();
    PushColumn(UiGroup, CS(Dt.Max));
    PushNewRow(UiGroup);

    PushColumn(UiGroup, CS(Dt.Avg));
    PushColumn(UiGroup, CS(Plat->dt*1000.0f));
    PushColumn(UiGroup, CS("ms"));

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      PushColumn(UiGroup, CS(TotalStats.Allocations));
      PushColumn(UiGroup, CS("Allocations"));

      PushColumn(UiGroup, CS(TotalStats.Pushes));
      PushColumn(UiGroup, CS("Pushes"));

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

      PushColumn(UiGroup, CS(TotalDrawCalls));
      PushColumn(UiGroup, CS("Draw Calls"));

      PushNewRow(UiGroup);
    }

    PushColumn(UiGroup, CS(Dt.Min));

    PushTableEnd(UiGroup);
  END_BLOCK("Status Bar");

  PushNewRow(UiGroup);
#endif

  if (DebugState->DisplayDebugMenu)
  {
    /* buffer_value_params DebugMenuParams = BufferValueParams(0, &Layout, UiStyleFromLightestColor(V3(1), V3(0.3f), V2(30))); */

    PushTableStart(UiGroup);

    if (Button(UiGroup, CS("PickedChunks"), (umm)"PickedChunks"))
    {
      DebugState->UIType = DebugUIType_PickedChunks;
    }

    if (Button(UiGroup, CS("Graphics"), (umm)"Graphics"))
    {
      DebugState->UIType = DebugUIType_Graphics;
    }

    if (Button(UiGroup, CS("Network"), (umm)"Network"))
    {
      DebugState->UIType = DebugUIType_Network;
    }

    if (Button(UiGroup, CS("Functions"), (umm)"Functions"))
    {
      DebugState->UIType = DebugUIType_CollatedFunctionCalls;
    }

    if (Button(UiGroup, CS("Callgraph"), (umm)"Callgraph"))
    {
      DebugState->UIType = DebugUIType_CallGraph;
    }

    if (Button(UiGroup, CS("Memory"), (umm)"Memory"))
    {
      DebugState->UIType = DebugUIType_Memory;
    }

    if (Button(UiGroup, CS("DrawCalls"), (umm)"DrawCalls"))
    {
      DebugState->UIType = DebugUIType_Memory;
    }

    PushTableEnd(UiGroup);
    PushNewRow(UiGroup);
  }
  else
  {
    DebugState->UIType = DebugUIType_None;
  }


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
    UiGroup->PressedInteraction = Interactable(V2(FLT_MIN), V2(FLT_MAX), StringHash("GameViewport"), 0);
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
