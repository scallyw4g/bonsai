// src/engine/editor.cpp:173:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, shape_layer *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle shape_layer", Element), Params))
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
        
        
        
        cs MemberName = CSz("Shape Type");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shape_type*, &Element->Type),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Rect) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_rect*, &Element->Rect),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Sphere) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_sphere*, &Element->Sphere),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Line) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_sphere*, &Element->Line),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Cylinder) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_cylinder*, &Element->Cylinder),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Plane) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_plane*, &Element->Plane),
          MemberName,
          Params
          );








        
      }
       if (Element->Type == ShapeType_Torus) 

      { 
        
        
        
        cs MemberName = {};
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(world_update_op_shape_params_torus*, &Element->Torus),
          MemberName,
          Params
          );








        
      }
      

      { 
        
        
        
        cs MemberName = CSz("Advanced");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(shape_layer_advanced_params*, &Element->Advanced),
          MemberName,
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

