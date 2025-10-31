// src/engine/editor.h:295:0
link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, world_chunk *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3331643E);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle world_chunk", Element, ThisHash), Params))
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
      if (Name.Count) { PushTableStart(Ui); }

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            

      { 
        
        
        
        cs MemberName = CSz("Next");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_chunk*, Element->Next),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Dim");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->Dim),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Occupancy");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, Element->Occupancy),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("xOccupancyBorder");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, Element->xOccupancyBorder),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("FaceMasks");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, Element->FaceMasks),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("IsOnFreelist");
                                                DoEditorUi(Ui,
          Window,
          Cast(b8*, &Element->IsOnFreelist),
          MemberName,
          ThisHash,
          &DefaultUiRenderParams_Checkbox
          );






                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Mesh");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(gpu_mapped_element_buffer*, &Element->Mesh),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("StandingSpots");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(voxel_position_cursor*, &Element->StandingSpots),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("DimInChunks");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->DimInChunks),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("WorldP");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3i*, &Element->WorldP),
          MemberName,
          ThisHash,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("FilledCount");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->FilledCount),
          MemberName,
          ThisHash,
          Params
          );








                PushNewRow(Ui);

      }
      

      { 
        
        
        
        cs MemberName = CSz("Entities");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_ptr_block_array*, &Element->Entities),
          MemberName,
          ThisHash,
          Params
          );








        
      }

      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      if (Name.Count) { PushTableEnd(Ui); }
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

