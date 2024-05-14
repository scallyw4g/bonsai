// src/engine/editor.cpp:452:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_resources *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_resources", Element), &DefaultUiRenderParams_Generic))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(engine_settings*,&Element->Settings),
        CSz("Settings"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(bonsai_stdlib*,&Element->Stdlib),
        CSz("Stdlib"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(renderer_2d*,&Element->Ui),
        CSz("Ui"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(application_api*,&Element->GameApi),
        CSz("GameApi"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(hotkeys*,&Element->Hotkeys),
        CSz("Hotkeys"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(maybe_ray*,&Element->MaybeMouseRay),
        CSz("MaybeMouseRay"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world*, Element->World),
        CSz("World"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(game_state*, Element->GameState),
        CSz("GameState"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(graphics*,&Element->Graphics),
        CSz("Graphics"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(file_traversal_node*,&Element->RequestedGameLibReloadNode),
        CSz("RequestedGameLibReloadNode"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(game_lib_reload_behavior*,&Element->RequestedGameLibReloadBehavior),
        CSz("RequestedGameLibReloadBehavior"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(heap_allocator*,&Element->Heap),
        CSz("Heap"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(memory_arena*, Element->GameMemory),
        CSz("GameMemory"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(memory_arena*, Element->WorldUpdateMemory),
        CSz("WorldUpdateMemory"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity*, Element->EntityTable),
        CSz("EntityTable"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(asset_system*,&Element->AssetSystem),
        CSz("AssetSystem"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*,&Element->FrameIndex),
        CSz("FrameIndex"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(tiered_mesh_freelist*,&Element->geo_u3d_MeshFreelist),
        CSz("geo_u3d_MeshFreelist"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(tiered_mesh_freelist*,&Element->world_chunk_MeshFreelist),
        CSz("world_chunk_MeshFreelist"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(debug_state*, Element->DebugState),
        CSz("DebugState"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(engine_debug*,&Element->EngineDebug),
        CSz("EngineDebug"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(level_editor*,&Element->Editor),
        CSz("Editor"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(maybe_picked_voxel*,&Element->MousedOverVoxel),
        CSz("MousedOverVoxel"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(maybe_standing_spot*,&Element->ClosestStandingSpotToCursor),
        CSz("ClosestStandingSpotToCursor"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(maybe_entity_ptr*,&Element->HoverEntity),
        CSz("HoverEntity"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(render_entity_to_texture_group*,&Element->RTTGroup),
        CSz("RTTGroup"),
        Params
        );
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
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

