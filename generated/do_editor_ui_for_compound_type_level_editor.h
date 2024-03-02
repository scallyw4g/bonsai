// src/engine/editor.cpp:439:0

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
        CSz("memory_arena Memory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->NoiseSelection,
        CSz("noise_selector NoiseSelection"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->NoisePreviewThumbnail,
        CSz("asset_thumbnail NoisePreviewThumbnail"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->EngineDebugViewModeToggleBits,
        CSz("u64 EngineDebugViewModeToggleBits"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectedColorIndex,
        CSz("u16 SelectedColorIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->HoverColorIndex,
        CSz("u16 HoverColorIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectionClicks,
        CSz("u32 SelectionClicks"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectionBase,
        CSz("cp SelectionBase"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SelectionRegion,
        CSz("rect3cp SelectionRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->CopyRegion,
        CSz("rect3cp CopyRegion"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Selection,
        CSz("selection_modification_state Selection"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Entity,
        CSz("selection_modification_state Entity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetThumbnails,
        CSz("asset_thumbnail_block_array AssetThumbnails"),
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

