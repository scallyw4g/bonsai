// src/engine/editor.cpp:433:0

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
        CSz("memory_arena Memory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->PickedChunks,
        CSz("picked_world_chunk_static_buffer PickedChunks"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Textures,
        CSz("texture_ptr_block_array Textures"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->UiDebug,
        CSz("ui_debug UiDebug"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Render,
        CSz("render_debug Render"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->DrawEntityCollisionVolumes,
        CSz("b8 DrawEntityCollisionVolumes"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->DrawWorldAxies,
        CSz("b8 DrawWorldAxies"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TriggerRuntimeBreak,
        CSz("b8 TriggerRuntimeBreak"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ResetAssetNodeView,
        CSz("b8 ResetAssetNodeView"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->PickedChunkState,
        CSz("u8 PickedChunkState"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Element->PickedChunk,
        CSz("world_chunk PickedChunk"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetWindowViewMode,
        CSz("asset_window_view_mode AssetWindowViewMode"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->SelectedAsset,
        CSz("asset_id SelectedAsset"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ModelIndex,
        CSz("u64 ModelIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->SelectedEntity,
        CSz("entity_id SelectedEntity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldEditDebugMesh,
        CSz("untextured_3d_geometry_buffer WorldEditDebugMesh"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldEditDebugThumb,
        CSz("asset_thumbnail WorldEditDebugThumb"),
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

