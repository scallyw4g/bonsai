// callsite
// src/engine/editor.cpp:188:0

// def (do_editor_ui_for_container)
// src/engine/editor.h:562:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, ui_toggle_hashtable *Container, cs Name, u32 ParentHash, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x11C9E45C);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        PushNewRow(Ui);
      }
    }
    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }
}

