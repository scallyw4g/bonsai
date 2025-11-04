// src/engine/editor.h:303:0
struct collision_event;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, collision_event *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x29EED98A);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle collision_event", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("FrameIndex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->FrameIndex),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Count");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->Count),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("MinP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(cp*, &Element->MinP),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("MaxP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(cp*, &Element->MaxP),
          MemberName,
          ThisHash,
          Params
          );







        


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

