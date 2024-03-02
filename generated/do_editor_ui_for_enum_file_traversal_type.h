// src/engine/editor.cpp:289:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, file_traversal_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_None"), UiId(Window, "enum FileTraversalType_None", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = FileTraversalType_None;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_Dir"), UiId(Window, "enum FileTraversalType_Dir", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = FileTraversalType_Dir;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_File"), UiId(Window, "enum FileTraversalType_File", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

