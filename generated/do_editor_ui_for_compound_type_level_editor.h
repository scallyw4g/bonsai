// src/engine/editor.cpp:335:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, level_editor *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle level_editor", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
        Element->Memory,
        CSz("Memory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->NoiseEditor,
        CSz("NoiseEditor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->LayeredBrushEditor,
        CSz("LayeredBrushEditor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->EngineDebugViewModeToggleBits,
        CSz("EngineDebugViewModeToggleBits"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectedColorIndex,
        CSz("SelectedColorIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->HoverColorIndex,
        CSz("HoverColorIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectionClicks,
        CSz("SelectionClicks"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectionBase,
        CSz("SelectionBase"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SelectionRegion,
        CSz("SelectionRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CopyRegion,
        CSz("CopyRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Selection,
        CSz("Selection"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Entity,
        CSz("Entity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetThumbnails,
        CSz("AssetThumbnails"),
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

