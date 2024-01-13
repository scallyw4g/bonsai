link_internal void
DoEditorUi(renderer_2d *Ui, render_settings *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Lighting, CSz("lighting_settings Lighting"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      
      DoEditorUi(Ui, Cast(b8*, &Element->UseSsao), CSz("b32 UseSsao"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->UseShadowMapping), CSz("b32 UseShadowMapping"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->UseLightingBloom), CSz("b32 UseLightingBloom"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->BravoilMyersOIT), CSz("b32 BravoilMyersOIT"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->BravoilMcGuireOIT), CSz("b32 BravoilMcGuireOIT"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->DrawMajorGrid), CSz("b32 DrawMajorGrid"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, Cast(b8*, &Element->DrawMinorGrid), CSz("b32 DrawMinorGrid"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);






      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->MajorGridDim, CSz("r32 MajorGridDim"), EDITOR_UI_FUNCTION_INSTANCE_NAMES,0, 32);





      PushNewRow(Ui);

      

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

