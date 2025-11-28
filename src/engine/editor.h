/* #define EDITOR_DEFAULT_SELECTION_THICKNESS (0.15f) */

enum ui_icon_index
{
  UiIconIndex_AngleDown,
  UiIconIndex_AngleLeft,
  UiIconIndex_AngleRight,
  UiIconIndex_AngleUp,

  UiIconIndex_ArrowDown,
  UiIconIndex_ArrowLeft,
  UiIconIndex_ArrowRight,
  UiIconIndex_ArrowUp,

  UiIconIndex_Clone,

  UiIconIndex_CrossCircle,
  UiIconIndex_Cross,

  UiIconIndex_Brush,

  UiIconIndex_Disk,

  UiIconIndex_EyeCrossed,
  UiIconIndex_EyeHatched,
  UiIconIndex_Eye,
  UiIconIndex_EyeOutline,

  UiIconIndex_FileExport,
  UiIconIndex_FileImport,

  UiIconIndex_Add,
  UiIconIndex_Tombstone,

  UiIconIndex_Trash,

  UiIconIndex_Count,
};

enum ui_editor_tool
poof(@gen_ui_toolbar)
{
  UiEditorTool_Brush,
  UiEditorTool_Prefab,
};

enum ui_layer_toolbar_actions
poof(@gen_ui_toolbar)
{
  LayerToolbarActions_NoAction   poof(@ui_skip),

  LayerToolbarActions_Rename          poof(@ui_display_name({})),
  LayerToolbarActions_Duplicate       ,
  LayerToolbarActions_ExportAsPrefab  ,
  LayerToolbarActions_Delete          ,
  //
  // .. ?
};

#if 0
enum ui_brush_actions
poof(@gen_ui_toolbar)
{
  UiBrushAction_NoAction poof(@ui_skip),

  UiBrushAction_New,
  UiBrushAction_Save,
  UiBrushAction_Duplicate,
};

enum ui_reorder_action
poof(@gen_ui_toolbar)
{
  UiReorderAction_NoAction    poof(@ui_skip),
  UiReorderAction_Hide        poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Eye)),
  UiReorderAction_ReorderUp   poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowUp)),
  UiReorderAction_ReorderDown poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowDown)),
  UiReorderAction_Duplicate   poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Clone)),
  UiReorderAction_SetBrush    poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Brush)),
};

enum ui_layer_edit_actions
poof(@gen_ui_toolbar)
{
  UiLayerEditAction_NoAction  poof(@ui_skip),
  UiLayerEditAction_Delete       poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Trash)),
};

enum ui_brush_layer_actions
poof(@gen_ui_toolbar)
{
  UiBrushLayerAction_NoAction  poof(@ui_skip),

  UiBrushLayerAction_MoveUp    poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleRight)),
  UiBrushLayerAction_MoveDown,
  UiBrushLayerAction_Duplicate,
  UiBrushLayerAction_Delete,
};
#endif


enum ui_editor_action
poof(@gen_ui_toolbar)
{
  UiEditorAction_NoAction       poof(@ui_skip),
  UiEditorAction_New            poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Add)),
  UiEditorAction_Show           poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeOutline)),
  UiEditorAction_Hide           poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Eye)),
  UiEditorAction_HideObstructed poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_EyeHatched)),
  UiEditorAction_ReorderUp      poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowUp)),
  UiEditorAction_ReorderDown    poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_ArrowDown)),
  UiEditorAction_Duplicate      poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Clone)),
  UiEditorAction_SetBrush       poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Brush)),
  UiEditorAction_ExportAsPrefab poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_FileExport)),
  UiEditorAction_Save           poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Disk)),
  UiEditorAction_Delete         poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_Trash)),
  UiEditorAction_Expand         poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleRight)),
  UiEditorAction_Collapse       poof(@ui_display_texture(.IconTexture = &Ui->IconTextureArray, .IconId = UiIconIndex_AngleDown)),
  UiEditorAction_Count          poof(@ui_skip),
};


//
// NOTE(Jesse): This is more-or-less duplicated in the face_index enum.  Coalesce them?
// @duplicate_face_index_enum
//
// TODO(Jesse): Rename this to `axis_dir` or something..
enum voxel_rule_direction
{
  VoxelRuleDir_PosX,
  VoxelRuleDir_NegX,

  VoxelRuleDir_PosY,
  VoxelRuleDir_NegY,

  VoxelRuleDir_PosZ,
  VoxelRuleDir_NegZ,

  VoxelRuleDir_Count poof(@ui_skip),
};
CAssert(VoxelRuleDir_Count == 6);

poof(string_and_value_tables(voxel_rule_direction))
#include <generated/string_and_value_tables_voxel_rule_direction.h>


enum shape_axis
{
  ShapeAxis_InferFromMajorAxis,

  ShapeAxis_PosX,
  ShapeAxis_NegX,

  ShapeAxis_PosY,
  ShapeAxis_NegY,

  ShapeAxis_PosZ,
  ShapeAxis_NegZ,

  ShapeAxis_Count poof(@ui_skip),
};

poof(string_and_value_tables(shape_axis))
#include <generated/string_and_value_tables_shape_axis.h>

struct world;

#define EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS r32 MinValue = 0.f, r32 MaxValue = 1.f
#define EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES MinValue, MaxValue

poof(
  func generic_button_group_for_enum(enum_t, type_poof_symbol NamePrefix, type_poof_symbol extra_poof_flags)
  {
    link_internal ui_toggle_button_group
    (NamePrefix)ButtonGroup_(enum_t.name)( renderer_2d *Ui,
                                         window_layout *Window,
                                                    cs  GroupName,
                                           enum_t.name *Element,
                                      ui_render_params *Params     = &DefaultUiRenderParams_Generic,
                          ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
    {
      ui_toggle_button_handle ButtonHandles[] =
      {
        enum_t.map(enum_v)
        {
          { UiDisplayType_Text, {{ CSz("enum_v.name.strip_all_prefix"), }}, {}, UiId(Window, Cast(void*, Element), Cast(void*, "enum_t.name enum_v.name")), enum_v.name },
        }
      };

      ui_toggle_button_handle_buffer ButtonBuffer = {
        ArrayCount(ButtonHandles),
        ButtonHandles
      };

      ui_toggle_button_group Result = DrawButtonGroupForEnum(Ui, &ButtonBuffer, GroupName, Cast(u32*, Element), Params, ui_toggle_button_group_flags(ExtraFlags(extra_poof_flags)));
      return Result;
    }
  }
)

// multi-select group
//
poof(
  func toggle_button_group_for_enum(enum_t)
  {
    enum_t.has_tag(bitfield)?
    {
      generic_button_group_for_enum(enum_t, {Toggle}, {|ToggleButtonGroupFlags_TypeMultiSelectButton})
    }
    {
      poof_error { Enum without @bitfield tag (enum_t.name) cannot create a multi-select button group. }
    }
  }
)

// radio group
//
poof(
  func radio_button_group_for_enum(enum_t)
  {
    enum_t.has_tag(bitfield)? // TODO(Jesse): Pretty sure this is bogus.
    {
      poof_error { Enum with @bitfield tag (enum_t.name) cannot create a radio button group. }
    }
    {
      link_internal void
      RadioSelect(ui_toggle_button_group *RadioGroup, enum_t.name Selection)
      {
        ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Selection;
        SetRadioButton(RadioGroup, ToggleHandle, True);
        /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
      }

      generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_TypeRadioButton})
    }
  }
)

poof(
  func gen_ui_toggle_button_handle_from_runtime_value(enum_t)
  {
    link_internal ui_toggle_button_handle
    ButtonHandleForEnumValue(renderer_2d *Ui, enum_t.name Value, ui_id BaseId)
    {
      Assert(BaseId.ElementBits == 0);
      Assert(BaseId.HashBits == 0);

      ui_toggle_button_handle Result;

      switch (Value)
      {
        enum_t.map(enum_v)
        {
          enum_v.has_tag(ui_skip)?
          {
            case enum_v.name: {} break;
          }
          {
            case enum_v.name:
            {
              Result =
              {
                enum_v.has_tag(ui_display_texture)?
                {
                  UiDisplayType_Icon,
                  enum_v.tag_value(ui_display_texture),
                  /// Tooltip
                  CSz("enum_v.name.strip_all_prefix"),
                }
                {
                  UiDisplayType_Text,
                  {{
                    /// Name
                    enum_v.has_tag(ui_display_name)?
                    {
                      enum_v.tag_value(ui_display_name)
                    }{
                      CSz("enum_v.name.strip_all_prefix")
                    },
                  }},

                  /// Tooltip
                  enum_v.has_tag(ui_display_name)?
                  {
                    CSz("enum_v.name.strip_all_prefix")
                  }{
                    {}
                  },
                }
                UiId(
                  BaseId.WindowBits,
                  BaseId.InteractionBits,
                  UiMaskAndCastPointer("enum_t.name enum_v.name"),
                  enum_v.name
                ),
                enum_v.name,
              };
            } break;
          }

        }
      }
      return Result;
    }
  }
)

poof(
  func toolbar_for_enum(enum_t)
  {
    gen_ui_toggle_button_handle_from_runtime_value(enum_t)

    radio_button_group_for_enum(enum_t)

    link_internal ui_toggle_button_group
    PushToolbar(     renderer_2d *Ui, 
                   window_layout *Window,
                              cs  GroupName,
                     enum_t.name *Element,
                             u64  Index = 0,
                ui_render_params *ContainerParams     = &DefaultUiRenderParams_Toolbar,
    ui_toggle_button_group_flags  ExtraFlags = ToggleButtonGroupFlags_None)
    {
      /* auto Result = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags); */

      ui_toggle_button_handle ButtonHandles[] =
      {
        enum_t.map(enum_v)
        {
          enum_v.has_tag(ui_skip)?{}
          {
            {
              enum_v.has_tag(ui_display_texture)?
              {
                UiDisplayType_Icon,
                enum_v.tag_value(ui_display_texture),
                /// Tooltip
                CSz("enum_v.name.strip_all_prefix"),
              }
              {
                UiDisplayType_Text,
                {{
                  /// Name
                  enum_v.has_tag(ui_display_name)?
                  {
                    enum_v.tag_value(ui_display_name)
                  }{
                    CSz("enum_v.name.strip_all_prefix")
                  },
                }},

                /// Tooltip
                enum_v.has_tag(ui_display_name)?
                {
                  CSz("enum_v.name.strip_all_prefix")
                }{
                  {}
                },
              }
              UiId(
                Cast(void*, Window),
                Cast(void*, Element),
                Cast(void*, "enum_t.name enum_v.name"),
                Cast(void*, Index)
              ),
              enum_v.name,
            },
          }
        }
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
  }
)


poof(
  func do_editor_ui_for_vector_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal b32
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        b32 Result = False;
        u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x(type.hash));

        type.member(0, (E) 
        {
          /* PushTableStart(Ui); */
            if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

            if (Value)
            {
              u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
                PushTableStart(Ui);
                  E.map_array(e_index)
                  {
                    Result |= DoEditorUi(Ui, Window, &Value->(E.name)[e_index], {}, ThisHash, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
                  }
                PushTableEnd(Ui);
                /* PushNewRow(Ui); */
              EndColumn(Ui, Start);
            }
          /* PushTableEnd(Ui); */
        })
        PushNewRow(Ui);
        return Result;
      }
    }
  }
)

poof(
  func do_editor_ui_for_primitive_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal b32
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        b32 Result = False;
        u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x(type.hash));

        Params = Params ? Params : &DefaultUiRenderParams_Blank;

        if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

        if (Value)
        {
          u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
            PushTableStart(Ui);
              if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; Result = True; }
                  PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
              if (Button(Ui, CSz("+"), UiId(Window, "increment", Value, ThisHash), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; Result = True; }
            PushTableEnd(Ui);
          EndColumn(Ui, Start);
        }
        else
        {
          PushColumn(Ui, CSz("(null)"), Params);
          PushNewRow(Ui);
        }

        return Result;
      }

    }
  }
)

poof(
  func do_editor_ui_for_compound_type_decl(type) @code_fragment
  {
    struct type;
    link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
  }
)

poof(
  func do_editor_ui_for_compound_type(type)
  {
    type.has_tag(do_editor_ui)?
    {
      /// NOTE(Jesse): I would really like to call do_editor_ui_for_compound_type_decl
      /// here, but C++ is a fucking garbage fire and doesn't let you redeclare default parameters.
      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Element, cs Name, u32 ParentHash, ui_render_params *Params)
    }
    {
      do_editor_ui_for_compound_type_decl(type)
    }
    {
      u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x(type.hash));

      if (Element)
      {
        // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
        // not drawing the toggl-y thing if we just want to dump the members.
        b32 DrawChildren = True;
        b32 DidToggle = False;
        if (Name.Count)
        {
          if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle type.name", Element, ThisHash), Params))
          {
            DidToggle = True;
            PushNewRow(Ui);
          }
          else
          {
            DrawChildren = False;
          }
        }

        if (DrawChildren)
        {
          if (Name.Count) { PushTableStart(Ui); }

          if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            type.map(member)
            {
              {
                /* member.has_tag(ui_null_behavior)? */
                /* { */
                /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
                /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
                /* }{} */

                {
                  member.has_tag(ui_display_condition)?  { if ((member.tag_value(ui_display_condition))) }{}
                  { /// NOTE(Jesse): this scope is here for the ui_display_condition if above..
                    /// Yes, it's pretty janky, but it's the best I could come up with in a few minutes.
                    ///
                    /// It also hides the Member and MemberName locals, which .. is fine ..
                    cs MemberName = member.has_tag(ui_display_name)? {member.tag_value(ui_display_name)}{CSz("member.name")};

                    member.has_tag(ui_skip)?
                    {
                    }
                    {
                      member.has_tag(ui_construct_as)?
                      {
                        auto Value = member.tag_value(ui_construct_as)(Element->member.name);
                        DoEditorUi(Ui, Window, &Value, MemberName, ThisHash, Params);
                      }
                      {
                        member.is_array?
                        {
                          member.has_tag(ui_display_name)? {poof_error(ui_display_name tag is incompatible with array members )}

                          if (ToggleButton(Ui,
                                CSz("v member.name[member.array]"),
                                CSz("> member.name[member.array]"),
                                UiId(Window, "toggle type.name member.type member.name", Element->(member.name), ThisHash),
                                Params ))
                          {
                            OPEN_INDENT_FOR_TOGGLEABLE_REGION();
                              PushNewRow(Ui);
                              member.has_tag(array_length)?
                                {
                                  s32 End = s32((member.tag_value(array_length)));
                                  Assert( End < member.array );
                                }{
                                  s32 End = member.array;
                                }
                              RangeIterator(ArrayIndex, End)
                              {
                                member.has_tag(custom_ui)?
                                {
                                  member.tag_value(custom_ui);
                                }
                                {
                                  DoEditorUi(Ui,
                                      Window,
                                      Element->(member.name)+ArrayIndex,
                                      FSz("member.name[%d]", ArrayIndex),
                                      ThisHash,
                                      Params);
                                }
                                member.is_primitive?  { PushNewRow(Ui); }
                              }
                            CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
                          }
                          PushNewRow(Ui);
                        }
                        {
                          member.has_tag(custom_ui)?
                          {
                            member.tag_value(custom_ui);
                          }
                          {
                            member.is_type(b32)?
                            {
                              auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name));
                              DoEditorUi(Ui,
                                         Window,
                                         Cast(u8*, Member),
                                         MemberName,
                                         ThisHash,
                                         &DefaultUiRenderParams_Checkbox
                                         member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                            }
                            {
                              member.is_union?
                              {
                                member.name?
                                {
                                  auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name));
                                  DoEditorUi(Ui,
                                             Window,
                                             Member,
                                             MemberName,
                                             ThisHash,
                                             Params
                                             member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                                }
                                {
                                  /// Don't display anythinig for unnamed unions
                                }
                              }
                              {
                                member.is_function?
                                {
                                }
                                {
                                  auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name));
                                  DoEditorUi(Ui,
                                             Window,
                                             Member,
                                             MemberName,
                                             ThisHash,
                                             Params
                                             member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                                }
                              }
                            }
                          }
                        }
                      }

                      member.is_primitive?
                      {
                        PushNewRow(Ui);
                      }
                    }

                  }
                }
              }
            }
          if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
          if (Name.Count) { PushTableEnd(Ui); }
        }
        else
        {
          PushNewRow(Ui);
        }

      }
      else
      {
        PushColumn(Ui, Name, Params);
        PushColumn(Ui, CSz("(null)"), Params);
        PushNewRow(Ui);
      }

    }
  }
)

poof(
  func do_editor_ui_for_enum(enum_t)
  {
    enum_t.has_tag(ui_display_radio)?
    {
      do_editor_ui_for_radio_enum(enum_t)
    }
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, enum_t.name *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
      {
        u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x(enum_t.hash));

        if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

        cs ElementName = ToStringPrefixless(*Element);
        ui_id ToggleButtonId = UiId(Window, "toggle enum_t.name", Element, ThisHash);
        if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
        {
          PushNewRow(Ui);
          enum_t.map(value)
          {
            if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
            if (Button(Ui, CSz("value.name.strip_all_prefix"), UiId(Window, "enum value.name", Element, ThisHash), Params))
            {
              enum_t.has_tag(bitfield)?
              {
                if ((value.name) == enum_t.name(0))
                {
                  *Element = enum_t.name(0);
                }
                else
                {
                  if ((*Element & value.name) == value.name)
                  {
                    *Element = enum_t.name(*Element&~value.name);
                  }
                  else
                  {
                    *Element = enum_t.name(*Element|value.name);
                  }
                }
              }
              {
                *Element = value.name;
              }

              SetToggleButton(Ui, ToggleButtonId, False);
            }
            PushNewRow(Ui);
          }
        }
        else
        {
          PushNewRow(Ui);
        }
      }
    }
  }
)

poof(
  func do_editor_ui_for_container(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Container, cs Name, u32 ParentHash, UI_FUNCTION_PROTO_NAMES)
    {
      u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x(type.hash));

      if (Container)
      {
        if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container, ThisHash), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
        {
          PushNewRow(Ui);
          IterateOver(Container, Element, ElementIndex)
          {
            if (Element)
            {
              DoEditorUi(Ui, Window, Element, CS(ElementIndex), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
              PushNewRow(Ui);
            }
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
  }
)


poof(
  func do_editor_ui_for_radio_enum(enum_t)
  {
    radio_button_group_for_enum(enum_t)

    link_internal ui_toggle_button_group
    DoEditorUi( renderer_2d *Ui,
                window_layout *Window,
                enum_t.name *Element,
                cs GroupName,
                u32 ParentHash,
                ui_render_params *Params = &DefaultUiRenderParams_Generic,
                ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None)
    {
      ui_toggle_button_group RadioGroup = RadioButtonGroup_(enum_t.name)(Ui, Window, GroupName, Element, Params, ExtraFlags);
      return RadioGroup;
    }
  }
)


poof(do_editor_ui_for_primitive_type({s64 u64 s32 u32 s16 u16 s8 u8}));
#include <generated/do_editor_ui_for_scalar_type_688724926.h>


poof(
  for_datatypes(struct)
  func (struct_t)
  {
    struct_t.has_tag(do_editor_ui)?  { do_editor_ui_for_compound_type_decl(struct_t); }
  }
)
#include <generated/for_datatypes_do_editor_ui.h>


link_internal b32
DebugSlider(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, r32 Min, r32 Max, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = {};
  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Generic);
    PushTableStart(Ui);
      if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

      auto Range = Max-Min;
      r32 PercFilled = ((*Value)-Min)/Range;

      r32 Width = 50.f;

      if (Value)
      {
        cs ValueText = CS(*Value);
        v2 TextDim = GetDim(GetDrawBounds(ValueText, &DefaultStyle));

        v2 Offset = V2(Width/2.f-TextDim.x/2.f, 0.f);

        Text(Ui, ValueText, &DefaultStyle, UiElementLayoutFlag_NoAdvance, Offset);
      }

      interactable_handle BargraphButton = PushButtonStart(Ui, UiId(Window, "debug_slider", Value));
        PushSliderBar(Ui, PercFilled, UI_WINDOW_BEZEL_DEFAULT_COLOR_SATURATED, UI_WINDOW_BEZEL_DEFAULT_COLOR_MUTED, Width); // Value marker
      PushButtonEnd(Ui);

      v2 Offset = {};
      if (Pressed(Ui, &BargraphButton, &Offset))
      {
        r32 NewPerc = Clamp01(Offset.x / Width);
        r32 NewValue = (Range*NewPerc) + Min;
        *Value = NewValue;
        Result = True;
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
  return Result;
}

link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  b32 Result = {};
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(Name.Start)));

  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Blank); }

  ui_id BaseInteraction = UiId(Window, 0, Value, ThisHash);
  b32 Editing = Ui->CurrentEditorInteraction == BaseInteraction;

  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    if (Value)
    {
      if (Editing)
      {
        if (Button(Ui, CSz("-"), UiId(BaseInteraction.E[0], UiMaskAndCastPointer("decrement"), BaseInteraction.E[2], BaseInteraction.E[3]))) { *Value = *Value - 1.f; Result = True; }
          Result |= DebugSlider(Ui, Window, Value, {}, MinValue, MaxValue);
        if (Button(Ui, CSz("+"), UiId(BaseInteraction.E[0], UiMaskAndCastPointer("increment"), BaseInteraction.E[2], BaseInteraction.E[3]))) { *Value = *Value + 1.f; Result = True; }
      }
      else
      {
        if (Button(Ui, FSz("%.2f", f64(*Value)), BaseInteraction)) { Ui->CurrentEditorInteraction = BaseInteraction; }
      }
    }
    else
    {
      PushColumn(Ui, CSz("(null)"));
    }
  EndColumn(Ui, Start);

  if (Editing)
  {
    if ( (Ui->Input->LMB.Clicked && Result == False) ||
          Ui->Input->Enter.Clicked )
    {
      // We clicked something that wasn't the widget, so overwrite the interaction
      Ui->CurrentEditorInteraction = {};
    }
  }

  return Result;
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, b8 *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Checkbox)
{
  UNPACK_UI_RENDER_PARAMS(Params);

  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(Name.Start)));

  interactable_handle ButtonHandle = PushButtonStart(Ui, UiId(Window, "toggle", Value, ThisHash), BStyle);

    PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Generic);

    if (Value)
    {
      if (*Value)
      {
        PushUntexturedQuad(Ui, V2(2.f, 2.f), V2(Params->FStyle->Font.Size.y)-4.f, zDepth_Border, &Global_DefaultCheckboxForeground, DefaultCheckboxPadding, UiElementLayoutFlag_NoAdvance );
      }
    }
    else
    {
      PushColumn(Ui, CSz("(null)"), Params);
    }

    PushUntexturedQuad(Ui, {}, V2(Params->FStyle->Font.Size.y), zDepth_Text, &Global_DefaultCheckboxBackground, DefaultCheckboxPadding, UiElementLayoutFlag_Default );



  PushButtonEnd(Ui);

  if (Clicked(Ui, &ButtonHandle))
   { *Value = !(*Value); }
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cs *Value, cs Name, u32 ParentHash, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  Value ?
    PushColumn(Ui, *Value, EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, void *Value, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Column)
{
  /* u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(Name.Start)); */
  if (Name.Count) { PushColumn(Ui, CS(Name), Params); }
  Value ?
    PushColumn(Ui, FSz("0x%x",umm(Value)), &DefaultUiRenderParams_Column) :
    PushColumn(Ui, CSz("(null)"), &DefaultUiRenderParams_Column);
  PushNewRow(Ui);
}


poof(do_editor_ui_for_vector_type({v4i v4 v3i v3 v2i v2 Quaternion m4}));
#include <generated/do_editor_ui_for_vector_type_688873645.h>


link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cp *Value, cs Name, u32 ParentHash, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ u32(u64(Name.Start)));

  DoEditorUi(Ui, Window, &Value->WorldP, CSz("WorldP"), ThisHash, Params);
  DoEditorUi(Ui, Window, &Value->Offset, CSz("Offset"), ThisHash, Params);
}

poof(string_and_value_tables(maybe_tag))
#include <generated/string_and_value_tables_maybe_tag.h>
poof(do_editor_ui_for_enum(maybe_tag))
#include <generated/do_editor_ui_for_enum_maybe_tag.h>

poof(string_and_value_tables(data_type))
#include <generated/string_and_value_tables_data_type.h>
poof(do_editor_ui_for_enum(data_type))
#include <generated/do_editor_ui_for_enum_data_type.h>

poof(do_editor_ui_for_enum(shape_axis))
#include <generated/do_editor_ui_for_enum_shape_axis.h>

poof(do_editor_ui_for_enum(voxel_rule_direction))
#include <generated/do_editor_ui_for_enum_voxel_rule_direction.h>

poof(do_editor_ui_for_compound_type(rect3))
#include <generated/do_editor_ui_for_compound_type_rect3.h>

poof(do_editor_ui_for_compound_type(rect3i))
#include <generated/do_editor_ui_for_compound_type_rect3i.h>

poof(do_editor_ui_for_compound_type(rect3cp))
#include <generated/do_editor_ui_for_compound_type_rect3cp.h>

poof(block_array_h(asset_thumbnail, {8}, {}))
#include <generated/block_array_h_asset_thumbnail_688856411.h>

poof(do_editor_ui_for_compound_type(ray))
#include <generated/do_editor_ui_for_compound_type_ray.h>

poof(do_editor_ui_for_compound_type(maybe_ray))
#include <generated/do_editor_ui_for_compound_type_maybe_ray.h>

enum selection_modification_mode poof(@gen_string_and_value_tables @do_editor_ui)
{
  SelectionModificationMode_None,       // Not modifying the selection
  SelectionModificationMode_Initialize, // Initializing a new selection area
  SelectionModificationMode_Modify,     // Modifying the current selection area
};

struct selection_modification_state
{
  // NOTE(Jesse): This is the source of truth for if we're modifying the selection
  face_index ClickedFace;
  v3 ClickedP[2];
};

enum ui_noise_type
{
  NoiseType_Perlin,
  NoiseType_Voronoi,
  NoiseType_White,
};

poof(string_and_value_tables(ui_noise_type))
#include <generated/string_and_value_tables_ui_noise_type.h>
poof(radio_button_group_for_enum(ui_noise_type));
#include <generated/radio_button_group_for_bitfield_enum_ui_noise_type.h>
poof(do_editor_ui_for_enum(ui_noise_type))
#include <generated/do_editor_ui_for_enum_ui_noise_type.h>

















enum world_edit_selection_mode
{
  SelectionMode_Noop,

  SelectionMode_ResizeSingleLinearAxis,
  SelectionMode_ResizeBothLinearAxies,
  SelectionMode_ResizeAllAxies,

  SelectionMode_TranslateLinear,
  SelectionMode_TranslatePlanar,
};
poof(string_and_value_tables(world_edit_selection_mode))
#include <generated/string_and_value_tables_world_edit_selection_mode.h>

enum world_edit_tool
{
  WorldEdit_Tool_Disabled,      // poof(@ui_skip)
  WorldEdit_Tool_Select,        // world_edit_selection_mode
  /* WorldEdit_Tool_Brush,         // world_edit_brush_type */
  WorldEdit_Tool_Eyedropper,
  WorldEdit_Tool_BlitEntity,
  /* WorldEdit_Tool_StandingSpots, // Recomputes standing spots for an area */
};

enum world_edit_brush_type
{
  WorldEditBrushType_Disabled poof(@no_serialize), // poof(@ui_skip)
  WorldEditBrushType_Layered,
  WorldEditBrushType_Simple,
};

enum world_edit_blend_mode
{
  WorldEdit_Mode_Union,
  WorldEdit_Mode_Intersection,
  WorldEdit_Mode_Difference,

  WorldEdit_Mode_SmoothUnion,
  WorldEdit_Mode_SmoothIntersection,
  WorldEdit_Mode_SmoothDifference,

  WorldEdit_Mode_Additive,    // Adds layer value to noise value
  WorldEdit_Mode_Subtractive, // Subtracts layer value from noise value
  WorldEdit_Mode_Multiply,
  /* WorldEdit_Mode_Threshold,   // Sets CurrentSample = SampleValue */
  WorldEdit_Mode_Disabled,    // Useful for turning the layer off
};

enum world_edit_color_blend_mode
{
  WorldEdit_ColorBlendMode_ValuePositive,
  WorldEdit_ColorBlendMode_ValueNegative,

  WorldEdit_ColorBlendMode_Surface,
  WorldEdit_ColorBlendMode_Always,

  // TODO(Jesse): Put back in?
  /* WorldEdit_ColorBlendMode_Additive, */
  /* WorldEdit_ColorBlendMode_Subtractive, */
  /* WorldEdit_ColorBlendMode_Multiply, */
  /* WorldEdit_ColorBlendMode_Divide, */

  WorldEdit_ColorBlendMode_Disabled, // Useful for turning the layer off
  WorldEdit_ColorBlendMode_FinalBlend,
};

enum world_edit_blend_mode_modifier poof(@bitfield)
{
  WorldEdit_Modifier_None           =     0,

  /* WorldEdit_ValueModifier_Surface   = (1<<0), */
  WorldEdit_ValueModifier_ClampPos  = (1<<1),
  WorldEdit_ValueModifier_ClampNeg  = (1<<2),
  WorldEdit_ValueModifier_Threshold = (1<<3),

  WorldEdit_ColorModifier_Discard   = (1<<4),

  // NOTE(Jesse): Unsupported for now, unclear if it will be again ..
  // WorldEdit_Modifier_Flood    = xxxx,
};








// TODO(Jesse): Probably do perlin/voronoi in terms of this?
struct generic_noise_params
{
  r32 Threshold = 3.0f;
   v3 Period    = {{8.f, 8.f, 8.f}}; poof(@ui_value_range(0.1f, 20.f))
  r32 Amplitude = 8.f;               poof(@ui_value_range(0.1f, 20.f))
   /* v3 HSVColor  = DEFAULT_HSV_COLOR; */
   v3 RGBColor  = DEFAULT_RGB_COLOR;
};

// TODO(Jesse): Get rid of zMin
#define UNPACK_NOISE_PARAMS(P)                                          \
  v3i WorldChunkDim = GetWorldChunkDim();                               \
  v3i           Dim = Chunk->Dim;                                       \
  r32        Thresh = Cast(generic_noise_params*, (P))->Threshold;      \
  s64          zMin = s64(Cast(generic_noise_params*, (P))->Threshold); \
  v3         Period = Cast(generic_noise_params*, (P))->Period;         \
  s32     Amplitude = s32(Cast(generic_noise_params*, (P))->Amplitude); \
  v3       RGBColor = Cast(generic_noise_params*, (P))->RGBColor;       \
  v3i     SrcToDest = {}


struct white_noise_params
poof(@do_editor_ui)
{
};


struct perlin_noise_params
poof(@do_editor_ui)
{
  v3 Period = {{8.f, 8.f, 8.f}}; poof(@ui_value_range(0.1f, 20.f))
};

poof(are_equal(perlin_noise_params))
#include <generated/are_equal_perlin_noise_params.h>

struct voronoi_noise_params
poof(@do_editor_ui)
{
  v3  Period    = {{10.f, 10.f, 10.f}}; poof(@ui_value_range(0.1f, 20.f))
  r32 Squareness;
  r32 MaskChance;
};


poof(string_and_value_tables(world_edit_blend_mode_modifier))
#include <generated/string_and_value_tables_world_edit_blend_mode_modifier.h>

poof(do_editor_ui_for_enum(world_edit_blend_mode_modifier))
#include <generated/do_editor_ui_for_radio_enum_world_edit_blend_mode_modifier.h>




poof(toggle_button_group_for_enum(engine_debug_view_mode))
#include <generated/toggle_button_group_for_enum_engine_debug_view_mode.h>



poof(do_editor_ui_for_radio_enum(asset_window_view_mode))
#include <generated/do_editor_ui_for_radio_enum_asset_window_view_mode.h>

poof(string_and_value_tables(world_edit_blend_mode))
#include <generated/string_and_value_tables_world_edit_blend_mode.h>

poof(do_editor_ui_for_enum(world_edit_blend_mode))
#include <generated/do_editor_ui_for_enum_QKyV0TwP.h>

poof(string_and_value_tables(world_edit_color_blend_mode))
#include <generated/string_and_value_tables_world_edit_color_blend_mode.h>
poof(do_editor_ui_for_enum(world_edit_color_blend_mode))
#include <generated/do_editor_ui_for_radio_enum_world_edit_color_blend_mode.h>


poof(do_editor_ui_for_radio_enum(world_edit_tool))
#include <generated/do_editor_ui_for_radio_enum_world_edit_tool.h>

poof(do_editor_ui_for_radio_enum(world_edit_brush_type))
#include <generated/do_editor_ui_for_radio_enum_world_edit_brush_type.h>








/* enum world_update_op_shape_type */
/* { */
/*   type_world_update_op_shape_params_noop, */

/*   type_world_update_op_shape_params_sphere, */
/*   type_world_update_op_shape_params_rect, */
/*   type_world_update_op_shape_params_cylinder, */
/*   type_world_update_op_shape_params_asset, */
/*   type_world_update_op_shape_params_chunk_data, */

/*   type_world_update_op_shape_params_count, */
/* }; */

/* poof(string_and_value_tables(world_update_op_shape_type)) */
/* #include <generated/string_and_value_tables_world_update_op_shape_type.h> */


struct asset;
struct world_update_op_shape_params_asset
{
  asset_id AssetId;
       u64 ModelIndex;
        cp Origin;
};

struct world_update_op_shape_params_chunk_data
{
  chunk_data Data;
          v3 SimSpaceOrigin;
};

struct world_update_op_shape_params_rect
poof(@do_editor_ui)
{
  v3 Dim;
};

struct world_update_op_shape_params_sphere
poof(@do_editor_ui)
{
   cp Location; poof(@ui_skip)
  f32 Radius; // 0 defaults to filling the minimum radius on the selection area
};


struct world_update_op_shape_params_line
{
   cp P0; poof(@ui_skip)
   cp P1; poof(@ui_skip)
  r32 Radius = 10.f;
};

struct world_update_op_shape_params_cylinder
poof(@do_editor_ui)
{
  r32 Radius = 4.f;
  r32 Height = 25.f;
};

struct world_update_op_shape_params_plane
poof(@do_editor_ui)
{
  shape_axis Orientation;
  f32 Thickness = 2.f;
};

struct world_update_op_shape_params_torus
poof(@do_editor_ui)
{
  f32 MajorRadius = 20.f;
  f32 MinorRadius = 3.f;
};


struct world_update_op_shape_params_pyramid
poof(@do_editor_ui)
{
  f32 Height;
};


// @sdf_shape_step(2): Add new asset struct here
//

#if 0
struct world_edit_shape
{
  world_update_op_shape_type Type;
  union poof(@d_union_type_target)
  {
    world_update_op_shape_params_sphere     world_update_op_shape_params_sphere;
    world_update_op_shape_params_rect       world_update_op_shape_params_rect;
    world_update_op_shape_params_asset      world_update_op_shape_params_asset;
    world_update_op_shape_params_chunk_data world_update_op_shape_params_chunk_data;
  };
};

struct world_edit_brush_constraints
{
  world_edit_shape         Shape;

  world_edit_blend_mode          Mode;
  world_edit_blend_mode_modifier Modifier;

  // NOTE(Jesse): If Modifier is Flood, this is set to where the flood should start
  //
  // NOTE(Jesse): This was causing problems; it was getting cold-set to the
  // position of the voxel hovered by the cursor, which made fireballs not work
  // correctly.  Since it's not actually used in any meaningful way I took it
  // off the brush struct and stuffed the value of the flood origin to the
  // center P of whatever the flood shape is.
  //
  /* v3 SimFloodOrigin; */
};

poof(do_editor_ui_for_compound_type(world_edit_brush_constraints))
#include <generated/do_editor_ui_for_compound_type_world_edit_brush.h>
#endif








enum shape_type
{
  ShapeType_Rect     = 0,
  ShapeType_Sphere   = 1,
  ShapeType_Line     = 2,
  ShapeType_Cylinder = 3,
  ShapeType_Plane    = 4,
  ShapeType_Torus    = 5,
  ShapeType_Pyramid  = 6,

  // @sdf_shape_step(1): Add shape types here
  //
};
poof(string_and_value_tables(shape_type))
#include <generated/string_and_value_tables_shape_type.h>

struct shape_layer_advanced_params
poof(@do_editor_ui)
{
  r32 Hollow;
  r32 Rounding;
   v3 Stretch;
   v3 Repeat;
   v3 Rotation; poof(@ui_value_range(-180.f, 180.f))
};

struct shape_layer
poof(@do_editor_ui)
{
  shape_type Type; poof(@ui_display_name(CSz("Shape Type")))

  // NOTE(Jesse): Intentionally not a d-union such that you can toggle between
  // them and your parameter selections stay intact.
  world_update_op_shape_params_rect     Rect;     poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Rect))
  world_update_op_shape_params_sphere   Sphere;   poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Sphere))
  world_update_op_shape_params_line     Line;     poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Line))
  world_update_op_shape_params_cylinder Cylinder; poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Cylinder))
  world_update_op_shape_params_plane    Plane;    poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Plane))
  world_update_op_shape_params_torus    Torus;    poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Torus))
  world_update_op_shape_params_pyramid  Pyramid;  poof(@ui_display_name({}) @ui_display_condition(Element->Type == ShapeType_Pyramid))
  // @sdf_shape_step(6): Add an instance of the new shape here
  //


  // NOTE(Jesse): Just in another struct for the UI
  shape_layer_advanced_params Advanced;
};

poof(gen_constructor(shape_layer))
#include <generated/gen_constructor_SMWhmUr7.h>

// NOTE(Jesse): This is intentionally not a d_union such that you can flip
// between different noise selections and your parameters stay intact.
struct noise_layer
poof(
    @do_editor_ui
    @version(2)
  )
{
  ui_noise_type Type; poof(@ui_display_name(CSz("Noise Type")))
  r32 Power;

  white_noise_params   White;   poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_White))
  perlin_noise_params  Perlin;  poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_Perlin))
  voronoi_noise_params Voronoi; poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_Voronoi))
};

struct noise_layer_1
{
  ui_noise_type Type; poof(@ui_display_name(CSz("Noise Type")))

  white_noise_params   White;   poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_White))
  perlin_noise_params  Perlin;  poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_Perlin))
  voronoi_noise_params Voronoi; poof(@ui_display_name({}) @ui_display_condition(Element->Type == NoiseType_Voronoi))
};

struct noise_layer_0
{
  ui_noise_type Type;

  perlin_noise_params  Perlin;
  voronoi_noise_params Voronoi;
};


link_internal void
Marshal(noise_layer_1 *Stored, noise_layer *Live)
{
  poof(default_marshal(noise_layer_1))
#include <generated/default_marshal_noise_layer_1.h>
}

link_internal void
Marshal(noise_layer_0 *Stored, noise_layer *Live)
{
  poof(default_marshal(noise_layer_0))
#include <generated/default_marshal_noise_layer_0.h>
}





enum brush_layer_type
{
  BrushLayerType_Noise,
  BrushLayerType_Shape,
  BrushLayerType_Brush,
};

poof(string_and_value_tables(brush_layer_type))
#include <generated/string_and_value_tables_enum_brush_layer_type.h>

poof(do_editor_ui_for_enum(brush_layer_type))
#include <generated/do_editor_ui_for_enum_brush_layer_type.h>

struct world_edit_brush;

struct brush_settings
poof(@do_editor_ui @serdes @version(1))
{
  brush_layer_type Type; poof(@ui_display_name(CSz("Brush Type")))

  noise_layer Noise; poof(@ui_display_name({}) @ui_display_condition(Element->Type == BrushLayerType_Noise))
  shape_layer Shape; poof(@ui_display_name({}) @ui_display_condition(Element->Type == BrushLayerType_Shape))

  /* world_edit_brush *Brush; poof(@ui_display_name({}) @ui_display_condition(Element->Type == BrushLayerType_Brush)) */
  world_edit_brush *Brush;
  poof(
    @ui_display_name({})
    @ui_display_condition(Element->Type == BrushLayerType_Brush)
    @custom_ui( DoWorldEditBrushPicker(Ui, Window, Element, ThisHash) )
  )

  //
  // Common across brush types
  //

  /* f32 Power     = 1.f; poof(@ui_value_range( 0.f, 25.f) @ui_display_condition(HasThresholdModifier(Element))) */
  /* f32 Threshold = 0.f; poof(@ui_value_range( 0.f,  1.f) @ui_display_condition(HasThresholdModifier(Element))) */
  r32 ValueBias = 0.f; poof(@ui_value_range(-1.f,  1.f))

  world_edit_blend_mode_modifier ValueModifier;
  world_edit_blend_mode          LayerBlendMode;
  world_edit_color_blend_mode    ColorMode;

  b8 Invert;

  // NOTE(Jesse): This is the relative offset from the base selection, which is
  // used to inflate or contract the area affected by the brush.
  //
  rect3i SelectionModifier;

  v3i BasisOffset; poof(@ui_skip)

  // NOTE(Jesse): The color picker operates in HSV, so we need this to be HSV for now
  v3 HSVColor = DEFAULT_HSV_COLOR;  poof(@custom_ui(PushColumn(Ui, CSz("HSVColor")); DoColorPickerToggle(Ui, Window, &Element->HSVColor, False, ThisHash)))

  b32 Disabled;
};

struct brush_settings_0
poof(@do_editor_ui @serdes)
{
  brush_layer_type Type; poof(@ui_display_name(CSz("Brush Type")))

  noise_layer Noise; poof(@ui_display_name({}) @ui_display_condition(Element->Type == BrushLayerType_Noise))
  shape_layer Shape; poof(@ui_display_name({}) @ui_display_condition(Element->Type == BrushLayerType_Shape))

  world_edit_brush *Brush;

  //
  // Common across brush types
  //

  r32 ValueBias = 0.f; poof(@ui_value_range(-1.f,  1.f))

  world_edit_blend_mode_modifier ValueModifier;
  world_edit_blend_mode          LayerBlendMode;
  world_edit_color_blend_mode    ColorMode;

  b8 Invert;

  // NOTE(Jesse): This is the relative offset from the base selection, which is
  // used to inflate or contract the area affected by the brush.
  //
  rect3i SelectionModifier;

  v3i BasisOffset; poof(@ui_skip)

  // NOTE(Jesse): The color picker operates in HSV, so we need this to be HSV for now
  v3 HSVColor = DEFAULT_HSV_COLOR;  poof(@custom_ui(PushColumn(Ui, CSz("HSVColor")); DoColorPickerToggle(Ui, Window, &Element->HSVColor, False, ThisHash)))
};

link_internal void
DoWorldEditBrushPicker(renderer_2d *Ui, window_layout *Window, brush_settings *Element, umm ParentHash);

poof(are_equal(brush_settings))
#include <generated/are_equal_struct.h>

poof(gen_constructor(brush_settings))
#include <generated/gen_constructor_lJ6fXxTn.h>

struct brush_layer
poof( @do_editor_ui
      @serdes )
{
  brush_settings Settings;     poof(@ui_display_name({}))
  brush_settings PrevSettings; poof(@no_serialize @ui_skip) // Change detection
};



// TODO(Jesse): Make this dynamic .. probably ..
#define MAX_BRUSH_LAYERS 16
#define BRUSH_PREVIEW_TEXTURE_DIM 256
struct layered_brush
poof(@serdes)
{
          s32 LayerCount;
  brush_layer Layers[MAX_BRUSH_LAYERS]; poof(@array_length(Element->LayerCount))
};



struct single_brush
{
  world_edit_blend_mode Mode;
};


struct asset_brush
{
  world_edit_blend_mode Mode;
  world_edit_blend_mode_modifier Modifier;
};







struct world_edit_brush
poof(@serdes)
{
  // NOTE(Jesse): This is so we can just copy the name of the brush in here and
  // not fuck around with allocating a single string when we load these in.
#define NameBuf_Len (256)
  char NameBuf[NameBuf_Len+1]; poof(@ui_text_box @ui_construct_as(CS))

  /* world_edit_shape               Shape; */
  world_edit_blend_mode          BrushBlendMode;
  world_edit_blend_mode_modifier Modifier;

  /* world_edit_brush_type Type; */
  /* union */
  /* { */
  /*   single_brush  Single; */
  /*   asset_brush   Asset; */
    layered_brush Layered;
  /* }; poof(@type_tag(world_edit_brush_type)) */
};

link_internal umm
Hash(world_edit_brush *Brush)
{
  umm Result = Hash(CS(Brush->NameBuf));
  return Result;
}

link_internal b32
AreEqual(world_edit_brush *Thing1, world_edit_brush *Thing2)
{
  b32 Result = StringsMatch(Thing1->NameBuf, Thing2->NameBuf);
  return Result;
}

/* poof(are_equal(world_edit_brush)) */
/* #include <generated/are_equal_world_edit_brush.h> */

poof(hashtable(world_edit_brush))
#include <generated/hashtable_world_edit_brush.h>

poof(maybe(world_edit_brush))
#include <generated/maybe_qVFPWNB0.h>

poof(hashtable_get(world_edit_brush, {cs}, {NameBuf}))
#include <generated/hashtable_get_SlF7m90R.h>

enum world_edit_flag
{
  WorldEditFlag_Tombstone = (1<<0),
  WorldEditFlag_Hidden    = (1<<1),
  WorldEditFlag_Dirty     = (1<<2),
  WorldEditFlag_Selected  = (1<<3),
};

struct world_edit 
poof(@do_editor_ui @serdes @block_array_IndexOfValue)
{
  // TODO(Jesse): Rename to Bounds?
  rect3cp Region = InvertedInfinityRectangle_rect3cp();
  world_edit_brush *Brush;

  v3 Rotation; poof(@ui_value_range(-180.f, 180.f))

  // TODO(Jese): Pack these into a Flags field
  // {
    u32 Flags; // world_edit_flag
    /* b32 Tombstone; */
    b32 Dirty;

    // NOTE(Jesse): Need this so we don't have to do an n^2 loop when doing
    // SelectEdit such that we can also makes
    //
    // Not my favorite, but it's also not the end of the world.
    b32 Selected;
  // }
  u32 Ordinal;
};

typedef world_edit* world_edit_ptr;

poof(are_equal(world_edit))
#include <generated/are_equal_aajSrrGo.h>

// TODO(Jesse): Add `add_tag` to poof so we can reinstate this
//
poof(block_array(world_edit, {128}))
#include <generated/block_array_world_edit_688735882.h>

poof(block_array(world_edit_ptr, {128}))
#include <generated/block_array_world_edit_ptr_688735882.h>

/* poof(add_tag(world_edit_block_array_index, block_array_IndexOfValue)) */

poof(block_array(world_edit_block_array_index, {128}))
#include <generated/block_array_world_edit_block_array_index_688735882.h>

poof(block_array_index_of_value(world_edit_block_array_index, world_edit_block_array_index_block_array, world_edit_block_array_index_block_array_index))
#include <generated/block_array_index_of_value_f9Xbjkcl.h>


enum world_edit_layer_flags
{
  WorldEditLayerFlag_Collapsed = (1 << 0),
  WorldEditLayerFlag_Hidden    = (1 << 1),
};

struct world_edit_layer
poof(@serdes @version(1))
{
  char NameBuf[NameBuf_Len+1]; poof(@ui_text_box @ui_construct_as(CS))

  // NOTE(Jesse): Array of indices into the world_edit block array
  world_edit_block_array_index_block_array EditIndices;

  u32 Flags; // world_edit_layer_flags
};
poof(block_array(world_edit_layer, {128}))
#include <generated/block_array_world_edit_layer_688735882.h>

struct world_edit_layer_0
poof(@serdes @default_marshal(world_edit_layer))
{
  char NameBuf[NameBuf_Len+1]; poof(@ui_text_box @ui_construct_as(CS))
  world_edit_block_array_index_block_array EditIndices;
};


enum prefab_spawn_callback
{
  PrefabSpawnCallback_None poof(@spawn_callback_skip),

  poof(
    for_datatypes(func)  @code_fragment
    func (func_t)
    {
      func_t.has_tag(prefab_spawn_callback)?
      {
        PrefabSpawnCallback_(func_t.name),
      }
    }
  )
#include <generated/(builtin.for_datatypes)_KhyFHEuP.h>
};

struct prefab
poof(@serdes @do_editor_ui)
{
  cs Name;
  world_edit_paged_list Edits;

  cs SpawnCallbackName;
  /* prefab_spawn_callback SpawnCallback; */
};

link_internal b32
AreEqual(prefab *E0, prefab *E1)
{
  return AreEqual(E0->Name, E1->Name);
}

link_internal umm
Hash(prefab *E0)
{
  return Hash(E0->Name);
}

poof(hashtable(prefab))
#include <generated/hashtable_pIp3Bn6L.h>

link_internal void
poof(@prefab_spawn_callback)
DefaultPrefabSpawnCallback(prefab *Prefab, cp SpawnPoint)
{
}

link_internal void
DispatchPrefabSpawnCallback(prefab_spawn_callback SpawnCallbackType, prefab *Prefab, cp SpawnPoint)
{
  switch(SpawnCallbackType)
  {
    case PrefabSpawnCallback_None: {} break;
    poof(
      func (prefab_spawn_callback enum_t) @code_fragment
      {
        enum_t.map(enum_v)
        {
          enum_v.has_tag(spawn_callback_skip)?
          {
          }
          {
            case enum_v.name: { enum_v.name.strip_all_prefix.to_capital_case(Prefab, SpawnPoint); } break; 
          }
        }
      }
    )
#include <generated/anonymous_fxISWGtl.h>

  }
}
// NOTE(Jesse): This isn't really meant to be used outside the the level_editor
// I just packed all the things together such that it's a bit more obvious
// they're all for doing the selection
struct selection_region poof(@do_editor_ui)
{
  u32 Clicks;

  // NOTE(Jesse): We need to save the first point we clicked such that we can
  // synthetically construct the region and construct it while we're clicking
  // on a second point.  The problem without this Base point is that we don't
  // know if the point we clicked is the min or the max of the current box.
  // There's probably a way to get around having this Base point, but it's not
  // really a big deal.
  cp Base;

  rect3cp Region     = InvertedInfinityRectangle_rect3cp();

  v3 Diff;           // When Changed is set, this should be nonzero.
  b32 InitialSelect; // Set when we go from a partial selection state -> fully selected

  selection_modification_mode  ModMode;
  selection_modification_state ModState;
};

struct level_editor
poof(@do_editor_ui)
{
  memory_arena *Memory;

  ui_editor_tool SelectedTool;

  selection_region Selection;

  asset_thumbnail_block_array AssetThumbnails;

  // Used for naming layers during NewLayer
  u32 NextLayerIndex;

  world_edit_layer_block_array       Layers;
  world_edit_layer_block_array_index SelectedLayerIndex = {INVALID_BLOCK_ARRAY_INDEX};

  world_edit_block_array                   Edits;
  world_edit_block_array_index_block_array SelectedEditIndices;

  world_edit                   *HotEdit;      // Hovered
  world_edit_block_array_index  HotEditIndex;

  // TODO(Jesse): This is a stupid form of stoarge.  We don't ever look anything
  // up, we just keep pointers into it.  Change to a paged-array and store the
  // indices such that we save space when doing serialize/deserialize;
  world_edit_brush_hashtable  LoadedBrushes;
  world_edit_brush           *CurrentBrush;
  s32 CurrentBrush_SelectedLayerIndex;

  prefab_hashtable Prefabs;
  prefab *SelectedPrefab;
};





link_internal b32
SelectionComplete(u32 SelectionClicks)
{
  return SelectionClicks == 2;
}


link_internal b32
SelectionIncomplete(u32 SelectionClicks)
{
  return !SelectionComplete(SelectionClicks);
}

link_internal void
ResetSelection(level_editor *Editor)
{
  Editor->Selection = {};
}

link_internal void
ResetSelectionIfIncomplete(level_editor *Editor)
{
  if (SelectionIncomplete(Editor->Selection.Clicks)) { ResetSelection(Editor); }
}

link_internal rect3
GetSelectionRect(world *World, level_editor *Editor)
{
  v3 SelectionMinP = GetSimSpaceP(World, Editor->Selection.Region.Min);
  v3 SelectionMaxP = GetSimSpaceP(World, Editor->Selection.Region.Max);

  rect3 Result = RectMinMax(SelectionMinP, SelectionMaxP);
  return Result;
}

link_internal v3i
GetSelectionDim(world *World, level_editor *Editor)
{
  v3 SelectionMinP = GetSimSpaceP(World, Editor->Selection.Region.Min);
  v3 SelectionMaxP = GetSimSpaceP(World, Editor->Selection.Region.Max);

  v3i Result = V3i(SelectionMaxP - SelectionMinP);
  return Result;
}

link_internal shape_axis
ComputeShapeAxisFromEditDim(v3 Dim)
{
  shape_axis Result = ShapeAxis_Count;

  f32 MaxAxisValue = Max(Max(Dim.x, Dim.y), Dim.z);
  if (MaxAxisValue == Dim.x) { Result = ShapeAxis_PosX; }
  if (MaxAxisValue == Dim.y) { Result = ShapeAxis_PosY; }
  if (MaxAxisValue == Dim.z) { Result = ShapeAxis_PosZ; }

  Assert(Result != ShapeAxis_Count);

  return Result;
}

link_internal world_edit_selection_mode
ComputeSelectionMode(input *Input)
{
  world_edit_selection_mode SelectionMode = {};

  // Intentionally an el-if chain from most specific, to least.  What's the alternative?
  //
  // Alt   is move
  // Shift is resize
  //
  if (Input->Shift.Pressed && Input->Ctrl.Pressed && Input->Alt.Pressed)
  {
    SelectionMode = SelectionMode_ResizeAllAxies;
  }
  else if (Input->Shift.Pressed && Input->Ctrl.Pressed)
  {
    SelectionMode = SelectionMode_ResizeBothLinearAxies;
  }
  else if (Input->Alt.Pressed && Input->Ctrl.Pressed)
  {
    SelectionMode = SelectionMode_TranslateLinear;
  }
  else if (Input->Shift.Pressed)
  {
    SelectionMode = SelectionMode_ResizeSingleLinearAxis;
  }
  else if (Input->Alt.Pressed)
  {
    SelectionMode =  SelectionMode_TranslatePlanar;
  }

  return SelectionMode;
}

link_internal f32
GetSelectionThicknessForDistance(f32 Distance)
{
  f32 Result = Clamp(0.25f, Distance / 3000.f, 12.f);
  return Result;
}

link_internal world_edit_layer *
TryGetSelectedLayer(level_editor *Editor)
{
  world_edit_layer *Result = TryGetPtr(&Editor->Layers, Editor->SelectedLayerIndex);
  return Result;
}

link_internal b32
HasThresholdModifier(brush_settings *Element)
{
  b32 Result = (Element->ValueModifier & WorldEdit_ValueModifier_Threshold);
  return Result;
}

#if 0
link_internal r32
GetPowerFor(world *World, world_edit *Edit, brush_settings *Settings)
{
  r32 Result = 0.f;
  switch (Settings->Type)
  {
    case BrushLayerType_Noise:
    {
      Result = Settings->Power;
    } break;

    case BrushLayerType_Shape:
    {
      switch (Settings->Shape.Type)
      {
        case ShapeType_Rect:
        {
          aabb Rect   = GetSimSpaceRect(World, Edit->Region);
            v3 Dim    = GetDim(Rect);
               Result = MaxChannel(Dim)/4.f;
        } break;

        case ShapeType_Sphere:
        {
          Result = Settings->Shape.Sphere.Radius;
        } break;

        case ShapeType_Line:
        case ShapeType_Cylinder:
        case ShapeType_Plane:
        {
          NotImplemented;
          Result = 0.f;
        } break;

        case ShapeType_Torus:
        {
          Result = Settings->Shape.Torus.MajorRadius;
        } break;
      }
    } break;
  }

  return Result;
}
#endif


link_internal level_editor *
GetEditor();

link_internal b32
CheckSettingsChanged(layered_brush *);

link_internal b32
CheckSettingsChanged(world_edit *);

link_internal b32 HardResetEditor(level_editor *Editor);

link_internal v3
GetHotVoxelForEditMode(engine_resources *Engine, world_edit_blend_mode WorldEditMode);

link_internal v3
GetHotVoxelForFlood(engine_resources *Engine, world_edit_blend_mode WorldEditMode, world_edit_blend_mode_modifier Modifier);

/* link_internal void */
/* ApplyBrushLayer(engine_resources *Engine, brush_layer *Layer, chunk_thumbnail *Preview, world_chunk *DestChunk, v3i SmallestMinOffset); */

link_internal v3i
GetSmallestMinOffset(layered_brush *LayeredBrush, v3i *LargestLayerDim = 0);

/* link_internal void */
/* DrawEditorPreview(engine_resources *Engine, shader *Shader); */

link_internal void
ColorPickerModal(engine_resources *Engine, ui_id ModalId, v3 *HSVDest, b32 ShowColorSwatch = True);

link_internal void
DoColorPicker(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch, u32 ParentHash);

link_internal void
DoColorPickerToggle(renderer_2d *Ui, window_layout *Window, v3 *HSVDest, b32 ShowColorSwatch, u32 ParentHash);

link_internal sort_key_buffer
GetEditsSortedByOrdianl(world_edit_block_array *Edits, memory_arena *TempMem);

link_internal rtt_framebuffer
ApplyBrush(world_edit_render_context *WorldEditRC, rect3cp EditBounds, v3 Axis, world_edit_brush *EditBrush, world_edit_blend_mode BlendMode, world_chunk *Chunk, rtt_framebuffer *Read, rtt_framebuffer *Write, rtt_framebuffer *Accum, b32, b32);

