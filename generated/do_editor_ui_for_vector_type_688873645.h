// src/engine/editor.h:664:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v4i *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[3], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v4 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[3], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v3i *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v3 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2i *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, Quaternion *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[3], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, m4 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        DoEditorUi(Ui, Window, &Value->E[0], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[1], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[2], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    DoEditorUi(Ui, Window, &Value->E[3], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
}

