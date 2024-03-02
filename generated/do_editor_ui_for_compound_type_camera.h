// src/engine/editor.cpp:362:0

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
        CSz("frustum Frust"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CurrentP,
        CSz("cp CurrentP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RenderSpacePosition,
        CSz("v3 RenderSpacePosition"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Pitch,
        CSz("r32 Pitch"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Roll,
        CSz("r32 Roll"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Yaw,
        CSz("r32 Yaw"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DistanceFromTarget,
        CSz("r32 DistanceFromTarget"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetPitch,
        CSz("r32 TargetPitch"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetRoll,
        CSz("r32 TargetRoll"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetYaw,
        CSz("r32 TargetYaw"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TargetDistanceFromTarget,
        CSz("r32 TargetDistanceFromTarget"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Blend,
        CSz("r32 Blend"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Front,
        CSz("v3 Front"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Right,
        CSz("v3 Right"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Up,
        CSz("v3 Up"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GhostId,
        CSz("entity_id GhostId"),
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

