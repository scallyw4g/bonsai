animation
AllocateAnimation(v3i KeyframeCount, memory_arena* Memory)
{
  animation Result = {};

  Result.xKeyframeCount = (u32)KeyframeCount.x;
  Result.xKeyframes = Allocate(keyframe, Memory, KeyframeCount.x);

  Result.yKeyframeCount = (u32)KeyframeCount.y;
  Result.yKeyframes = Allocate(keyframe, Memory, KeyframeCount.y);

  Result.zKeyframeCount = (u32)KeyframeCount.z;
  Result.zKeyframes = Allocate(keyframe, Memory, KeyframeCount.z);

  return Result;
}

loaded_collada_mesh
LoadMeshData(xml_token_stream* XmlTokens, counted_string* GeometryId, memory_arena* TempMemory, heap_allocator* Heap)
{
  loaded_collada_mesh Result = {};


  counted_string PositionsSelector = FormatCountedString(TempMemory, CSz("geometry%.*s float_array%.*s-positions-array"), GeometryId->Count, GeometryId->Start, GeometryId->Count, GeometryId->Start);
  xml_tag* PositionTag             = GetFirstMatchingTag(XmlTokens, &PositionsSelector);
  counted_string PositionString    = PositionTag->Value;
  u32 PositionCount                = ToU32(GetPropertyValue(PositionTag, CS("count")));
  v3_cursor Positions              = ParseV3Array(PositionCount, AnsiStream(PositionString), TempMemory);

  counted_string NormalsSelector = FormatCountedString(TempMemory, CSz("geometry%.*s float_array%.*s-normals-array"), GeometryId->Count, GeometryId->Start, GeometryId->Count, GeometryId->Start);
  xml_tag* NormalTag             = GetFirstMatchingTag(XmlTokens, &NormalsSelector);
  counted_string NormalString    = NormalTag->Value;
  u32 NormalCount                = ToU32(GetPropertyValue(NormalTag, CS("count")));
  v3_cursor Normals              = ParseV3Array(NormalCount, AnsiStream(NormalString), TempMemory);

  counted_string IndecesSelector = FormatCountedString(TempMemory, CSz("geometry%.*s polylist p"), GeometryId->Count, GeometryId->Start);
  ansi_stream Indices            = AnsiStream(GetFirstMatchingTag(XmlTokens, &IndecesSelector)->Value);


  counted_string PolylistSelector = FormatCountedString(TempMemory, CSz("geometry%.*s polylist"), GeometryId->Count, GeometryId->Start);
  xml_tag* Polylist               = GetFirstMatchingTag(XmlTokens, &PolylistSelector);

  if (Polylist)
  {
    u32 TotalTriangleCount = ToU32(GetPropertyValue(Polylist, CS("count")));

    counted_string VertexCountSelector = FormatCountedString(TempMemory, CSz("geometry%.*s polylist vcount"), GeometryId->Count, GeometryId->Start);
    ansi_stream Triangles              = AnsiStream(GetFirstMatchingTag(XmlTokens, &VertexCountSelector)->Value);

    untextured_3d_geometry_buffer Mesh = {};
    AllocateMesh(&Mesh, TotalTriangleCount*3, Heap);

    v3 MaxP = V3(f32_MIN);
    v3 MinP = V3(f32_MAX);

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
        Assert(Mesh.At < Mesh.End);

        u32 PositionIndex = ToU32(PopWordCounted(&Indices));
        u32 NormalIndex = ToU32(PopWordCounted(&Indices));
        Assert(PositionIndex < TotalElements(&Positions));
        Assert(NormalIndex < TotalElements(&Normals));

        v3 P = Positions.Start[PositionIndex];
        Mesh.Verts[Mesh.At] = P;
        MaxP = Max(P, MaxP);
        MinP = Min(P, MinP);

        Mesh.Normals[Mesh.At] = Normalize(Normals.Start[NormalIndex]);

        Mesh.At++;
      }
    }

    Result.Mesh = Mesh;
    Result.Dim = MaxP - MinP;
  }
  else
  {
#if 1
    NotImplemented;
#else
    counted_string TrianglesSelector = FormatCountedString(TempMemory, CSz("geometry%S triangles"), GeometryId);
    xml_tag* TrianglesTag = GetFirstMatchingTag(XmlTokens, &TrianglesSelector);

    for (u32 TriangleIndex = 0;
        TriangleIndex < TotalTriangleCount;
        ++TriangleIndex)
    {
      for (u32 VertIndex = 0;
          VertIndex < 3;
          ++VertIndex)
      {
        Assert(Mesh.At < Mesh.End);

        u32 PositionIndex = ToU32(PopWordCounted(&Indices));
        u32 NormalIndex = ToU32(PopWordCounted(&Indices));
        Assert(PositionIndex < TotalElements(&Positions));
        Assert(NormalIndex < TotalElements(&Normals));

        v3 P = Positions.Start[PositionIndex];
        Mesh.Verts[Mesh.At] = P;
        MaxP = Max(P, MaxP);
        MinP = Min(P, MinP);

        Mesh.Normals[Mesh.At] = Normalize(Normals.Start[NormalIndex]);

        Mesh.At++;
      }
    }
#endif

  }

  Assert(Result.Mesh.At == Result.Mesh.End);
  return Result;
}

xml_tag*
ParseKeyframesForAxis(xml_token_stream* XmlTokens, char Axis, xml_tag** TimeTag, counted_string* GeometryName)
{
  memory_arena *TranArena = GetTranArena();
  counted_string xChannelSelector = FormatCountedString(TranArena, CSz("library_animations channel:target=%.*s/location.%c"), GeometryName->Count, GeometryName->Start, Axis);
  xml_tag* xChannel = GetFirstMatchingTag(XmlTokens, &xChannelSelector);
  counted_string* xChannelId = GetPropertyValue(xChannel, CS("source"));

  counted_string xInputSelector = FormatCountedString(TranArena, CSz("library_animations sampler%.*s input:semantic=INPUT"), xChannelId->Count, xChannelId->Start);
  xml_tag* xInputSourceTag = GetFirstMatchingTag(XmlTokens, &xInputSelector);
  counted_string* xInputLocationSourceId = GetPropertyValue(xInputSourceTag, CS("source"));

  counted_string xInputLocationSelector = FormatCountedString(TranArena, CSz("library_animations animation source%.*s float_array"), xInputLocationSourceId->Count, xInputLocationSourceId->Start);
  xml_tag* xKeyframeTimeTag = GetFirstMatchingTag(XmlTokens, &xInputLocationSelector);



  counted_string xOutputSelector = FormatCountedString(TranArena, CSz("library_animations sampler%.*s input:semantic=OUTPUT"), xChannelId->Count, xChannelId->Start);
  xml_tag* xOutputSourceTag = GetFirstMatchingTag(XmlTokens, &xOutputSelector);
  counted_string* xOutputLocationSourceId = GetPropertyValue(xOutputSourceTag, CS("source"));

  counted_string xOutputLocationSelector = FormatCountedString(TranArena, CSz("library_animations animation source%.*s float_array"), xOutputLocationSourceId->Count, xOutputLocationSourceId->Start);
  xml_tag* xKeyframePositionsTag = GetFirstMatchingTag(XmlTokens, &xOutputLocationSelector);


  *TimeTag = xKeyframeTimeTag;
  return xKeyframePositionsTag;
}

r32
CopyKeyframeData(keyframe* DestKeyframes, r32_stream* KeyframePositions, r32_stream*  KeyframeTimes)
{
  u32 KeyframeCount = (u32)TotalElements(KeyframePositions);
  Assert(KeyframeCount == (u32)TotalElements(KeyframeTimes));

  r32 MaxKeyframeTime = 0.0f;
  for (u32 KeyframeIndex = 0;
      KeyframeIndex < KeyframeCount;
      ++KeyframeIndex)
  {
    DestKeyframes[KeyframeIndex].Value = KeyframePositions->Start[KeyframeIndex];
    DestKeyframes[KeyframeIndex].tEnd = KeyframeTimes->Start[KeyframeIndex];
    MaxKeyframeTime = Max(MaxKeyframeTime, KeyframeTimes->Start[KeyframeIndex]);
  }

  return MaxKeyframeTime;
}

model
LoadCollada(memory_arena *Memory, heap_allocator *Heap, const char * FilePath)
{
  Info("Loading .dae file : %s", FilePath);

  ansi_stream AnsiXml = AnsiStreamFromFile(FilePath, Memory);
  if (!AnsiXml.Start) { model M = {}; return M; }

  xml_token_stream XmlTokens = TokenizeXmlStream(&AnsiXml, Memory);
  model Result = {};

  if (XmlTokens.Start)
  {
    counted_string VisualSceneElementSelector = CS("library_visual_scenes node:type=NODE instance_geometry");
    xml_tag_stream SceneElements = GetAllMatchingTags(&XmlTokens, &VisualSceneElementSelector, Memory);

    u32 SceneObjects = (u32)TotalElements(&SceneElements);

    // NOTE(Jesse): At the moment we only support loading one meshed object per
    // .dae file but in the future we could support more!
    Assert(SceneObjects <= 1);

    for ( u32 ObjectIndex = 0;
         ObjectIndex < SceneObjects;
         ++ObjectIndex )
    {
      xml_tag* Tag = SceneElements.Start[ObjectIndex];
      counted_string* GeometryName = GetPropertyValue(Tag, CS("name"));
      counted_string* GeometryId = GetPropertyValue(Tag, CS("url"));

      Assert(GeometryName && GeometryId);

      loaded_collada_mesh ColladaMesh = LoadMeshData(&XmlTokens, GeometryId, GetTranArena(), Heap);
      Result.Mesh = ColladaMesh.Mesh;
      Result.Dim = Voxel_Position(ColladaMesh.Dim);

      xml_tag* xKeyframeTimeTag = 0;
      xml_tag* yKeyframeTimeTag = 0;
      xml_tag* zKeyframeTimeTag = 0;

      xml_tag* xKeyframePositionsTag = ParseKeyframesForAxis(&XmlTokens, 'X', &xKeyframeTimeTag, GeometryName);
      xml_tag* yKeyframePositionsTag = ParseKeyframesForAxis(&XmlTokens, 'Y', &yKeyframeTimeTag, GeometryName);
      xml_tag* zKeyframePositionsTag = ParseKeyframesForAxis(&XmlTokens, 'Z', &zKeyframeTimeTag, GeometryName);

      u32 xKeyframeCount = ToU32(GetPropertyValue(xKeyframeTimeTag, CS("count")));
      u32 yKeyframeCount = ToU32(GetPropertyValue(yKeyframeTimeTag, CS("count")));
      u32 zKeyframeCount = ToU32(GetPropertyValue(zKeyframeTimeTag, CS("count")));

      Assert( xKeyframeCount == ToU32(GetPropertyValue(xKeyframePositionsTag, CS("count"))) );
      Assert( yKeyframeCount == ToU32(GetPropertyValue(yKeyframePositionsTag, CS("count"))) );
      Assert( zKeyframeCount == ToU32(GetPropertyValue(zKeyframePositionsTag, CS("count"))) );

      r32_stream xKeyframeTimes = ParseFloatArray(xKeyframeCount, AnsiStream(xKeyframeTimeTag->Value), Memory);
      r32_stream yKeyframeTimes = ParseFloatArray(yKeyframeCount, AnsiStream(yKeyframeTimeTag->Value), Memory);
      r32_stream zKeyframeTimes = ParseFloatArray(zKeyframeCount, AnsiStream(zKeyframeTimeTag->Value), Memory);

      r32_stream xKeyframePositions = ParseFloatArray(xKeyframeCount, AnsiStream(xKeyframePositionsTag->Value), Memory);
      r32_stream yKeyframePositions = ParseFloatArray(yKeyframeCount, AnsiStream(yKeyframePositionsTag->Value), Memory);
      r32_stream zKeyframePositions = ParseFloatArray(zKeyframeCount, AnsiStream(zKeyframePositionsTag->Value), Memory);

      animation Animation = AllocateAnimation(V3i((s32)xKeyframeCount, (s32)yKeyframeCount, (s32)zKeyframeCount), Memory);


      r32 xMaxKeyframeTime = CopyKeyframeData(Animation.xKeyframes, &xKeyframePositions, &xKeyframeTimes);
      r32 yMaxKeyframeTime = CopyKeyframeData(Animation.yKeyframes, &yKeyframePositions, &yKeyframeTimes);
      r32 zMaxKeyframeTime = CopyKeyframeData(Animation.zKeyframes, &zKeyframePositions, &zKeyframeTimes);


      Animation.tEnd = Max(Max(xMaxKeyframeTime, yMaxKeyframeTime), zMaxKeyframeTime);
      Result.Animation = Animation;
    }
  }

  return Result;
}
