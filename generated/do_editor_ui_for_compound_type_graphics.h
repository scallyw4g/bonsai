// src/engine/editor.cpp:404:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, graphics *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle graphics", Element), &DefaultUiRenderParams_Generic))
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
      DoEditorUi(Ui,
        Window,
&Element->Settings,
        CSz("Settings"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SunBasis,
        CSz("SunBasis"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FogPower,
        CSz("FogPower"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->FogColor,
        CSz("FogColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SkyColor,
        CSz("SkyColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GameCamera,
        CSz("GameCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DebugCamera,
        CSz("DebugCamera"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Exposure,
        CSz("Exposure"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->gBuffer,
        CSz("gBuffer"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->AoGroup,
        CSz("AoGroup"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->SG,
        CSz("SG"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Transparency,
        CSz("Transparency"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Lighting,
        CSz("Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Gaussian,
        CSz("Gaussian"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CompositeGroup,
        CSz("CompositeGroup"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v GpuBuffers[2]"), CSz("> GpuBuffers[2]"), UiId(Window, "toggle graphics gpu_mapped_element_buffer GpuBuffers", Element->GpuBuffers), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 2)
        {
          DoEditorUi(Ui, Window, Element->GpuBuffers+ArrayIndex, FSz("GpuBuffers[%d]", ArrayIndex), Params);
          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui,
        Window,
&Element->GpuBufferWriteIndex,
        CSz("GpuBufferWriteIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->Memory,
        CSz("Memory"),
        Params
        );
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

