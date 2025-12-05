// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:2575:0

// def (anonymous)
// external/bonsai_stdlib/src/ui/ui.cpp:2575:0

case type_ui_render_command_window_start:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_window_start.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_window_start.Layout));
} break;




case type_ui_render_command_table_start:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_table_start.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_table_start.Layout));
} break;


case type_ui_render_command_column_start:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_column_start.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_column_start.Layout));
} break;


case type_ui_render_command_text:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_text.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_text.Layout));
} break;


case type_ui_render_command_textured_quad:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_textured_quad.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_textured_quad.Layout));
} break;

case type_ui_render_command_untextured_quad:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_untextured_quad.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_untextured_quad.Layout));
} break;

case type_ui_render_command_untextured_quad_at:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_untextured_quad_at.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_untextured_quad_at.Layout));
} break;





case type_ui_render_command_layout_start:
{
  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->ui_render_command_layout_start.Layout));
  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->ui_render_command_layout_start.Layout));
} break;





