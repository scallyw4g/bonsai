// src/engine/editor.cpp:454:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_resources *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_resources", Element), &DefaultUiRenderParams_Generic))
    {
      PushNewRow(Ui);

      PushTableStart(Ui);
      PushForceUpdateBasis(Ui, V2(20.f, 0.f));
      DoEditorUi(Ui,
        Window,
&Element->Settings,
        CSz("engine_settings Settings"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Stdlib,
        CSz("bonsai_stdlib Stdlib"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Ui,
        CSz("renderer_2d Ui"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GameApi,
        CSz("application_api GameApi"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Hotkeys,
        CSz("hotkeys Hotkeys"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MaybeMouseRay,
        CSz("maybe_ray MaybeMouseRay"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->World,
        CSz("world World"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->GameState,
        CSz("game_state GameState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Graphics,
        CSz("graphics Graphics"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RequestedGameLibReloadNode,
        CSz("file_traversal_node RequestedGameLibReloadNode"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RequestedGameLibReloadBehavior,
        CSz("game_lib_reload_behavior RequestedGameLibReloadBehavior"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Heap,
        CSz("heap_allocator Heap"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->GameMemory,
        CSz("memory_arena GameMemory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->WorldUpdateMemory,
        CSz("memory_arena WorldUpdateMemory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->EntityTable,
        CSz("entity EntityTable"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetSystem,
        CSz("asset_system AssetSystem"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FrameIndex,
        CSz("u32 FrameIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MeshFreelist,
        CSz("tiered_mesh_freelist MeshFreelist"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->DebugState,
        CSz("debug_state DebugState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->EngineDebug,
        CSz("engine_debug EngineDebug"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Editor,
        CSz("level_editor Editor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MousedOverVoxel,
        CSz("maybe_picked_voxel MousedOverVoxel"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ClosestStandingSpotToCursor,
        CSz("maybe_standing_spot ClosestStandingSpotToCursor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->HoverEntity,
        CSz("maybe_entity_ptr HoverEntity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RTTGroup,
        CSz("render_entity_to_texture_group RTTGroup"),
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

