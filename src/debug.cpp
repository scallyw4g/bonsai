#if BONSAI_INTERNAL

#include <stdio.h>
#include <debug_data_system.cpp>
#include <debug_render_system.cpp>


void
InitDebugState(debug_state *DebugState)
{
  GlobalDebugState = DebugState;

  GlobalDebugState->Initialized = True;
  u32 TotalThreadCount = GetTotalThreadCount();

  InitDebugMemoryAllocationSystem(DebugState);

  InitScopeTrees(ThreadsafeDebugMemoryAllocator(), TotalThreadCount);

  AllocateMesh(&GlobalDebugState->LineMesh, 1024, ThreadsafeDebugMemoryAllocator());

  if (!InitDebugOverlayFramebuffer(&GlobalDebugState->TextRenderGroup, ThreadsafeDebugMemoryAllocator(), "texture_atlas.bmp"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.TextGeo, 1024, ThreadsafeDebugMemoryAllocator());
  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.UIGeo, 1024, ThreadsafeDebugMemoryAllocator());

  GlobalDebugState->TextRenderGroup.SolidUIShader = MakeSolidUIShader(ThreadsafeDebugMemoryAllocator());

  GlobalDebugState->SelectedArenas = Allocate(selected_arenas, ThreadsafeDebugMemoryAllocator(), 1);

  return;
}

void
DebugFrameEnd(platform *Plat, game_state *GameState)
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
      DebugDrawNetworkHud(&Group, &Plat->Network, GameState->ServerState, &Layout);
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

  return;
}

void
DebugFrameBegin(hotkeys *Hotkeys, r32 Dt)
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

#endif // DEBUG


