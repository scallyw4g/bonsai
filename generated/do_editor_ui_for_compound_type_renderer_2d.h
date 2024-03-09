// src/engine/editor.cpp:324:0

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
        CSz("TextGroup"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SpriteTextureArray,
        CSz("SpriteTextureArray"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->InteractionStackTop,
        CSz("InteractionStackTop"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->MouseP,
        CSz("MouseP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->MouseDP,
        CSz("MouseDP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->ScreenDim,
        CSz("ScreenDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Input,
        CSz("Input"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ToggleTable,
        CSz("ToggleTable"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v MinimizedWindowBuffer[64]"), CSz("> MinimizedWindowBuffer[64]"), UiId(Window, "toggle renderer_2d window_layout MinimizedWindowBuffer", Element->MinimizedWindowBuffer), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 64)
        {
          DoEditorUi(Ui, Window, Element->MinimizedWindowBuffer+ArrayIndex, FSz("MinimizedWindowBuffer[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
        Element->HighestWindow,
        CSz("HighestWindow"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Hover,
        CSz("Hover"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Clicked,
        CSz("Clicked"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Pressed,
        CSz("Pressed"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->RequestedForceCapture),
        CSz("RequestedForceCapture"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Geo,
        CSz("Geo"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->TexturedQuadShader,
        CSz("TexturedQuadShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->CommandBuffer,
        CSz("CommandBuffer"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RenderCommandArena,
        CSz("RenderCommandArena"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->UiToggleArena,
        CSz("UiToggleArena"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v DebugColors[128]"), CSz("> DebugColors[128]"), UiId(Window, "toggle renderer_2d v3 DebugColors", Element->DebugColors), Params ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 128)
        {
          DoEditorUi(Ui, Window, Element->DebugColors+ArrayIndex, FSz("DebugColors[%d]", ArrayIndex), Params);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->SolidGeoCountLastFrame,
        CSz("SolidGeoCountLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TextGeoCountLastFrame,
        CSz("TextGeoCountLastFrame"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->ColorPalette,
        CSz("ColorPalette"),
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

