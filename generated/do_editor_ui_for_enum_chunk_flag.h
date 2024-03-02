// src/engine/editor.cpp:262:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_flag *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Uninitialized"), UiId(Window, "enum Chunk_Uninitialized", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Uninitialized;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Queued"), UiId(Window, "enum Chunk_Queued", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_Queued;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_VoxelsInitialized"), UiId(Window, "enum Chunk_VoxelsInitialized", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = Chunk_VoxelsInitialized;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("Chunk_Garbage"), UiId(Window, "enum Chunk_Garbage", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

