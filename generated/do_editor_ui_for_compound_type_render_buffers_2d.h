// src/engine/editor.cpp:348:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, render_buffers_2d *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle render_buffers_2d", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      OPEN_INDENT_FOR_TOGGLEABLE_REGION();
      DoEditorUi(Ui,
        Window,
&Element->SolidUIVertexBuffer,
        CSz("SolidUIVertexBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SolidUIColorBuffer,
        CSz("SolidUIColorBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SolidUIUVBuffer,
        CSz("SolidUIUVBuffer"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DebugTextureArray,
        CSz("DebugTextureArray"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->TextTextureUniform,
        CSz("TextTextureUniform"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Text2DShader,
        CSz("Text2DShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SolidUIShader,
        CSz("SolidUIShader"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Geo,
        CSz("Geo"),
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

