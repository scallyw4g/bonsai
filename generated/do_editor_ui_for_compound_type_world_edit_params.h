// src/engine/editor.h:700:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_edit_params *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_edit_params", Element), Params))
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
        Cast(world_edit_mode*,&Element->Mode),
        CSz("Mode"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_edit_mode_modifier*,&Element->Modifier),
        CSz("Modifier"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*,&Element->Iterations),
        CSz("Iterations"),
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

