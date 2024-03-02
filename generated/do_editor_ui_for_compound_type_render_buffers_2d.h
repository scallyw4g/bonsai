// src/engine/editor.cpp:405:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_buffers_2d *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_buffers_2d", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&(Element->SolidUIVertexBuffer),
        CSz("u32 SolidUIVertexBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->SolidUIColorBuffer),
        CSz("u32 SolidUIColorBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->SolidUIUVBuffer),
        CSz("u32 SolidUIUVBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->DebugTextureArray),
        CSz("texture DebugTextureArray"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->TextTextureUniform),
        CSz("s32 TextTextureUniform"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&(Element->Text2DShader),
        CSz("shader Text2DShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->SolidUIShader),
        CSz("shader SolidUIShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&(Element->Geo),
        CSz("textured_2d_geometry_buffer Geo"),
        Params
        );
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

