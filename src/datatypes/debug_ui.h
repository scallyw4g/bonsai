
#define DEBUG_MAX_UI_WINDOW_SLICES 1024.0f
#define DISABLE_CLIPPING V2(FLT_MAX)


debug_global rect2 NullClipRect = {};

enum ui_render_command_type
{
  RenderCommand_Noop,

  RenderCommand_WindowInteractions,

  RenderCommand_TableStart,
  RenderCommand_TableEnd,

  RenderCommand_ButtonStart,
  RenderCommand_ButtonEnd,

  RenderCommand_Column,
  RenderCommand_NewRow,

  RenderCommand_Count
};

struct ui_render_command_window_interaction
{
  window_layout* Window;
};

struct ui_render_command_column
{
  counted_string String;
};

struct ui_render_command_button_start
{
};

struct ui_render_command_button_end
{
};

struct ui_render_command_table
{
  table_handle Handle;
};

struct ui_render_command
{
  ui_render_command_type Type;
  union
  {
    ui_render_command_window_interaction WindowInteraction;
    ui_render_command_table Table;
    ui_render_command_column Column;
  };
};

#define MAX_UI_RENDER_COMMAND_COUNT (256*1024)

struct ui_render_command_buffer
{
  u32 CommandCount;
  ui_render_command Commands[MAX_UI_RENDER_COMMAND_COUNT];
};


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
