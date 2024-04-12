// src/engine/editor.cpp:416:0

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
        Cast(b8*,&Element->Initialized),
        CSz("Initialized"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->RenderGate),
        CSz("RenderGate"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(render_settings*,&Element->Settings),
        CSz("Settings"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(r32*,&Element->FogPower),
        CSz("FogPower"),
        Params
        );






      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(camera*,&Element->GameCamera),
        CSz("GameCamera"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(camera*,&Element->DebugCamera),
        CSz("DebugCamera"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(r32*,&Element->Exposure),
        CSz("Exposure"),
        Params
        );






      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_chunk_ptr_paged_list*,&Element->MainDrawList),
        CSz("MainDrawList"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_chunk_ptr_paged_list*,&Element->ShadowMapDrawList),
        CSz("ShadowMapDrawList"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(g_buffer_render_group*, Element->gBuffer),
        CSz("gBuffer"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(ao_render_group*, Element->AoGroup),
        CSz("AoGroup"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(shadow_render_group*, Element->SG),
        CSz("SG"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(texture*,&Element->ColorPaletteTexture),
        CSz("ColorPaletteTexture"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(transparency_render_group*,&Element->Transparency),
        CSz("Transparency"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(lighting_render_group*,&Element->Lighting),
        CSz("Lighting"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(gaussian_render_group*,&Element->Gaussian),
        CSz("Gaussian"),
        Params
        );






      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(composite_render_group*,&Element->CompositeGroup),
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
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->GpuBufferWriteIndex),
        CSz("GpuBufferWriteIndex"),
        Params
        );






      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(memory_arena*, Element->Memory),
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

