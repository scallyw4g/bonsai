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

  return Result;
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

void
Dump(hashtable<xml_tag*> * Hash)
{
  for (u32 ElementIndex = 0;
      ElementIndex < Hash->Size;
      ++ElementIndex)
  {
    xml_tag* Element = Hash->Table[ElementIndex];
    if (Element)
    {
      Log("");
      Print(ElementIndex);
      Print(Element);
      while (Element = Element->NextInHash)
      {
        Print(Element);
      }
    }
  }

  return;
}

void
Dump(xml_token_stream *Stream, umm TokenCount)
{
  for (u32 TokenIndex = 0;
      TokenIndex < TokenCount;
      ++TokenIndex)
  {
    Print(Stream->Start + TokenIndex);
  }
}

animation
AllocateAnimation(u32 KeyframeCount, memory_arena* Memory)
{
  animation Result = {};

  Result.KeyframeCount = KeyframeCount;
  Result.Keyframes = Allocate(keyframe, Memory, KeyframeCount);

  return Result;
}

chunk_data*
LoadMeshData(xml_token_stream* XmlTokens, counted_string* GeometryId, memory_arena* PermMemory, memory_arena* TempMemory)
{
  counted_string PositionsSelector   = FormatCountedString(TempMemory, "geometry%.*s float_array%.*s-positions-array", GeometryId->Count, GeometryId->Start, GeometryId->Count, GeometryId->Start);
  counted_string NormalsSelector     = FormatCountedString(TempMemory, "geometry%.*s float_array%.*s-normals-array", GeometryId->Count, GeometryId->Start, GeometryId->Count, GeometryId->Start);
  counted_string VertexCountSelector = FormatCountedString(TempMemory, "geometry%.*s polylist vcount", GeometryId->Count, GeometryId->Start);
  counted_string VertIndicesSelector = FormatCountedString(TempMemory, "geometry%.*s polylist p", GeometryId->Count, GeometryId->Start);
  counted_string PolylistSelector    = FormatCountedString(TempMemory, "geometry%.*s polylist", GeometryId->Count, GeometryId->Start);

  ansi_stream Triangles         = AnsiStream(GetFirstMatchingTag(XmlTokens, &VertexCountSelector)->Value);
  ansi_stream VertIndices       = AnsiStream(GetFirstMatchingTag(XmlTokens, &VertIndicesSelector)->Value);

  xml_tag* Polylist             = GetFirstMatchingTag(XmlTokens, &PolylistSelector);
  u32 TotalTriangleCount        = StringToInt(GetPropertyValue(Polylist, CS("count")));

  xml_tag* PositionTag          = GetFirstMatchingTag(XmlTokens, &PositionsSelector);
  xml_tag* NormalTag            = GetFirstMatchingTag(XmlTokens, &NormalsSelector);
  counted_string PositionString = PositionTag->Value;
  counted_string NormalString   = NormalTag->Value;
  u32 PositionCount             = StringToInt(GetPropertyValue(PositionTag, CS("count")));
  u32 NormalCount               = StringToInt(GetPropertyValue(NormalTag, CS("count")));

  v3_stream Positions           = ParseV3Array(PositionCount, AnsiStream(PositionString), TempMemory);
  v3_stream Normals             = ParseV3Array(NormalCount, AnsiStream(NormalString), TempMemory);

  chunk_data* Result = AllocateChunk(PermMemory, Chunk_Dimension(0,0,0));

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
      Assert(Result->Mesh.At < Result->Mesh.End);

      u32 PositionIndex = (u32)StringToInt(PopWordCounted(&VertIndices));
      u32 NormalIndex = (u32)StringToInt(PopWordCounted(&VertIndices));
      Assert(PositionIndex < TotalElements(&Positions));
      Assert(NormalIndex < TotalElements(&Normals));

      Result->Mesh.Verts[Result->Mesh.At] = Positions.Start[PositionIndex];
      Result->Mesh.Normals[Result->Mesh.At] = Normalize(Normals.Start[NormalIndex]);

      Result->Mesh.At++;
    }
  }

  return Result;
}

model
LoadCollada(memory_arena *Memory, const char * FilePath)
{
  Info("Loading .dae file : %s \n", FilePath);

  ansi_stream AnsiXml = AnsiStreamFromFile(FilePath, Memory);
  if (!AnsiXml.Start) { model M = {}; return M; }

  xml_token_stream XmlTokens = TokenizeXmlStream(&AnsiXml, Memory);
  model Result = {};

  counted_string VisualSceneElementSelector = CS("library_visual_scenes node:type=NODE instance_geometry");
  xml_tag_stream SceneElements = GetAllMatchingTags(&XmlTokens, &VisualSceneElementSelector, Memory);

  u32 SceneObjects = TotalElements(&SceneElements);
  for ( u32 ObjectIndex = 0;
       ObjectIndex < SceneObjects;
       ++ObjectIndex )
  {
    xml_tag* Tag = SceneElements.Start[ObjectIndex];
    counted_string* GeometryName = GetPropertyValue(Tag, CS("name"));
    counted_string* GeometryId = GetPropertyValue(Tag, CS("url"));

    Assert(GeometryName && GeometryId);

    Result.Chunk = LoadMeshData(&XmlTokens, GeometryId, Memory, TranArena);
  }

  { // Load animation data

    counted_string xKeyframeTimesSelector     = CS("library_animations animation:id=Cube_location_X source:id=Cube_location_X-input float_array:id=Cube_location_X-input-array");
    counted_string xKeyframePositionsSelector = CS("library_animations animation:id=Cube_location_X source:id=Cube_location_X-output float_array:id=Cube_location_X-output-array");
    counted_string yKeyframePositionsSelector = CS("library_animations animation:id=Cube_location_Y source:id=Cube_location_Y-output float_array:id=Cube_location_Y-output-array");
    counted_string zKeyframePositionsSelector = CS("library_animations animation:id=Cube_location_Z source:id=Cube_location_Z-output float_array:id=Cube_location_Z-output-array");

    xml_tag* xKeyframeTimeTag = GetFirstMatchingTag(&XmlTokens, &xKeyframeTimesSelector);
    xml_tag* xKeyframePositionsTag = GetFirstMatchingTag(&XmlTokens, &xKeyframePositionsSelector);
    xml_tag* yKeyframePositionsTag = GetFirstMatchingTag(&XmlTokens, &yKeyframePositionsSelector);
    xml_tag* zKeyframePositionsTag = GetFirstMatchingTag(&XmlTokens, &zKeyframePositionsSelector);

    if (xKeyframeTimeTag)
    {
      u32 TotalKeyframeCount = StringToInt(GetPropertyValue(xKeyframeTimeTag, CS("count")));
      Assert( TotalKeyframeCount == StringToInt(GetPropertyValue(xKeyframePositionsTag, CS("count"))) );

      r32_stream xKeyframeTimes = ParseFloatArray(TotalKeyframeCount, AnsiStream(xKeyframeTimeTag->Value), Memory);
      r32_stream xKeyframePositions = ParseFloatArray(TotalKeyframeCount, AnsiStream(xKeyframePositionsTag->Value), Memory);
      r32_stream yKeyframePositions = ParseFloatArray(TotalKeyframeCount, AnsiStream(yKeyframePositionsTag->Value), Memory);
      r32_stream zKeyframePositions = ParseFloatArray(TotalKeyframeCount, AnsiStream(zKeyframePositionsTag->Value), Memory);

      animation Animation = AllocateAnimation(TotalKeyframeCount, Memory);

      for (u32 KeyframeIndex = 0;
          KeyframeIndex < TotalKeyframeCount;
          ++KeyframeIndex)
      {
        Animation.Keyframes[KeyframeIndex].PositionInterp.x = xKeyframePositions.Start[KeyframeIndex];
        Animation.Keyframes[KeyframeIndex].PositionInterp.y = yKeyframePositions.Start[KeyframeIndex];
        Animation.Keyframes[KeyframeIndex].PositionInterp.z = zKeyframePositions.Start[KeyframeIndex];
        Animation.Keyframes[KeyframeIndex].tEnd = xKeyframeTimes.Start[KeyframeIndex];
      }

      Animation.tEnd = xKeyframeTimes.Start[TotalKeyframeCount-1];

      Result.Animation = Animation;
    }
  }

  Result.Chunk->Flags = Chunk_Initialized;
  return Result;
}
