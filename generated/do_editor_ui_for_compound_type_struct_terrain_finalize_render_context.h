// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, terrain_finalize_render_context *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1AC2947);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle terrain_finalize_render_context", Element, ThisHash), Params))
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
            CSz("v Uniforms[1]"),
            CSz("> Uniforms[1]"),
            UiId(Window, "toggle terrain_finalize_render_context shader_uniform Uniforms", Element->Uniforms, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = 1;

          RangeIterator(ArrayIndex, End)
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
        
        
        
        cs MemberName = CSz("DestTex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(texture*, &Element->DestTex),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FBO");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(framebuffer*, &Element->FBO),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Ignored");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->Ignored),
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

