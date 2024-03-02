

#define EDITOR_UI_FUNCTION_PROTO_DEFAULTS  ui_style *Style = &DefaultStyle, \
                                                v4 Padding = {{5, 2, 5, 2}}, \
                         column_render_params ColumnParams = ColumnRenderParam_LeftAlign

#define EDITOR_UI_FUNCTION_PROTO_ARGUMENTS ui_style *Style, \
                                                 v4  Padding, \
                               column_render_params  ColumnParams

#define EDITOR_UI_FUNCTION_INSTANCE_NAMES Style, Padding, ColumnParams

