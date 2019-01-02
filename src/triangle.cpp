function void
SeedTriangulation(current_triangles* CurrentTriangles, memory_arena* Memory)
{
  //  1    2
  //
  //  0    3

  CurrentTriangles->Count = 0;

  u32 xMax = WORLD_CHUNK_DIM.x;
  u32 yMax = WORLD_CHUNK_DIM.y;

  // FIXME(Jesse): Memory leak
  voxel_position* TempPoints = AllocateProtection(voxel_position, Memory, 4, False);
  TempPoints[0] = Voxel_Position( 0   , 0   , 0);
  TempPoints[1] = Voxel_Position( 0   , yMax, 0);
  TempPoints[2] = Voxel_Position( xMax, yMax, 0);
  TempPoints[3] = Voxel_Position( xMax, 0   , 0);
 
  triangle* Seed0 = Triangle(&TempPoints[0], &TempPoints[1], &TempPoints[2], Memory);
  PushTriangle(CurrentTriangles, Seed0);

  triangle* Seed1 = Triangle(&TempPoints[2], &TempPoints[3], &TempPoints[0], Memory);
  PushTriangle(CurrentTriangles, Seed1);

  return;
}

function b32
HasUnfilledNeighbors(u32 Index, world_chunk* Chunk, chunk_dimension ChunkDim)
{
  TIMED_FUNCTION();

  chunk_data *ChunkData = Chunk->Data;

  Assert(IsSet(Chunk, Chunk_Initialized) || IsSet(Chunk, Chunk_MeshComplete));

  u32 VolumeChunkDim = Volume(ChunkDim);
  voxel_position CurrentP = GetPosition(Index, ChunkDim);

  voxel_position RightVoxel = CurrentP + Voxel_Position(1, 0, 0);
  voxel_position LeftVoxel  = CurrentP - Voxel_Position(1, 0, 0);
  voxel_position TopVoxel   = CurrentP + Voxel_Position(0, 0, 1);
  voxel_position BotVoxel   = CurrentP - Voxel_Position(0, 0, 1);
  voxel_position FrontVoxel = CurrentP + Voxel_Position(0, 1, 0);
  voxel_position BackVoxel  = CurrentP - Voxel_Position(0, 1, 0);

  u32 RightVoxelReadIndex = GetIndexUnsafe(RightVoxel, ChunkDim);
  u32 LeftVoxelReadIndex  = GetIndexUnsafe(LeftVoxel, ChunkDim);
  u32 TopVoxelReadIndex   = GetIndexUnsafe(TopVoxel, ChunkDim);
  u32 BotVoxelReadIndex   = GetIndexUnsafe(BotVoxel, ChunkDim);
  u32 FrontVoxelReadIndex = GetIndexUnsafe(FrontVoxel, ChunkDim);
  u32 BackVoxelReadIndex  = GetIndexUnsafe(BackVoxel, ChunkDim);

  b32 Result = False;

  if (RightVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, RightVoxelReadIndex);

  if (LeftVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, LeftVoxelReadIndex);

  if (BotVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, BotVoxelReadIndex);

  if (TopVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, TopVoxelReadIndex);

  if (FrontVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, FrontVoxelReadIndex);

  if (BackVoxelReadIndex < VolumeChunkDim)
    Result |= NotFilledInChunk( ChunkData, BackVoxelReadIndex);

  return Result;
}

function void
GetBoundingVoxels(world_chunk* Chunk, boundary_voxels* Dest)
{
  /* TIMED_FUNCTION(); */
  for (s32 z = 0;
      z < WORLD_CHUNK_DIM.z;
      ++z)
  {
    for (s32 y = 0;
        y < WORLD_CHUNK_DIM.y;
        ++y)
    {
      for (s32 x = 0;
          x < WORLD_CHUNK_DIM.x;
          ++x)
      {
        voxel_position P = Voxel_Position(x, y, z);
        u32 vIndex = GetIndex(P, WORLD_CHUNK_DIM);
        voxel *V = Chunk->Data->Voxels + vIndex;
        if (IsSet(V, Voxel_Filled) &&
            HasUnfilledNeighbors(vIndex, Chunk, WORLD_CHUNK_DIM))
        {
          Assert(Dest->At < Dest->End);
          Dest->Points[Dest->At++] = P;

          Dest->Min = Min(P, Dest->Min);
          Dest->Max = Max(P, Dest->Max);
        }
      }
    }
  }

  return;
}

inline void
BufferTriangle(untextured_3d_geometry_buffer* Dest, triangle* Triangle, v3 Normal, s32 ColorIndex)
{
  v3 VertBuffer[3];
  v3 NormalBuffer[3] = {Normal, Normal, Normal};

  VertBuffer[0] = V3(Triangle->Points[0]);
  VertBuffer[1] = V3(Triangle->Points[1]);
  VertBuffer[2] = V3(Triangle->Points[2]);

  v4 FaceColors[FACE_VERT_COUNT];
  FillColorArray(ColorIndex, FaceColors, FACE_VERT_COUNT);

  BufferVertsChecked(
    Dest,
    3,
    VertBuffer,
    NormalBuffer,
    FaceColors);

}

inline void
BufferTriangle(untextured_3d_geometry_buffer *Mesh, v3 *Verts, v3 Normal, s32 ColorIndex)
{
  v3 VertBuffer[3];
  v3 NormalBuffer[3] = {Normal, Normal, Normal};

  // TODO(Jesse): Is this necessary to avoid some pointer aliasing bug?
  memcpy( VertBuffer, Verts, 9 * sizeof(r32) );

  v4 FaceColors[FACE_VERT_COUNT];
  FillColorArray(ColorIndex, FaceColors, FACE_VERT_COUNT);;

  BufferVertsChecked(
    Mesh,
    3,
    VertBuffer,
    NormalBuffer,
    FaceColors);

}

#if 0
function void
TriangulateUntilIndex(untextured_3d_geometry_buffer* Dest, current_triangles* CurrentTriangles, memory_arena* TempMem, u32 MaxIndex)
{
  Assert(MaxIndex < CurrentTriangles->SurfacePoints->End);
  Dest->At = 0;
  SeedTriangulation(CurrentTriangles, TempMem);

  for (u32 PointIndex = 0;
      PointIndex <= MaxIndex;
      ++PointIndex)
  {
    voxel_position* Point = CurrentTriangles->SurfacePoints->Points + PointIndex;
    triangle* Closest = FindClosestFace(CurrentTriangles, Point);

    SplitFace(CurrentTriangles, Closest, Point, TempMem);
  }

  for (u32 FaceIndex = 0;
      FaceIndex < CurrentTriangles->Count;
      ++FaceIndex)
  {
    triangle* Face = CurrentTriangles->Tris[FaceIndex];
    BufferTriangle(Dest, Face, V3(0,0,1), RED);
  }

  u32 NextIndex = MaxIndex + 1;
  if (NextIndex < CurrentTriangles->SurfacePoints->End)
  {
    voxel_position* ThisPoint = CurrentTriangles->SurfacePoints->Points + MaxIndex;
    DrawVoxel( Dest, V3(*ThisPoint), GREEN, V3(0.25f));

    voxel_position* NextPoint = CurrentTriangles->SurfacePoints->Points + NextIndex;
    triangle* Closest = FindClosestFace(CurrentTriangles, NextPoint);
    DrawVoxel( Dest, V3(*NextPoint), TEAL, V3(0.25f));
    BufferTriangle(Dest, Closest, V3(0,0,1), TEAL);
  }

  return;
}

function void
Triangulate(untextured_3d_geometry_buffer* Dest, current_triangles* CurrentTriangles, memory_arena* TempMem)
{
  if (CurrentTriangles->SurfacePoints->End >= 3)
  {
    for (u32 PointIndex = 0;
        PointIndex < CurrentTriangles->SurfacePoints->End;
        ++PointIndex)
    {
      voxel_position* Point = CurrentTriangles->SurfacePoints->Points + PointIndex;
      triangle* Closest = FindClosestFace(CurrentTriangles, Point);

      SplitFace(CurrentTriangles, Closest, Point, TempMem);
    }

    for (u32 FaceIndex = 0;
        FaceIndex < CurrentTriangles->Count;
        ++FaceIndex)
    {
      triangle* Face = CurrentTriangles->Tris[FaceIndex];
      BufferTriangle(Dest, Face, V3(0,0,1), RED);
    }

  }

  return;
}

function void
TriangulateSingle(untextured_3d_geometry_buffer* Dest, current_triangles* CurrentTriangles, voxel_position* Point, triangle* Closest, memory_arena* TempMem)
{
  SplitFace(CurrentTriangles, Closest, Point, TempMem);

  for (u32 FaceIndex = 0;
      FaceIndex < CurrentTriangles->Count;
      ++FaceIndex)
  {
    triangle* Face = CurrentTriangles->Tris[FaceIndex];
    BufferTriangle(Dest, Face, V3(0,0,1), RED);
  }

  return;
}
#endif
