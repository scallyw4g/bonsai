// callsite
// src/engine/editor.cpp:3953:0

// def (poof_func.anonymous)
// src/engine/editor.cpp:3953:0
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Perlin", Element, 0);
  if (Button(Ui, CSz("Perlin Noise"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Perlin;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Voronoi", Element, 0);
  if (Button(Ui, CSz("Voronoi Noise"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Voronoi;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_VoronoiLattice", Element, 0);
  if (Button(Ui, CSz("VoronoiLattice Noise"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_VoronoiLattice;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_White", Element, 0);
  if (Button(Ui, CSz("White Noise"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_White;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_RectLattice", Element, 0);
  if (Button(Ui, CSz("RectLattice Noise"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_RectLattice;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}


