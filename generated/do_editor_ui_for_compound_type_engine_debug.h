// src/engine/editor.cpp:394:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, engine_debug *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle engine_debug", Element), &DefaultUiRenderParams_Generic))
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
        Element->Memory,
        CSz("Memory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ViewMode,
        CSz("ViewMode"),
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

