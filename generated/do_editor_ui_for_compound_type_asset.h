// src/engine/editor.h:305:0
struct asset;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, asset *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x318DDBAB);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle asset", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("LoadState");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(asset_load_state*, &Element->LoadState),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Id");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(asset_id*, &Element->Id),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("LRUFrameIndex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->LRUFrameIndex),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Type");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(asset_type*, &Element->Type),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("(anonymous)");
                                                                






        


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

