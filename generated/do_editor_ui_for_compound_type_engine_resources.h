// src/engine/editor.cpp:434:0

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

