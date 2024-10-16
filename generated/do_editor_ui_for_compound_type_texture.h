// src/engine/editor.cpp:332:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, texture *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle texture", Element), Params))
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
        Cast(u32*,&Element->ID),
        CSz("ID"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v2i*,&Element->Dim),
        CSz("Dim"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->Slices),
        CSz("Slices"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->Channels),
        CSz("Channels"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->IsDepthTexture),
        CSz("IsDepthTexture"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(texture_storage_format*,&Element->Format),
        CSz("Format"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cs*,&Element->DebugName),
        CSz("DebugName"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->Queued),
        CSz("Queued"),
        &DefaultUiRenderParams_Checkbox
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

