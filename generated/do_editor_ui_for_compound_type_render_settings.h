// src/engine/editor.cpp:200:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_settings *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_settings", Element), &DefaultUiRenderParams_Generic))
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
        Cast(b8*,&Element->UseSsao),
        CSz("UseSsao"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseShadowMapping),
        CSz("UseShadowMapping"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->UseLightingBloom),
        CSz("UseLightingBloom"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMyersOIT),
        CSz("BravoilMyersOIT"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->BravoilMcGuireOIT),
        CSz("BravoilMcGuireOIT"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMajorGrid),
        CSz("DrawMajorGrid"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawMinorGrid),
        CSz("DrawMinorGrid"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MajorGridDim,
        CSz("MajorGridDim"),
        Params
,0, 32 );





      PushNewRow(Ui);

      

      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ToneMappingType,
        CSz("ToneMappingType"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Lighting,
        CSz("Lighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ApplicationResolution,
        CSz("ApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ShadowMapResolution,
        CSz("ShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LuminanceMapResolution,
        CSz("LuminanceMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iApplicationResolution,
        CSz("iApplicationResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iShadowMapResolution,
        CSz("iShadowMapResolution"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->iLuminanceMapResolution,
        CSz("iLuminanceMapResolution"),
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

