// callsite
// src/engine/editor.cpp:3490:0

// def (anonymous)
// src/engine/editor.cpp:3490:0
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Rect", Element, 0);
  if (Button(Ui, CSz("Shape(Rect)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Rect;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Sphere", Element, 0);
  if (Button(Ui, CSz("Shape(Sphere)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Sphere;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Line", Element, 0);
  if (Button(Ui, CSz("Shape(Line)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Line;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Cylinder", Element, 0);
  if (Button(Ui, CSz("Shape(Cylinder)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Cylinder;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Plane", Element, 0);
  if (Button(Ui, CSz("Shape(Plane)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Plane;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Torus", Element, 0);
  if (Button(Ui, CSz("Shape(Torus)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Torus;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Pyramid", Element, 0);
  if (Button(Ui, CSz("Shape(Pyramid)"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Pyramid;
  }
  PushNewRow(Ui);
}


