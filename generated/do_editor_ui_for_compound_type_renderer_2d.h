// src/engine/editor.cpp:315:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, renderer_2d *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle renderer_2d", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, Element->TextGroup, CSz("render_buffers_2d TextGroup"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->SpriteTextureArray, CSz("texture SpriteTextureArray"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->InteractionStackTop, CSz("u64 InteractionStackTop"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, Element->MouseP, CSz("v2 MouseP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->MouseDP, CSz("v2 MouseDP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->ScreenDim, CSz("v2 ScreenDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->Input, CSz("input Input"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->ToggleTable, CSz("ui_toggle_hashtable ToggleTable"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      if (ToggleButton(Ui, CSz("v window_layout MinimizedWindowBuffer[64]"), CSz("> window_layout MinimizedWindowBuffer[64]"), UiId(Window, "toggle renderer_2d window_layout MinimizedWindowBuffer", Element->MinimizedWindowBuffer), EDITOR_UI_FUNCTION_INSTANCE_NAMES ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 64)
        {
          DoEditorUi(Ui, Window, Element->MinimizedWindowBuffer+ArrayIndex, FSz("window_layout MinimizedWindowBuffer[%d]", ArrayIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui, Window, Element->HighestWindow, CSz("window_layout HighestWindow"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->Hover, CSz("interactable Hover"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Clicked, CSz("interactable Clicked"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Pressed, CSz("interactable Pressed"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, Cast(b8*, &Element->RequestedForceCapture), CSz("b32 RequestedForceCapture"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->Geo, CSz("untextured_2d_geometry_buffer Geo"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->TexturedQuadShader, CSz("shader TexturedQuadShader"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, Element->CommandBuffer, CSz("ui_render_command_buffer CommandBuffer"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->RenderCommandArena, CSz("memory_arena RenderCommandArena"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->UiToggleArena, CSz("memory_arena UiToggleArena"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      if (ToggleButton(Ui, CSz("v v3 DebugColors[128]"), CSz("> v3 DebugColors[128]"), UiId(Window, "toggle renderer_2d v3 DebugColors", Element->DebugColors), EDITOR_UI_FUNCTION_INSTANCE_NAMES ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 128)
        {
          DoEditorUi(Ui, Window, Element->DebugColors+ArrayIndex, FSz("v3 DebugColors[%d]", ArrayIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui, Window, &Element->SolidGeoCountLastFrame, CSz("u32 SolidGeoCountLastFrame"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->TextGeoCountLastFrame, CSz("u32 TextGeoCountLastFrame"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, Element->ColorPalette, CSz("v3_cursor ColorPalette"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

