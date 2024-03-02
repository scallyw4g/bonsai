// src/engine/editor.cpp:267:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, voxel *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle voxel", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->Flags),
        CSz("u8 Flags"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Transparency),
        CSz("u8 Transparency"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Color),
        CSz("u16 Color"),
        Params
        );





      PushNewRow(Ui);
      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      PushTableEnd(Ui);
    }

    PushNewRow(Ui);
  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

