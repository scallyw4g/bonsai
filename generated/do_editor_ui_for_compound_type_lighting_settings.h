// src/engine/editor.cpp:150:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->AutoDayNightCycle,
        CSz("AutoDayNightCycle"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->tDay,
        CSz("tDay"),
        Params
,-PI32, PI32 );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DawnIntensity,
        CSz("DawnIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DawnColor,
        CSz("DawnColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SunIntensity,
        CSz("SunIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SunColor,
        CSz("SunColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DuskIntensity,
        CSz("DuskIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DuskColor,
        CSz("DuskColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MoonIntensity,
        CSz("MoonIntensity"),
        Params
,0.f, 3.f );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MoonColor,
        CSz("MoonColor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CurrentSunColor,
        CSz("CurrentSunColor"),
        Params
        );
      CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
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

