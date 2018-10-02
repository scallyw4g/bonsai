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

v3_static_array
ParseV3Array(umm ElementCount, ansi_stream FloatStream, memory_arena* Memory)
{
  v3_static_array Result = V3_Static_Array(ElementCount, Memory);

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
}

r32_static_array
ParseFloatArray(umm TotalFloatCount, ansi_stream FloatStream, memory_arena* Memory)
{
  r32_static_array Result = R32_Static_Array(TotalFloatCount, Memory);

  for (umm DestIndex = 0;
      DestIndex < TotalFloatCount;
      ++DestIndex)
  {
    counted_string Float = PopWordCounted(&FloatStream);
    Result.Start[DestIndex] = StringToFloat(&Float);
  }
}


void
Dump(v3_static_array* Array)
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

  counted_string PositionsSelector   = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh source#Cube-mesh-positions float_array#Cube-mesh-positions-array");
  counted_string NormalsSelector     = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh source#Cube-mesh-normals float_array#Cube-mesh-normals-array");
  counted_string VertexCountSelector = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh polylist vcount");
  counted_string VertIndicesSelector = CS("?xml COLLADA library_geometries geometry#Cube-mesh mesh polylist p");

  counted_string PositionString   = GetFirstMatchingTag(&XmlTokens, &PositionsSelector, Memory)->Value;
  counted_string NormalString     = GetFirstMatchingTag(&XmlTokens, &NormalsSelector, Memory)->Value;
  ansi_stream Triangles  = AnsiStream(GetFirstMatchingTag(&XmlTokens, &VertexCountSelector, Memory)->Value);
  ansi_stream VertIndices = AnsiStream(GetFirstMatchingTag(&XmlTokens, &VertIndicesSelector, Memory)->Value);

  u32 TotalTriangleCount = 0;

  // TODO(Jesse): This is complete garbage and should be gotten from XML
  {
    counted_string CurrentTriangle = PopWordCounted(&Triangles);
    while (CurrentTriangle.Count)
    {
      ++TotalTriangleCount;
      CurrentTriangle = PopWordCounted(&Triangles);
    }
    Triangles.At = Triangles.Start;
  }


  umm TotalVertexCount = 3*TotalTriangleCount;
  v3_static_array Positions = ParseV3Array(TotalVertexCount, AnsiStream(PositionString), Memory);
  v3_static_array Normals = ParseV3Array(TotalVertexCount, AnsiStream(NormalString), Memory);

  Assert(TotalElements(&Positions) == TotalVertexCount);
  Assert(TotalElements(&Normals) == TotalVertexCount);

  Dump(&Positions);


  untextured_3d_geometry_buffer Buffer = {};
  AllocateMesh(&Buffer, TotalVertexCount, Memory);

  counted_string CurrentTriangle = PopWordCounted(&Triangles);
  umm NormalIndex = 0;
  while (CurrentTriangle.Count)
  {
    Assert(StringsMatch(CurrentTriangle, CS("3")));

    for (u32 VertIndex = 0;
        VertIndex < 3;
        ++VertIndex)
    {
      Assert(Buffer.At < Buffer.End);

      u32 CurrentIndex = (u32)StringToInt(PopWordCounted(&VertIndices));
      Assert(CurrentIndex < TotalElements(&Positions));

      Buffer.Verts[Buffer.At] = Positions.Start[CurrentIndex];
      Buffer.Normals[Buffer.At] = Normals.Start[NormalIndex++];
      Buffer.At++;
    }


    CurrentTriangle = PopWordCounted(&Triangles);
  }

  model Result = {};
  return Result;
}
