// src/engine/editor.cpp:290:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk", Element), &DefaultUiRenderParams_Generic))
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
        Element->Next,
        CSz("Next"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Flags,
        CSz("Flags"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Dim,
        CSz("Dim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Voxels,
        CSz("Voxels"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->VoxelLighting,
        CSz("VoxelLighting"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Meshes,
        CSz("Meshes"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->StandingSpots,
        CSz("StandingSpots"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorldP,
        CSz("WorldP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->FilledCount,
        CSz("FilledCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
        Cast(b8*,&Element->DrawBoundingVoxels),
        CSz("DrawBoundingVoxels"),
        &DefaultUiRenderParams_Checkbox
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->PointsToLeaveRemaining,
        CSz("PointsToLeaveRemaining"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->TriCount,
        CSz("TriCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->EdgeBoundaryVoxelCount,
        CSz("EdgeBoundaryVoxelCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->_Pad0,
        CSz("_Pad0"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->Entities,
        CSz("Entities"),
        Params
        );





      
      if (ToggleButton(Ui, CSz("v _Pad1[32]"), CSz("> _Pad1[32]"), UiId(Window, "toggle world_chunk u8 _Pad1", Element->_Pad1), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 32)
        {
          DoEditorUi(Ui, Window, Element->_Pad1+ArrayIndex, FSz("_Pad1[%d]", ArrayIndex), Params);
 PushNewRow(Ui); 
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      PushNewRow(Ui);
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

