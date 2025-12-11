// callsite
// src/engine/editor.cpp:217:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:705:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_shape_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2F2468DA);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle brush_shape_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Rect"), UiId(Window, "enum ShapeType_Rect", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Rect;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Sphere"), UiId(Window, "enum ShapeType_Sphere", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Sphere;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Line"), UiId(Window, "enum ShapeType_Line", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Line;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Cylinder"), UiId(Window, "enum ShapeType_Cylinder", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Cylinder;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Plane"), UiId(Window, "enum ShapeType_Plane", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Plane;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Torus"), UiId(Window, "enum ShapeType_Torus", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Torus;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Pyramid"), UiId(Window, "enum ShapeType_Pyramid", Element, ThisHash), Params))
    {
      Result = True;
            *Element = ShapeType_Pyramid;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}


