// src/engine/editor.cpp:391:0







































































































link_internal b32
IsValid(ui_brush_layer_actions Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiBrushLayerAction_NoAction:
    case UiBrushLayerAction_MoveUp:
    case UiBrushLayerAction_MoveDown:
    case UiBrushLayerAction_Duplicate:
    case UiBrushLayerAction_Delete:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_brush_layer_actions Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case UiBrushLayerAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiBrushLayerAction_MoveUp: { Result = CSz("MoveUp"); } break;
    case UiBrushLayerAction_MoveDown: { Result = CSz("MoveDown"); } break;
    case UiBrushLayerAction_Duplicate: { Result = CSz("Duplicate"); } break;
    case UiBrushLayerAction_Delete: { Result = CSz("Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_brush_layer_actions Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiBrushLayerAction_NoAction: { Result = CSz("UiBrushLayerAction_NoAction"); } break;
    case UiBrushLayerAction_MoveUp: { Result = CSz("UiBrushLayerAction_MoveUp"); } break;
    case UiBrushLayerAction_MoveDown: { Result = CSz("UiBrushLayerAction_MoveDown"); } break;
    case UiBrushLayerAction_Duplicate: { Result = CSz("UiBrushLayerAction_Duplicate"); } break;
    case UiBrushLayerAction_Delete: { Result = CSz("UiBrushLayerAction_Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_brush_layer_actions
UiBrushLayerActions(counted_string S)
{
  ui_brush_layer_actions Result = {};

    if (StringsMatch(S, CSz("UiBrushLayerAction_NoAction"))) { return UiBrushLayerAction_NoAction; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_MoveUp"))) { return UiBrushLayerAction_MoveUp; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_MoveDown"))) { return UiBrushLayerAction_MoveDown; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_Duplicate"))) { return UiBrushLayerAction_Duplicate; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_Delete"))) { return UiBrushLayerAction_Delete; }


  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_brush_layer_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_brush_layer_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_brush_layer_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_NoAction")), UiBrushLayerAction_NoAction },
    { CSz("MoveUp"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveUp")), UiBrushLayerAction_MoveUp },
    { CSz("MoveDown"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveDown")), UiBrushLayerAction_MoveDown },
    { CSz("Duplicate"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Duplicate")), UiBrushLayerAction_Duplicate },
    { CSz("Delete"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Delete")), UiBrushLayerAction_Delete },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_brush_layer_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
      CSz("MoveUp"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveUp"),
        Cast(void*, Index)
      ),
      UiBrushLayerAction_MoveUp,
    },

        {
      CSz("MoveDown"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_MoveDown"),
        Cast(void*, Index)
      ),
      UiBrushLayerAction_MoveDown,
    },

        {
      CSz("Duplicate"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Duplicate"),
        Cast(void*, Index)
      ),
      UiBrushLayerAction_Duplicate,
    },

        {
      CSz("Delete"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_layer_actions UiBrushLayerAction_Delete"),
        Cast(void*, Index)
      ),
      UiBrushLayerAction_Delete,
    },


  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ExtraFlags);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}















link_internal b32
IsValid(selection_modification_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
        case SelectionModificationMode_None:
    case SelectionModificationMode_Initialize:
    case SelectionModificationMode_Modify:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(selection_modification_mode Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case SelectionModificationMode_None: { Result = CSz("None"); } break;
    case SelectionModificationMode_Initialize: { Result = CSz("Initialize"); } break;
    case SelectionModificationMode_Modify: { Result = CSz("Modify"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(selection_modification_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case SelectionModificationMode_None: { Result = CSz("SelectionModificationMode_None"); } break;
    case SelectionModificationMode_Initialize: { Result = CSz("SelectionModificationMode_Initialize"); } break;
    case SelectionModificationMode_Modify: { Result = CSz("SelectionModificationMode_Modify"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal selection_modification_mode
SelectionModificationMode(counted_string S)
{
  selection_modification_mode Result = {};

    if (StringsMatch(S, CSz("SelectionModificationMode_None"))) { return SelectionModificationMode_None; }
  if (StringsMatch(S, CSz("SelectionModificationMode_Initialize"))) { return SelectionModificationMode_Initialize; }
  if (StringsMatch(S, CSz("SelectionModificationMode_Modify"))) { return SelectionModificationMode_Modify; }


  return Result;
}




link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, selection_modification_mode *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x48A2A15);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle selection_modification_mode", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum SelectionModificationMode_None", Element, ThisHash), Params))
    {
            *Element = SelectionModificationMode_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Initialize"), UiId(Window, "enum SelectionModificationMode_Initialize", Element, ThisHash), Params))
    {
            *Element = SelectionModificationMode_Initialize;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Modify"), UiId(Window, "enum SelectionModificationMode_Modify", Element, ThisHash), Params))
    {
            *Element = SelectionModificationMode_Modify;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
}


































link_internal b32
IsValid(ui_layer_edit_actions Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiLayerEditAction_NoAction:
    case UiLayerEditAction_SetBrush:
    case UiLayerEditAction_Duplicate:
    case UiLayerEditAction_Delete:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_layer_edit_actions Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case UiLayerEditAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiLayerEditAction_SetBrush: { Result = CSz("SetBrush"); } break;
    case UiLayerEditAction_Duplicate: { Result = CSz("Duplicate"); } break;
    case UiLayerEditAction_Delete: { Result = CSz("Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_layer_edit_actions Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiLayerEditAction_NoAction: { Result = CSz("UiLayerEditAction_NoAction"); } break;
    case UiLayerEditAction_SetBrush: { Result = CSz("UiLayerEditAction_SetBrush"); } break;
    case UiLayerEditAction_Duplicate: { Result = CSz("UiLayerEditAction_Duplicate"); } break;
    case UiLayerEditAction_Delete: { Result = CSz("UiLayerEditAction_Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_layer_edit_actions
UiLayerEditActions(counted_string S)
{
  ui_layer_edit_actions Result = {};

    if (StringsMatch(S, CSz("UiLayerEditAction_NoAction"))) { return UiLayerEditAction_NoAction; }
  if (StringsMatch(S, CSz("UiLayerEditAction_SetBrush"))) { return UiLayerEditAction_SetBrush; }
  if (StringsMatch(S, CSz("UiLayerEditAction_Duplicate"))) { return UiLayerEditAction_Duplicate; }
  if (StringsMatch(S, CSz("UiLayerEditAction_Delete"))) { return UiLayerEditAction_Delete; }


  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_layer_edit_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_layer_edit_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_layer_edit_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_NoAction")), UiLayerEditAction_NoAction },
    { CSz("SetBrush"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_SetBrush")), UiLayerEditAction_SetBrush },
    { CSz("Duplicate"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Duplicate")), UiLayerEditAction_Duplicate },
    { CSz("Delete"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Delete")), UiLayerEditAction_Delete },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_layer_edit_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
      CSz("S"),
      CSz("SetBrush"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_SetBrush"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_SetBrush,
    },

        {
      CSz("D"),
      CSz("Duplicate"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Duplicate"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_Duplicate,
    },

        {
      CSz("X"),
      CSz("Delete"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_edit_actions UiLayerEditAction_Delete"),
        Cast(void*, Index)
      ),
      UiLayerEditAction_Delete,
    },


  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ExtraFlags);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}







link_internal b32
IsValid(ui_reorder_action Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiReorderAction_NoAction:
    case UiReorderAction_ReorderUp:
    case UiReorderAction_ReorderDown:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_reorder_action Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case UiReorderAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiReorderAction_ReorderUp: { Result = CSz("ReorderUp"); } break;
    case UiReorderAction_ReorderDown: { Result = CSz("ReorderDown"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_reorder_action Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiReorderAction_NoAction: { Result = CSz("UiReorderAction_NoAction"); } break;
    case UiReorderAction_ReorderUp: { Result = CSz("UiReorderAction_ReorderUp"); } break;
    case UiReorderAction_ReorderDown: { Result = CSz("UiReorderAction_ReorderDown"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_reorder_action
UiReorderAction(counted_string S)
{
  ui_reorder_action Result = {};

    if (StringsMatch(S, CSz("UiReorderAction_NoAction"))) { return UiReorderAction_NoAction; }
  if (StringsMatch(S, CSz("UiReorderAction_ReorderUp"))) { return UiReorderAction_ReorderUp; }
  if (StringsMatch(S, CSz("UiReorderAction_ReorderDown"))) { return UiReorderAction_ReorderDown; }


  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_reorder_action Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_reorder_action( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_reorder_action *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_reorder_action UiReorderAction_NoAction")), UiReorderAction_NoAction },
    { CSz("ReorderUp"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_reorder_action UiReorderAction_ReorderUp")), UiReorderAction_ReorderUp },
    { CSz("ReorderDown"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_reorder_action UiReorderAction_ReorderDown")), UiReorderAction_ReorderDown },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_reorder_action *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
      CSz("^"),
      CSz("ReorderUp"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_reorder_action UiReorderAction_ReorderUp"),
        Cast(void*, Index)
      ),
      UiReorderAction_ReorderUp,
    },

        {
      CSz("v"),
      CSz("ReorderDown"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_reorder_action UiReorderAction_ReorderDown"),
        Cast(void*, Index)
      ),
      UiReorderAction_ReorderDown,
    },


  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ExtraFlags);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}























































link_internal b32
IsValid(ui_brush_actions Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiBrushAction_NoAction:
    case UiBrushAction_New:
    case UiBrushAction_Save:
    case UiBrushAction_Duplicate:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_brush_actions Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case UiBrushAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiBrushAction_New: { Result = CSz("New"); } break;
    case UiBrushAction_Save: { Result = CSz("Save"); } break;
    case UiBrushAction_Duplicate: { Result = CSz("Duplicate"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_brush_actions Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiBrushAction_NoAction: { Result = CSz("UiBrushAction_NoAction"); } break;
    case UiBrushAction_New: { Result = CSz("UiBrushAction_New"); } break;
    case UiBrushAction_Save: { Result = CSz("UiBrushAction_Save"); } break;
    case UiBrushAction_Duplicate: { Result = CSz("UiBrushAction_Duplicate"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_brush_actions
UiBrushActions(counted_string S)
{
  ui_brush_actions Result = {};

    if (StringsMatch(S, CSz("UiBrushAction_NoAction"))) { return UiBrushAction_NoAction; }
  if (StringsMatch(S, CSz("UiBrushAction_New"))) { return UiBrushAction_New; }
  if (StringsMatch(S, CSz("UiBrushAction_Save"))) { return UiBrushAction_Save; }
  if (StringsMatch(S, CSz("UiBrushAction_Duplicate"))) { return UiBrushAction_Duplicate; }


  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_brush_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_brush_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_brush_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_actions UiBrushAction_NoAction")), UiBrushAction_NoAction },
    { CSz("New"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_actions UiBrushAction_New")), UiBrushAction_New },
    { CSz("Save"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_actions UiBrushAction_Save")), UiBrushAction_Save },
    { CSz("Duplicate"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_brush_actions UiBrushAction_Duplicate")), UiBrushAction_Duplicate },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_brush_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
      CSz("New"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_actions UiBrushAction_New"),
        Cast(void*, Index)
      ),
      UiBrushAction_New,
    },

        {
      CSz("Save"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_actions UiBrushAction_Save"),
        Cast(void*, Index)
      ),
      UiBrushAction_Save,
    },

        {
      CSz("Duplicate"),
      {},
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_brush_actions UiBrushAction_Duplicate"),
        Cast(void*, Index)
      ),
      UiBrushAction_Duplicate,
    },


  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ExtraFlags);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}









































































link_internal b32
IsValid(ui_layer_toolbar_actions Value)
{
  b32 Result = False;
  switch (Value)
  {
        case LayerToolbarActions_NoAction:
    case LayerToolbarActions_Rename:
    case LayerToolbarActions_Duplicate:
    case LayerToolbarActions_Delete:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_layer_toolbar_actions Type)
{
  Assert(IsValid(Type));
  counted_string Result = {};

  switch (Type)
  {
        case LayerToolbarActions_NoAction: { Result = CSz("NoAction"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("Rename"); } break;
    case LayerToolbarActions_Duplicate: { Result = CSz("Duplicate"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_layer_toolbar_actions Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case LayerToolbarActions_NoAction: { Result = CSz("LayerToolbarActions_NoAction"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("LayerToolbarActions_Rename"); } break;
    case LayerToolbarActions_Duplicate: { Result = CSz("LayerToolbarActions_Duplicate"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("LayerToolbarActions_Delete"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_layer_toolbar_actions
UiLayerToolbarActions(counted_string S)
{
  ui_layer_toolbar_actions Result = {};

    if (StringsMatch(S, CSz("LayerToolbarActions_NoAction"))) { return LayerToolbarActions_NoAction; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Rename"))) { return LayerToolbarActions_Rename; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Duplicate"))) { return LayerToolbarActions_Duplicate; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Delete"))) { return LayerToolbarActions_Delete; }


  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_layer_toolbar_actions Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_layer_toolbar_actions( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_layer_toolbar_actions *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { CSz("NoAction"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_NoAction")), LayerToolbarActions_NoAction },
    { CSz("Rename"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Rename")), LayerToolbarActions_Rename },
    { CSz("Duplicate"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Duplicate")), LayerToolbarActions_Duplicate },
    { CSz("Delete"), {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Delete")), LayerToolbarActions_Delete },

  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags|ToggleButtonGroupFlags_TypeRadioButton));
  return Result;
}




link_internal ui_toggle_button_group
PushToolbar(     renderer_2d *Ui, 
  window_layout *Window,
  cs  GroupName,
  ui_layer_toolbar_actions *Element,
  u64  Index = 0,
  ui_render_params *Params     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
      CSz("R"),
      CSz("Rename"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Rename"),
        Cast(void*, Index)
      ),
      LayerToolbarActions_Rename,
    },

        {
      CSz("D"),
      CSz("Duplicate"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Duplicate"),
        Cast(void*, Index)
      ),
      LayerToolbarActions_Duplicate,
    },

        {
      CSz("X"),
      CSz("Delete"),
      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_layer_toolbar_actions LayerToolbarActions_Delete"),
        Cast(void*, Index)
      ),
      LayerToolbarActions_Delete,
    },


  };

  ui_toggle_button_handle_buffer ButtonBuffer = {
    ArrayCount(ButtonHandles),
    ButtonHandles
  };

  ui_toggle_button_group Result = {};
  Result.Ui = Ui;
  Result.Flags = ui_toggle_button_group_flags(ToggleButtonGroupFlags_TypeClickButton | ExtraFlags);
  Result.Buttons = ButtonBuffer;
  Result.EnumStorage = Cast(u32*, Element);

  DrawButtonGroup(&Result, GroupName);
  return Result;
}























































































































































































































































