// callsite
// src/engine/editor.cpp:4064:0

// def (poof_func.anonymous)
// src/engine/editor.cpp:4064:0
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Rect", Element, 0);
  if (Button(Ui, CSz("Rect"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Rect;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Sphere", Element, 0);
  if (Button(Ui, CSz("Sphere"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Sphere;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Line", Element, 0);
  if (Button(Ui, CSz("Line"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Line;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Cylinder", Element, 0);
  if (Button(Ui, CSz("Cylinder"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Cylinder;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Plane", Element, 0);
  if (Button(Ui, CSz("Plane"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Plane;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Torus", Element, 0);
  if (Button(Ui, CSz("Torus"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Torus;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select ShapeType_Pyramid", Element, 0);
  if (Button(Ui, CSz("Pyramid"), ButtonId))
  {
    Element->Type = BrushLayerType_Shape;
    Element->Shape.Type = ShapeType_Pyramid;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}



