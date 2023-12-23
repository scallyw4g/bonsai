link_internal void
DoEditorUi(renderer_2d *Ui, engine_debug *Element, const char* Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  /* PushTableStart(Ui); */
  if (ToggleButton(Ui, FSz("v %s", Name), FSz("> %s", Name), umm(Element) ^ umm(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
  {
    PushForceUpdateBasis(Ui, V2(20.f, 0.f));
    /* Padding.x += 20.f; */
    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->PickedChunks, "picked_world_chunk_static_buffer PickedChunks", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Textures, "texture_cursor Textures", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->UiDebug, "ui_debug UiDebug", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->Render, "render_debug Render", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DrawEntityCollisionVolumes, "b8 DrawEntityCollisionVolumes", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->DrawWorldAxies, "b8 DrawWorldAxies", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->TriggerRuntimeBreak, "b8 TriggerRuntimeBreak", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->ResetAssetNodeView, "b8 ResetAssetNodeView", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->PickedChunkState, "u8 PickedChunkState", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, Element->PickedChunk, "world_chunk PickedChunk", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, &Element->SelectedAsset, "file_traversal_node SelectedAsset", EDITOR_UI_FUNCTION_INSTANCE_NAMES);




    PushNewRow(Ui);
    DoEditorUi(Ui, Element->HoverEntity, "entity HoverEntity", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    DoEditorUi(Ui, Element->SelectedEntity, "entity SelectedEntity", EDITOR_UI_FUNCTION_INSTANCE_NAMES);



    PushNewRow(Ui);
    PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
  }
  else
  {
    PushNewRow(Ui);
  }
}

