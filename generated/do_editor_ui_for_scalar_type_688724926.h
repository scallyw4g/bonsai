// callsite
// src/engine/editor.h:824:0

// def (do_editor_ui_for_primitive_type)
// src/engine/editor.h:440:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, s64 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D1BBC);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, u64 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D1BBE);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, s32 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D052B);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, u32 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D052D);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, s16 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D30A5);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, u16 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290D30A7);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, s8 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F14E4);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, u8 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F14E6);

  Params = Params ? Params : &DefaultUiRenderParams_Blank;

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

  return Result;
}



