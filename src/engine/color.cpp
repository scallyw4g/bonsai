
link_internal v3_cursor *
GetColorPalette()
{
  v3_cursor *Palette = &GetEngineResources()->World->ColorPalette;
  return Palette;
}

link_internal v3
GetColorData(u32 ColorIndex)
{
  v3_cursor *Palette = &GetEngineResources()->World->ColorPalette;
  Assert(ColorIndex < AtElements(Palette));
  v3 Result = Palette->Start[ColorIndex] / 255.f;
  return Result;
}

