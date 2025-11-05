// src/engine/editor.h:305:0
struct nested_type;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, nested_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x10BA4CBE);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle nested_type", Element, ThisHash), Params))
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
        
        
        
        cs MemberName = CSz("NestedFoo");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(s32*, &Element->NestedFoo),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("NestedBar");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(f32*, &Element->NestedBar),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("Array");
                                

        if (ToggleButton(Ui,
            CSz("v Array[4]"),
            CSz("> Array[4]"),
            UiId(Window, "toggle nested_type u32 Array", Element->Array, ThisHash),
            Params ))
        {
          OPEN_INDENT_FOR_TOGGLEABLE_REGION();
          PushNewRow(Ui);
                    s32 End = 4;

          RangeIterator(ArrayIndex, End)
          {
                        DoEditorUi(Ui,
              Window,
              Element->Array+ArrayIndex,
              FSz("Array[%d]", ArrayIndex),
              ThisHash,
              Params);

             PushNewRow(Ui); 
          }
          CLOSE_INDENT_FOR_TOGGLEABLE_REGION();
        }
        PushNewRow(Ui);



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

