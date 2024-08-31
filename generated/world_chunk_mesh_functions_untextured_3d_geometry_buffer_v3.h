// src/engine/world_chunk.cpp:2056:0

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3( voxel *Voxels,
  v3i SrcChunkDim,

  v3i SrcChunkMin,
  v3i SrcChunkMax,

  // TODO(Jesse)(immediate, poof): removing the braces here causes poof to bail .. why?
  untextured_3d_geometry_buffer *DestGeometry,
  untextured_3d_geometry_buffer *DestTransparentGeometry,
  memory_arena *TempMemory,

  // NOTE(Jesse): This is so we can offset vertices such that we center
  // entity models about 0 and rotation works properly.
  v3 VertexOffset = {})
{
  /* HISTOGRAM_FUNCTION(); */
  TIMED_FUNCTION();

  Assert(DestGeometry->Type == DataType_v3);

  v3 VertexData[VERTS_PER_FACE];
  matl Materials[VERTS_PER_FACE];

  auto SrcMinP = SrcChunkMin;
  auto MaxDim = Min(SrcChunkDim, SrcChunkMax);

  auto TmpDim = MaxDim-SrcMinP;

  u32 TmpVol = u32(Volume(TmpDim));
  auto TempVoxels = Allocate(voxel, TempMemory, TmpVol);

  // NOTE(Jesse): It's necessary to copy the voxel data because the meshing
  // algorithm unsets the face flags for the voxels instead of marking them
  // as being processed.  When complete, there should be no face-flags left on
  // this data. (This is not asserted, but maybe should be?)
  //
  // TODO(Jesse): Assert there are no face flags left in this copy at the end of
  // this process?
  //
  // TODO(Jesse): Copy data into here as the algorithm proceedes instead of in
  // one shot at the start?
  //
  u32 TmpIndex = 0;
  for ( s32 zIndex = 0; zIndex < TmpDim.z ; ++zIndex )
  {
    for ( s32 yIndex = 0; yIndex < TmpDim.y ; ++yIndex )
    {
      for ( s32 xIndex = 0; xIndex < TmpDim.x ; ++xIndex )
      {
        voxel_position SrcP = SrcMinP + Voxel_Position(xIndex, yIndex, zIndex);
        s32 SrcIndex = GetIndex(SrcP, SrcChunkDim);
        /* Assert(TmpIndex < TmpVol); */
        TempVoxels[TmpIndex] = Voxels[SrcIndex];
        TmpIndex++;
      }
    }
  }

  Assert(TmpIndex == TmpVol);

  s32 Index = 0;
  for ( s32 z = 0; z < TmpDim.z ; ++z )
  {
    for ( s32 y = 0; y < TmpDim.y ; ++y )
    {
      for ( s32 x = 0; x < TmpDim.x ; ++x )
      {
        v3i TmpVoxP = V3i(x,y,z);
        voxel *Voxel = TempVoxels + Index;

        FillArray(VertexMaterial(Voxel->Color, Voxel->Transparency, 0), Materials, VERTS_PER_FACE);

        untextured_3d_geometry_buffer *Dest = {};
        if (Voxel->Transparency) { Dest = DestTransparentGeometry; } else { Dest = DestGeometry; }

        if (Dest)
        {
          if (Voxel->Flags & Voxel_RightFace)
          {
            v3 Dim = DoXStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_RightFace, Voxel->Color, Voxel->Transparency);
            RightFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_RightFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_LeftFace)
          {
            v3 Dim = DoXStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_LeftFace, Voxel->Color, Voxel->Transparency);
            LeftFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_LeftFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BottomFace)
          {
            v3 Dim = DoZStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_BottomFace, Voxel->Color, Voxel->Transparency);
            BottomFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_BottomFaceNormalData, Materials);
          }

          if (Voxel->Flags & Voxel_TopFace)
          {
            v3 Dim = DoZStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_TopFace, Voxel->Color, Voxel->Transparency);
            TopFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_TopFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_FrontFace)
          {
            v3 Dim = DoYStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_FrontFace, Voxel->Color, Voxel->Transparency);
            FrontFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_FrontFaceNormalData, Materials);
          }
          if (Voxel->Flags & Voxel_BackFace)
          {
            v3 Dim = DoYStepping(TempVoxels, TmpDim, TmpVoxP, Voxel_BackFace, Voxel->Color, Voxel->Transparency);
            BackFaceVertexData( VertexOffset+TmpVoxP, Dim, VertexData);
            BufferFaceData(Dest, VertexData, v3_BackFaceNormalData, Materials);
          }
        }

        ++Index;
      }
    }
  }

  if (DestGeometry) DestGeometry->Timestamp = __rdtsc();
  if (DestTransparentGeometry) DestTransparentGeometry->Timestamp = __rdtsc();
}

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3( vox_data *Vox,
  untextured_3d_geometry_buffer *DestGeometry,
  untextured_3d_geometry_buffer *DestTransparentGeometry,
  memory_arena *TempMemory,
  v3  VertexOffset = {})
{
  BuildWorldChunkMeshFromMarkedVoxels_Greedy_v3(Vox->ChunkData->Voxels, Vox->ChunkData->Dim, {}, Vox->ChunkData->Dim, DestGeometry, DestTransparentGeometry, TempMemory, VertexOffset);
}

link_internal void
BuildMipMesh_v3( voxel *Voxels,
  v3i  VoxDim,

  v3i  InnerMin,
  v3i  InnerMax,

  world_chunk_mesh_bitfield  MeshBit,

  untextured_3d_geometry_buffer *DestGeometry,
  memory_arena *TempMemory )
{
  /* TIMED_FUNCTION(); */

  /* /1* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); *1/ */
  /* /1* Assert(IsSet(DestChunk, Chunk_VoxelsInitialized)); *1/ */

  /* /1* Assert(InnerMin-InnerMax *1/ */

  /* voxel_position rightVoxel; */
  /* voxel_position leftVoxel; */
  /* voxel_position topVoxel; */
  /* voxel_position botVoxel; */
  /* voxel_position frontVoxel; */
  /* voxel_position backVoxel; */

  /* s32 rightVoxelReadIndex; */
  /* s32 leftVoxelReadIndex; */
  /* s32 topVoxelReadIndex; */
  /* s32 botVoxelReadIndex; */
  /* s32 frontVoxelReadIndex; */
  /* s32 backVoxelReadIndex; */


  /* vert_t.name VertexData[VERTS_PER_FACE]; */
  /* /1* v3 FaceColors[VERTS_PER_FACE]; *1/ */
  /* /1* v2 TransEmiss[VERTS_PER_FACE]; *1/ */
  /* matl Materials[VERTS_PER_FACE]; */

  /* s32 MipLevel = MeshBit; */
  /* v3i InnerDim = InnerMax - InnerMin; */

  /* Assert(VoxDim >= InnerMax); */
  /* Assert(MipLevel == 2 || MipLevel == 4 || MipLevel == 8 || MipLevel == 16); */

  /* // Filter is 1 larger on each dim than the Src needs such that we can filter */
  /* // voxels on the exterior down to an additional cell */
  /* auto FilterDim = 2+ InnerDim/MipLevel; */

  /* s32 FilterVoxelCount = s32(Volume(FilterDim)); */
  /* auto FilterVoxels = Allocate(voxel, TempMemory, FilterVoxelCount); */

  /* DimIterator(tX, tY, tZ, FilterDim) */
  /* { */
  /*   s32 FilterIndex = GetIndex(tX, tY, tZ, FilterDim); */
  /*   FilterVoxels[FilterIndex].Flags = Voxel_Filled; */
  /* } */


  /* // Filter Src voxels on the exterior edge down to _all_ the exterior filter cells */
  /* // */
  /* /1* for ( s32 zIndex = VoxDim.z; zIndex >= 0; --zIndex ) *1/ */
  /* for ( s32 zIndex = 0; zIndex < VoxDim.z; zIndex ++ ) */
  /* for ( s32 yIndex = 0; yIndex < VoxDim.y; yIndex ++ ) */
  /* for ( s32 xIndex = 0; xIndex < VoxDim.x; xIndex ++ ) */
  /* { */
  /*   v3i BaseP = V3i(xIndex, yIndex, zIndex); */

  /*   // NOTE(Jesse): This has a +(MipLevel-1) because the Src voxels */
  /*   // absolute positions don't line up with the filter. */
  /*   v3i FilterP = (BaseP+(MipLevel-1))/MipLevel; */

  /*   // NOTE(Jesse): We constrain the filter output to be one filter cell */
  /*   // larger than the inner dim on each side, but the whole input could be */
  /*   // larger than that still.  At the moment it is in Z only. */
  /*   s32 FilterIndex = TryGetIndex(FilterP, FilterDim); */
  /*   if (FilterIndex > -1) */
  /*   { */
  /*     for (s32 MipIndex = 0; MipIndex < MipLevel; ++MipIndex) */
  /*     { */
  /*       for (s32 DirIndex = 0; DirIndex < 3; ++DirIndex) */
  /*       { */
  /*         v3i Dir = {}; */
  /*         Dir.E[DirIndex] = MipIndex; */

  /*         v3i SrcP = BaseP + Dir; */

  /*         // Skip voxels contributing to the inner range */
  /*         if (IsInsideRange(InnerMin, SrcP, InnerMax)) continue; // Exclusive */

  /*         s32 SrcIndex = TryGetIndex(SrcP, VoxDim); */
  /*         if (SrcIndex > -1) */
  /*         { */
  /*           // FilterVoxels have the Filled flag set; don't carry forward the Src face flags */
  /*           FilterVoxels[FilterIndex].Flags &= Voxels[SrcIndex].Flags; */
  /*         } */
  /*       } */
  /*     } */
  /*   } */
  /* } */


  /* // Filter src voxels on the interior down to their target filter cell */
  /* /1* for ( s32 zIndex = InnerMax.z; zIndex >= InnerMin.z; zIndex -= MipLevel ) *1/ */
  /* for ( s32 zIndex = InnerMin.z; zIndex < InnerMax.z; zIndex += MipLevel ) */
  /* for ( s32 yIndex = InnerMin.y; yIndex < InnerMax.y; yIndex += MipLevel ) */
  /* for ( s32 xIndex = InnerMin.x; xIndex < InnerMax.x; xIndex += MipLevel ) */
  /* { */
  /*   v3i BaseP = V3i(xIndex, yIndex, zIndex); */

  /*   voxel Aggregate = {}; */
  /*   s32 FillCount = 0; */
  /*   for (s32 MipIndex = 0; MipIndex < MipLevel; ++MipIndex) */
  /*   { */
  /*     for (s32 DirIndex = 2; DirIndex >= 0; --DirIndex) */
  /*     /1* for (s32 DirIndex = 0; DirIndex < 3; ++DirIndex) *1/ */
  /*     { */
  /*       v3i Dir = {}; */
  /*       if (DirIndex == 2) */
  /*       { */
  /*         Dir.E[DirIndex] = -MipIndex; */
  /*       } */
  /*       else */
  /*       { */
  /*         Dir.E[DirIndex] = MipIndex; */
  /*       } */

  /*       v3i SrcP = BaseP + Dir; */
  /*       s32 SrcIndex = TryGetIndex(SrcP, VoxDim); */
  /*       if (SrcIndex > -1) */
  /*       { */
  /*         if ( (Voxels[SrcIndex].Flags & Voxel_Filled) ) */
  /*         { */
  /*           ++FillCount; */

  /*           if ( (Voxels[SrcIndex].Flags & VoxelFaceMask) ) */
  /*           { */
  /*             if ((Aggregate.Flags & Voxel_Filled) == 0) */
  /*             { */
  /*               Aggregate.Flags = Voxel_Filled; */
  /*               Aggregate.Color = Voxels[SrcIndex].Color; */
  /*             } */
  /*           } */
  /*         } */
  /*       } */
  /*     } */
  /*   } */

  /*   v3i TmpP = 1+ (BaseP/MipLevel); */
  /*   s32 ThisTmpIndex = GetIndex(TmpP, FilterDim); */

  /*   // NOTE(Jesse): Doing a conditional filter on the number of filled */
  /*   // voxels helps with popping quite a bit, but it also has the effect */
  /*   // that we can 'see' through thin surfaces. */
  /*   // */
  /*   // We might be able to fix this by running MarkBoundaryVoxels twice */
  /*   // such that we only consider assigning colors from exterior voxels ..? */
/* /1* #if 1 *1/ */
  /*   FilterVoxels[ThisTmpIndex] = Aggregate; */
/* /1* #else *1/ */
/* /1*         if (FillCount >= ((MipLevel*MipLevel)/2)) *1/ */
/* /1*         { *1/ */
/* /1*           FilterVoxels[ThisTmpIndex] = Aggregate; *1/ */
/* /1*         } *1/ */
/* /1*         else *1/ */
/* /1*         { *1/ */
/* /1*           FilterVoxels[ThisTmpIndex] = {}; *1/ */
/* /1*         } *1/ */
/* /1* #endif *1/ */
  /* } */


  /* MarkBoundaryVoxels_NoExteriorFaces( FilterVoxels, FilterDim, InnerMin, InnerMax ); */


  /* for ( s32 z = 1; z < FilterDim.z-1; z++ ) */
  /* { */
  /*   for ( s32 y = 1; y < FilterDim.y-1; y++ ) */
  /*   { */
  /*     for ( s32 x = 1; x < FilterDim.x-1; x++ ) */
  /*     { */
  /*       v3i TmpVoxP = Voxel_Position(x,y,z); */
  /*       s32 Index = GetIndex(TmpVoxP, FilterDim); */
  /*       voxel *Voxel = FilterVoxels + Index; */

  /*       voxel_position ActualP = TmpVoxP-1; */

  /*       /1* v3 Color = GetColorData(Voxel->Color); *1/ */
  /*       f32 Trans = (f32)Voxel->Transparency / 255.f; */
  /*       // TODO(Jesse): This copy could be avoided in multiple ways, and should be. */
  /*       FillArray(VertexMaterial(Voxel->Color, Trans, 0.f), Materials, VERTS_PER_FACE); */

  /*       if (Voxel->Flags & Voxel_RightFace) */
  /*       { */
  /*         v3 Dim = DoXStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_RightFace, Voxel->Color, Voxel->Transparency); */
  /*         RightFaceVertexData( ActualP*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, RightFaceNormalData, Materials); */
  /*       } */
  /*       if (Voxel->Flags & Voxel_LeftFace) */
  /*       { */
  /*         v3 Dim = DoXStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_LeftFace, Voxel->Color, Voxel->Transparency); */
  /*         LeftFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, LeftFaceNormalData, Materials); */
  /*       } */


  /*       if (Voxel->Flags & Voxel_BottomFace) */
  /*       { */
  /*         v3 Dim = DoZStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_BottomFace, Voxel->Color, Voxel->Transparency); */
  /*         BottomFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, BottomFaceNormalData, Materials); */
  /*       } */
  /*       if (Voxel->Flags & Voxel_TopFace) */
  /*       { */
  /*         v3 Dim = DoZStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_TopFace, Voxel->Color, Voxel->Transparency); */
  /*         TopFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, TopFaceNormalData, Materials); */
  /*       } */


  /*       if (Voxel->Flags & Voxel_FrontFace) */
  /*       { */
  /*         v3 Dim = DoYStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_FrontFace, Voxel->Color, Voxel->Transparency); */
  /*         FrontFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, FrontFaceNormalData, Materials); */
  /*       } */
  /*       if (Voxel->Flags & Voxel_BackFace) */
  /*       { */
  /*         v3 Dim = DoYStepping(FilterVoxels, FilterDim, TmpVoxP, Voxel_BackFace, Voxel->Color, Voxel->Transparency); */
  /*         BackFaceVertexData( V3(ActualP)*MipLevel, Dim*MipLevel, VertexData); */
  /*         BufferFaceData(DestGeometry, VertexData, BackFaceNormalData, Materials); */
  /*       } */
  /*     } */
  /*   } */
  /* } */

  /* DestGeometry->Timestamp = __rdtsc(); */
}

link_internal void
BuildWorldChunkMeshFromMarkedVoxels_Naieve_v3( voxel *Voxels,
  chunk_dimension VoxDim,

  chunk_dimension SrcChunkMin,
  chunk_dimension SrcChunkMax,

  untextured_3d_geometry_buffer *DestGeometry,
  untextured_3d_geometry_buffer *DestTransparentGeometry )
{
  /* TIMED_FUNCTION(); */

  /* // NOTE(Jesse): Use the greedy function. */
  /* /1* InvalidCodePath(); *1/ */

  /* /1* Assert(IsSet(SrcChunk, Chunk_VoxelsInitialized)); *1/ */
  /* /1* Assert(IsSet(DestChunk, Chunk_VoxelsInitialized)); *1/ */

  /* voxel_position rightVoxel; */
  /* voxel_position leftVoxel; */
  /* voxel_position topVoxel; */
  /* voxel_position botVoxel; */
  /* voxel_position frontVoxel; */
  /* voxel_position backVoxel; */

  /* s32 rightVoxelReadIndex; */
  /* s32 leftVoxelReadIndex; */
  /* s32 topVoxelReadIndex; */
  /* s32 botVoxelReadIndex; */
  /* s32 frontVoxelReadIndex; */
  /* s32 backVoxelReadIndex; */

  /* /1* random_series ColorEntropy = {33453}; *1/ */


  /* v3 Diameter = V3(1.0f); */
  /* vert_t.name VertexData[VERTS_PER_FACE]; */
  /* /1* v3 FaceColors[VERTS_PER_FACE]; *1/ */
  /* /1* v2 TransEmiss[VERTS_PER_FACE] = {}; *1/ */
  /* matl Materials[VERTS_PER_FACE]; */

  /* auto MinDim = SrcChunkMin; */
  /* auto MaxDim = Min(VoxDim, SrcChunkMax); // SrcChunkMin+DestChunkDim+1 */
  /* for ( s32 z = MinDim.z; z < MaxDim.z ; ++z ) */
  /* { */
  /*   for ( s32 y = MinDim.y; y < MaxDim.y ; ++y ) */
  /*   { */
  /*     for ( s32 x = MinDim.x; x < MaxDim.x ; ++x ) */
  /*     { */
  /*       voxel_position DestP  = Voxel_Position(x,y,z); */
  /*       s32 Index = GetIndex(DestP, VoxDim); */
  /*       voxel *Voxel = Voxels + Index; */

  /*       // TODO(Jesse): This copy could be avoided in multiple ways, and should be. */
  /*       /1* FillColorArray(Voxel->Color, FaceColors, ColorPallette, VERTS_PER_FACE); *1/ */

/* /1* #if VOXEL_DEBUG_COLOR *1/ */
  /*       /1* v3 Color = Abs(Voxel->DebugColor); *1/ */
/* /1* #else *1/ */
  /*       /1* v3 Color = GetColorData(Voxel->Color); *1/ */
/* /1* #endif *1/ */

  /*       f32 Trans = (f32)Voxel->Transparency / 255.f; */
  /*       FillArray(VertexMaterial(Voxel->Color, Trans, 0.f), Materials, VERTS_PER_FACE); */

  /*       (buffer_t.name) *Dest = {}; */
  /*       if (Voxel->Transparency) { Dest = DestTransparentGeometry; } else { Dest = DestGeometry; } */

  /*       if (Dest) */
  /*       { */
  /*         if (Voxel->Flags & Voxel_RightFace) */
  /*         { */
  /*           RightFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, RightFaceNormalData, Materials); */
  /*         } */
  /*         if (Voxel->Flags & Voxel_LeftFace) */
  /*         { */
  /*           LeftFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, LeftFaceNormalData, Materials); */
  /*         } */
  /*         if (Voxel->Flags & Voxel_BottomFace) */
  /*         { */
  /*           BottomFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, BottomFaceNormalData, Materials); */
  /*         } */
  /*         if (Voxel->Flags & Voxel_TopFace) */
  /*         { */
  /*           TopFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, TopFaceNormalData, Materials); */
  /*         } */
  /*         if (Voxel->Flags & Voxel_FrontFace) */
  /*         { */
  /*           FrontFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, FrontFaceNormalData, Materials); */
  /*         } */
  /*         if (Voxel->Flags & Voxel_BackFace) */
  /*         { */
  /*           BackFaceVertexData( V3(DestP-SrcChunkMin), Diameter, VertexData); */
  /*           BufferFaceData(Dest, VertexData, BackFaceNormalData, Materials); */
  /*         } */
  /*       } */
  /*     } */
  /*   } */
  /* } */

  /* if (DestGeometry) DestGeometry->Timestamp = __rdtsc(); */
  /* if (DestTransparentGeometry) DestTransparentGeometry->Timestamp = __rdtsc(); */
}


