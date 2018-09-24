struct xml_tag
{
  counted_string Selector;

  umm OpeningStartOffset; // Relative to start of stream
  // umm OpeningEndOffset;   // Relative to start of stream

  umm ClosingStartOffset; // Relative to start of stream
  umm ClosingEndOffset;   // Relative to start of stream
};


xml_tag
AdvanceStreamToEndOfOpeningTag(ansi_stream *XmlStream, counted_string XmlStreamTag)
{
  xml_tag Result = {};

  Result.Selector = XmlStreamTag;
  Result.OpeningStartOffset = XmlStreamTag.Start - XmlStream->Start;

  while (Remaining(XmlStream))
  {
    counted_string EndTag = PopWordCounted(XmlStream);
    if (EndTag.Start[EndTag.Count-1] == '>')
    {
      // Result.OpeningEndOffset = (EndTag.Start - XmlStream->Start) - Result.OpeningStartOffset + EndTag.Count;
      break;
    }
  }

  return Result;
}

xml_tag
GetFirstMatchingTag(ansi_stream *XmlStream, ansi_stream* SelectorStream, counted_string *ParentSelector)
{
  xml_tag Result = {};
  /* RuntimeBreak(); */

  counted_string SelectorTag = PopWordCounted(SelectorStream);
  while (Remaining(XmlStream))
  {
    counted_string XmlStreamTag = PopXmlTag(XmlStream);

    if (StringsMatch(&SelectorTag, &XmlStreamTag))
    {
      Result = AdvanceStreamToEndOfOpeningTag(XmlStream, XmlStreamTag);
      GetFirstMatchingTag(XmlStream, SelectorStream, &SelectorTag);
    }
  }

  return Result;
}

mesh_metadata
GetColladaMetadata(ansi_stream *Stream, memory_arena* PermMemory)
{
  // ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry source#Cube-mesh-positions float_array#Cube-mesh-positions-array");

  ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry");
  xml_tag PositionsTag = GetFirstMatchingTag(Stream, &SelectorStream, 0);
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
