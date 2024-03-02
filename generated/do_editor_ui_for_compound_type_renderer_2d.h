// src/engine/editor.cpp:418:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, renderer_2d *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle renderer_2d", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
        Element->TextGroup,
        CSz("render_buffers_2d TextGroup"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SpriteTextureArray,
        CSz("texture SpriteTextureArray"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->InteractionStackTop,
        CSz("u64 InteractionStackTop"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->MouseP,
        CSz("v2 MouseP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->MouseDP,
        CSz("v2 MouseDP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->ScreenDim,
        CSz("v2 ScreenDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Input,
        CSz("input Input"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ToggleTable,
        CSz("ui_toggle_hashtable ToggleTable"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v window_layout MinimizedWindowBuffer[64]"), CSz("> window_layout MinimizedWindowBuffer[64]"), UiId(Window, "toggle renderer_2d window_layout MinimizedWindowBuffer", Element->MinimizedWindowBuffer), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 64)
        {
          DoEditorUi(Ui, Window, Element->MinimizedWindowBuffer+ArrayIndex, FSz("window_layout MinimizedWindowBuffer[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
        Element->HighestWindow,
        CSz("window_layout HighestWindow"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Hover,
        CSz("interactable Hover"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Clicked,
        CSz("interactable Clicked"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Pressed,
        CSz("interactable Pressed"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->RequestedForceCapture),
        CSz("b32 RequestedForceCapture"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Geo,
        CSz("untextured_2d_geometry_buffer Geo"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->TexturedQuadShader,
        CSz("shader TexturedQuadShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->CommandBuffer,
        CSz("ui_render_command_buffer CommandBuffer"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RenderCommandArena,
        CSz("memory_arena RenderCommandArena"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->UiToggleArena,
        CSz("memory_arena UiToggleArena"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v v3 DebugColors[128]"), CSz("> v3 DebugColors[128]"), UiId(Window, "toggle renderer_2d v3 DebugColors", Element->DebugColors), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 128)
        {
          DoEditorUi(Ui, Window, Element->DebugColors+ArrayIndex, FSz("v3 DebugColors[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->SolidGeoCountLastFrame,
        CSz("u32 SolidGeoCountLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TextGeoCountLastFrame,
        CSz("u32 TextGeoCountLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->ColorPalette,
        CSz("v3_cursor ColorPalette"),
        Params
        );
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

