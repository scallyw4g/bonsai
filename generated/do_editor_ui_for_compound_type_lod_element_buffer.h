// src/engine/editor.cpp:243:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, lod_element_buffer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle lod_element_buffer", Element), Params))
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
        Cast(u32*, &Element->MeshMask),
        CSz("MeshMask"),
        Params
        );







            PushNewRow(Ui);

                  if (ToggleButton(Ui, CSz("v GpuBufferHandles[MeshIndex_Count]"), CSz("> GpuBufferHandles[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer gpu_element_buffer_handles GpuBufferHandles", Element->GpuBufferHandles), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
                    DoEditorUi(Ui, Window, Element->GpuBufferHandles+ArrayIndex, FSz("GpuBufferHandles[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                  if (ToggleButton(Ui, CSz("v E[MeshIndex_Count]"), CSz("> E[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer geo_u3d_ptr E", Element->E), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
                    DoEditorUi(Ui, Window, Element->E+ArrayIndex, FSz("E[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
      PushNewRow(Ui);



      
                  if (ToggleButton(Ui, CSz("v Locks[MeshIndex_Count]"), CSz("> Locks[MeshIndex_Count]"), UiId(Window, "toggle lod_element_buffer bonsai_futex Locks", Element->Locks), Params ))
      {
        OPEN_INDENT_FOR_TOGGLEABLE_REGION();
        PushNewRow(Ui);
        RangeIterator(ArrayIndex, MeshIndex_Count)
        {
                    DoEditorUi(Ui, Window, Element->Locks+ArrayIndex, FSz("Locks[%d]", ArrayIndex), Params);

          
        }
        CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
      }
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

