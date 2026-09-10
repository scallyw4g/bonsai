// callsite
// src/engine/editor.cpp:257:0

// def (do_editor_ui_for_container)
// src/engine/editor.h:819:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v3_cursor *Container, cs Name, u32 ParentHash, ui_render_params *Params, primitive_value_changed_record_block_array *ChangeRecords )
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x24260F23);

  if (Container)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);
      IterateOver(Container, Element, ElementIndex)
      {
        if (Element)
        {
          DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, Params, ChangeRecords);
          PushNewRow(Ui);
        }
        // TODO(Jesse): Do we want this ..?
        /* else */
        /* { */
        /*   PushColumn(Ui, CSz("(null)")); */
        /*   PushNewRow(Ui); */
        /* } */
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

