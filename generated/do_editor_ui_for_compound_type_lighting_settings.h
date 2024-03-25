// src/engine/editor.cpp:197:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lighting_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lighting_settings", Element), &DefaultUiRenderParams_Generic))
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

