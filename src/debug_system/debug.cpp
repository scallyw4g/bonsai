#if BONSAI_INTERNAL


#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <heap_memory_types.cpp>

#include <debug_data_system.cpp>


// FIXME(Jesse): this is a hack so that we can use BindShaderUniforms.  The
// Debug system never actually binds a camera, so there's not any reason for
// this to be required for it to use that function, but C isn't smart enough to
// figure that out.
global_variable chunk_dimension WORLD_CHUNK_DIM = Chunk_Dimension(0,0,0);
//

global_variable memory_arena* TranArena = PlatformAllocateArena();
#include <debug_render_system.cpp>

global_variable debug_state Internal_DebugState = {};

function void
DebugFrameEnd(platform *Plat, server_state* ServerState)
{
  TIMED_FUNCTION();

  debug_state *DebugState              = GetDebugState();
  debug_text_render_group *RG          = &DebugState->TextRenderGroup;
  textured_2d_geometry_buffer *TextGeo = &RG->TextGeo;

  layout Layout         = {};
  ui_render_group Group = {};
  min_max_avg_dt Dt     = {};

  Group.TextGroup       = RG;
  Group.Input           = &Plat->Input;
  Group.ScreenDim       = V2(Plat->WindowWidth, Plat->WindowHeight);
  Group.MouseP          = V2(Plat->MouseP.x, Plat->MouseP.y);

  SetFontSize(&Group.Font, DEBUG_FONT_SIZE);


  TIMED_BLOCK("Draw Status Bar");
    Dt = ComputeMinMaxAvgDt();
    BufferColumn(Dt.Max, 6, &Group, &Layout, WHITE);
    NewLine(&Layout, &Group.Font);

    BufferColumn(Dt.Avg, 6, &Group, &Layout, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &Group, &Layout, WHITE);
    BufferValue("ms", &Group, &Layout, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Allocations", &Group, &Layout, WHITE);

      BufferThousands(TotalStats.Pushes, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Pushes", &Group, &Layout, WHITE);

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

      BufferColumn(TotalDrawCalls, 6, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Draw Calls", &Group, &Layout, WHITE);

      NewLine(&Layout, &Group.Font);
    }

    BufferColumn(Dt.Min, 6, &Group, &Layout, WHITE);
  END_BLOCK("Status Bar");

  SetFontSize(&Group.Font, 32);
  NewLine(&Layout, &Group.Font);
  NewLine(&Layout, &Group.Font);

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_Graphics:
    {
      DebugDrawGraphicsHud(&Group, DebugState, &Layout);
    } break;

    case DebugUIType_Network:
    {
      DebugDrawNetworkHud(&Group, &Plat->Network, ServerState, &Layout);
    } break;

    case DebugUIType_CallGraph:
    {
      BufferValue("Call Graphs", &Group, &Layout, WHITE);
      DebugDrawCallGraph(&Group, DebugState, &Layout, Dt.Max);
      DebugDrawCycleThreadGraph(&Group, DebugState, &Layout);
    } break;

    case DebugUIType_Memory:
    {
      BufferValue("Memory Arenas", &Group, &Layout, WHITE);
      NewLine(&Layout, &Group.Font);
      v2 BasisP = Layout.At;
      DebugDrawMemoryHud(&Group, DebugState, BasisP);
    } break;

    case DebugUIType_DrawCalls:
    {
      BufferValue("Draw  Calls", &Group, &Layout, WHITE);
      DebugDrawDrawCalls(&Group, &Layout);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(RG, &RG->UIGeo, Group.ScreenDim);
  FlushBuffer(RG, TextGeo, Group.ScreenDim);

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

  if ( Hotkeys->Debug_ToggleGlobalDebugBreak )
  {
    Global_TriggerRuntimeBreak = !Global_TriggerRuntimeBreak;
  }

  if ( Hotkeys->Debug_ToggleProfile )
  {
    Hotkeys->Debug_ToggleProfile = False;
    State->DebugDoScopeProfiling = !State->DebugDoScopeProfiling;
  }

  if ( Hotkeys->Debug_NextUiState )
  {
    Hotkeys->Debug_NextUiState = False;
    State->UIType = (debug_ui_type)(((s32)State->UIType + 1) % (s32)DebugUIType_Count);
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
