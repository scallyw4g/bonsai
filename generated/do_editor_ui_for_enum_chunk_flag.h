// src/engine/editor.cpp:166:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_flag *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Uninitialized"), UiId(Window, "enum Chunk_Uninitialized", Element), Params))
    {
      *Element = Chunk_Uninitialized;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Queued"), UiId(Window, "enum Chunk_Queued", Element), Params))
    {
      *Element = Chunk_Queued;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("VoxelsInitialized"), UiId(Window, "enum Chunk_VoxelsInitialized", Element), Params))
    {
      *Element = Chunk_VoxelsInitialized;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Garbage"), UiId(Window, "enum Chunk_Garbage", Element), Params))
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

