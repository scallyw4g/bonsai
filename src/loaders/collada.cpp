#include <string.cpp>
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
GetFirstMatchingTag(ansi_stream *XmlStream, ansi_stream* SelectorStream, counted_string *ParentSelectorName)
{
  xml_tag Result = {};

  counted_string SearchSelector = PopWordCounted(SelectorStream);

  while (Remaining(XmlStream))
  {
    counted_string MatchTagName = PopXmlTag(XmlStream);

    if (StringsMatch(&SearchSelector, &MatchTagName))
    {
      // NOTE(Jesse): This omits the opening '<' in favor of not having to add
      // it every time we pop a selector off the selector stream
      char* ParentClosingTag = FormatString("/%.*s>", SearchSelector.Count, SearchSelector.Start);
      counted_string Parent = CountedString(ParentClosingTag);

      xml_tag ChildTag = GetFirstMatchingTag(XmlStream, SelectorStream, &Parent);
      if (ChildTag.Selector.Count)
      {
        Result = ChildTag;
      }
    }

    if (StringsMatch(ParentSelectorName, &MatchTagName))
    {
      break;
    }
  }

  return Result;
}

mesh_metadata
GetColladaMetadata(ansi_stream *Stream, memory_arena* PermMemory)
{
  // ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry source#Cube-mesh-positions float_array#Cube-mesh-positions-array");

  ansi_stream SelectorStream = AnsiStream("COLLADA library_geometries geometry");

  counted_string ZeroString = {};
  xml_tag PositionsTag = GetFirstMatchingTag(Stream, &SelectorStream, &ZeroString);
  /* v3* Verts = ParseV3Array(PositionsTag, PermMemory); */

  mesh_metadata Result = {};
  return Result;
}

model
LoadCollada(memory_arena *PermMem, const char * FilePath)
{
  Info("Loading .dae file : %s \n", FilePath);

  binary_stream_u8 BinaryStream = BinaryStreamFromFile(FilePath, PermMem);
  if (!BinaryStream.Start) { model M = {}; return M; }

  ansi_stream Stream = AnsiStream(&BinaryStream);
  mesh_metadata Stats = GetColladaMetadata(&Stream, PermMem);


  model Result = {};
  return Result;
}
