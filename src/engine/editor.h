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
          { CSz("enum_v.name.strip_all_prefix"), UiId(Window, Cast(void*, Element), Cast(void*, "enum_t.name enum_v.name")), enum_v.name },
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
poof(
  func toggle_button_group_for_enum(enum_t)
  {
    enum_t.has_tag(bitfield)?
    {
      generic_button_group_for_enum(enum_t, {Toggle}, {|ToggleButtonGroupFlags_MultiSelectButtons})
    }
    {
      poof_error { Enum without @bitfield tag (enum_t.name) cannot create a multi-select button group. }
    }
  }
)

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

      generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_RadioButtons})
    }
  }
)

// TODO(Jesse) This is now the same as the radio button path, remove it
poof(
  func radio_button_group_for_bitfield_enum(enum_t)
  {
    generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_RadioButtons})
  }
)




poof(
  func do_editor_ui_for_vector_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
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
                    DoEditorUi(Ui, Window, &Value->(E.name)[e_index], {}, Params, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
                  }
                PushTableEnd(Ui);
                /* PushNewRow(Ui); */
              EndColumn(Ui, Start);
            }
          /* PushTableEnd(Ui); */
        })
        PushNewRow(Ui);
      }
    }
  }
)

poof(
  func do_editor_ui_for_primitive_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        Params = Params ? Params : &DefaultUiRenderParams_Blank;

        if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Column); }

        if (Value)
        {
          u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
            PushTableStart(Ui);
              if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value), &DefaultUiRenderParams_Button)) { *Value = *Value - 1; }
                  PushColumn(Ui, CS(*Value), &DefaultUiRenderParams_Generic);
              if (Button(Ui, CSz("+"), UiId(Window, "increment", Value), &DefaultUiRenderParams_Button)) { *Value = *Value + 1; }
            PushTableEnd(Ui);
          EndColumn(Ui, Start);
        }
        else
        {
          PushColumn(Ui, CSz("(null)"), Params);
          PushNewRow(Ui);
        }

      }

      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, volatile type.name *Value, cs Name, ui_render_params *Params)
      {
        DoEditorUi(Ui, Window, ((type.name)*) Value, Name, Params);
      }

    }
  }
)

poof(
  func do_editor_ui_for_compound_type(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
    {
      if (Element)
      {
        // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
        // support not drawing the toggl-y thing if we just want to dump the members.
        b32 DrawChildren = True;
        b32 DidToggle = False;
        if (Name.Count)
        {
          if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle type.name", Element), &DefaultUiRenderParams_Generic))
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
          PushTableStart(Ui);
          if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            type.map(member)
            {
              member.has_tag(ui_skip)?
              {
              }
              {
                member.is_array?
                {
                  if (ToggleButton(Ui, CSz("v member.name[member.array]"), CSz("> member.name[member.array]"), UiId(Window, "toggle type.name member.type member.name", Element->(member.name)), Params ))
                  {
                    OPEN_INDENT_FOR_TOGGLEABLE_REGION();
                      PushNewRow(Ui);
                      RangeIterator(ArrayIndex, member.array)
                      {
                        DoEditorUi(Ui, Window, Element->(member.name)+ArrayIndex, FSz("member.name[%d]", ArrayIndex), Params);
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
                      DoEditorUi(Ui,
                                 Window,
                                 Cast(b8*, member.is_pointer?{}{&}Element->(member.name)),
                                 CSz("member.name"),
                                 &DefaultUiRenderParams_Checkbox
                                 member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                    }
                    {
                      member.is_union?
                      {
                      }
                      {
                        member.is_function?
                        {
                        }
                        {
                          DoEditorUi(Ui,
                                     Window,
                                     // Cast to remove const/volatile keywords if they're there
                                     Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)),
                                     CSz("member.name"),
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
          if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
          PushTableEnd(Ui);
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
    link_internal void
    DoEditorUi(renderer_2d *Ui, window_layout *Window, enum_t.name *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
    {
      if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

      cs ElementName = ToStringPrefixless(*Element);
      ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
      if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
      {
        PushNewRow(Ui);
        enum_t.map(value)
        {
          if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
          if (Button(Ui, CSz("value.name.strip_all_prefix"), UiId(Window, "enum value.name", Element), Params))
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
)

poof(
  func do_editor_ui_for_container(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Container, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      if (Container)
      {
        if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, Name.Start, Container), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
        {
          PushNewRow(Ui);
          IterateOver(Container, Element, ElementIndex)
          {
            DoEditorUi(Ui, Window, Element, CS(ElementIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

link_internal void
DebugSlider(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, r32 Min, r32 Max, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Generic);
    PushTableStart(Ui);
      if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Blank); }

      auto Range = Max-Min;
      r32 PercFilled = ((*Value)-Min)/Range;

      r32 Width = 100.f;

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
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, r32 *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
{
  if (Name.Count) { PushColumn(Ui, Name, &DefaultUiRenderParams_Blank); }

  u32 Start = StartColumn(Ui, &DefaultUiRenderParams_Blank);
    PushTableStart(Ui);
      if (Value)
      {
        if (Button(Ui, CSz("-"), UiId(Window, "decrement", Value))) { *Value = *Value - 1.f; }
          DebugSlider(Ui, Window, Value, {}, MinValue, MaxValue);
        if (Button(Ui, CSz("+"), UiId(Window, "increment", Value))) { *Value = *Value + 1.f; }
      }
      else
      {
        PushColumn(Ui, CSz("(null)"));
      }
    PushTableEnd(Ui);
  EndColumn(Ui, Start);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, b8 *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Checkbox)
{
  UNPACK_UI_RENDER_PARAMS(Params);

  interactable_handle ButtonHandle = PushButtonStart(Ui, UiId(Window, "toggle", Value), Style);

    PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Generic);

    if (Value)
    {
      if (*Value)
      {
        PushUntexturedQuad(Ui, V2(2.f, 2.f), V2(Params->Style->Font.Size.y)-4.f, zDepth_Border, &Global_DefaultCheckboxForeground, DefaultCheckboxPadding, UiElementLayoutFlag_NoAdvance );
      }
    }
    else
    {
      PushColumn(Ui, CSz("(null)"), Params);
    }

    PushUntexturedQuad(Ui, {}, V2(Params->Style->Font.Size.y), zDepth_Text, &Global_DefaultCheckboxBackground, DefaultCheckboxPadding, UiElementLayoutFlag_Default );



  PushButtonEnd(Ui);

  if (Clicked(Ui, &ButtonHandle))
   { *Value = !(*Value); }
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cs *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
  Value ?
    PushColumn(Ui, *Value, EDITOR_UI_FUNCTION_INSTANCE_NAMES) :
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
}

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, void *Value, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Column)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), Params); }
  Value ?
    PushColumn(Ui, FSz("0x%x",umm(Value)), &DefaultUiRenderParams_Column) :
    PushColumn(Ui, CSz("(null)"), &DefaultUiRenderParams_Column);
  PushNewRow(Ui);
}


poof(do_editor_ui_for_vector_type({v4i v4 v3i v3 v2i v2 Quaternion m4}));
#include <generated/do_editor_ui_for_vector_type_688873645.h>


link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, cp *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  DoEditorUi(Ui, Window, &Value->WorldP, CSz("WorldP"));
  DoEditorUi(Ui, Window, &Value->Offset, CSz("Offset"));
}


poof(do_editor_ui_for_compound_type(rect3))
#include <generated/do_editor_ui_for_compound_type_rect3.h>

poof(do_editor_ui_for_compound_type(rect3i))
#include <generated/do_editor_ui_for_compound_type_rect3i.h>

poof(do_editor_ui_for_compound_type(rect3cp))
#include <generated/do_editor_ui_for_compound_type_rect3cp.h>

poof(block_array_h(asset_thumbnail, {8}, {}))
#include <generated/block_array_h_asset_thumbnail_688856411.h>

struct selection_modification_state
{
  face_index ClickedFace;
  v3 ClickedP[2];
};

enum level_editor_flags
{
  LevelEditorFlags_Noop                              = (1 << 0),
  /* LevelEditorFlags_RecomputeStandingSpotsOnLevelLoad = (1 << 1), */
};

poof(radio_button_group_for_bitfield_enum(level_editor_flags));
#include <generated/radio_button_group_for_bitfield_enum_level_editor_flags.h>

enum ui_noise_type
{
  NoiseType_Perlin,
  NoiseType_Voronoi,
  NoiseType_White,
};

poof(string_and_value_tables(ui_noise_type))
#include <generated/string_and_value_tables_ui_noise_type.h>
poof(radio_button_group_for_bitfield_enum(ui_noise_type));
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
  WorldEdit_Tool_Brush,         // world_edit_brush_type
  WorldEdit_Tool_Eyedropper,
  WorldEdit_Tool_BlitEntity,
  /* WorldEdit_Tool_StandingSpots, // Recomputes standing spots for an area */
};

enum world_edit_brush_type
{
  WorldEdit_BrushType_Disabled,   // poof(@ui_skip)
  WorldEdit_BrushType_Selection,
  WorldEdit_BrushType_Single,
  WorldEdit_BrushType_Asset,
  WorldEdit_BrushType_Entity,
  /* WorldEdit_BrushType_Shape, */
  /* WorldEdit_BrushType_Noise, */
  WorldEdit_BrushType_Layered,
};

// TODO(Jesse): Rename to .. something something behavior ?
enum world_edit_mode
{
  WorldEdit_Mode_Attach,
  WorldEdit_Mode_Remove,
  WorldEdit_Mode_Paint,
  WorldEdit_Mode_Disabled, // Useful for turning the layer off
};

// TODO(Jesse): Rename to reflect that it's the iteration pattern ..?
enum world_edit_mode_modifier
{
  WorldEdit_Modifier_Default  =     0,
  WorldEdit_Modifier_Flood    = (1<<0),
  WorldEdit_Modifier_Surface  = (1<<1),
};

struct world_edit_params
{
  world_edit_mode Mode;
  world_edit_mode_modifier Modifier;

  s32 Iterations = 1; // NOTE(Jesse): Set this to do the filter multiple times.
};









// TODO(Jesse): Probably do perlin/voronoi in terms of this?
struct generic_noise_params
{
  r32 Threshold = 3.0f;
  v3  Period    = {{8.f, 8.f, 8.f}}; poof(@ui_range(0, 100))
  r32 Amplitude = 8.f;               poof(@ui_range(0, 100))
  u16 Color     = 1; // White
};

// TODO(Jesse): Get rid of zMin
#define UNPACK_NOISE_PARAMS(P) \
  v3i WorldChunkDim = GetWorldChunkDim(); \
  v3i Dim = Chunk->Dim; \
  r32 Thresh  = Cast(generic_noise_params*, (P))->Threshold; \
  s64 zMin    = s64(Cast(generic_noise_params*, (P))->Threshold); \
  v3  Period     = Cast(generic_noise_params*, (P))->Period; \
  s32 Amplitude  = s32(Cast(generic_noise_params*, (P))->Amplitude); \
  u16 Color      = Cast(generic_noise_params*, (P))->Color; \
  v3i SrcToDest  = {-1*Global_ChunkApronMinDim}; \
  u16 ColorIndex = Cast(generic_noise_params*, (P))->Color


struct white_noise_params
{
  r32 Threshold = 0.5f;
};

struct perlin_noise_params
{
  r32 Threshold = 3.0f;
  v3  Period    = {{8.f, 8.f, 8.f}}; poof(@ui_range(0, 100))
  r32 Amplitude = 8.f;               poof(@ui_range(0, 100))
};

poof(are_equal(perlin_noise_params))
#include <generated/are_equal_perlin_noise_params.h>

struct voronoi_noise_params
{
  r32 Threshold = 1.5f;
  v3  Period    = {{10.f, 10.f, 10.f}}; poof(@ui_range(0, 100))
  r32 Amplitude = 8.f;                  poof(@ui_range(0, 100))

  r32 Squareness;
  r32 MaskChance;
};

poof(do_editor_ui_for_radio_enum(world_edit_mode_modifier))
#include <generated/do_editor_ui_for_radio_enum_world_edit_mode_modifier.h>

poof(string_and_value_tables(world_edit_mode_modifier))
#include <generated/string_and_value_tables_world_edit_mode_modifier.h>



poof(toggle_button_group_for_enum(engine_debug_view_mode))
#include <generated/toggle_button_group_for_enum_engine_debug_view_mode.h>



poof(do_editor_ui_for_radio_enum(asset_window_view_mode))
#include <generated/do_editor_ui_for_radio_enum_asset_window_view_mode.h>

poof(string_and_value_tables(world_edit_mode))
#include <generated/string_and_value_tables_world_edit_mode.h>
/* poof(radio_button_group_for_bitfield_enum(world_edit_mode)); */
/* #include <generated/radio_button_group_for_bitfield_enum_world_edit_mode.h> */

poof(do_editor_ui_for_radio_enum(world_edit_mode))
#include <generated/do_editor_ui_for_radio_enum_world_edit_mode.h>

poof(do_editor_ui_for_radio_enum(world_edit_tool))
#include <generated/do_editor_ui_for_radio_enum_world_edit_tool.h>

poof(do_editor_ui_for_radio_enum(world_edit_brush_type))
#include <generated/do_editor_ui_for_radio_enum_world_edit_brush_type.h>

poof(do_editor_ui_for_compound_type(world_edit_params))
#include <generated/do_editor_ui_for_compound_type_world_edit_params.h>







enum world_update_op_shape_type
{
  type_world_update_op_shape_params_noop,

  type_world_update_op_shape_params_sphere,
  type_world_update_op_shape_params_rect,
  type_world_update_op_shape_params_asset,
  type_world_update_op_shape_params_chunk_data,

  type_world_update_op_shape_params_count,
};

poof(string_and_value_tables(world_update_op_shape_type))
#include <generated/string_and_value_tables_world_update_op_shape_type.h>


struct world_update_op_shape_params_rect
{
  // Sim-space positions
  rect3 Region;
};

struct asset;
struct world_update_op_shape_params_asset
{
  /* asset *Asset; */
  /* model *Model; */

  asset_id AssetId;
  u64      ModelIndex;
  cp Origin;
};

struct world_update_op_shape_params_chunk_data
{
  chunk_data Data;
  v3 SimSpaceOrigin;
  /* cp Origin; */
};

struct world_update_op_shape_params_sphere
{
  cp  Location;
  f32 Radius = 10.f;
};

struct world_edit_shape
{
  world_update_op_shape_type Type;

  union {
    world_update_op_shape_params_sphere     world_update_op_shape_params_sphere;
    world_update_op_shape_params_rect       world_update_op_shape_params_rect;
    world_update_op_shape_params_asset      world_update_op_shape_params_asset;
    world_update_op_shape_params_chunk_data world_update_op_shape_params_chunk_data;
  };
};

struct world_edit_brush
{
  world_edit_shape         Shape;

  world_edit_mode          Mode;
  world_edit_mode_modifier Modifier;

  // NOTE(Jesse): If Modifier is Flood, this is set to where the flood should start
  v3 SimFloodOrigin;
};

poof(do_editor_ui_for_compound_type(world_edit_brush))
#include <generated/do_editor_ui_for_compound_type_world_edit_brush.h>









enum shape_type
{
  // NOTE(Jesse): Having this none value is kinda janky; I'd prefer to not for
  // this enum, but since we have to overlap with type_world_update_op_shape_params
  // we kinda have to have it (so that the none value draws if we accidentally set it to 0.
  //
  // type_world_update_op_shape_params doesn't strictly require a 0 value,
  // but it's really nice to have the assertion in DoWorldUpdate
  //
  // Once this code matures a bit we can probably take this _None value
  // out.  It's strictly so we get a visual trigger in the UI.
  //
  ShapeType_None   = type_world_update_op_shape_params_noop,
  ShapeType_Sphere = type_world_update_op_shape_params_sphere,
  ShapeType_Rect   = type_world_update_op_shape_params_rect,
};
poof(string_and_value_tables(shape_type))
#include <generated/string_and_value_tables_shape_type.h>

struct shape_layer
{
  shape_type Type = ShapeType_Sphere;

  // NOTE(Jesse): Intentionally not a d-union such that you can toggle between
  // them and your parameter selections stay intact.
  world_update_op_shape_params_sphere Sphere;
  world_update_op_shape_params_rect   Rect;
};

// NOTE(Jesse): This is intentionally not a d_union such that you can flip
// between different noise selections and your parameters stay intact.
struct noise_layer poof(@version(1))
{
  ui_noise_type Type;

  white_noise_params   White;
  perlin_noise_params  Perlin;
  voronoi_noise_params Voronoi;
};

struct noise_layer_0
{
  ui_noise_type Type;

  perlin_noise_params  Perlin;
  voronoi_noise_params Voronoi;
};


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
};

poof(do_editor_ui_for_radio_enum(brush_layer_type))
#include <generated/do_editor_ui_for_radio_enum_brush_layer_type.h>

// TODO(Jesse): Rename to `brush` ..?
struct brush_settings poof(@version(2))
{
  brush_layer_type Type;

  noise_layer Noise;
  shape_layer Shape;

  //
  // Common across brush types
  //
  world_edit_mode          Mode;
  world_edit_mode_modifier Modifier;
  s32 Iterations = 1; // NOTE(Jesse): How many times to do the filter.

  // NOTE(Jesse): This is the relative offset from the base selection.
  // Used to inflate or contract the area affected by the brush.
  //
  // TODO(Jesse): Rename to dilation
  rect3i Offset;

  v3i NoiseBasisOffset;

  u16 Color = 1; // Default to white
  b8 Invert;
};
poof(are_equal(brush_settings))
#include <generated/are_equal_brush_settings.h>


struct brush_settings_1
{
  brush_layer_type Type;

  noise_layer Noise;
  shape_layer Shape;

  //
  // Common across brush types
  //
  world_edit_mode          Mode;
  world_edit_mode_modifier Modifier;
  s32 Iterations = 1; // NOTE(Jesse): How many times to do the filter.

  // NOTE(Jesse): This is the relative offset from the base selection.
  // Used to inflate or contract the area affected by the brush.
  rect3i Offset;

  v3i NoiseBasisOffset;

  u16 Color = 1; // Default to white
};

struct brush_settings_0
{
  brush_layer_type Type;

  noise_layer Noise;
  shape_layer Shape;

  //
  // Common across brush types
  //
  world_edit_mode          Mode;
  world_edit_mode_modifier Modifier;
  s32 Iterations = 1; // NOTE(Jesse): How many times to do the filter.

  // NOTE(Jesse): This is the relative offset from the base selection.
  // Used to inflate or contract the area affected by the brush.
  rect3i Offset;

  u16 Color = 1; // Default to white
};

link_internal void
Marshal(brush_settings_1 *Stored, brush_settings *Live)
{
  poof(default_marshal(brush_settings_1))
#include <generated/default_marshal_brush_settings_1.h>
}

link_internal void
Marshal(brush_settings_0 *Stored, brush_settings *Live)
{
  poof(default_marshal(brush_settings_0))
#include <generated/default_marshal_brush_settings_0.h>
}


struct brush_layer
{
  brush_settings Settings;
  brush_settings PrevSettings; poof(@no_serialize @ui_skip) // Change detection

  chunk_thumbnail Preview; poof(@no_serialize)
};



#define NameBuf_Len (256)
// TODO(Jesse): Make this dynamic .. probably ..
#define MAX_BRUSH_LAYERS 16
struct layered_brush_editor poof(@version(2))
{
  // NOTE(Jesse): This is so we can just copy the name of the brush in here and
  // not fuck around with allocating a single string when we load these in.
  char NameBuf[NameBuf_Len+1]; poof(@no_serialize @ui_text_box)

  s32 LayerCount;
  brush_layer Layers[MAX_BRUSH_LAYERS]; poof(@array_length(LayerCount))

  b8 SeedBrushWithSelection;
  b8 BrushFollowsCursor;

  chunk_thumbnail Preview; poof(@no_serialize)
};

struct layered_brush_editor_1
{
  char NameBuf[NameBuf_Len];

  s32 LayerCount = 1;
  brush_layer Layers[MAX_BRUSH_LAYERS]; poof(@array_length(LayerCount))

  b8 SeedBrushWithSelection;
  b8 BrushFollowsCursor;

  chunk_thumbnail Preview; poof(@no_serialize)
};

struct layered_brush_editor_0
{
  s32 LayerCount = 1;
  brush_layer Layers[MAX_BRUSH_LAYERS]; poof(@array_length(LayerCount))

  chunk_thumbnail Preview; poof(@no_serialize)
  b8 SeedBrushWithSelection; poof(@no_serialize)
};

link_internal void
Marshal(layered_brush_editor_1 *Stored, layered_brush_editor *Live)
{
  poof(default_marshal(layered_brush_editor_1))
#include <generated/default_marshal_layered_brush_editor_1.h>
}

link_internal void
Marshal(layered_brush_editor_0 *Stored, layered_brush_editor *Live)
{
  poof(default_marshal(layered_brush_editor_0))
#include <generated/default_marshal_layered_brush_editor_0.h>
}









struct level_editor
{
  memory_arena *Memory;

  world_edit_tool       Tool;
  world_edit_tool       PreviousTool; // So we can 'pop' back to the last tool on select/eyedropper

  world_edit_brush_type BrushType;
  world_edit_params     Params;

  b8 SelectionFollowsCursor;

  /* brush_layer Noise; */
  /* brush_layer Shape; */
  layered_brush_editor LayeredBrushEditor;

  b32 RootChunkNeedsNewMesh;

  cp  MostRecentSelectionRegionMin;
  cp  NextSelectionRegionMin;
  cp  EditorPreviewRegionMin;

  u64 EngineDebugViewModeToggleBits;

  u16 SelectedColorIndex;
  u16 HoverColorIndex;

  b32 SelectionChanged;
  u32 SelectionClicks;
  cp  SelectionBase;

  rect3cp SelectionRegion;
  rect3cp PrevSelectionRegion; // Change detection
                               //
  rect3cp CopyRegion;

  // Recorded when accel-clicking on the selection to manipulate it
  selection_modification_state Selection;
  selection_modification_state Entity;

  asset_thumbnail_block_array AssetThumbnails;
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
  Editor->SelectionClicks = 0;
  Editor->SelectionBase = {};
  Editor->SelectionRegion = {};
}

link_internal void
ResetSelectionIfIncomplete(level_editor *Editor)
{
  if (SelectionIncomplete(Editor->SelectionClicks)) { ResetSelection(Editor); }
}


link_internal b32 HardResetEditor(level_editor *Editor);

link_internal v3
GetHotVoxelForEditMode(engine_resources *Engine, world_edit_mode WorldEditMode);

link_internal v3
GetHotVoxelForFlood(engine_resources *Engine, world_edit_mode WorldEditMode, world_edit_mode_modifier Modifier);

link_internal void
ApplyBrushLayer(engine_resources *Engine, brush_layer *Layer, world_chunk *DestChunk, v3i SmallestMinOffset);

link_internal v3i
GetSmallestMinOffset(layered_brush_editor *LayeredBrush, v3i *LargestLayerDim = 0);


