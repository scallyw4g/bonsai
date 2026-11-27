// callsite
// src/engine/editor.cpp:4155:0

// def (poof_func.anonymous)
// src/engine/editor.cpp:4155:0
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Perlin", Element, 0);
  if (Button(Ui, CSz("Perlin"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Perlin;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Noise.Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Voronoi", Element, 0);
  if (Button(Ui, CSz("Voronoi"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Voronoi;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Noise.Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_VoronoiLattice", Element, 0);
  if (Button(Ui, CSz("VoronoiLattice"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_VoronoiLattice;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Noise.Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_White", Element, 0);
  if (Button(Ui, CSz("White"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_White;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Noise.Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}

{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_RectLattice", Element, 0);
  if (Button(Ui, CSz("RectLattice"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_RectLattice;
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Type), ChangeRecordId);
    MaybePushChangeRecord(UiChangeEvents, PrimitiveType_u32, Cast(u32*, &Element->Noise.Type), ChangeRecordId);
  }
  PushNewRow(Ui);
}



