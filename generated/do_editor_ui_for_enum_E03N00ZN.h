// callsite
// src/engine/editor.h:1416:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:693:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, brush_layer_type *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x136838E8);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle brush_layer_type", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Perlin"), UiId(Window, "enum BrushLayerType_Noise_Perlin", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Noise_Perlin;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Voronoi"), UiId(Window, "enum BrushLayerType_Noise_Voronoi", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Noise_Voronoi;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("White"), UiId(Window, "enum BrushLayerType_Noise_White", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Noise_White;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Rect"), UiId(Window, "enum BrushLayerType_Shape_Rect", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Rect;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Sphere"), UiId(Window, "enum BrushLayerType_Shape_Sphere", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Sphere;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Line"), UiId(Window, "enum BrushLayerType_Shape_Line", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Line;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Cylinder"), UiId(Window, "enum BrushLayerType_Shape_Cylinder", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Cylinder;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Plane"), UiId(Window, "enum BrushLayerType_Shape_Plane", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Plane;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Torus"), UiId(Window, "enum BrushLayerType_Shape_Torus", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Torus;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Pyramid"), UiId(Window, "enum BrushLayerType_Shape_Pyramid", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Shape_Pyramid;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Brush"), UiId(Window, "enum BrushLayerType_Brush", Element, ThisHash), Params))
    {
      Result = True;
            *Element = BrushLayerType_Brush;


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


