// callsite
// src/engine/editor.h:985:0

// def (do_editor_ui_for_vector_type)
// src/engine/editor.h:403:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v4i *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F60E2);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[3], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v4 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F1413);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[3], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v3i *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F60AD);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v3 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F13DE);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2i *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F6078);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F13A9);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, Quaternion *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x42A58E4);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[3], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, m4 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x290F140A);

    /* PushTableStart(Ui); */
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

  if (Value)
  {
    u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
        Result |= DoEditorUi(Ui, Window, &Value->E[0], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[1], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[2], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
    Result |= DoEditorUi(Ui, Window, &Value->E[3], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );

    PushTableEnd(Ui);
    /* PushNewRow(Ui); */
    EndColumn(Ui, Start);
  }
  /* PushTableEnd(Ui); */

  PushNewRow(Ui);
  return Result;
}


