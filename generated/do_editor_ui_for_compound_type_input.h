// src/engine/editor.cpp:143:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, input *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle input", Element), Params))
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
        Cast(input_event*, &Element->Enter),
        CSz("Enter"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Escape),
        CSz("Escape"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Alt),
        CSz("Alt"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Ctrl),
        CSz("Ctrl"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Shift),
        CSz("Shift"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Backspace),
        CSz("Backspace"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Delete),
        CSz("Delete"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F12),
        CSz("F12"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F11),
        CSz("F11"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F10),
        CSz("F10"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F9),
        CSz("F9"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F8),
        CSz("F8"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F7),
        CSz("F7"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F6),
        CSz("F6"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F5),
        CSz("F5"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F4),
        CSz("F4"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F3),
        CSz("F3"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F2),
        CSz("F2"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F1),
        CSz("F1"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Dot),
        CSz("Dot"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Minus),
        CSz("Minus"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->FSlash),
        CSz("FSlash"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Space),
        CSz("Space"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Underscore),
        CSz("Underscore"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N0),
        CSz("N0"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N1),
        CSz("N1"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N2),
        CSz("N2"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N3),
        CSz("N3"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N4),
        CSz("N4"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N5),
        CSz("N5"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N6),
        CSz("N6"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N7),
        CSz("N7"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N8),
        CSz("N8"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N9),
        CSz("N9"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->A),
        CSz("A"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->B),
        CSz("B"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->C),
        CSz("C"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->D),
        CSz("D"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->E),
        CSz("E"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->F),
        CSz("F"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->G),
        CSz("G"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->H),
        CSz("H"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->I),
        CSz("I"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->J),
        CSz("J"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->K),
        CSz("K"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->L),
        CSz("L"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->M),
        CSz("M"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->N),
        CSz("N"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->O),
        CSz("O"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->P),
        CSz("P"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Q),
        CSz("Q"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->R),
        CSz("R"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->S),
        CSz("S"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->T),
        CSz("T"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->U),
        CSz("U"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->V),
        CSz("V"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->W),
        CSz("W"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->X),
        CSz("X"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Y),
        CSz("Y"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->Z),
        CSz("Z"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->RMB),
        CSz("RMB"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->LMB),
        CSz("LMB"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(input_event*, &Element->MMB),
        CSz("MMB"),
        Params
        );








      
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->MouseWheelDelta),
        CSz("MouseWheelDelta"),
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

