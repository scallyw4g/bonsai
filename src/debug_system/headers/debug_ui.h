
#define DEBUG_MAX_UI_WINDOW_SLICES 1024.0f
#define DISABLE_CLIPPING V2(f32_MAX)

#define DEBUG_UI_OUTLINE_VALUES  0
#define DEBUG_UI_OUTLINE_BUTTONS 0
#define DEBUG_UI_OUTLINE_TABLES  0

/******************************               ********************************/
/******************************   Rendering   ********************************/
/******************************               ********************************/




struct window_layout
{
  counted_string Title;

  v2 Basis;
  v2 MaxClip;

  u64 InteractionStackIndex;

  r32 zBackground;
  r32 zText;
  r32 zBorder;
  r32 zTitleBar;

  window_layout* NextHotWindow;
};



/***************************                **********************************/
/*************************** Command Buffer **********************************/
/***************************                **********************************/



enum debug_texture_array_slice
{
  DebugTextureArraySlice_Font,
  DebugTextureArraySlice_Viewport,

  DebugTextureArraySlice_Count,
};

enum column_render_params
{
  ColumnRenderParam_LeftAlign  = 0,
  ColumnRenderParam_RightAlign = (1 << 0),
};

enum quad_render_params
{
  QuadRenderParam_NoAdvance     =  0,
  QuadRenderParam_AdvanceLayout = (1 << 0),
  QuadRenderParam_AdvanceClip   = (1 << 1),

  QuadRenderParam_Default = (QuadRenderParam_AdvanceLayout|QuadRenderParam_AdvanceClip),
};

enum button_end_params
{
  ButtonEndParam_NoOp                    = 0,
  ButtonEndParam_DiscardButtonDrawBounds = (1 << 0),
};

enum relative_position
{
  Position_None,

  Position_LeftOf,
  Position_RightOf,
  Position_Above,
  Position_Below,
};

enum z_depth
{
  zDepth_Background,
  zDepth_Text,
  zDepth_TitleBar,
  zDepth_Border,
};






struct font
{
  v2 Size;
};

// TODO(Jesse, id: 77, tags: font, cleanup): Axe this!
debug_global font Global_Font = {
  .Size = V2(26, 34),
};

struct ui_element_reference
{
  u32 Index;
};

struct ui_style
{
  v3 Color;

  v3 HoverColor;
  v3 PressedColor;
  v3 ClickedColor;
  v3 ActiveColor;

  font Font;

  b32 IsActive;
};


function ui_style UiStyleFromLightestColor(v3 Color);
debug_global ui_style DefaultUiStyle = UiStyleFromLightestColor(V3(1));

struct layout
{
  v2 Basis;
  v2 At;
  rect2 DrawBounds;

  v4 Padding;

  layout* Prev;
};









struct ui_render_command_border
{
  window_layout* Window;
  rect2 Bounds;
  v3 Color;
};

struct ui_render_command_window_start
{
  layout Layout;
  window_layout* Window;
};

struct ui_render_command_window_end
{
  window_layout* Window;
};

struct ui_render_command_column_end
{
};

struct ui_render_command_column_start
{
  layout Layout;
  ui_style Style;
  r32 Width;
  r32 MaxWidth;
  column_render_params Params;
};

struct ui_render_command_text
{
  layout Layout;
  ui_style Style;
  counted_string String;
};

struct ui_render_command_text_at
{
  counted_string Text;
  v2 At;
  v2 MaxClip;
};

struct ui_render_command_untextured_quad
{
  layout Layout;
  ui_style Style;
  v2 QuadDim;
  z_depth zDepth;
  quad_render_params Params;
};

struct ui_render_command_untextured_quad_at
{
  layout Layout;
  ui_style Style;
  v2 QuadDim;
  z_depth zDepth;
};

struct ui_render_command_textured_quad
{
  layout Layout;
  debug_texture_array_slice TextureSlice;
  z_depth zDepth;
};

struct ui_render_command_button_start
{
  umm ID;
  ui_style Style;
};

struct ui_render_command_button_end
{
  button_end_params Params;
};

struct ui_render_command_table_start
{
  layout Layout;
  ui_style Style;

  relative_position Position;
  ui_element_reference RelativeTo;
};

meta(
  d_union(
  ui_render_command,
  {
    ui_render_command_window_start,
    ui_render_command_window_end,

    ui_render_command_button_start,
    ui_render_command_button_end,

    ui_render_command_table_start,

    ui_render_command_column_start,
    ui_render_command_column_end,

    ui_render_command_text,
    ui_render_command_text_at,

    ui_render_command_textured_quad,
    ui_render_command_untextured_quad,
    ui_render_command_untextured_quad_at,

    ui_render_command_border,

    ui_render_command_new_row   enum_only,
    ui_render_command_table_end enum_only,
  })
)
#include <metaprogramming/output/d_union_ui_render_command>




#define MAX_UI_RENDER_COMMAND_COUNT (1024*1024)
struct ui_render_command_buffer
{
  u32 CommandCount;
  ui_render_command Commands[MAX_UI_RENDER_COMMAND_COUNT];
};







/******************************               ********************************/
/******************************      Misc     ********************************/
/******************************               ********************************/



enum clip_status
{
  ClipStatus_NoClipping,
  ClipStatus_PartialClipping,
  ClipStatus_FullyClipped
};

struct clip_result
{
  clip_status ClipStatus;
  v2 MaxClip;

  rect2 PartialClip;
};

struct sort_key
{
  u64 Value;
  u64 Index;
};

struct window_sort_params
{
  u32 Count;
  u64 LowestInteractionStackIndex;

  sort_key* SortKeys;
};

struct find_button_start_result
{
  u32 Index;
  ui_render_command_button_start* Command;
};







// FIXME(Jesse): Pretty sure this is unnecessary if we just draw the shadow first
// @shadow_epsilon
#define DEBUG_FONT_SHADOW_EPSILON (0.0000001f)

function r32
GetZ(z_depth zDepth, window_layout* Window)
{
  // @shadow_epsilon
  r32 Result = DEBUG_FONT_SHADOW_EPSILON;

  if (Window)
  {
    switch (zDepth)
    {
      case zDepth_Background:
      {
        Result = Window->zBackground;
      } break;
      case zDepth_Text:
      {
        Result = Window->zText;
      } break;
      case zDepth_TitleBar:
      {
        Result = Window->zTitleBar;
      } break;
      case zDepth_Border:
      {
        Result = Window->zBorder;
      } break;

      InvalidDefaultCase;
    }
  }

  return Result;
}

function ui_style
StandardStyling(v3 StartingColor, v3 HoverMultiplier = V3(1.3f), v3 ClickMultiplier = V3(1.2f))
{
  ui_style Result = {};
  Result.Color = StartingColor;
  Result.HoverColor = StartingColor*HoverMultiplier;
  Result.ClickedColor = StartingColor*ClickMultiplier;

  return Result;
}

function ui_style
UiStyleFromLightestColor(v3 Color)
{
  ui_style Style  = {
    .Color        = Color,
    .HoverColor   = Color*0.75f,
    .PressedColor = Color,
    .ClickedColor = Color,
    .ActiveColor  = Color,

    .Font         = Global_Font,

    .IsActive     = False,
  };

  return Style;
}


function window_layout
WindowLayout(const char* Title, v2 Basis, v2 MaxClip = V2(1800, 800))
{
  local_persist u32 NextWindowStackIndex = 0;

  window_layout Window = {};
  Window.Basis = Basis;
  Window.MaxClip = MaxClip;
  Window.Title = CS(Title);
  Window.InteractionStackIndex = NextWindowStackIndex++;

  return Window;
}

v2 GetAbsoluteMaxClip(window_layout* Window);

function rect2
GetWindowBounds(window_layout *Window)
{
  v2 TopLeft = Window->Basis;
  v2 BottomRight = GetAbsoluteMaxClip(Window);
  rect2 Result = RectMinMax(TopLeft, BottomRight);
  return Result;
}

