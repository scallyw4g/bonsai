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

global_variable memory_arena* TranArena = PlatformAllocateArena();
#include <interactable.cpp>
#include <debug_render_system.cpp>

global_variable debug_state Internal_DebugState = {};
global_variable interactable NullInteraction = {};

function void
DebugFrameEnd(platform *Plat, server_state* ServerState)
{
  TIMED_FUNCTION();

  debug_state *DebugState            = GetDebugState();
  debug_text_render_group *TextGroup = &DebugState->TextRenderGroup;

  min_max_avg_dt Dt           = {};
  layout Layout               = {};

  debug_ui_render_group *UiGroup = &DebugState->UiGroup;

  UiGroup->TextGroup             = TextGroup;
  UiGroup->GameGeo               = &DebugState->GameGeo;
  UiGroup->GameGeoShader         = &DebugState->GameGeoShader;
  UiGroup->Input                 = &Plat->Input;
  UiGroup->ScreenDim             = V2(Plat->WindowWidth, Plat->WindowHeight);
  UiGroup->MouseP                = &Plat->MouseP;
  UiGroup->MouseDP               = &Plat->MouseDP;

  if ( ! (Plat->Input.LMB.IsDown || Plat->Input.RMB.IsDown))
  {
    UiGroup->PressedInteraction = NullInteraction;
  }

  SetFontSize(&UiGroup->Font, DEBUG_FONT_SIZE);

  TIMED_BLOCK("Draw Status Bar");
    Dt = ComputeMinMaxAvgDt();
    BufferColumn(Dt.Max, 6, UiGroup, &Layout, WHITE);
    NewLine(&Layout);

    BufferColumn(Dt.Avg, 6, UiGroup, &Layout, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, UiGroup, &Layout, WHITE);
    BufferValue("ms", UiGroup, &Layout, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, UiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &UiGroup->Font);
      BufferValue("Allocations", UiGroup, &Layout, WHITE);

      BufferThousands(TotalStats.Pushes, UiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &UiGroup->Font);
      BufferValue("Pushes", UiGroup, &Layout, WHITE);

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

      BufferColumn(TotalDrawCalls, 6, UiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &UiGroup->Font);
      BufferValue("Draw Calls", UiGroup, &Layout, WHITE);

      NewLine(&Layout);
    }

    BufferColumn(Dt.Min, 6, UiGroup, &Layout, WHITE);
  END_BLOCK("Status Bar");

  SetFontSize(&UiGroup->Font, 32);
  NewLine(&Layout);
  NewLine(&Layout);


  ui_style ButtonStyling = {};
  ButtonStyling.Padding = V2(65.0f);
  ButtonStyling.Color = V3(1.0f);
  ButtonStyling.HoverColor = V3(0.65f);
  ButtonStyling.ActiveColor = V3(0.65f);
  ButtonStyling.ClickColor = V3(1, 0, 0);

  if (DebugState->DisplayDebugMenu)
  {
    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_PickedChunks;
    const char* ButtonName = "PickedChunks";
    umm InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_PickedChunks;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_Graphics;
    ButtonName = "Graphics";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_Graphics;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_Network;
    ButtonName = "Network";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_Network;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_CollatedFunctionCalls;
    ButtonName = "Functions";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_CollatedFunctionCalls;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_CallGraph;
    ButtonName = "Call Graph";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_CallGraph;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_Memory;
    ButtonName = "Memory";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_Memory;
    }

    ButtonStyling.IsActive = DebugState->UIType == DebugUIType_DrawCalls;
    ButtonName = "Draw Calls";
    InteractionId = (umm)ButtonName;
    if (Button(ButtonName, UiGroup, &Layout, InteractionId, V2(0), &ButtonStyling))
    {
      DebugState->UIType = DebugUIType_DrawCalls;
    }

    NewLine(&Layout);
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
      DrawPickedChunks(UiGroup, Layout.At + V2(100, 0));
    } break;

    case DebugUIType_Graphics:
    {
      DebugDrawGraphicsHud(UiGroup, DebugState, &Layout);
    } break;

    case DebugUIType_Network:
    {
      DebugDrawNetworkHud(UiGroup, &Plat->Network, ServerState, &Layout);
    } break;

    case DebugUIType_CollatedFunctionCalls:
    {
      DebugDrawCollatedFunctionCalls(UiGroup, DebugState, Layout.At);
    } break;

    case DebugUIType_CallGraph:
    {
      rect2 CycleGraphClip = DebugDrawCallGraph(UiGroup, DebugState, &Layout, Dt.Max);
      DebugDrawCycleThreadGraph(UiGroup, DebugState, V2(CycleGraphClip.Max.x + 20, Layout.At.y));
    } break;

    case DebugUIType_Memory:
    {
      local_persist window_layout MemoryArenaWindow = WindowLayout(Layout.At);
      DebugDrawMemoryHud(UiGroup, DebugState, &MemoryArenaWindow);
    } break;

    case DebugUIType_DrawCalls:
    {
      DebugDrawDrawCalls(UiGroup, &Layout);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(TextGroup, &TextGroup->UIGeo, UiGroup->ScreenDim);
  FlushBuffer(TextGroup, &TextGroup->TextGeo, UiGroup->ScreenDim);

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
      (Plat->Input.LMB.IsDown || Plat->Input.RMB.IsDown))
  {
    UiGroup->PressedInteraction = Interactable(V2(FLT_MIN), V2(FLT_MAX), StringHash("GameViewport") );
  }

  if (DebugState->DoChunkPicking && Plat->Input.LMB.WasPressed)
  {
    DebugState->DoChunkPicking = False;
  }

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
