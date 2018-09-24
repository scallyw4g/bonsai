struct xml_tag
{
  counted_string Selector;
  umm TagStartOffset; // Relative to start of stream
  // umm TagEndOffset;   // Relative to start of stream
};

xml_tag
GetFirstMatchingTag(ansi_stream *Stream, ansi_stream* Selector)
{
  xml_tag Result = {};
  /* RuntimeBreak(); */

  counted_string SelectorTag = PopWordCounted(Selector);
  while (Remaining(Stream))
  {
    counted_string TagName = PopXmlTag(Stream);

    if (StringsMatch(&SelectorTag, &TagName))
    {
      Result.Selector = TagName;
      Result.TagStartOffset = TagName.Start - Stream->Start;

      while (Remaining(Stream))
      {
        counted_string EndTag = PopWordCounted(Stream);
        if (EndTag.Start[EndTag.Count-1] == '>')
        {
          /* Result.TagEndOffset = ?? ; */
          goto finished;
        }
      }
    }
  }

  finished:

  return Result;
}

mesh_metadata
GetColladaMetadata(ansi_stream *Stream, memory_arena* PermMemory)
{
  ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry source#Cube-mesh-positions float_array#Cube-mesh-positions-array");
  xml_tag PositionsTag = GetFirstMatchingTag(Stream, &SelectorStream);
  /* v3* Verts = ParseV3Array(PositionsTag, PermMemory); */

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
