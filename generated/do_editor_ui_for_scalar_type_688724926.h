link_internal void
DoEditorUi(renderer_2d *Ui, s64 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, u64 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, s32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, u32 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, s16 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, u16 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, s8 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}
link_internal void
DoEditorUi(renderer_2d *Ui, u8 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }
  else
  {
    PushColumn(Ui, CSz("(null)"));
  }
}

