// src/engine/editor.cpp:435:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_debug", Element), &DefaultUiRenderParams_Generic))
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
&Element->PickedChunks,
        CSz("PickedChunks"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Textures,
        CSz("Textures"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->UiDebug,
        CSz("UiDebug"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Render,
        CSz("Render"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DrawEntityCollisionVolumes,
        CSz("DrawEntityCollisionVolumes"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DrawWorldAxies,
        CSz("DrawWorldAxies"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TriggerRuntimeBreak,
        CSz("TriggerRuntimeBreak"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ResetAssetNodeView,
        CSz("ResetAssetNodeView"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->PickedChunkState,
        CSz("PickedChunkState"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->PickedChunk,
        CSz("PickedChunk"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetWindowViewMode,
        CSz("AssetWindowViewMode"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SelectedAsset,
        CSz("SelectedAsset"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ModelIndex,
        CSz("ModelIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectedEntity,
        CSz("SelectedEntity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldEditDebugMesh,
        CSz("WorldEditDebugMesh"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldEditDebugThumb,
        CSz("WorldEditDebugThumb"),
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

