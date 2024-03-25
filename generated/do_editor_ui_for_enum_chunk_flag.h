// src/engine/editor.cpp:248:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, chunk_flag *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Uninitialized"), UiId(Window, "enum Chunk_Uninitialized", Element), Params))
    {
      *Element = Chunk_Uninitialized;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Queued"), UiId(Window, "enum Chunk_Queued", Element), Params))
    {
      *Element = Chunk_Queued;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("VoxelsInitialized"), UiId(Window, "enum Chunk_VoxelsInitialized", Element), Params))
    {
      *Element = Chunk_VoxelsInitialized;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Garbage"), UiId(Window, "enum Chunk_Garbage", Element), Params))
    {
      *Element = Chunk_Garbage;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

