// src/engine/editor.cpp:441:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_debug", Element), Params))
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
        Cast(memory_arena*, Element->Memory),
        CSz("Memory"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(engine_debug_view_mode*,&Element->ViewMode),
        CSz("ViewMode"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(picked_world_chunk_static_buffer*,&Element->PickedChunks),
        CSz("PickedChunks"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(texture_ptr_block_array*,&Element->Textures),
        CSz("Textures"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(ui_debug*,&Element->UiDebug),
        CSz("UiDebug"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(render_debug*,&Element->Render),
        CSz("Render"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->DrawEntityCollisionVolumes),
        CSz("DrawEntityCollisionVolumes"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->DrawWorldAxies),
        CSz("DrawWorldAxies"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->TriggerRuntimeBreak),
        CSz("TriggerRuntimeBreak"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(b8*,&Element->ResetAssetNodeView),
        CSz("ResetAssetNodeView"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u8*,&Element->PickedChunkState),
        CSz("PickedChunkState"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(world_chunk*, Element->PickedChunk),
        CSz("PickedChunk"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(asset_window_view_mode*,&Element->AssetWindowViewMode),
        CSz("AssetWindowViewMode"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(asset_id*,&Element->SelectedAsset),
        CSz("SelectedAsset"),
        Params
        );







      
      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*,&Element->ModelIndex),
        CSz("ModelIndex"),
        Params
        );







      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_id*,&Element->SelectedEntity),
        CSz("SelectedEntity"),
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

