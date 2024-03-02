// src/engine/editor.cpp:129:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s64 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s64 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (s64*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u64 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u64 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (u64*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s32 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s32 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (s32*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u32 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u32 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (u32*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s16 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s16 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (s16*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u16 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u16 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (u16*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s8 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s8 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (s8*) Value, Name, Params);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u8 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  Params = Params ? Params : &DefaultUiRenderParams_Column;

  if (Name) { PushColumn(Ui, Name, Params); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, Params);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u8 *Value, cs Name, ui_render_params *Params)
{
  DoEditorUi(Ui, Window, (u8*) Value, Name, Params);
}

