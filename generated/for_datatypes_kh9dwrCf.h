// callsite
// src/engine/editor.cpp:430:0

// def ((builtin.for_datatypes))
// src/engine/editor.cpp:430:0


















































link_internal b32
IsValid(ui_editor_action Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiEditorAction_NoAction:
    case UiEditorAction_New:
    case UiEditorAction_Show:
    case UiEditorAction_Hide:
    case UiEditorAction_HideObstructed:
    case UiEditorAction_ReorderUp:
    case UiEditorAction_ReorderDown:
    case UiEditorAction_Duplicate:
    case UiEditorAction_SetBrush:
    case UiEditorAction_ExportAsPrefab:
    case UiEditorAction_Save:
    case UiEditorAction_Delete:
    case UiEditorAction_Expand:
    case UiEditorAction_Collapse:
    case UiEditorAction_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_editor_action Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case UiEditorAction_NoAction: { Result = CSz("NoAction"); } break;
      case UiEditorAction_New: { Result = CSz("New"); } break;
      case UiEditorAction_Show: { Result = CSz("Show"); } break;
      case UiEditorAction_Hide: { Result = CSz("Hide"); } break;
      case UiEditorAction_HideObstructed: { Result = CSz("HideObstructed"); } break;
      case UiEditorAction_ReorderUp: { Result = CSz("ReorderUp"); } break;
      case UiEditorAction_ReorderDown: { Result = CSz("ReorderDown"); } break;
      case UiEditorAction_Duplicate: { Result = CSz("Duplicate"); } break;
      case UiEditorAction_SetBrush: { Result = CSz("SetBrush"); } break;
      case UiEditorAction_ExportAsPrefab: { Result = CSz("ExportAsPrefab"); } break;
      case UiEditorAction_Save: { Result = CSz("Save"); } break;
      case UiEditorAction_Delete: { Result = CSz("Delete"); } break;
      case UiEditorAction_Expand: { Result = CSz("Expand"); } break;
      case UiEditorAction_Collapse: { Result = CSz("Collapse"); } break;
      case UiEditorAction_Count: { Result = CSz("Count"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_editor_action Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiEditorAction_NoAction: { Result = CSz("UiEditorAction_NoAction"); } break;
    case UiEditorAction_New: { Result = CSz("UiEditorAction_New"); } break;
    case UiEditorAction_Show: { Result = CSz("UiEditorAction_Show"); } break;
    case UiEditorAction_Hide: { Result = CSz("UiEditorAction_Hide"); } break;
    case UiEditorAction_HideObstructed: { Result = CSz("UiEditorAction_HideObstructed"); } break;
    case UiEditorAction_ReorderUp: { Result = CSz("UiEditorAction_ReorderUp"); } break;
    case UiEditorAction_ReorderDown: { Result = CSz("UiEditorAction_ReorderDown"); } break;
    case UiEditorAction_Duplicate: { Result = CSz("UiEditorAction_Duplicate"); } break;
    case UiEditorAction_SetBrush: { Result = CSz("UiEditorAction_SetBrush"); } break;
    case UiEditorAction_ExportAsPrefab: { Result = CSz("UiEditorAction_ExportAsPrefab"); } break;
    case UiEditorAction_Save: { Result = CSz("UiEditorAction_Save"); } break;
    case UiEditorAction_Delete: { Result = CSz("UiEditorAction_Delete"); } break;
    case UiEditorAction_Expand: { Result = CSz("UiEditorAction_Expand"); } break;
    case UiEditorAction_Collapse: { Result = CSz("UiEditorAction_Collapse"); } break;
    case UiEditorAction_Count: { Result = CSz("UiEditorAction_Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_editor_action
UiEditorAction(counted_string S)
{
  ui_editor_action Result = {};

    if (StringsMatch(S, CSz("UiEditorAction_NoAction"))) { return UiEditorAction_NoAction; }
  if (StringsMatch(S, CSz("UiEditorAction_New"))) { return UiEditorAction_New; }
  if (StringsMatch(S, CSz("UiEditorAction_Show"))) { return UiEditorAction_Show; }
  if (StringsMatch(S, CSz("UiEditorAction_Hide"))) { return UiEditorAction_Hide; }
  if (StringsMatch(S, CSz("UiEditorAction_HideObstructed"))) { return UiEditorAction_HideObstructed; }
  if (StringsMatch(S, CSz("UiEditorAction_ReorderUp"))) { return UiEditorAction_ReorderUp; }
  if (StringsMatch(S, CSz("UiEditorAction_ReorderDown"))) { return UiEditorAction_ReorderDown; }
  if (StringsMatch(S, CSz("UiEditorAction_Duplicate"))) { return UiEditorAction_Duplicate; }
  if (StringsMatch(S, CSz("UiEditorAction_SetBrush"))) { return UiEditorAction_SetBrush; }
  if (StringsMatch(S, CSz("UiEditorAction_ExportAsPrefab"))) { return UiEditorAction_ExportAsPrefab; }
  if (StringsMatch(S, CSz("UiEditorAction_Save"))) { return UiEditorAction_Save; }
  if (StringsMatch(S, CSz("UiEditorAction_Delete"))) { return UiEditorAction_Delete; }
  if (StringsMatch(S, CSz("UiEditorAction_Expand"))) { return UiEditorAction_Expand; }
  if (StringsMatch(S, CSz("UiEditorAction_Collapse"))) { return UiEditorAction_Collapse; }
  if (StringsMatch(S, CSz("UiEditorAction_Count"))) { return UiEditorAction_Count; }


  return Result;
}


link_internal ui_toggle_button_handle
ButtonHandleForEnumValue(renderer_2d *Ui, ui_editor_action Value, ui_id BaseId)
{
  Assert(BaseId.ElementBits == 0);
  Assert(BaseId.HashBits == 0);

  ui_toggle_button_handle Result;

  switch (Value)
  {
            case UiEditorAction_NoAction: {} break;


        case UiEditorAction_New:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Add,
        
        CSz("New"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_New"),
          UiEditorAction_New
        ),
        UiEditorAction_New,
      };
    } break;


        case UiEditorAction_Show:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeOutline,
        
        CSz("Show"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Show"),
          UiEditorAction_Show
        ),
        UiEditorAction_Show,
      };
    } break;


        case UiEditorAction_Hide:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Eye,
        
        CSz("Hide"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Hide"),
          UiEditorAction_Hide
        ),
        UiEditorAction_Hide,
      };
    } break;


        case UiEditorAction_HideObstructed:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeHatched,
        
        CSz("HideObstructed"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_HideObstructed"),
          UiEditorAction_HideObstructed
        ),
        UiEditorAction_HideObstructed,
      };
    } break;


        case UiEditorAction_ReorderUp:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowUp,
        
        CSz("ReorderUp"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_ReorderUp"),
          UiEditorAction_ReorderUp
        ),
        UiEditorAction_ReorderUp,
      };
    } break;


        case UiEditorAction_ReorderDown:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowDown,
        
        CSz("ReorderDown"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_ReorderDown"),
          UiEditorAction_ReorderDown
        ),
        UiEditorAction_ReorderDown,
      };
    } break;


        case UiEditorAction_Duplicate:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Clone,
        
        CSz("Duplicate"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Duplicate"),
          UiEditorAction_Duplicate
        ),
        UiEditorAction_Duplicate,
      };
    } break;


        case UiEditorAction_SetBrush:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Brush,
        
        CSz("SetBrush"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_SetBrush"),
          UiEditorAction_SetBrush
        ),
        UiEditorAction_SetBrush,
      };
    } break;


        case UiEditorAction_ExportAsPrefab:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_FileExport,
        
        CSz("ExportAsPrefab"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_ExportAsPrefab"),
          UiEditorAction_ExportAsPrefab
        ),
        UiEditorAction_ExportAsPrefab,
      };
    } break;


        case UiEditorAction_Save:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Disk,
        
        CSz("Save"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Save"),
          UiEditorAction_Save
        ),
        UiEditorAction_Save,
      };
    } break;


        case UiEditorAction_Delete:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Trash,
        
        CSz("Delete"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Delete"),
          UiEditorAction_Delete
        ),
        UiEditorAction_Delete,
      };
    } break;


        case UiEditorAction_Expand:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleRight,
        
        CSz("Expand"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Expand"),
          UiEditorAction_Expand
        ),
        UiEditorAction_Expand,
      };
    } break;


        case UiEditorAction_Collapse:
    {
      Result =
      {
                UiDisplayType_Icon,
        .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleDown,
        
        CSz("Collapse"),

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_action UiEditorAction_Collapse"),
          UiEditorAction_Collapse
        ),
        UiEditorAction_Collapse,
      };
    } break;


        case UiEditorAction_Count: {} break;



  }
  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_editor_action Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_editor_action( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_editor_action *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("NoAction"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_NoAction")), UiEditorAction_NoAction },
    { UiDisplayType_Text, {{ CSz("New"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_New")), UiEditorAction_New },
    { UiDisplayType_Text, {{ CSz("Show"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Show")), UiEditorAction_Show },
    { UiDisplayType_Text, {{ CSz("Hide"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Hide")), UiEditorAction_Hide },
    { UiDisplayType_Text, {{ CSz("HideObstructed"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_HideObstructed")), UiEditorAction_HideObstructed },
    { UiDisplayType_Text, {{ CSz("ReorderUp"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_ReorderUp")), UiEditorAction_ReorderUp },
    { UiDisplayType_Text, {{ CSz("ReorderDown"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_ReorderDown")), UiEditorAction_ReorderDown },
    { UiDisplayType_Text, {{ CSz("Duplicate"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Duplicate")), UiEditorAction_Duplicate },
    { UiDisplayType_Text, {{ CSz("SetBrush"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_SetBrush")), UiEditorAction_SetBrush },
    { UiDisplayType_Text, {{ CSz("ExportAsPrefab"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_ExportAsPrefab")), UiEditorAction_ExportAsPrefab },
    { UiDisplayType_Text, {{ CSz("Save"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Save")), UiEditorAction_Save },
    { UiDisplayType_Text, {{ CSz("Delete"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Delete")), UiEditorAction_Delete },
    { UiDisplayType_Text, {{ CSz("Expand"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Expand")), UiEditorAction_Expand },
    { UiDisplayType_Text, {{ CSz("Collapse"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Collapse")), UiEditorAction_Collapse },
    { UiDisplayType_Text, {{ CSz("Count"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_action UiEditorAction_Count")), UiEditorAction_Count },

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
  ui_editor_action *Element,
  u64  Index = 0,
  ui_render_params *ContainerParams     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
        
        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Add,
      
      CSz("New"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_New"),
        Cast(void*, Index)
      ),
      UiEditorAction_New,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeOutline,
      
      CSz("Show"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Show"),
        Cast(void*, Index)
      ),
      UiEditorAction_Show,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Eye,
      
      CSz("Hide"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Hide"),
        Cast(void*, Index)
      ),
      UiEditorAction_Hide,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeHatched,
      
      CSz("HideObstructed"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_HideObstructed"),
        Cast(void*, Index)
      ),
      UiEditorAction_HideObstructed,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowUp,
      
      CSz("ReorderUp"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_ReorderUp"),
        Cast(void*, Index)
      ),
      UiEditorAction_ReorderUp,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowDown,
      
      CSz("ReorderDown"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_ReorderDown"),
        Cast(void*, Index)
      ),
      UiEditorAction_ReorderDown,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Clone,
      
      CSz("Duplicate"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Duplicate"),
        Cast(void*, Index)
      ),
      UiEditorAction_Duplicate,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Brush,
      
      CSz("SetBrush"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_SetBrush"),
        Cast(void*, Index)
      ),
      UiEditorAction_SetBrush,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_FileExport,
      
      CSz("ExportAsPrefab"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_ExportAsPrefab"),
        Cast(void*, Index)
      ),
      UiEditorAction_ExportAsPrefab,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Disk,
      
      CSz("Save"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Save"),
        Cast(void*, Index)
      ),
      UiEditorAction_Save,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Trash,
      
      CSz("Delete"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Delete"),
        Cast(void*, Index)
      ),
      UiEditorAction_Delete,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleRight,
      
      CSz("Expand"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Expand"),
        Cast(void*, Index)
      ),
      UiEditorAction_Expand,
    },

        {
            UiDisplayType_Icon,
      .IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleDown,
      
      CSz("Collapse"),

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_action UiEditorAction_Collapse"),
        Cast(void*, Index)
      ),
      UiEditorAction_Collapse,
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

  DrawButtonGroup(&Result, GroupName, &DefaultUiRenderParams_Button, ContainerParams);
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
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case SelectionModificationMode_None: { Result = CSz("None"); } break;
      case SelectionModificationMode_Initialize: { Result = CSz("Initialize"); } break;
      case SelectionModificationMode_Modify: { Result = CSz("Modify"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
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




link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, selection_modification_mode *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
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
      Result = True;
            *Element = SelectionModificationMode_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Initialize"), UiId(Window, "enum SelectionModificationMode_Initialize", Element, ThisHash), Params))
    {
      Result = True;
            *Element = SelectionModificationMode_Initialize;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Modify"), UiId(Window, "enum SelectionModificationMode_Modify", Element, ThisHash), Params))
    {
      Result = True;
            *Element = SelectionModificationMode_Modify;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}














































































link_internal b32
IsValid(ui_editor_tool Value)
{
  b32 Result = False;
  switch (Value)
  {
        case UiEditorTool_Brush:
    case UiEditorTool_Prefab:
    case UiEditorTool_Entity:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(ui_editor_tool Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case UiEditorTool_Brush: { Result = CSz("Brush"); } break;
      case UiEditorTool_Prefab: { Result = CSz("Prefab"); } break;
      case UiEditorTool_Entity: { Result = CSz("Entity"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_editor_tool Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case UiEditorTool_Brush: { Result = CSz("UiEditorTool_Brush"); } break;
    case UiEditorTool_Prefab: { Result = CSz("UiEditorTool_Prefab"); } break;
    case UiEditorTool_Entity: { Result = CSz("UiEditorTool_Entity"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal ui_editor_tool
UiEditorTool(counted_string S)
{
  ui_editor_tool Result = {};

    if (StringsMatch(S, CSz("UiEditorTool_Brush"))) { return UiEditorTool_Brush; }
  if (StringsMatch(S, CSz("UiEditorTool_Prefab"))) { return UiEditorTool_Prefab; }
  if (StringsMatch(S, CSz("UiEditorTool_Entity"))) { return UiEditorTool_Entity; }


  return Result;
}


link_internal ui_toggle_button_handle
ButtonHandleForEnumValue(renderer_2d *Ui, ui_editor_tool Value, ui_id BaseId)
{
  Assert(BaseId.ElementBits == 0);
  Assert(BaseId.HashBits == 0);

  ui_toggle_button_handle Result;

  switch (Value)
  {
            case UiEditorTool_Brush:
    {
      Result =
      {
                UiDisplayType_Text,
        {{
            
                        CSz("Brush")
,
          }},

        
                {}
,

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_tool UiEditorTool_Brush"),
          UiEditorTool_Brush
        ),
        UiEditorTool_Brush,
      };
    } break;


        case UiEditorTool_Prefab:
    {
      Result =
      {
                UiDisplayType_Text,
        {{
            
                        CSz("Prefab")
,
          }},

        
                {}
,

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_tool UiEditorTool_Prefab"),
          UiEditorTool_Prefab
        ),
        UiEditorTool_Prefab,
      };
    } break;


        case UiEditorTool_Entity:
    {
      Result =
      {
                UiDisplayType_Text,
        {{
            
                        CSz("Entity")
,
          }},

        
                {}
,

        UiId(
          BaseId.WindowBits,
          BaseId.InteractionBits,
          UiMaskAndCastPointer("ui_editor_tool UiEditorTool_Entity"),
          UiEditorTool_Entity
        ),
        UiEditorTool_Entity,
      };
    } break;



  }
  return Result;
}


link_internal void
RadioSelect(ui_toggle_button_group *RadioGroup, ui_editor_tool Selection)
{
  ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
  SetRadioButton(RadioGroup, ToggleHandle, True);
  /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
}

link_internal ui_toggle_button_group
RadioButtonGroup_ui_editor_tool( renderer_2d *Ui,
  window_layout *Window,
  cs  GroupName,
  ui_editor_tool *Element,
  ui_render_params *Params     = &DefaultUiRenderParams_Generic,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  ui_toggle_button_handle ButtonHandles[] =
  {
        { UiDisplayType_Text, {{ CSz("Brush"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_tool UiEditorTool_Brush")), UiEditorTool_Brush },
    { UiDisplayType_Text, {{ CSz("Prefab"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_tool UiEditorTool_Prefab")), UiEditorTool_Prefab },
    { UiDisplayType_Text, {{ CSz("Entity"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "ui_editor_tool UiEditorTool_Entity")), UiEditorTool_Entity },

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
  ui_editor_tool *Element,
  u64  Index = 0,
  ui_render_params *ContainerParams     = &DefaultUiRenderParams_Toolbar,
  ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
{
  /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

  ui_toggle_button_handle ButtonHandles[] =
  {
            {
            UiDisplayType_Text,
      {{
          
                    CSz("Brush")
,
        }},

      
            {}
,

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_tool UiEditorTool_Brush"),
        Cast(void*, Index)
      ),
      UiEditorTool_Brush,
    },

        {
            UiDisplayType_Text,
      {{
          
                    CSz("Prefab")
,
        }},

      
            {}
,

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_tool UiEditorTool_Prefab"),
        Cast(void*, Index)
      ),
      UiEditorTool_Prefab,
    },

        {
            UiDisplayType_Text,
      {{
          
                    CSz("Entity")
,
        }},

      
            {}
,

      UiId(
        Cast(void*, Window),
        Cast(void*, Element),
        Cast(void*, "ui_editor_tool UiEditorTool_Entity"),
        Cast(void*, Index)
      ),
      UiEditorTool_Entity,
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

  DrawButtonGroup(&Result, GroupName, &DefaultUiRenderParams_Button, ContainerParams);
  return Result;
}










































































































































































link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, prefab_spawn_callback *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x20C4D66A);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle prefab_spawn_callback", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum PrefabSpawnCallback_None", Element, ThisHash), Params))
    {
      Result = True;
            *Element = PrefabSpawnCallback_None;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("DefaultPrefabSpawnCallback"), UiId(Window, "enum PrefabSpawnCallback_DefaultPrefabSpawnCallback", Element, ThisHash), Params))
    {
      Result = True;
            *Element = PrefabSpawnCallback_DefaultPrefabSpawnCallback;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}



































































































