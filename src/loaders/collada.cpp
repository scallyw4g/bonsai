#include <xml.cpp>

r32
StringToFloat(counted_string* String)
{
  const char* Temp = FormatString(TranArena, "%s", String->Start);

  r32 Result = atof(Temp);
  return Result;
}

s32
StringToInt(counted_string String)
{
  const char* Temp = FormatString(TranArena, "%s", String.Start);

  s32 Result = atoi(Temp);
  return Result;
}

s32
StringToInt(counted_string* String)
{
  s32 Result = StringToInt(*String);
  return Result;
}

v3_stream
ParseV3Array(umm ElementCount, ansi_stream FloatStream, memory_arena* Memory)
{
  v3_stream Result = V3_Stream(ElementCount, Memory);

  for (umm DestIndex = 0;
      DestIndex < ElementCount;
      ++DestIndex)
  {
    for (u32 Inner = 0;
         Inner < 3;
        ++Inner)
    {
      counted_string Float = PopWordCounted(&FloatStream);
      Result.Start[DestIndex].E[Inner] = StringToFloat(&Float);
    }
    ++Result.At;
  }

  return Result;
}

r32_stream
ParseFloatArray(umm TotalFloatCount, ansi_stream FloatStream, memory_arena* Memory)
{
  r32_stream Result = R32_Stream(TotalFloatCount, Memory);

  for (umm DestIndex = 0;
      DestIndex < TotalFloatCount;
      ++DestIndex)
  {
    counted_string Float = PopWordCounted(&FloatStream);
    Result.Start[DestIndex] = StringToFloat(&Float);
  }
}

void
Dump(v3_stream* Array)
{
  umm ElementCount = AtElements(Array);

  for (umm ElementIndex = 0;
      ElementIndex < ElementCount;
      ++ElementIndex)
  {
    Print(Array->Start[ElementIndex]);
  }

  return;
}

model
LoadCollada(memory_arena *Memory, const char * FilePath)
{
  Info("Loading .dae file : %s \n", FilePath);

  ansi_stream AnsiXml = AnsiStreamFromFile(FilePath, Memory);
  if (!AnsiXml.Start) { model M = {}; return M; }

  xml_token_stream XmlTokens = TokenizeXmlStream(&AnsiXml, Memory);
  model Result = {};

  { // Load vertex/normal data
    counted_string PositionsSelector   = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh source#Cube-mesh-positions float_array#Cube-mesh-positions-array");
    counted_string NormalsSelector     = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh source#Cube-mesh-normals float_array#Cube-mesh-normals-array");
    counted_string VertexCountSelector = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh polylist vcount");
    counted_string VertIndicesSelector = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh polylist p");
    counted_string PolylistSelector    = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh polylist");


    ansi_stream Triangles         = AnsiStream(GetFirstMatchingTag(&XmlTokens, &VertexCountSelector, Memory)->Value);
    ansi_stream VertIndices       = AnsiStream(GetFirstMatchingTag(&XmlTokens, &VertIndicesSelector, Memory)->Value);

    xml_tag* Polylist             = GetFirstMatchingTag(&XmlTokens, &PolylistSelector, Memory);
    u32 TotalTriangleCount        = StringToInt(GetPropertyValue(Polylist, CS("count")));

    xml_tag* PositionTag          = GetFirstMatchingTag(&XmlTokens, &PositionsSelector, Memory);
    xml_tag* NormalTag            = GetFirstMatchingTag(&XmlTokens, &NormalsSelector, Memory);
    counted_string PositionString = PositionTag->Value;
    counted_string NormalString   = NormalTag->Value;
    u32 PositionCount             = StringToInt(GetPropertyValue(PositionTag, CS("count")));
    u32 NormalCount               = StringToInt(GetPropertyValue(NormalTag, CS("count")));
    v3_stream Positions     = ParseV3Array(PositionCount, AnsiStream(PositionString), Memory);
    v3_stream Normals       = ParseV3Array(NormalCount, AnsiStream(NormalString), Memory);

    Result.Chunk = AllocateChunk(Memory, Chunk_Dimension(0,0,0));

    for (u32 TriangleIndex = 0;
        TriangleIndex < TotalTriangleCount;
        ++TriangleIndex)
    {
      counted_string CurrentTriangle = PopWordCounted(&Triangles);
      Assert(StringsMatch(CurrentTriangle, CS("3")));

      for (u32 VertIndex = 0;
          VertIndex < 3;
          ++VertIndex)
      {
        Assert(Result.Chunk->Mesh.At < Result.Chunk->Mesh.End);

        u32 PositionIndex = (u32)StringToInt(PopWordCounted(&VertIndices));
        u32 NormalIndex = (u32)StringToInt(PopWordCounted(&VertIndices));
        Assert(PositionIndex < TotalElements(&Positions));
        Assert(NormalIndex < TotalElements(&Normals));

        Result.Chunk->Mesh.Verts[Result.Chunk->Mesh.At] = Positions.Start[PositionIndex];
        Result.Chunk->Mesh.Normals[Result.Chunk->Mesh.At] = Normalize(Normals.Start[NormalIndex]);

        Result.Chunk->Mesh.At++;
      }
    }

  }

  Result.Chunk->Flags = Chunk_Initialized;
  return Result;
}
