// src/engine/editor.cpp:207:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, file_traversal_type *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum FileTraversalType_None", Element), Params))
    {
      *Element = FileTraversalType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Dir"), UiId(Window, "enum FileTraversalType_Dir", Element), Params))
    {
      *Element = FileTraversalType_Dir;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("File"), UiId(Window, "enum FileTraversalType_File", Element), Params))
    {
      *Element = FileTraversalType_File;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

