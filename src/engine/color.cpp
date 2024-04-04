
link_internal v3_cursor *
GetColorPalette()
{
  v3_cursor *Palette = &GetEngineResources()->Graphics->ColorPalette;
  return Palette;
}

link_internal v3
GetPaletteData(u32 ColorIndex)
{
  v3_cursor *Palette = GetColorPalette();
  v3 Result = GetPaletteData(Palette, ColorIndex);
  return Result;
}

link_internal v3
GetColorData(u32 ColorIndex)
{
  v3_cursor *Palette = GetColorPalette();
  v3 Result = GetColorData(Palette, ColorIndex);
  return Result;
}

