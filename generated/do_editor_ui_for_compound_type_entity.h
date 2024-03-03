// src/engine/editor.cpp:335:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Id,
        CSz("Id"),
        Params
        );





      
DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui,
        Window,
&Element->EulerAngles,
        CSz("EulerAngles"),
        Params
,-PI32, PI32 );





      
      DoEditorUi(Ui,
        Window,
&Element->Scale,
        CSz("Scale"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->_CollisionVolumeRadius,
        CSz("_CollisionVolumeRadius"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Physics,
        CSz("Physics"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetId,
        CSz("AssetId"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ModelIndex,
        CSz("ModelIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->LastResolvedCollision,
        CSz("LastResolvedCollision"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LastResolvedPosInfo,
        CSz("LastResolvedPosInfo"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Emitter,
        CSz("Emitter"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->State,
        CSz("State"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Behavior,
        CSz("Behavior"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Carrying,
        CSz("Carrying"),
        Params
        );





      
  if (EntityUserDataEditorUi) {EntityUserDataEditorUi(Ui, Window, &Element->UserType, &Element->UserData, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);}
                 else                        {DoEditorUi(Ui, Window, &Element->UserType, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
;




      PushNewRow(Ui);

  if (EntityUserDataEditorUi) { /* User took control, skip this because it's intended */ }
                 else                        {DoEditorUi(Ui, Window, &Element->UserData, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
;




      PushNewRow(Ui);
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

