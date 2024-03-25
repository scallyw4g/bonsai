// src/engine/editor.cpp:152:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, input *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle input", Element), &DefaultUiRenderParams_Generic))
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
&Element->Enter,
        CSz("Enter"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Escape,
        CSz("Escape"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Alt,
        CSz("Alt"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Ctrl,
        CSz("Ctrl"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Shift,
        CSz("Shift"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Backspace,
        CSz("Backspace"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Delete,
        CSz("Delete"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F12,
        CSz("F12"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F11,
        CSz("F11"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F10,
        CSz("F10"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F9,
        CSz("F9"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F8,
        CSz("F8"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F7,
        CSz("F7"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F6,
        CSz("F6"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F5,
        CSz("F5"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F4,
        CSz("F4"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F3,
        CSz("F3"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F2,
        CSz("F2"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F1,
        CSz("F1"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Dot,
        CSz("Dot"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Minus,
        CSz("Minus"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FSlash,
        CSz("FSlash"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Space,
        CSz("Space"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Underscore,
        CSz("Underscore"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N0,
        CSz("N0"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N1,
        CSz("N1"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N2,
        CSz("N2"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N3,
        CSz("N3"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N4,
        CSz("N4"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N5,
        CSz("N5"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N6,
        CSz("N6"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N7,
        CSz("N7"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N8,
        CSz("N8"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N9,
        CSz("N9"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->A,
        CSz("A"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->B,
        CSz("B"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->C,
        CSz("C"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->D,
        CSz("D"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->E,
        CSz("E"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->F,
        CSz("F"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->G,
        CSz("G"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->H,
        CSz("H"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->I,
        CSz("I"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->J,
        CSz("J"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->K,
        CSz("K"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->L,
        CSz("L"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->M,
        CSz("M"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->N,
        CSz("N"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->O,
        CSz("O"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->P,
        CSz("P"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Q,
        CSz("Q"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->R,
        CSz("R"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->S,
        CSz("S"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->T,
        CSz("T"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->U,
        CSz("U"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->V,
        CSz("V"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->W,
        CSz("W"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->X,
        CSz("X"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Y,
        CSz("Y"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Z,
        CSz("Z"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RMB,
        CSz("RMB"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LMB,
        CSz("LMB"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MMB,
        CSz("MMB"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MouseWheelDelta,
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

