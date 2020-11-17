#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1

#include <debug.cpp>
#include <tests/test_utils.cpp>


bonsai_function void
TestTable(debug_ui_render_group* Group)
{
  PushTableStart(Group);
  for (u32 Index = 0;
      Index < 2;
      ++Index)
  {
    interactable_handle Interaction = PushButtonStart(Group, (umm)"TestButtonInteraction"^(umm)Index);
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
    PushButtonStart(Group, (umm)"Ignored");
      PushColumn(Group, CS("String1"));
      PushColumn(Group, CS("String2"));
      PushColumn(Group, CS("String3"));
      PushUntexturedQuadAt(Group, V2(0), V2(100), zDepth_Background);
    PushButtonEnd(Group);
    PushNewRow(Group);
  PushTableEnd(Group);

  PushTableStart(Group);
    PushButtonStart(Group, (umm)"Ignored");
      PushColumn(Group, CS("String1"));
      PushColumn(Group, CS("String2"));
      PushColumn(Group, CS("String3"));
      PushUntexturedQuadAt(Group, V2(0), V2(100), zDepth_Background);
      PushNewRow(Group);
    PushButtonEnd(Group);
  PushTableEnd(Group);
}

s32 main()
{
  TestSuiteBegin("ui_command_buffer");

  memory_arena Arena           = {};
  debug_ui_render_group* Group = Allocate(debug_ui_render_group, &Arena, 1);
  Group->CommandBuffer         = Allocate(ui_render_command_buffer, &Arena, 1);
  Group->Input                 = Allocate(input, &Arena, 1);;
  Group->MouseP                = Allocate(v2, &Arena, 1);;

  local_persist window_layout Window = WindowLayout("TestWindow", V2(0));

  PushWindowStart(Group, &Window);
    TestTable(Group);
  PushWindowEnd(Group, &Window);
  FlushCommandBuffer(Group, Group->CommandBuffer);

  TestTable(Group);
  FlushCommandBuffer(Group, Group->CommandBuffer);

  TestSuiteEnd();
}

