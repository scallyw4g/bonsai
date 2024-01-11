link_internal void
DoEditorUi(renderer_2d *Ui, chunk_flag *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Uninitialized"), UiId(Element, "enum Chunk_Uninitialized Chunk_Uninitialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Uninitialized;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Queued"), UiId(Element, "enum Chunk_Queued Chunk_Queued"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Queued;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_VoxelsInitialized"), UiId(Element, "enum Chunk_VoxelsInitialized Chunk_VoxelsInitialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_VoxelsInitialized;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Garbage"), UiId(Element, "enum Chunk_Garbage Chunk_Garbage"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Garbage;
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

