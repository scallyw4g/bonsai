#if BONSAI_INTERNAL


#include <bonsai_types.h>
#include <unix_platform.cpp>
#include <heap_memory_types.cpp>

#include <debug_data_system.cpp>

// FIXME(Jesse): this is a hack so that we can use BindShaderUniforms.  The
// Debug system never actually binds a camera, so there's not any reason for
// this to be required for it to use that function, but C isn't smart enough to
// figure that out.
//
// FIXME(Jesse): we now have camera code in here that potentially needs to know
// this value!  I think it works by accident because the camera always looks at
// the origin.
global_variable chunk_dimension WORLD_CHUNK_DIM = Chunk_Dimension(0,0,0);
//

#include <canonical_position.cpp>
#include <render_position.cpp>
#include <camera.cpp>

global_variable memory_arena* TranArena = PlatformAllocateArena();
#include <debug_render_system.cpp>

global_variable debug_state Internal_DebugState = {};

function void
DebugFrameEnd(platform *Plat, server_state* ServerState)
{
  TIMED_FUNCTION();

  debug_state *DebugState            = GetDebugState();
  debug_text_render_group *TextGroup = &DebugState->TextRenderGroup;

  min_max_avg_dt Dt           = {};
  layout Layout               = {};

  debug_ui_render_group DebugUiGroup = {};
  DebugUiGroup.TextGroup             = TextGroup;
  DebugUiGroup.GameGeo               = &DebugState->GameGeo;
  DebugUiGroup.GameGeoShader         = &DebugState->GameGeoShader;
  DebugUiGroup.Input                 = &Plat->Input;
  DebugUiGroup.ScreenDim             = V2(Plat->WindowWidth, Plat->WindowHeight);
  DebugUiGroup.MouseP                = V2(Plat->MouseP.x, Plat->MouseP.y);

  SetFontSize(&DebugUiGroup.Font, DEBUG_FONT_SIZE);

  TIMED_BLOCK("Draw Status Bar");
    Dt = ComputeMinMaxAvgDt();
    BufferColumn(Dt.Max, 6, &DebugUiGroup, &Layout, WHITE);
    NewLine(&Layout, &DebugUiGroup.Font);

    BufferColumn(Dt.Avg, 6, &DebugUiGroup, &Layout, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &DebugUiGroup, &Layout, WHITE);
    BufferValue("ms", &DebugUiGroup, &Layout, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, &DebugUiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &DebugUiGroup.Font);
      BufferValue("Allocations", &DebugUiGroup, &Layout, WHITE);

      BufferThousands(TotalStats.Pushes, &DebugUiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &DebugUiGroup.Font);
      BufferValue("Pushes", &DebugUiGroup, &Layout, WHITE);

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

      BufferColumn(TotalDrawCalls, 6, &DebugUiGroup, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &DebugUiGroup.Font);
      BufferValue("Draw Calls", &DebugUiGroup, &Layout, WHITE);

      NewLine(&Layout, &DebugUiGroup.Font);
    }

    BufferColumn(Dt.Min, 6, &DebugUiGroup, &Layout, WHITE);
  END_BLOCK("Status Bar");

  SetFontSize(&DebugUiGroup.Font, 32);
  NewLine(&Layout, &DebugUiGroup.Font);
  NewLine(&Layout, &DebugUiGroup.Font);

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_PickedChunks:
    {
      BufferValue("PickedChunks", &DebugUiGroup, &Layout, WHITE);
      NewLine(&Layout, &DebugUiGroup.Font);
      DrawPickedChunks(&DebugUiGroup, Layout.At);
    } break;

    case DebugUIType_Graphics:
    {
      DebugDrawGraphicsHud(&DebugUiGroup, DebugState, &Layout);
    } break;

    case DebugUIType_Network:
    {
      DebugDrawNetworkHud(&DebugUiGroup, &Plat->Network, ServerState, &Layout);
    } break;

    case DebugUIType_CollatedFunctionCalls:
    {
      DebugDrawCollatedFunctionCalls(&DebugUiGroup, DebugState, Layout.At);
    } break;

    case DebugUIType_CallGraph:
    {
      BufferValue("Call Graphs", &DebugUiGroup, &Layout, WHITE);
      clip_rect CycleGraphClip = DebugDrawCallGraph(&DebugUiGroup, DebugState, &Layout, Dt.Max);
      DebugDrawCycleThreadGraph(&DebugUiGroup, DebugState, V2(CycleGraphClip.Max.x + 20, Layout.At.y));
    } break;

    case DebugUIType_Memory:
    {
      BufferValue("Memory Arenas", &DebugUiGroup, &Layout, WHITE);
      NewLine(&Layout, &DebugUiGroup.Font);
      v2 BasisP = Layout.At;
      DebugDrawMemoryHud(&DebugUiGroup, DebugState, BasisP);
    } break;

    case DebugUIType_DrawCalls:
    {
      BufferValue("Draw  Calls", &DebugUiGroup, &Layout, WHITE);
      DebugDrawDrawCalls(&DebugUiGroup, &Layout);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(TextGroup, &TextGroup->UIGeo, DebugUiGroup.ScreenDim);
  FlushBuffer(TextGroup, &TextGroup->TextGeo, DebugUiGroup.ScreenDim);

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

  /* glBindFramebuffer(GL_FRAMEBUFFER, 0); */
  /* DrawTexturedQuad(&GetDebugState()->TextRenderGroup.DebugTextureShader); */

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
