link_internal void
DoEditorUi(renderer_2d *Ui, file_traversal_type *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, umm(Element)^umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_None"), umm(Element)^umm("FileTraversalType_None"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = FileTraversalType_None;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_Dir"), umm(Element)^umm("FileTraversalType_Dir"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      *Element = FileTraversalType_Dir;
    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("")); // Skip the first Name column
    if (Button(Ui, CSz("FileTraversalType_File"), umm(Element)^umm("FileTraversalType_File"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
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

