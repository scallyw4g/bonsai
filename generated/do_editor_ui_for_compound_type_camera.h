// src/engine/editor.cpp:282:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, camera *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle camera", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
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
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
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

