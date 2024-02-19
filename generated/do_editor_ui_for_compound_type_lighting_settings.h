// src/engine/editor.cpp:185:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->AutoDayNightCycle, CSz("b8 AutoDayNightCycle"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->tDay, CSz("r32 tDay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,-PI32, PI32);





      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->DawnIntensity, CSz("f32 DawnIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->DawnColor, CSz("v3 DawnColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->SunIntensity, CSz("f32 SunIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->SunColor, CSz("v3 SunColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->DuskIntensity, CSz("f32 DuskIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->DuskColor, CSz("v3 DuskColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->MoonIntensity, CSz("f32 MoonIntensity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0.f, 3.f);





      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->MoonColor, CSz("v3 MoonColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->CurrentSunColor, CSz("v3 CurrentSunColor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

