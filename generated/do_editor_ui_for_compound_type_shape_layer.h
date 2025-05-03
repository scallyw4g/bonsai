// src/engine/editor.cpp:112:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_layer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shape_layer", Element), Params))
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
        Cast(shape_type*, &Element->Type),
        CSz("Type"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_update_op_shape_params_sphere*, &Element->Sphere),
        CSz("Sphere"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_update_op_shape_params_rect*, &Element->Rect),
        CSz("Rect"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(f32*, &Element->Threshold),
        CSz("Threshold"),
        Params
        , 0.f, 1.f );








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

