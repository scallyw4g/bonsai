// src/engine/editor.cpp:285:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, texture *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle texture", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->ID,
        CSz("ID"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Dim,
        CSz("Dim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Slices,
        CSz("Slices"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Channels,
        CSz("Channels"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->IsDepthTexture),
        CSz("IsDepthTexture"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DebugName,
        CSz("DebugName"),
        Params
        );





      PushNewRow(Ui);
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

