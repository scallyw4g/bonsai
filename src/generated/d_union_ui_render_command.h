enum ui_render_command_type
{
  type_ui_render_command_noop,
  type_ui_render_command_window_start,
  type_ui_render_command_window_end,
  type_ui_render_command_button_start,
  type_ui_render_command_button_end,
  type_ui_render_command_table_start,
  type_ui_render_command_column_start,
  type_ui_render_command_column_end,
  type_ui_render_command_text,
  type_ui_render_command_text_at,
  type_ui_render_command_textured_quad,
  type_ui_render_command_untextured_quad,
  type_ui_render_command_untextured_quad_at,
  type_ui_render_command_border,
  type_ui_render_command_new_row,
  type_ui_render_command_table_end,
};

struct ui_render_command
{
  enum ui_render_command_type Type;

  union
  {
    struct ui_render_command_window_start ui_render_command_window_start;
    struct ui_render_command_window_end ui_render_command_window_end;
    struct ui_render_command_button_start ui_render_command_button_start;
    struct ui_render_command_button_end ui_render_command_button_end;
    struct ui_render_command_table_start ui_render_command_table_start;
    struct ui_render_command_column_start ui_render_command_column_start;
    struct ui_render_command_text ui_render_command_text;
    struct ui_render_command_text_at ui_render_command_text_at;
    struct ui_render_command_textured_quad ui_render_command_textured_quad;
    struct ui_render_command_untextured_quad ui_render_command_untextured_quad;
    struct ui_render_command_untextured_quad_at ui_render_command_untextured_quad_at;
    struct ui_render_command_border ui_render_command_border;
  };
};


