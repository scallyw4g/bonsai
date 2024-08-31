
#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>

link_internal void
TestTable(renderer_2d* Group)
{
  PushTableStart(Group);
  for (u32 Index = 0;
      Index < 2;
      ++Index)
  {
    interactable_handle Interaction = PushButtonStart(Group, UiId(0, "TestButtonInteraction0", Index));
      PushColumn(Group, CS("String1"));
      PushColumn(Group, CS("String2"));
      PushColumn(Group, CS("String3"));
      PushColumn(Group, CS("String4"));
      PushColumn(Group, CS("String5"));
      PushColumn(Group, CS("String6"));
      PushColumn(Group, CS("String7"));
      PushColumn(Group, CS("String8"));
      PushNewRow(Group);
    PushButtonEnd(Group);

    TestThat(!Hover(Group, &Interaction));
    TestThat(!Clicked(Group, &Interaction));
    TestThat(!Pressed(Group, &Interaction));
  }
  PushTableEnd(Group);

  PushTableStart(Group);
    PushButtonStart(Group, UiId(0, "TestButtonInteraction1", 0u));
      PushColumn(Group, CS("String1"));
      PushColumn(Group, CS("String2"));
      PushColumn(Group, CS("String3"));
      PushUntexturedQuadAt(Group, V2(0), V2(100), zDepth_Background);
    PushButtonEnd(Group);
    PushNewRow(Group);
  PushTableEnd(Group);

  PushTableStart(Group);
    PushButtonStart(Group, UiId(0, "TestButtonInteraction2", 0u));
      PushColumn(Group, CS("String1"));
      PushColumn(Group, CS("String2"));
      PushColumn(Group, CS("String3"));
      PushUntexturedQuadAt(Group, V2(0), V2(100), zDepth_Background);
      PushNewRow(Group);
    PushButtonEnd(Group);
  PushTableEnd(Group);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("ui_command_buffer", ArgCount, Args);

  SetThreadLocal_ThreadIndex(0);

  // NOTE(Jesse): Sorry for this...
  engine_resources Engine = {};
  Global_EngineResources = &Engine;

  memory_arena Arena = {};
  heap_allocator Heap = InitHeap(Megabytes(128));

  renderer_2d Renderer = {};

  v2 MouseP = {}, MouseDP = {}, ScreenDim = {};
  input Input = {};

  InitRenderer2D(&Renderer, &Heap, &Arena, &MouseP, &MouseDP, &ScreenDim, &Input, True);

  local_persist window_layout Window = WindowLayout("TestWindow", V2(0));

  PushWindowStart(&Renderer, &Window);
    TestTable(&Renderer);
  PushWindowEnd(&Renderer, &Window);

  layout DefaultLayout = {};
  DefaultLayout.DrawBounds = InvertedInfinityRectangle();
  render_state RenderState = { .Layout = &DefaultLayout, .ClipRect = DISABLE_CLIPPING };

  /* DrawUi(renderer_2d *Group, ui_render_command_buffer *CommandBuffer) */
  /* FlushCommandBuffer(renderer_2d *Group, render_state *RenderState, ui_render_command_buffer *CommandBuffer, layout *DefaultLayout) */
  FlushCommandBuffer(&Renderer, &RenderState, Renderer.CommandBuffer, &DefaultLayout);

  TestTable(&Renderer);
  FlushCommandBuffer(&Renderer, &RenderState, Renderer.CommandBuffer, &DefaultLayout);

  TestSuiteEnd();
}

