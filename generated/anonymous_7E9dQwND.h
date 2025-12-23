// callsite
// src/engine/editor.cpp:3470:0

// def (anonymous)
// src/engine/editor.cpp:3470:0
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Perlin", Element, 0);
  if (Button(Ui, CSz("Noise(Perlin)"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Perlin;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_Voronoi", Element, 0);
  if (Button(Ui, CSz("Noise(Voronoi)"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_Voronoi;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_White", Element, 0);
  if (Button(Ui, CSz("Noise(White)"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_White;
  }
  PushNewRow(Ui);
}
{
  PushColumn(Ui, CSz("|"));
  ui_id ButtonId = UiId(Window, "brush type select NoiseType_RectLattice", Element, 0);
  if (Button(Ui, CSz("Noise(RectLattice)"), ButtonId))
  {
    Element->Type = BrushLayerType_Noise;
    Element->Noise.Type = NoiseType_RectLattice;
  }
  PushNewRow(Ui);
}


