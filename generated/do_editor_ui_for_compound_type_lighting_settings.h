link_internal void
DoEditorUi(renderer_2d *Ui, lighting_settings *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->AutoDayNightCycle, CSz("b8 AutoDayNightCycle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->tDay, CSz("r32 tDay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,-PI32, PI32);





      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->SunP, CSz("v3 SunP"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,-1.f, 1.f);





      
      DoEditorUi(Ui, &Element->DawnColor, CSz("v3 DawnColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->SunColor, CSz("v3 SunColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->DuskColor, CSz("v3 DuskColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->MoonColor, CSz("v3 MoonColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->SunIntensity, CSz("f32 SunIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->MoonIntensity, CSz("f32 MoonIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->DawnIntensity, CSz("f32 DawnIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->DuskIntensity, CSz("f32 DuskIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->CurrentSunColor, CSz("v3 CurrentSunColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, &Element->Test, CSz("u32 Test"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
    PushNewRow(Ui);
  }

}

