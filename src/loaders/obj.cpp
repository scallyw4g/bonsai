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
model
LoadObj(memory_arena *PermMem, const char * FilePath)
{
  Info("Loading .obj file : %s \n", FilePath);

  umm Length = 0;
  binary_stream_u8 BinaryStream = BinaryStreamFromFile(FilePath, PermMem);
  if (!BinaryStream.Start) { model M = {}; return M; }

  ansi_stream Stream = AnsiStream(&BinaryStream);
  mesh_metadata Stats = GetObjMetadata(Stream, TranArena);

  v3_static_array TempVerts       = V3_Static_Array(Stats.VertCount, TranArena);
  v3_static_array TempNormals     = V3_Static_Array(Stats.NormalCount, TranArena);

  u32_static_array VertIndicies   = U32_Static_Array(Stats.FaceCount*3, TranArena);
  u32_static_array NormalIndicies = U32_Static_Array(Stats.FaceCount*3, TranArena);

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
      v3 Normal = Normalize({{ PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena), PopFloat(&Stream, TranArena) }});
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
      s32 vIndex[3] = {};
      s32 nIndex[3] = {};

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

  untextured_3d_geometry_buffer Mesh = {};
  AllocateMesh(&Mesh, Stats.FaceCount*3, PermMem);

  for( u32 Index = 0;
       Index < VertIndicies.At;
       ++Index )
  {
    u32 vIndex = VertIndicies.Elements[Index];
    u32 nIndex = NormalIndicies.Elements[Index];

    v3 Vertex = TempVerts.Elements[vIndex];
    v3 Normal = TempNormals.Elements[nIndex];

    Mesh.Verts[Mesh.At] = Vertex;
    Mesh.Normals[Mesh.At] = Normal;
    Mesh.At++;

    Assert(Mesh.At <= Mesh.End);
  }

  Assert(Mesh.At == Mesh.End);

  model Result = {};
  Result.Chunk = Allocate(chunk_data, PermMem, 1);;
  Result.Chunk->Mesh = Mesh;
  SetFlag(&Result, Chunk_Initialized);

  return Result;
}
