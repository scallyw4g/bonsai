link_internal void
DoEditorUi(renderer_2d *Ui, chunk_flag *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Uninitialized"), umm(Element)^umm("Chunk_Uninitialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Uninitialized;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Queued"), umm(Element)^umm("Chunk_Queued"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Queued;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_VoxelsInitialized"), umm(Element)^umm("Chunk_VoxelsInitialized"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_VoxelsInitialized;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Garbage"), umm(Element)^umm("Chunk_Garbage"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

