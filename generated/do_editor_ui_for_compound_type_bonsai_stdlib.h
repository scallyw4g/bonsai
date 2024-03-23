// src/engine/editor.cpp:158:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bonsai_stdlib *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle bonsai_stdlib", Element), &DefaultUiRenderParams_Generic))
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
&Element->Os,
        CSz("Os"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Plat,
        CSz("Plat"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->ThreadStates,
        CSz("ThreadStates"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AllTextures,
        CSz("AllTextures"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DebugSystem,
        CSz("DebugSystem"),
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

