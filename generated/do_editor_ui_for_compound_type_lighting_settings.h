// src/engine/editor.cpp:220:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->AutoDayNightCycle,
        CSz("b8 AutoDayNightCycle"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->tDay,
        CSz("r32 tDay"),
        Params
,-PI32, PI32 );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DawnIntensity,
        CSz("f32 DawnIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DawnColor,
        CSz("v3 DawnColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SunIntensity,
        CSz("f32 SunIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SunColor,
        CSz("v3 SunColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DuskIntensity,
        CSz("f32 DuskIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DuskColor,
        CSz("v3 DuskColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MoonIntensity,
        CSz("f32 MoonIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MoonColor,
        CSz("v3 MoonColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CurrentSunColor,
        CSz("v3 CurrentSunColor"),
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

