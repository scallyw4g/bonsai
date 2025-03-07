// src/engine/editor.cpp:396:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_render_context *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_render_context", Element), Params))
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
        // Cast to remove const/volatile keywords if they're there
        Cast(shader*, &Element->Program),
        CSz("Program"),
        Params
        );







      
                  if (ToggleButton(Ui, CSz("v Uniforms[4]"), CSz("> Uniforms[4]"), UiId(Window, "toggle world_edit_render_context shader_uniform Uniforms", Element->Uniforms), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 4)
        {
                    DoEditorUi(Ui, Window, Element->Uniforms+ArrayIndex, FSz("Uniforms[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                  if (ToggleButton(Ui, CSz("v PingPongFBOs[2]"), CSz("> PingPongFBOs[2]"), UiId(Window, "toggle world_edit_render_context framebuffer PingPongFBOs", Element->PingPongFBOs), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 2)
        {
                    DoEditorUi(Ui, Window, Element->PingPongFBOs+ArrayIndex, FSz("PingPongFBOs[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                  if (ToggleButton(Ui, CSz("v PingPongTextures[2]"), CSz("> PingPongTextures[2]"), UiId(Window, "toggle world_edit_render_context texture PingPongTextures", Element->PingPongTextures), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 2)
        {
                    DoEditorUi(Ui, Window, Element->PingPongTextures+ArrayIndex, FSz("PingPongTextures[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3*, Element->ChunkDim),
        CSz("ChunkDim"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3*, Element->WorldspaceBasis),
        CSz("WorldspaceBasis"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3*, Element->ChunkResolution),
        CSz("ChunkResolution"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->Type),
        CSz("Type"),
        Params
        );







            PushNewRow(Ui);
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

