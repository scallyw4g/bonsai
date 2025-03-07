// src/engine/editor.cpp:326:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk", Element), Params))
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
        Cast(world_chunk*, Element->Next),
        CSz("Next"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(chunk_flag*, &Element->Flags),
        CSz("Flags"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3i*, &Element->Dim),
        CSz("Dim"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*, Element->Occupancy),
        CSz("Occupancy"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*, Element->xOccupancyBorder),
        CSz("xOccupancyBorder"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*, Element->FaceMasks),
        CSz("FaceMasks"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(voxel*, Element->Voxels),
        CSz("Voxels"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(voxel_lighting*, Element->VoxelLighting),
        CSz("VoxelLighting"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(gpu_mapped_element_buffer*, &Element->Mesh),
        CSz("Mesh"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(voxel_position_cursor*, &Element->StandingSpots),
        CSz("StandingSpots"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3i*, &Element->DimInChunks),
        CSz("DimInChunks"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3i*, &Element->WorldP),
        CSz("WorldP"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->FilledCount),
        CSz("FilledCount"),
        Params
        );







            PushNewRow(Ui);

                              DoEditorUi(Ui,
        Window,
        Cast(b8*, &Element->DrawBoundingVoxels),
        CSz("DrawBoundingVoxels"),
        &DefaultUiRenderParams_Checkbox
        );





            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->PointsToLeaveRemaining),
        CSz("PointsToLeaveRemaining"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*, &Element->TriCount),
        CSz("TriCount"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->EdgeBoundaryVoxelCount),
        CSz("EdgeBoundaryVoxelCount"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u32*, &Element->_Pad0),
        CSz("_Pad0"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_ptr_block_array*, &Element->Entities),
        CSz("Entities"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(s32*, &Element->DEBUG_OwnedByThread),
        CSz("DEBUG_OwnedByThread"),
        Params
        );







            PushNewRow(Ui);

                  if (ToggleButton(Ui, CSz("v _Pad1[20]"), CSz("> _Pad1[20]"), UiId(Window, "toggle world_chunk u8 _Pad1", Element->_Pad1), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, 20)
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

