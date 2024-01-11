link_internal void
DoEditorUi(renderer_2d *Ui, engine_debug *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->PickedChunks, CSz("picked_world_chunk_static_buffer PickedChunks"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Textures, CSz("texture_cursor Textures"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->UiDebug, CSz("ui_debug UiDebug"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->Render, CSz("render_debug Render"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DrawEntityCollisionVolumes, CSz("b8 DrawEntityCollisionVolumes"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->DrawWorldAxies, CSz("b8 DrawWorldAxies"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->TriggerRuntimeBreak, CSz("b8 TriggerRuntimeBreak"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ResetAssetNodeView, CSz("b8 ResetAssetNodeView"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->PickedChunkState, CSz("u8 PickedChunkState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->PickedChunk, CSz("world_chunk PickedChunk"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->SelectedAsset, CSz("asset_id SelectedAsset"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, &Element->ModelIndex, CSz("u64 ModelIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      PushNewRow(Ui);
      DoEditorUi(Ui, Element->SelectedEntity, CSz("entity SelectedEntity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);



      PushNewRow(Ui);

      PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
    }
    else
    {
      PushNewRow(Ui);
    }
  }
  else
  {
    PushColumn(Ui, FSz("%S = (null)", Name));
    PushNewRow(Ui);
  }

}

