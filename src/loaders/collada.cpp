#include <xml.cpp>

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
