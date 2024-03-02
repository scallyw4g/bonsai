#define EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS r32 MinValue = 0.f, r32 MaxValue = 1.f
#define EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES MinValue, MaxValue

poof(
  func generic_button_group_for_enum(enum_t, type_poof_symbol NamePrefix, type_poof_symbol extra_poof_flags)
  {
    link_internal b32
    ToggledOn(ui_toggle_button_group *ButtonGroup, enum_t.name Enum)
    {
      b32 Result = ButtonGroup->ToggleBits & (1 << Enum);
      return Result;
    }

    // NOTE(Jesse): This could be implemented by reconstructing the button ID
    // but I'm very unsure that's worth it.  Seems like just
    link_internal b32
    Clicked(ui_toggle_button_group *ButtonGroup, enum_t.name Enum)
    {
      b32 Result = False;
      NotImplemented;
      return Result;
    }

    link_internal ui_toggle_button_group
    (NamePrefix)ButtonGroup_(enum_t.name)( renderer_2d *Ui,
                                           window_layout *Window,
                                           const char *ToggleGroupIdentifier,
                                           ui_toggle_button_group_flags ExtraFlags = ToggleButtonGroupFlags_None,
                                           ui_relative_position_reference *RelativePosition = 0,
                                           UI_FUNCTION_PROTO_DEFAULTS )
    {
      cs ButtonNames[] =
      {
        enum_t.map(value)
        {
          CSz("value.name.strip_all_prefix"),
        }
      };

      u32 ButtonCount = ArrayCount(ButtonNames);

      ui_toggle_button_handle_buffer ButtonBuffer = UiToggleButtonHandleBuffer(ButtonCount, GetTranArena());
      IterateOver(&ButtonBuffer, Button, ButtonIndex)
      {
        cs Name = ButtonNames[ButtonIndex];
        *Button = UiToggle(Name, Window, ToggleGroupIdentifier, (void*)Name.Start);
      }

      ui_toggle_button_group Result = UiToggleButtonGroup(Ui, &ButtonBuffer, ui_toggle_button_group_flags(ExtraFlags(extra_poof_flags)), UI_FUNCTION_INSTANCE_NAMES);

      return Result;
    }
  }
)

poof(
  func toggle_button_group_for_enum(enum_t)
  {
    generic_button_group_for_enum(enum_t, {Toggle}, {|ToggleButtonGroupFlags_None})
  }
)

poof(
  func radio_button_group_for_enum(enum_t)
  {
    link_internal void
    RadioSelect(ui_toggle_button_group *RadioGroup, enum_t.name Selection)
    {
      NotImplemented;
    }

    link_internal void
    GetRadioEnum(ui_toggle_button_group *RadioGroup, enum_t.name *Result)
    {
      if (RadioGroup->ToggleBits)
      {
        Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1); // Radio group can 
      }

      s32 Index = s32(GetIndexOfNthSetBit(u32(RadioGroup->ToggleBits), 1));
      *Result = enum_t.name(Max(0, Index));
    }

    generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_RadioButtons})
  }
)

poof(
  func radio_button_group_for_bitfield_enum(enum_t)
  {
    link_internal void
    RadioSelect(ui_toggle_button_group *RadioGroup, enum_t.name Selection)
    {
      Assert(CountBitsSet_Kernighan(u32(Selection)) == 1);
      u32 Index = GetIndexOfNthSetBit(u32(Selection), 1);
      ui_toggle_button_handle *ToggleHandle = RadioGroup->Buttons.Start + Index;
      SetRadioButton(RadioGroup, ToggleHandle, True);
      /* Ensure( ToggleRadioButton(RadioGroup, ToggleHandle) ); */
    }

    link_internal void
    GetRadioEnum(ui_toggle_button_group *RadioGroup, enum_t.name *Result)
    {
      if (RadioGroup->ToggleBits)
      {
        Assert(CountBitsSet_Kernighan(RadioGroup->ToggleBits) == 1);
        // NOTE(Jesse): This is better; it asserts that we've actually got a bitfield
        Assert((((enum_t.map(value).sep(||) {RadioGroup->ToggleBits == value.name}))));
        /* Assert((((enum_t.map(value).sep(|) {value.name})) & RadioGroup->ToggleBits) != 0); */
      }

      *Result = Cast((enum_t.name), RadioGroup->ToggleBits);
    }

    generic_button_group_for_enum(enum_t, {Radio}, {|ToggleButtonGroupFlags_RadioButtons})
  }
)




poof(
  func do_editor_ui_for_vector_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        type.member(0, (E) 
        {
          /* PushTableStart(Ui); */
            if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

            if (Value)
            {
              u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                PushTableStart(Ui);
                  E.map_array(e_index)
                  {
                    DoEditorUi(Ui, Window, &Value->(E.name)[e_index], {}, EDITOR_UI_FUNCTION_INSTANCE_NAMES, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
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
      DoEditorUi(renderer_2d *Ui, window_layout *Window, type.name *Value, cs Name, ui_render_params *Params, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        Params = Params ? Params : &DefaultUiRenderParams_Column;

        if (Name) { PushColumn(Ui, Name, Params); }

        if (Value)
        {
          u32 Start = StartColumn(Ui, Params);
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
        if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle type.name", Element), &DefaultUiRenderParams_Generic))
        {
          PushNewRow(Ui);

          PushTableStart(Ui);
          PushForceUpdateBasis(Ui, V2(20.f, 0.f));
            type.map(member)
            {
              member.has_tag(ui_skip)?
              {
              }
              {
                member.is_array?
                {
                  if (ToggleButton(Ui, CSz("v member.type member.name[member.array]"), CSz("> member.type member.name[member.array]"), UiId(Window, "toggle type.name member.type member.name", Element->(member.name)), Params ))
                  {
                    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
                      PushNewRow(Ui);
                      RangeIterator(ArrayIndex, member.array)
                      {
                        DoEditorUi(Ui, Window, Element->(member.name)+ArrayIndex, FSz("member.type member.name[%d]", ArrayIndex), Params);
                        member.is_primitive?  { PushNewRow(Ui); }
                      }
                    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
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
                                 CSz("member.type member.name"),
                                 Params
                                 member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                    }
                    {
                      DoEditorUi(Ui,
                                 Window,
                                 member.is_pointer?{}{&}Element->(member.name),
                                 CSz("member.type member.name"),
                                 Params
                                 member.has_tag(ui_value_range)?{, member.tag_value(ui_value_range) });
                    }
                  }
                }
              }

              member.is_primitive?
              {
                PushNewRow(Ui);
              }
            }
          PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
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
    DoEditorUi(renderer_2d *Ui, window_layout *Window, enum_t.name *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

      cs ElementName = ToString(*Element);
      if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
      {
        PushNewRow(Ui);
        enum_t.map(value)
        {
          PushColumn(Ui, CSz("")); // Skip the first Name column
          if (Button(Ui, CSz("value.name"), UiId(Window, "enum value.name", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
          {
            enum_t.has_tag(bitfield)?
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
            {
              *Element = value.name;
            }
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
                cs Name,

                // TODO(Jesse): Should these be systemic in the DoEditorUi API?
                ui_relative_position_reference *RelativePosition = 0,

                EDITOR_UI_FUNCTION_PROTO_DEFAULTS )
    {
        if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); PushNewRow(Ui); }
        ui_toggle_button_group RadioGroup = RadioButtonGroup_(enum_t.name)(Ui, Window, "enum_t.name radio group");
        GetRadioEnum(&RadioGroup, Element);
        return RadioGroup;
    }
  }
)

/* link_internal void */
/* DoEditorUi(renderer_2d *Ui, aabb *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS); */

/* link_internal void */
/* DoEditorUi(renderer_2d *Ui, v3i *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS); */

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
  NoiseType_None,
  NoiseType_Perlin,
  NoiseType_Voronoi,
};

struct perlin_noise_params
{
  r32 Threshold = 0.5f;
  r32 Period    = 10.f;   poof(@ui_range(0, 100))
  r32 Amplitude = 10.f;   poof(@ui_range(0, 100))
};

struct voronoi_noise_params
{
  r32 Threshold;
  r32 Period;   poof(@ui_range(0, 100))

  r32 Squareness;
  r32 MaskChance;
};

// NOTE(Jesse): This is intentionally not a d_union such that you can flip
// between different noise selections and your parameters stay intact.
struct noise_selector
{
  ui_noise_type Type;
  perlin_noise_params  PerlinParams;
  voronoi_noise_params VoronoiParams;
};


struct level_editor
{
  memory_arena *Memory;

  noise_selector NoiseSelection;
  asset_thumbnail NoisePreviewThumbnail;

  u64 EngineDebugViewModeToggleBits;

  u16 SelectedColorIndex;
  u16 HoverColorIndex;

  u32 SelectionClicks;
  cp SelectionBase;

  rect3cp SelectionRegion;
  rect3cp CopyRegion;

  // Recorded when accel-clicking on the selection to manipulate it
  selection_modification_state Selection;
  selection_modification_state Entity;

  asset_thumbnail_block_array AssetThumbnails;
};

link_internal b32
SelectionIncomplete(u32 SelectionClicks)
{
  return SelectionClicks == 0 || SelectionClicks == 1;
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

struct maybe_v3
{
  maybe_tag Tag;
  v3 V3;
};

enum world_edit_selection_mode
{
  SelectionMode_Noop,

  SelectionMode_Resize,
  SelectionMode_TranslateLinear,
  SelectionMode_TranslatePlanar,
};

enum world_edit_mode
{
  WorldEditMode_Paint,
  WorldEditMode_Attach,
  WorldEditMode_Remove,
};

enum world_edit_tool
{
  WorldEditTool_Select,      // world_edit_selection_mode
  WorldEditTool_Single,
  WorldEditTool_Eyedropper,
  WorldEditTool_Brush,       // world_edit_brush_type
  WorldEditTool_BlitEntity,
  WorldEditTool_RecomputeStandingSpots,
};

enum world_edit_brush_type
{
  WorldEditTool_BrushType_Asset,
  WorldEditTool_BrushType_Entity,
  WorldEditTool_BrushType_Noise
};

#if 0
enum world_edit_mode
{
  WorldEditMode_Select                 = (1 << 0),

  WorldEditMode_FillSelection          = (1 << 1),
  WorldEditMode_PaintSelection         = (1 << 2),
  WorldEditMode_DeleteSelection        = (1 << 3),

  WorldEditMode_Eyedropper             = (1 << 4),

  WorldEditMode_AddSingle              = (1 << 5),
  WorldEditMode_RemoveSingle           = (1 << 6),
  WorldEditMode_PaintSingle            = (1 << 7),

  WorldEditMode_AssetBrush             = (1 << 8),
  WorldEditMode_EntityBrush            = (1 << 9),
  WorldEditMode_BlitEntity             = (1 << 10),

  WorldEditMode_NoiseBrush             = (1 << 11),

  WorldEditMode_RecomputeStandingSpots = (1 << 12),
};
#endif


poof(toggle_button_group_for_enum(engine_debug_view_mode))
#include <generated/toggle_button_group_for_enum_engine_debug_view_mode.h>


poof(string_and_value_tables(ui_noise_type))
#include <generated/string_and_value_tables_ui_noise_type.h>
poof(radio_button_group_for_bitfield_enum(ui_noise_type));
#include <generated/radio_button_group_for_bitfield_enum_ui_noise_type.h>
poof(do_editor_ui_for_enum(ui_noise_type))
#include <generated/do_editor_ui_for_enum_ui_noise_type.h>


poof(do_editor_ui_for_radio_enum(asset_window_view_mode))
#include <generated/do_editor_ui_for_radio_enum_asset_window_view_mode.h>

/* poof(string_and_value_tables(world_edit_mode)) */
/* #include <generated/string_and_value_tables_world_edit_mode.h> */
/* poof(radio_button_group_for_bitfield_enum(world_edit_mode)); */
/* #include <generated/radio_button_group_for_bitfield_enum_world_edit_mode.h> */

poof(do_editor_ui_for_radio_enum(world_edit_mode))
#include <generated/do_editor_ui_for_radio_enum_world_edit_mode.h>

poof(do_editor_ui_for_radio_enum(world_edit_tool))
#include <generated/do_editor_ui_for_radio_enum_world_edit_tool.h>

poof(do_editor_ui_for_radio_enum(world_edit_brush_type))
#include <generated/do_editor_ui_for_radio_enum_world_edit_brush_type.h>


link_internal b32 HardResetEditor(level_editor *Editor);
