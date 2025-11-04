// src/engine/editor.h:303:0
struct gpu_element_buffer_handles;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, gpu_element_buffer_handles *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0xA286F82);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle gpu_element_buffer_handles", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("VAO");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->VAO),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Handles");
                                

        if (ToggleButton(Ui,
            CSz("v Handles[3]"),
            CSz("> Handles[3]"),
            UiId(Window, "toggle gpu_element_buffer_handles u32 Handles", Element->Handles, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = 3;

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->Handles+ArrayIndex,
              FSz("Handles[%d]", ArrayIndex),
              ThisHash,
              Params);

             PushNewRow(Ui); 
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);



                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("ElementCount");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u32*, &Element->ElementCount),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("ElementType");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(data_type*, &Element->ElementType),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Mapped");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(b8*, &Element->Mapped),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Pad");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(b8*, &Element->Pad),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Flags");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u16*, &Element->Flags),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



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

