// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x21E7B9F7);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_render_context", Element, ThisHash), Params))
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
      if (Name.Count) { PushTableStart(Ui); }

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            

      { 
        
        
        
        cs MemberName = CSz("Program");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shader*, &Element->Program),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Uniforms");
                                

        if (ToggleButton(Ui,
            CSz("v Uniforms[4]"),
            CSz("> Uniforms[4]"),
            UiId(Window, "toggle world_edit_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 4)
          {
                        DoEditorUi(Ui,
              Window,
              Element->Uniforms+ArrayIndex,
              FSz("Uniforms[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("PingPongFBOs");
                                

        if (ToggleButton(Ui,
            CSz("v PingPongFBOs[3]"),
            CSz("> PingPongFBOs[3]"),
            UiId(Window, "toggle world_edit_render_context framebuffer PingPongFBOs", Element->PingPongFBOs, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 3)
          {
                        DoEditorUi(Ui,
              Window,
              Element->PingPongFBOs+ArrayIndex,
              FSz("PingPongFBOs[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("PingPongTextures");
                                

        if (ToggleButton(Ui,
            CSz("v PingPongTextures[3]"),
            CSz("> PingPongTextures[3]"),
            UiId(Window, "toggle world_edit_render_context texture PingPongTextures", Element->PingPongTextures, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
          RangeIterator(ArrayIndex, 3)
          {
                        DoEditorUi(Ui,
              Window,
              Element->PingPongTextures+ArrayIndex,
              FSz("PingPongTextures[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkDim");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->ChunkDim),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("WorldspaceChunkBasis");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->WorldspaceChunkBasis),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ChunkResolution");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, Element->ChunkResolution),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Type");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->Type),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }

      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      if (Name.Count) { PushTableEnd(Ui); }
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

