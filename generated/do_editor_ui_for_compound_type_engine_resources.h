// src/engine/editor.cpp:363:0

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
        CSz("Settings"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Stdlib,
        CSz("Stdlib"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Ui,
        CSz("Ui"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GameApi,
        CSz("GameApi"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Hotkeys,
        CSz("Hotkeys"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MaybeMouseRay,
        CSz("MaybeMouseRay"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->World,
        CSz("World"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->GameState,
        CSz("GameState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Graphics,
        CSz("Graphics"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RequestedGameLibReloadNode,
        CSz("RequestedGameLibReloadNode"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RequestedGameLibReloadBehavior,
        CSz("RequestedGameLibReloadBehavior"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Heap,
        CSz("Heap"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->GameMemory,
        CSz("GameMemory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->WorldUpdateMemory,
        CSz("WorldUpdateMemory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->EntityTable,
        CSz("EntityTable"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->AssetSystem,
        CSz("AssetSystem"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FrameIndex,
        CSz("FrameIndex"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->MeshFreelist,
        CSz("MeshFreelist"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->DebugState,
        CSz("DebugState"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->EngineDebug,
        CSz("EngineDebug"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Editor,
        CSz("Editor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MousedOverVoxel,
        CSz("MousedOverVoxel"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->ClosestStandingSpotToCursor,
        CSz("ClosestStandingSpotToCursor"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->HoverEntity,
        CSz("HoverEntity"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->RTTGroup,
        CSz("RTTGroup"),
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

