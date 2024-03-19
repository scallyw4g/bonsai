// src/engine/editor.cpp:307:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, camera *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle camera", Element), &DefaultUiRenderParams_Generic))
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
&Element->Frust,
        CSz("Frust"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CurrentP,
        CSz("CurrentP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RenderSpacePosition,
        CSz("RenderSpacePosition"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Pitch,
        CSz("Pitch"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Roll,
        CSz("Roll"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Yaw,
        CSz("Yaw"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DistanceFromTarget,
        CSz("DistanceFromTarget"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetPitch,
        CSz("TargetPitch"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetRoll,
        CSz("TargetRoll"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetYaw,
        CSz("TargetYaw"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetDistanceFromTarget,
        CSz("TargetDistanceFromTarget"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Blend,
        CSz("Blend"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Front,
        CSz("Front"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Right,
        CSz("Right"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Up,
        CSz("Up"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GhostId,
        CSz("GhostId"),
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

