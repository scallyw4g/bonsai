// src/engine/editor.cpp:335:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_brush *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_brush", Element), Params))
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
                        if (ToggleButton(Ui, CSz("v NameBuf[(256) + 1]"), CSz("> NameBuf[(256) + 1]"), UiId(Window, "toggle world_edit_brush char  NameBuf", Element->NameBuf), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (256) + 1)
        {
                    DoEditorUi(Ui, Window, Element->NameBuf+ArrayIndex, FSz("NameBuf[%d]", ArrayIndex), Params);

           PushNewRow(Ui); 
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_blend_mode*, &Element->Mode),
        CSz("Mode"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_blend_mode_modifier*, &Element->Modifier),
        CSz("Modifier"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*, &Element->Ordinal),
        CSz("Ordinal"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(layered_brush*, &Element->Layered),
        CSz("Layered"),
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

