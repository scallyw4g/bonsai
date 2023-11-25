link_internal void
DoEditorUi(renderer_2d *Ui, v4i *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[2], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[3], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}
link_internal void
DoEditorUi(renderer_2d *Ui, v4 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[2], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[3], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}
link_internal void
DoEditorUi(renderer_2d *Ui, v3i *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[2], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}
link_internal void
DoEditorUi(renderer_2d *Ui, v3 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[2], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}
link_internal void
DoEditorUi(renderer_2d *Ui, v2 *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}
link_internal void
DoEditorUi(renderer_2d *Ui, Quaternion *Value, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  if (Value)
  {
    u32 Start = StartColumn(Ui);
    PushTableStart(Ui);
    DoEditorUi(Ui, &Value->E[0], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[1], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[2], 0);
    PushNewRow(Ui);
    DoEditorUi(Ui, &Value->E[3], 0);
    PushNewRow(Ui);
    PushTableEnd(Ui);
    EndColumn(Ui, Start);
  }

}

