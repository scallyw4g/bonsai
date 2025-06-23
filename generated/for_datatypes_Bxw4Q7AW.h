// src/engine/editor.cpp:516:0












































































































































































































































































































link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit", Element), Params))
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
        
        
        
        cs MemberName = CSz("Region");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(rect3cp*, &Element->Region),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Brush");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_edit_brush*, Element->Brush),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Tombstone");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->Tombstone),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Selected");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->Selected),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Ordinal");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->Ordinal),
          MemberName,
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






























































link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, selection_region *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle selection_region", Element), Params))
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
        
        
        
        cs MemberName = CSz("Clicks");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->Clicks),
          MemberName,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Base");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(cp*, &Element->Base),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Region");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(rect3cp*, &Element->Region),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Diff");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->Diff),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("InitialSelect");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->InitialSelect),
          MemberName,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("ModMode");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(selection_modification_mode*, &Element->ModMode),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("ModState");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(selection_modification_state*, &Element->ModState),
          MemberName,
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






























































































































































































































































































































































































































