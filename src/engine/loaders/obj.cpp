struct mesh_metadata
{
  u32 VertCount;
  u32 NormalCount;
  u32 UVCount;
  u32 FaceCount;
};

mesh_metadata
GetObjMetadata(ansi_stream Cursor, memory_arena *Memory)
{
  mesh_metadata Result = {};

  while (Cursor.At < Cursor.End)
  {
    char *LineType = PopWord(&Cursor, Memory);
    if (LineType == 0) { break; }

    if ( StringsMatch(LineType, "v"))
    {
      ++Result.VertCount;
    }
    else if ( StringsMatch(LineType, "vt") )
    {
      ++Result.UVCount;
    }
    else if ( StringsMatch(LineType, "vn") )
    {
      ++Result.NormalCount;
    }
    else if ( StringsMatch(LineType, "f") )
    {
      ++Result.FaceCount;
    }
    else
    {
      // Irrelevant.
    }
  }

  return Result;
}

/*
 * This loader doesn't support ngon faces.  The mesh must be triangulated
 * before exporting from blender.
 *
 * Note that triangulating a mesh with bent normals at export time (in blender)
 * seems to not re-bend the triangulated normals and everything looks borked.
 */
link_internal model
LoadObj(memory_arena *PermMem, heap_allocator *Heap, const char * FilePath)
{
  Info("Loading .obj file : %s \n", FilePath);

  u8_stream BinaryStream = U8_StreamFromFile(FilePath, PermMem);
  if (!BinaryStream.Start) { model M = {}; return M; }

  memory_arena *TranArena = GetTranArena();
  ansi_stream Stream = AnsiStream(&BinaryStream);
  mesh_metadata Stats = GetObjMetadata(Stream, TranArena);

  v3_cursor TempVerts       = V3Cursor(Stats.VertCount, TranArena);
  v3_cursor TempNormals     = V3Cursor(Stats.NormalCount, TranArena);

  u32_cursor VertIndicies   = U32Cursor(Stats.FaceCount*3, TranArena);
  u32_cursor NormalIndicies = U32Cursor(Stats.FaceCount*3, TranArena);

  while (Stream.At < Stream.End)
  {
    char *LineType = PopWord(&Stream, TranArena);
    if (LineType == 0) { break; }

    if ( StringsMatch(LineType, "v") )
    {
      v3 Vert = {{ PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena) }};
      Push(Vert, &TempVerts);
    }
    else if ( StringsMatch(LineType, "vn") )
    {
      v3 Normal = Normalize( V3(PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena) ) );
      Push(Normal, &TempNormals);
    }
#if 0
    else if ( StringsMatch(LineType, "vt") )
    {
      v2 UV = PopV2(&Stream, "%f %f %f\n");
      Push(UV, &TempUVs);
    }
#endif
    else if ( StringsMatch(LineType, "f") )
    {
      u32 vIndex[3] = {};
      u32 nIndex[3] = {};

      // We don't currently support vertex-only meshes
      Assert(Stats.VertCount);
      Assert(Stats.NormalCount);

      if (Stats.UVCount)
      {
        vIndex[0] = PopU32(&Stream, TranArena, "/");
        /* Discard UV*/ PopU32(&Stream, TranArena, "/");
        nIndex[0] = PopU32(&Stream, TranArena);

        vIndex[1] = PopU32(&Stream, TranArena, "/");
        /* Discard UV*/ PopU32(&Stream, TranArena, "/");
        nIndex[1] = PopU32(&Stream, TranArena);

        vIndex[2] = PopU32(&Stream, TranArena, "/");
        /* Discard UV*/ PopU32(&Stream, TranArena, "/");
        nIndex[2] = PopU32(&Stream, TranArena);
      }
      else
      {
        vIndex[0] = PopU32(&Stream, TranArena, "//");
        nIndex[0] = PopU32(&Stream, TranArena);

        vIndex[1] = PopU32(&Stream, TranArena, "//");
        nIndex[1] = PopU32(&Stream, TranArena);

        vIndex[2] = PopU32(&Stream, TranArena, "//");
        nIndex[2] = PopU32(&Stream, TranArena);
      }

      Push(vIndex[0]-1, &VertIndicies );
      Push(vIndex[1]-1, &VertIndicies );
      Push(vIndex[2]-1, &VertIndicies );

      Push(nIndex[0]-1, &NormalIndicies );
      Push(nIndex[1]-1, &NormalIndicies );
      Push(nIndex[2]-1, &NormalIndicies );

    }
    else
    {
      // Irrelevant.
    }

    continue;
  }

  Assert(Remaining(&TempVerts) == 0 );
  Assert(Remaining(&TempNormals) == 0 );
  Assert(Remaining(&VertIndicies) == 0 );
  Assert(Remaining(&NormalIndicies) == 0 );

  untextured_3d_geometry_buffer *Mesh = Allocate_untextured_3d_geometry_buffer(Heap, DataType_v3, Stats.FaceCount*3);

  v3 MinV = V3(f32_MAX);
  v3 MaxV = V3(f32_MIN);

  geometry_buffer_stub_v3 Stub = GetBufferStub_v3(Mesh);

  u32 VertCount = (u32)AtElements(&VertIndicies);
  for( u32 Index = 0;
           Index < VertCount;
         ++Index )
  {
    u32 vIndex = VertIndicies.Start[Index];
    u32 nIndex = NormalIndicies.Start[Index];

    v3 Vertex = TempVerts.Start[vIndex];
    v3 Normal = TempNormals.Start[nIndex];

    MinV = Min(Vertex, MinV);
    MaxV = Max(Vertex, MaxV);

    Stub.Verts[Mesh->At] = Vertex;
    Stub.Normals[Mesh->At] = Normal;
    Mesh->At++;

    Assert(Mesh->At <= Mesh->End);
  }

  Assert(Mesh->At == Mesh->End);

  model Result = {};
  AtomicReplaceMesh(&Result.Meshes, MeshBit_Lod0, Mesh, __rdtsc());
  Result.Dim = V3i(MaxV-MinV)+1;

  return Result;
}
