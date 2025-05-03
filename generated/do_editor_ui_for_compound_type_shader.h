// src/engine/editor.cpp:410:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shader *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shader", Element), Params))
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
        Cast(u32*, &Element->ID),
        CSz("ID"),
        Params
        );








            PushNewRow(Ui);

                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(shader_uniform_buffer*, &Element->Uniforms),
        CSz("Uniforms"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cs*, &Element->VertexSourceFilename),
        CSz("VertexSourceFilename"),
        Params
        );








            PushNewRow(Ui);

                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(cs*, &Element->FragSourceFilename),
        CSz("FragSourceFilename"),
        Params
        );








            PushNewRow(Ui);

                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s64*, &Element->VertexTimeModifiedWhenLoaded),
        CSz("VertexTimeModifiedWhenLoaded"),
        Params
        );








            PushNewRow(Ui);

                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s64*, &Element->FragmentTimeModifiedWhenLoaded),
        CSz("FragmentTimeModifiedWhenLoaded"),
        Params
        );








            PushNewRow(Ui);

                                    DoEditorUi(Ui,
        Window,
        Cast(b8*, &Element->HotReloaded),
        CSz("HotReloaded"),
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

