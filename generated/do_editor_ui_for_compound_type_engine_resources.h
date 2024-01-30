// src/engine/editor.cpp:336:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_resources *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_resources", Element), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, Window, &Element->Stdlib, CSz("bonsai_stdlib Stdlib"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Ui, CSz("renderer_2d Ui"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->GameApi, CSz("application_api GameApi"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Hotkeys, CSz("hotkeys Hotkeys"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->MaybeMouseRay, CSz("maybe_ray MaybeMouseRay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, Element->World, CSz("world World"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->GameState, CSz("game_state GameState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->Graphics, CSz("graphics Graphics"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->Heap, CSz("heap_allocator Heap"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, Element->Memory, CSz("memory_arena Memory"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, Element->EntityTable, CSz("entity EntityTable"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->CurrentUnnamedAssetIndex, CSz("u64 CurrentUnnamedAssetIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      if (ToggleButton(Ui, CSz("v asset AssetTable[(256)]"), CSz("> asset AssetTable[(256)]"), UiId(Window, "toggle engine_resources asset AssetTable", Element->AssetTable), EDITOR_UI_FUNCTION_INSTANCE_NAMES ))
      {
        PushForceUpdateBasis(Ui, V2(20.f, 0.f));
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, (256))
        {
          DoEditorUi(Ui, Window, Element->AssetTable+ArrayIndex, FSz("asset AssetTable[%d]", ArrayIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          
        }
        PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
      }
      PushNewRow(Ui);



      
      DoEditorUi(Ui, Window, &Element->AssetFutex, CSz("bonsai_futex AssetFutex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->AssetMemory, CSz("heap_allocator AssetMemory"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->FrameIndex, CSz("u32 FrameIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, Window, &Element->MeshFreelist, CSz("tiered_mesh_freelist MeshFreelist"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, Element->DebugState, CSz("debug_state DebugState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Window, &Element->EngineDebug, CSz("engine_debug EngineDebug"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->Editor, CSz("level_editor Editor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->MousedOverVoxel, CSz("maybe_picked_voxel MousedOverVoxel"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->ClosestStandingSpotToCursor, CSz("maybe_standing_spot ClosestStandingSpotToCursor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->HoverEntity, CSz("maybe_entity_ptr HoverEntity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Window, &Element->RTTGroup, CSz("render_entity_to_texture_group RTTGroup"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

