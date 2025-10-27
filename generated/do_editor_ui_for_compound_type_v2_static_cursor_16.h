// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, v2_static_cursor_16 *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x1498EFCB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle v2_static_cursor_16", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("Start");
                                

        if (ToggleButton(Ui,
            CSz("v Start[16]"),
            CSz("> Start[16]"),
            UiId(Window, "toggle v2_static_cursor_16 v2 Start", Element->Start, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = s32(Element->At);
          Assert( End < 16 );

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->Start+ArrayIndex,
              FSz("Start[%d]", ArrayIndex),
              ThisHash,
              Params);

            
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);




        
      }
      

      { 
        
        
        
        cs MemberName = CSz("At");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->At),
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

