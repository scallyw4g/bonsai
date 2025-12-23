// callsite
// src/engine/world_chunk.cpp:1748:0

// def (world_chunk_mesh_functions)
// src/engine/world_chunk.cpp:1547:0
link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Naieve_v3_u8( voxel *Voxels,
  u64 *FaceMasks,
  v3i  SrcChunkDim,

  v3i  SrcChunkMin,
  v3i  SrcChunkMax,

  // TODO(Jesse)(immediate, poof): @poof_parens_bug
  untextured_3d_geometry_buffer *Dest,
  untextured_3d_geometry_buffer *Unused,

  // NOTE(Jesse): This is so we can offset vertices such that we center
  // entity models about 0 and rotation works properly.
  v3_u8  VertexOffset = {})
{
  /* HISTOGRAM_FUNCTION(); */
  /* TIMED_FUNCTION(); */

  Assert(SrcChunkMin == V3i(0));
  Assert(SrcChunkMax == V3i(0));
  Assert(SrcChunkDim == V3i(64, 66, 66));

  v3_u8 VertexData[VERTS_PER_FACE];
  v3_u8 NormalData[VERTS_PER_FACE];
  matl Materials[VERTS_PER_FACE];

  for ( s32 zBlock = 1; zBlock < SrcChunkDim.z-1; ++zBlock )
  {
    s32 z = zBlock-1;
    for ( s32 yBlock = 1; yBlock < SrcChunkDim.y-1; ++yBlock )
    {
      s32 y = yBlock-1;
      s32 OccupancyIndex = GetIndex(yBlock, zBlock, SrcChunkDim.yz);

      u64 LeftFaces  = FaceMasks[(OccupancyIndex*6)+0];
      u64 RightFaces = FaceMasks[(OccupancyIndex*6)+1];
      u64 FrontFaces = FaceMasks[(OccupancyIndex*6)+2];
      u64 BackFaces  = FaceMasks[(OccupancyIndex*6)+3];
      u64 TopFaces   = FaceMasks[(OccupancyIndex*6)+4];
      u64 BotFaces   = FaceMasks[(OccupancyIndex*6)+5];

      v3 Dim = V3(1.f, 1.f, 1.f);

      f32 BendStrength = 8.f;
      f32 NormalFactor = 1.f/BendStrength;

      u64 BaseVoxelOffset = u64(GetIndex(0, yBlock, zBlock, SrcChunkDim));
      while (LeftFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&LeftFaces);
        u64 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);

        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(-1,0,0) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        LeftFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }

      while (RightFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&RightFaces);
        u64 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);
        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(1,0,0) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        RightFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }

      while (FrontFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&FrontFaces);
        u64 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);
        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,1,0) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        FrontFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }

      while (BackFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&BackFaces);
        u64 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);
        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,-1,0) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        BackFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }

      while (TopFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&TopFaces);
        u64 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);
        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,0,1) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        TopFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }

      while (BotFaces)
      {
        u64 This = UnsetLeastSignificantSetBit(&BotFaces);
        u32 xOffset = GetIndexOfSingleSetBit(This);
        v3 P = V3(s32(xOffset), y, z);
        u16 HSV = Voxels[BaseVoxelOffset+xOffset].PackedHSV;

        u16 PNormal  = Voxels[BaseVoxelOffset+xOffset].Normal;
        v3 Normal    = Normalize((UnpackV3_15b(PNormal)) + (V3(0,0,-1) * NormalFactor));
        /* Assert(Length(Normal) > 0.f); */

        Assert(BufferHasRoomFor(Dest, VERTS_PER_FACE));

        v3_u8 *DestVerts          = Cast( v3_u8*, Dest->Verts)   + Dest->At;
        v3_u8 *DestNormals        = Cast( v3_u8*, Dest->Normals) + Dest->At;
        vertex_material *DestMats       =                     Dest->Mat      + Dest->At;

        BottomFaceVertexData( VertexOffset+P, Dim, DestVerts);
        FillArray(Normal, DestNormals, VERTS_PER_FACE);
        FillArray(VertexMaterial(HSV, 0.f, 0.f), DestMats, VERTS_PER_FACE);
        Dest->At += VERTS_PER_FACE;
      }



    }

  }
}

