struct xml_tag
{
  const char* Selector;
  umm StartOffset;
  umm EndOffset;
};

v3*
ParseV3Array(xml_tag Tag, memory_arena* Memory)
{
  return 0;
}

xml_tag
GetFirstMatchingTag(ansi_stream *Stream, ansi_stream* Selector)
{
  xml_tag Result = {};
  /* RuntimeBreak(); */

  const char* SelectorTag = PopWord(Selector, TranArena);
  while (Remaining(Stream))
  {
    const char* TagName = PopXmlTag(Stream, TranArena);

    if (StringsMatch(SelectorTag, TagName))
    {
      Result.Selector = TagName;
      break;
    }
  }

  return Result;
}

mesh_metadata
GetColladaMetadata(ansi_stream *Stream, memory_arena* PermMemory)
{
  ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry source#Cube-mesh-positions float_array#Cube-mesh-positions-array");
  xml_tag PositionsTag = GetFirstMatchingTag(Stream, &SelectorStream);
  v3* Verts = ParseV3Array(PositionsTag, PermMemory);

  mesh_metadata Result = {};
  return Result;
}

model
LoadCollada(memory_arena *PermMem, const char * FilePath)
{
  Info("Loading .dae file : %s \n", FilePath);

  umm Length = 0;
  binary_stream_u8 BinaryStream = BinaryStreamFromFile(FilePath, PermMem);
  if (!BinaryStream.Start) { model M = {}; return M; }

  ansi_stream Stream = AnsiStream(&BinaryStream);
  mesh_metadata Stats = GetColladaMetadata(&Stream, PermMem);


  model Result = {};
  return Result;
}
