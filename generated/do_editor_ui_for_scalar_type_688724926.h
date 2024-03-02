// src/engine/editor.cpp:124:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (s64*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (u64*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (s32*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (u32*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (s16*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (u16*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, s8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile s8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (s8*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, u8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile u8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, (u8*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

