link_internal void
DoEditorUi(renderer_2d *Ui, s64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile s64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (s64*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile u64 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (u64*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, s32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile s32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (s32*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile u32 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (u32*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, s16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile s16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (s16*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile u16 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (u16*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, s8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile s8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (s8*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, u8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
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
DoEditorUi(renderer_2d *Ui, volatile u8 *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, (u8*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

