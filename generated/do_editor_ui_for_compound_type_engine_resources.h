link_internal void
DoEditorUi(renderer_2d *Ui, engine_resources *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, "toggle engine_resources"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui, &Element->Stdlib, CSz("bonsai_stdlib Stdlib"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Ui, CSz("renderer_2d Ui"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->GameApi, CSz("application_api GameApi"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Hotkeys, CSz("hotkeys Hotkeys"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->MaybeMouseRay, CSz("maybe_ray MaybeMouseRay"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Element->World, CSz("world World"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->GameState, CSz("game_state GameState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->Graphics, CSz("graphics Graphics"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->Heap, CSz("heap_allocator Heap"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Element->Memory, CSz("memory_arena Memory"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, Element->EntityTable, CSz("entity EntityTable"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->CurrentUnnamedAssetIndex, CSz("u64 CurrentUnnamedAssetIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      RangeIterator(ArrayIndex, (256))
      {
        DoEditorUi(Ui, Element->AssetTable+ArrayIndex, CSz("asset AssetTable"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        
      }



      
      DoEditorUi(Ui, &Element->AssetFutex, CSz("bonsai_futex AssetFutex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->AssetMemory, CSz("heap_allocator AssetMemory"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->FrameIndex, CSz("u32 FrameIndex"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      PushNewRow(Ui);

      DoEditorUi(Ui, &Element->MeshFreelist, CSz("tiered_mesh_freelist MeshFreelist"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, Element->DebugState, CSz("debug_state DebugState"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
      DoEditorUi(Ui, &Element->EngineDebug, CSz("engine_debug EngineDebug"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->Editor, CSz("level_editor Editor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->MousedOverVoxel, CSz("maybe_picked_voxel MousedOverVoxel"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->ClosestStandingSpotToCursor, CSz("maybe_standing_spot ClosestStandingSpotToCursor"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->HoverEntity, CSz("maybe_entity_ptr HoverEntity"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);







      
      DoEditorUi(Ui, &Element->RTTGroup, CSz("render_entity_to_texture_group RTTGroup"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
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

