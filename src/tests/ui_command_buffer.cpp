
#define PLATFORM_WINDOW_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1
#define BONSAI_DEBUG_SYSTEM_LOADER_API 1

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

link_internal void
TestFloatDrag(renderer_2d *Ui, window_layout *Window)
{
  r32 Value = 2.f;
  cs Name = CSz("float");
  u32 ParentHash = 42;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(Name.Start)));
  ui_id BaseId = UiId(Window, 0, &Value, ThisHash);
  ui_id DragId = UiId(BaseId.E[0], UiMaskAndCastPointer("drag"), BaseId.E[2], BaseId.E[3]);
  base_ptr_relative_edit_block_array Changes = BasePtrRelativeEditBlockArray(GetTranArena());
  Changes.BasePtr = u64(&Value);

  Ui->Active.Id = BaseId;
  Ui->Pressed.Id = DragId;
  Ui->Clicked.Id = DragId;
  Ui->Input->LMB.Pressed = True;
  Ui->Input->LMB.Clicked = True;
  *Ui->ScreenDim = V2(1024.f, 512.f);
  *Ui->MouseDP = {};
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Ui->Active.Id == BaseId);
  TestThat(Count(&Changes) == 0);

  Ui->Input->LMB.Clicked = False;
  *Ui->MouseDP = V2(256.f, 0.f);
  TestThat(DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 2.25f);
  TestThat(Count(&Changes) == 1);
  TestThat(GetPtr(&Changes, 0)->ClickedFieldId == BaseId);

  Ui->Input->Shift.Pressed = True;
  *Ui->MouseDP = V2(0.f, -128.f);
  TestThat(DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 4.75f);
  TestThat(Count(&Changes) == 1);

  Ui->Input->Shift.Pressed = False;
  *Ui->MouseDP = V2(-256.f, 128.f);
  TestThat(DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 4.25f);

  *Ui->ScreenDim = {};
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 4.25f);

  *Ui->ScreenDim = V2(1024.f, 512.f);
  Ui->Input->LMB.Pressed = False;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 4.25f);

  Ui->Input->LMB.Pressed = True;
  Ui->Pressed.Id = BaseId;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 4.25f);

  Ui->Input->LMB.Clicked = True;
  Ui->Clicked.Id = BaseId;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(!IsValid(&Ui->Active.Id));
}

link_internal void
TestFloatTextEdit(renderer_2d *Ui, window_layout *Window)
{
  r32 Value = 1.f;
  cs Name = CSz("float text");
  u32 ParentHash = 17;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(Name.Start)));
  ui_id BaseId = UiId(Window, 0, &Value, ThisHash);
  ui_id DragId = UiId(BaseId.E[0], UiMaskAndCastPointer("drag"), BaseId.E[2], BaseId.E[3]);
  base_ptr_relative_edit_block_array Changes = BasePtrRelativeEditBlockArray(GetTranArena());
  Changes.BasePtr = u64(&Value);
  ui_id ClickedField = {};

  Ui->Active.Id = BaseId;
  Ui->Pressed = {};
  Ui->Clicked.Id = DragId;
  *Ui->Input = {};
  Ui->Input->RMB.Clicked = True;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes, &ClickedField));
  TestThat(Ui->Active.Id == DragId);
  TestThat(ClickedField == BaseId);
  TestThat(Value == 1.f);

  *Ui->Input = {};
  Ui->Input->N2.Clicked = True;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  *Ui->Input = {};
  Ui->Input->Dot.Clicked = True;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  *Ui->Input = {};
  Ui->Input->N5.Clicked = True;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(StringsMatch(CS(Ui->TextEdit.TextBuffer), CSz("2.5")));
  TestThat(Value == 1.f);

  *Ui->Input = {};
  Ui->Input->Enter.Clicked = True;
  TestThat(DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == 2.5f);
  TestThat(Count(&Changes) == 1);
  TestThat(GetPtr(&Changes, 0)->ClickedFieldId == BaseId);

  cs InvalidValues[] = {CSz(""), CSz("   "), CSz("1.2oops"), CSz("1e999"), CSz("nan")};
  for (cs Invalid : InvalidValues)
  {
    *Ui->Input = {};
    Ui->Input->RMB.Clicked = True;
    Ui->Clicked.Id = DragId;
    DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes);
    CopyString(Invalid.Start, Ui->TextEdit.TextBuffer, Invalid.Count);
    *Ui->Input = {};
    Ui->Input->Enter.Clicked = True;
    TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
    TestThat(Value == 2.5f);
    TestThat(Count(&Changes) == 1);
  }

  *Ui->Input = {};
  Ui->Input->RMB.Clicked = True;
  Ui->Clicked.Id = DragId;
  DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes);
  CopyString("-1.25e2", Ui->TextEdit.TextBuffer, 7);
  *Ui->Input = {};
  Ui->Input->LMB.Clicked = True;
  Ui->Clicked.Id = BaseId;
  TestThat(DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == -125.f);

  *Ui->Input = {};
  Ui->Input->RMB.Clicked = True;
  Ui->Clicked.Id = DragId;
  DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes);
  CopyString("3", Ui->TextEdit.TextBuffer, 1);
  *Ui->Input = {};
  Ui->Input->Escape.Clicked = True;
  TestThat(!DoEditorUi(Ui, Window, &Value, Name, ParentHash, &DefaultUiRenderParams_Column, &Changes));
  TestThat(Value == -125.f);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("ui_command_buffer", ArgCount, Args);

  engine_resources Engine = {};

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

  TestFloatDrag(&Renderer, &Window);
  TestFloatTextEdit(&Renderer, &Window);

  TestSuiteEnd();
}

