


link_internal void
DrawStandingSpot(untextured_3d_geometry_buffer *Mesh, v3 RenderSpot_MinP, v3 TileDim, u32 ColorIndex = BLUE, r32 Thickness = DEFAULT_LINE_THICKNESS)
{
#if 0
  untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Mesh, VERTS_PER_VOXEL);
  auto MinP = RenderSpot_MinP - Thickness;
  DrawVoxel_MinDim(&AABBDest, MinP, ColorIndex, TileDim + (Thickness*2.f));
#else
  v3 HalfTileDim = TileDim/2.f;
  v3 QuarterTileDim = HalfTileDim/2.f;

  untextured_3d_geometry_buffer AABBDest = ReserveBufferSpace(Mesh, VERTS_PER_VOXEL);

  auto MinP = RenderSpot_MinP-Thickness+V3(QuarterTileDim.xy,0.f)+V3(0,0,2);
  DrawVoxel_MinDim(&AABBDest, MinP, ColorIndex, HalfTileDim + Thickness*2.f);

  /* DEBUG_DrawAABB( &AABBDest, */
  /*                 AABBMinDim( , TileDrawDim), */
  /*                 ColorIndex, Thickness); */
#endif
}

link_internal maybe_ray
ComputeRayFromCursor(platform *Plat, m4* ViewProjection, camera *Camera, v3i WorldChunkDim)
{
  maybe_ray Result = {};

  m4 InverseViewProjection = {};
  b32 Inverted = Inverse((r32*)ViewProjection, (r32*)&InverseViewProjection);
  if (Inverted)
  {
    v3 MouseMinWorldP = Unproject( Plat->MouseP,
                                   0.0f,
                                   V2(Plat->WindowWidth, Plat->WindowHeight),
                                   &InverseViewProjection);

    v3 MouseMaxWorldP = Unproject( Plat->MouseP,
                                   1.0f,
                                   V2(Plat->WindowWidth, Plat->WindowHeight),
                                   &InverseViewProjection);

    v3 RayDirection = Normalize(MouseMaxWorldP - MouseMinWorldP);

    v3 CameraOffset = Camera->ViewingTarget.Offset + V3(Camera->ViewingTarget.WorldP * WorldChunkDim);
    /* v3 CameraOffset = V3(0); */
    Result.Ray = { .Origin = MouseMinWorldP + CameraOffset, .Dir = RayDirection };
    Result.Tag = Maybe_Yes;
  }

  return Result;
}

void
RenderAoTexture(ao_render_group *AoGroup)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);
  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  GL.UseProgram(AoGroup->Shader.ID);

  GL.Uniform3fv(AoGroup->SsaoKernelUniform, SSAO_KERNEL_SIZE, (r32*)AoGroup->SsaoKernel);

  BindShaderUniforms(&AoGroup->Shader);

  RenderQuad();

  AssertNoGlErrors;

  return;
}

void
UpdateLightingTextures(game_lights *Lights)
{
  // TODO(Jesse, id: 120, tags: allocation, speed): Allocate lights such that this swizzle is unneeded
  v3 *PosData = AllocateProtection(v3, TranArena, MAX_LIGHTS, False);
  v3 *ColorData = AllocateProtection(v3, TranArena, MAX_LIGHTS, False);

  for (s32 LightIndex = 0;
      LightIndex < Lights->Count;
      ++LightIndex)
  {
    PosData[LightIndex] = Lights->Lights[LightIndex].Position;
    ColorData[LightIndex] = Lights->Lights[LightIndex].Color;
  }

  u32 Type = GL_TEXTURE_2D;

  GL.BindTexture(Type, Lights->PositionTex->ID);
  GL.TexImage2D( Type, 0, GL_RGB32F,
                Lights->PositionTex->Dim.x, Lights->PositionTex->Dim.y,
                0,  GL_RGB, GL_FLOAT, PosData);
  AssertNoGlErrors;

  GL.BindTexture(Type, Lights->ColorTex->ID);
  GL.TexImage2D( Type, 0, GL_RGB32F,
                Lights->ColorTex->Dim.x, Lights->ColorTex->Dim.y,
                0,  GL_RGB, GL_FLOAT, ColorData);
  AssertNoGlErrors;

  return;
}

link_internal void
Debug_DrawTextureToDebugQuad( shader *DebugShader )
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(256));

  GL.UseProgram(DebugShader->ID);
  BindShaderUniforms(DebugShader);

  RenderQuad();

  AssertNoGlErrors;

  return;
}

link_internal void
DrawGBufferToFullscreenQuad( platform *Plat, graphics *Graphics )
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));

  GL.UseProgram(Graphics->gBuffer->LightingShader.ID);

  UpdateLightingTextures(Graphics->Lights);
  Graphics->SG->MVP = NdcToScreenSpace * Graphics->SG->MVP;

  BindShaderUniforms(&Graphics->gBuffer->LightingShader);

  RenderQuad();

  AssertNoGlErrors;

  return;
}

link_internal gpu_mapped_element_buffer *
GetCurrentGpuMap(graphics *Graphics)
{
  gpu_mapped_element_buffer* GpuMap = Graphics->GpuBuffers + Graphics->GpuBufferWriteIndex;
  return GpuMap;
}

#if 0
void
DEBUG_CopyTextureToMemory(texture *Texture)
{
  glBindTexture(GL_TEXTURE_2D, Texture->ID);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  u8 *raw_img = (u8*)calloc(sizeof(u8), Texture->Dim.x * Texture->Dim.y * 4);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, raw_img);
  //glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, raw_img);
  AssertNoGlErrors;
  free(raw_img);
  return;
}
#endif

#if 1
#if 1
inline m4
GetShadowMapMVP(light *Sun)
{
  // Compute the MVP matrix from the light's point of view
  /* v3 Translate = GetRenderP(Camera->Target, Camera); */
  m4 depthProjectionMatrix = Orthographic(SHADOW_MAP_X,
                                          SHADOW_MAP_Y,
                                          SHADOW_MAP_Z_MIN,
                                          SHADOW_MAP_Z_MAX);

  v3 Front = Normalize(Sun->Position);
  v3 Right = Cross( Front, V3(0,1,0) );
  v3 Up = Cross(Right, Front);

  v3 Target = V3(0);
  m4 depthViewMatrix =  LookAt(Sun->Position, Target, Up);

  return depthProjectionMatrix * depthViewMatrix;
}
#endif


link_internal void
RenderShadowMap(gpu_mapped_element_buffer *GpuMap, graphics *Graphics)
{
  TIMED_FUNCTION();

  shadow_render_group *SG = Graphics->SG;

  GL.BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  GL.UseProgram(SG->DepthShader.ID);

  SetViewport(V2(SHADOW_MAP_RESOLUTION_X, SHADOW_MAP_RESOLUTION_Y));

  SG->MVP = GetShadowMapMVP(&SG->Sun);
  GL.UniformMatrix4fv(SG->MVP_ID, 1, GL_FALSE, &SG->MVP.E[0].E[0]);

  Draw(GpuMap->Buffer.At);

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

  return;
}
#endif

link_internal void
RenderGBuffer(gpu_mapped_element_buffer *GpuMap, graphics *Graphics)
{
  TIMED_FUNCTION();

  auto RG = Graphics->gBuffer;

  GL.BindFramebuffer(GL_FRAMEBUFFER, RG->FBO.ID);
  GL.UseProgram(RG->gBufferShader.ID);

  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  BindShaderUniforms(&RG->gBufferShader);

  FlushBuffersToCard(GpuMap);
  Draw(GpuMap->Buffer.At);

  AssertNoGlErrors;

  return;
}

link_internal void
RenderPostBuffer(post_processing_group *PostGroup, untextured_3d_geometry_buffer *Mesh)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, PostGroup->FBO.ID);
  GL.UseProgram(PostGroup->Shader.ID);

  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  BindShaderUniforms(&PostGroup->Shader);

  TIMED_BLOCK("PostBuffer - Bind and buffer data");
    u32 AttributeIndex = 0;
    BufferVertsToCard(PostGroup->VertexBuffer, Mesh, &AttributeIndex);
    BufferColorsToCard(PostGroup->ColorBuffer, Mesh, &AttributeIndex);
  END_BLOCK("PostBuffer - Bind and buffer data");

  Draw(Mesh->At);
  Mesh->At = 0;

  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
}

inline bool
IsRightChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx+1) % (int)ChunkDim.x == 0;
}

inline bool
IsLeftChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx) % (int)ChunkDim.x == 0;
}

inline bool
IsTopChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return ((idx/(int)ChunkDim.x)+1) % (int)ChunkDim.y == 0;
}

inline bool
IsBottomChunkBoundary( chunk_dimension ChunkDim, int idx )
{
  return (idx/(int)ChunkDim.x) % (int)ChunkDim.y == 0;
}

#if 0
inline r32
DistanceToPlane(plane *Plane, v3 P)
{
  r32 x = Plane->P.x;
  r32 y = Plane->P.y;
  r32 z = Plane->P.z;

  r32 a = Plane->Normal.x;
  r32 b = Plane->Normal.y;
  r32 c = Plane->Normal.z;

  r32 d = Plane->d;
  Assert(a*x + b*y + c*z + d == 0);

  r32 Distance = a*P.x + b*P.y + c*P.z + d;
  return Distance;
}

inline bool
IsInfrustum(chunk_dimension WorldChunkDim, camera *Camera, canonical_position P)
{
  bool Result = true;

  v3 TestP = GetRenderP(WorldChunkDim, P, Camera);

  Result &= (DistanceToPlane(&Camera->Frust.Top, TestP)   > -1*WorldChunkDim.y);
  Result &= (DistanceToPlane(&Camera->Frust.Bot, TestP)   > -1*WorldChunkDim.y);
  Result &= (DistanceToPlane(&Camera->Frust.Left, TestP)  > -1*WorldChunkDim.x);
  Result &= (DistanceToPlane(&Camera->Frust.Right, TestP) > -1*WorldChunkDim.x);

  return Result;
}
#endif

inline voxel_position
Clamp01( voxel_position V )
{
  voxel_position Result = V;
  if (Result.x < 0) Result.x = 0;
  if (Result.y < 0) Result.y = 0;
  if (Result.z < 0) Result.z = 0;

  if (Result.x > 1) Result.x = 1;
  if (Result.y > 1) Result.y = 1;
  if (Result.z > 1) Result.z = 1;

  return Result;
}

#if 0
void
BuildExteriorBoundaryVoxels( world_chunk *chunk, chunk_dimension Dim, world_chunk *Neighbor, voxel_position NeighborVector )
{
  voxel_position nvSq = (NeighborVector*NeighborVector);

  voxel_position AbsInvNeighborVector = ((nvSq-1)*(nvSq-1));

  voxel_position LocalPlane = ClampPositive(Dim-1) * AbsInvNeighborVector + 1;

  voxel_position LocalOffset = ClampPositive(Dim*NeighborVector - nvSq);

  voxel_position Start = Voxel_Position(0,0,0);

  for ( int z = Start.z; z < LocalPlane.z; ++z )
  {
    for ( int y = Start.y; y < LocalPlane.y; ++y )
    {
      for ( int x = Start.x; x < LocalPlane.x; ++x )
      {
        voxel_position LocalVoxelP = Voxel_Position(x+LocalOffset.x, y+LocalOffset.y, z+LocalOffset.z);

        if ( chunk && !IsFilledInChunk(chunk->Data, LocalVoxelP, Dim) )
          continue;

        voxel_position NeighborP = ClampPositive(
            (LocalVoxelP - (Dim * NeighborVector))
            - nvSq);

        if ( Neighbor && !IsFilledInChunk(Neighbor->Data, NeighborP, Dim) )
        {
          voxel *Voxel = &chunk->Data->Voxels[GetIndex(LocalVoxelP, chunk->Data, Dim)];

          if (NeighborVector.x > 0)
            SetFlag(Voxel, Voxel_RightFace);

          if (NeighborVector.x < 0)
            SetFlag(Voxel, Voxel_LeftFace);

          if (NeighborVector.y > 0)
            SetFlag(Voxel, Voxel_TopFace);

          if (NeighborVector.y < 0)
            SetFlag(Voxel, Voxel_BottomFace);

          if (NeighborVector.z > 0)
            SetFlag(Voxel, Voxel_FrontFace);

          if (NeighborVector.z < 0)
            SetFlag(Voxel, Voxel_BackFace);

          boundary_voxel BoundaryVoxel(Voxel, LocalVoxelP);
          PushBoundaryVoxel( chunk->Data, &BoundaryVoxel, Dim);
        }
      }
    }
  }


  return;
}

inline bool
IsInfrustum( chunk_dimension WorldChunkDim, camera *Camera, world_chunk *Chunk )
{
  v3 ChunkMid = WorldChunkDim/2.0f;
  canonical_position P1 = Canonical_Position(  ChunkMid, Chunk->WorldP );
  bool Result = IsInfrustum(WorldChunkDim, Camera, P1 );
  return Result;
}
#endif

inline void
ClearFramebuffers(graphics *Graphics)
{
  TIMED_FUNCTION();
  GL.ClearColor(f32_MAX, f32_MAX, f32_MAX, f32_MAX);
  GL.ClearDepth(f32_MAX);

#if DEBUG_SYSTEM_API
#if DEBUG_SYSTEM_INTERNAL_BUILD
  GetDebugState()->ClearFramebuffers(&GetDebugState()->PickedChunksRenderGroup);
#else
  if (GetDebugState()) GetDebugState()->ClearFramebuffers(&GetDebugState()->PickedChunksRenderGroup);
#endif

  /* GL.BindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID); */
  /* GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
#endif

  GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // FIXME(Jesse): This is taking _forever_ on Linux (GLES) .. does it take
  // forever on other Linux systems?
  GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* glBindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName); */
  /* glClear(GL_DEPTH_BUFFER_BIT); */

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return;
}

void
BufferChunkMesh(graphics *Graphics, untextured_3d_geometry_buffer *Dest, untextured_3d_geometry_buffer *Src,
                chunk_dimension WorldChunkDim, world_position WorldP, r32 Scale = 1.0f, v3 Offset = V3(0), Quaternion Rot = {})
{
  /* TIMED_FUNCTION(); */

  if (!Src || Src->At == 0)
    return;

#if DEBUG_CHUNK_AABB
  DEBUG_DrawChunkAABB(Dest, Graphics, WorldP, WorldChunkDim, PINK, 0.1f);
#endif

  v3 ModelBasisP =
    GetRenderP( WorldChunkDim, Canonical_Position(Offset, WorldP), Graphics->Camera);

  auto CopyBuffer = ReserveBufferSpace( Dest, Src->At);
  if (Length(Rot.xyz) == 0.f)
  {
    BufferVertsChecked(&CopyBuffer, Src->At,
                       Src->Verts, Src->Normals, Src->Colors,
                       ModelBasisP, V3(Scale));
  }
  else
  {

    BufferVertsChecked(&CopyBuffer, Src->At,
                       Src->Verts, Src->Normals, Src->Colors,
                       ModelBasisP, V3(Scale), Rot);
  }

  return;
}

#if 0
line
FindIntersectingLine(
  world_chunk *Chunk,
  voxel_position OffsetVector,
  int FirstFilledIndex)
{
  voxel_position MinP = Chunk->Data->BoundaryVoxels[FirstFilledIndex].Offset;
  voxel_position MaxP = MinP;

  int CurrentMaxLen = 0;
  int CurrentMinLen = 0;

  for (int VoxelIndex = FirstFilledIndex;
      VoxelIndex < Chunk->Data->BoundaryVoxelCount;
      ++ VoxelIndex)
  {
    voxel_position P  = Chunk->Data->BoundaryVoxels[VoxelIndex].Offset;

    s32 OriginToP = LengthSq(P);

    // Max
    if ( OriginToP > CurrentMaxLen )
    {
      MaxP = P + 1;
      CurrentMaxLen = OriginToP;
    }

    // Min
    if ( OriginToP < CurrentMinLen )
    {
      MinP = P;
      CurrentMinLen = OriginToP;
    }

  }

  line Result(MinP, MaxP);

  return Result;
}
#endif

#if 0
inline void
SetupAndBuildExteriorBoundary(
  world *World,
  world_chunk *Chunk,
  voxel_position OffsetVector,
  chunk_flag Flag)
{
  if ( IsSet(Chunk, Flag ) )
  {
    world_chunk *Neighbor = GetWorldChunk( World, Chunk->WorldP + OffsetVector );

    if ( Neighbor && IsSet( Neighbor, Chunk_VoxelsInitialized) )
    {
      UnSetFlag( Chunk, Flag );
      BuildExteriorBoundaryVoxels( Chunk, World->ChunkDim, Neighbor, OffsetVector);
    }
  }

}
#endif

#if 0
aabb
FindBoundaryVoxelsAABB(chunk_data *Chunk, chunk_dimension Dim)
{

  chunk_dimension MinP = Dim;
  chunk_dimension MaxP = {};

  for ( s32 VoxelIndex = 0;
        VoxelIndex < Chunk->BoundaryVoxelCount;
        ++VoxelIndex)
  {
    voxel_position P = Chunk->BoundaryVoxels[VoxelIndex].Offset;

    if (P.x < MinP.x)
      MinP.x = P.x;
    if (P.x > MaxP.x)
      MaxP.x = P.x;

    if (P.y < MinP.y)
      MinP.y = P.y;
    if (P.y > MaxP.y)
      MaxP.y = P.y;

    if (P.z < MinP.z)
      MinP.z = P.z;
    if (P.z > MaxP.z)
      MaxP.z = P.z;
  }

  v3 Center = V3(MaxP - MinP);
  v3 Radius = Center/2;

  return aabb( Center, Radius );
}
#endif


link_internal edge*
Edge(voxel_position P0, voxel_position P1, memory_arena* Memory)
{
  edge* Result = Allocate(edge, Memory, 1);
  Result->P0 = P0;
  Result->P1 = P1;
  Result->LengthSq = LengthSq(P1 - P0);

  return Result;
}

#if 0
link_internal void
Triangulate(untextured_3d_geometry_buffer* Dest, world_chunk* Chunk, chunk_dimension ChunkDim, memory_arena* TempMem)
{
  TIMED_FUNCTION();

  u32 WorldChunkVolume = Volume(ChunkDim);


  u32 EdgesAddedCount = 0;
  edge** EdgesAdded = Allocate(edge*, TempMem, WorldChunkVolume);

  u32 TriangleCount = 0;
  triangle** Triangles = Allocate(triangle*, TempMem, WorldChunkVolume);

  boundary_voxels BoundaryVoxelList = GetBoundingVoxels(Chunk, TempMem);
  voxel_position* BoundaryVoxels = BoundaryVoxelList.Points;

  if (BoundaryVoxelList.Count >= 3)
  {
    u32 LastBoundaryVoxelIndex = BoundaryVoxelList.Count -1;
    triangle* BaseTriangle = 0;
    {
      voxel_position P0 = BoundaryVoxels[0];
      Swap(BoundaryVoxels, BoundaryVoxels + LastBoundaryVoxelIndex);
      --LastBoundaryVoxelIndex;

      edge* ShortestEdge = 0;
      edge* SecondShortestEdge = 0;

      u32 ShortestEdgeIndex = 0;
      u32 SecondShortestEdgeIndex = 0;

      for (u32 InnerIndex = 0;
          InnerIndex < LastBoundaryVoxelIndex;
          ++InnerIndex)
      {
        voxel_position P1 = BoundaryVoxels[InnerIndex];
        edge* TestEdge = Edge(P0, P1, TempMem);

        if (!ShortestEdge)
        {
          ShortestEdge = TestEdge;
          SecondShortestEdge = TestEdge;
        }

        if (TestEdge->LengthSq < ShortestEdge->LengthSq)
        {
          SecondShortestEdge = ShortestEdge;
          SecondShortestEdgeIndex = ShortestEdgeIndex;
          ShortestEdge = TestEdge;
          ShortestEdgeIndex = InnerIndex;
        }
        else if (TestEdge->LengthSq < SecondShortestEdge->LengthSq)
        {
          SecondShortestEdge = TestEdge;
          SecondShortestEdgeIndex = InnerIndex;
        }
      }

      Swap(BoundaryVoxels + ShortestEdgeIndex, BoundaryVoxels + LastBoundaryVoxelIndex);
      --LastBoundaryVoxelIndex;

      Swap(BoundaryVoxels + SecondShortestEdgeIndex, BoundaryVoxels + LastBoundaryVoxelIndex);
      --LastBoundaryVoxelIndex;

      edge* ThirdEdge = Edge(ShortestEdge->P1, SecondShortestEdge->P1, TempMem);

      EdgesAdded[EdgesAddedCount++] = ShortestEdge;
      EdgesAdded[EdgesAddedCount++] = SecondShortestEdge;
      EdgesAdded[EdgesAddedCount++] = ThirdEdge;

      BaseTriangle = Triangle(ShortestEdge, SecondShortestEdge, ThirdEdge, TempMem);
      Triangles[TriangleCount++] = BaseTriangle;
    }


    voxel_position Minimum = Min(BaseTriangle->Edges[0]->P0, BaseTriangle->Edges[0]->P1);
    Minimum = Min(Minimum, BaseTriangle->Edges[1]->P1);

    voxel_position Maximum = Max(BaseTriangle->Edges[0]->P0, BaseTriangle->Edges[0]->P1);
    Maximum = Max(Maximum, BaseTriangle->Edges[1]->P1);

    v3 Radius = (V3(Maximum) - V3(Minimum)) / 2.0f;
    v3 Center = V3(Minimum) + Radius;

    aabb MeshAABB(Center, Radius);

    while (LastBoundaryVoxelIndex)
    {
      r32 ShortestDistance = f32_MAX;
      u32 ShortestDistanceIndex = UINT_MAX;
      v3 ShortestP = V3(f32_MAX);
      for (u32 BoundaryVoxelIndex = 0;
          BoundaryVoxelIndex < LastBoundaryVoxelIndex;
          ++BoundaryVoxelIndex)
      {
        v3 P = V3(BoundaryVoxels[BoundaryVoxelIndex]);
        r32 DistanceToAABBCenter = LengthSq(P - Center);
        if (DistanceToAABBCenter < ShortestDistance)
        {
          ShortestDistance = DistanceToAABBCenter;
          ShortestDistanceIndex = BoundaryVoxelIndex;
          ShortestP = P;
        }
      }

      r32 LeastEdgeSum = f32_MAX;
      edge* FoundEdge = 0;
      for (u32 EdgeIndex = 0;
          EdgeIndex < EdgesAddedCount;
          ++EdgeIndex)
      {
        edge* CurrentEdge = EdgesAdded[EdgeIndex];

        r32 TotalDistance = LengthSq(CurrentEdge->P0 - ShortestP);
        TotalDistance += LengthSq(CurrentEdge->P1 - ShortestP);

        if (TotalDistance < LeastEdgeSum)
        {
          LeastEdgeSum = TotalDistance;
          FoundEdge = CurrentEdge;
        }
      }

      edge* NewEdge0 = Edge(FoundEdge->P0, BoundaryVoxels[ShortestDistanceIndex], TempMem);
      edge* NewEdge1 = Edge(FoundEdge->P1, BoundaryVoxels[ShortestDistanceIndex], TempMem);
      EdgesAdded[EdgesAddedCount++] = NewEdge0;
      EdgesAdded[EdgesAddedCount++] = NewEdge1;

      Swap(BoundaryVoxels+ShortestDistanceIndex, BoundaryVoxels+LastBoundaryVoxelIndex);
      --LastBoundaryVoxelIndex;

      triangle* NewTri = Triangle(FoundEdge, NewEdge0, NewEdge1, TempMem);
      Triangles[TriangleCount++] = NewTri;
    }
  }

  for (u32 TriangleIndex = 0;
      TriangleIndex < TriangleCount;
      ++TriangleIndex)
  {
    BufferTriangle(Dest, Triangles[TriangleIndex], RED);
  }

  return;
}
#endif

inline b32
IsBoundaryVoxel(world_chunk *Chunk, voxel_position Offset, chunk_dimension Dim)
{
  s32 VoxelIndex = GetIndex(Offset, Dim);
  voxel *V = &Chunk->Voxels[VoxelIndex];

  b32 Result = False;
  Result |= IsSet( V, Voxel_BackFace);
  Result |= IsSet( V, Voxel_FrontFace);
  Result |= IsSet( V, Voxel_TopFace);
  Result |= IsSet( V, Voxel_BottomFace);
  Result |= IsSet( V, Voxel_LeftFace);
  Result |= IsSet( V, Voxel_RightFace);

  return Result;
}

inline void
CheckAndIncrementCurrentP( world_chunk *Chunk, chunk_dimension Dim, voxel_position *CurrentP, s32 *CurrentClosestDistanceSq, voxel_position TargetP, voxel_position TestP)
{
  if ( IsInsideDim(Dim, TestP) )
  {
    s32 DistSq = LengthSq(TargetP - TestP);

    if ( (DistSq > *CurrentClosestDistanceSq) && IsBoundaryVoxel(Chunk, TestP, Dim) )
    {
      *CurrentP = TestP;
      *CurrentClosestDistanceSq = DistSq;
    }
  }

  return;
}

voxel_position
TraverseSurfaceToBoundary( world *World, world_chunk *Chunk, voxel_position StartingP, voxel_position IterDir)
{
  s32 CurrentClosestDistanceSq = 0;
  voxel_position TargetP = (IterDir * World->ChunkDim) - IterDir;

  voxel_position CurrentP = StartingP;

  voxel_position Up      = Voxel_Position(WORLD_Y);
  voxel_position Right   = Voxel_Position(WORLD_X);
  voxel_position Forward = Voxel_Position(WORLD_Z);


  voxel_position LoopStartingP = {};
  while (IsInsideDim(World->ChunkDim, CurrentP) )
  {
    LoopStartingP = CurrentP;

    // Single axies
    voxel_position PUp      = CurrentP + Up;
    voxel_position PDown    = CurrentP - Up;

    voxel_position PRight    = CurrentP + Right;
    voxel_position PLeft     = CurrentP - Right;

    voxel_position PForward = CurrentP + Forward;
    voxel_position PBack    = CurrentP - Forward;

    // Diagonal axies
    voxel_position PUpRight   = PUp   + Right;
    voxel_position PUpLeft    = PUp   - Right;
    voxel_position PDownRight = PDown + Right;
    voxel_position PDownLeft  = PDown - Right;

    // Forward diagonals
    voxel_position FwdUpRight   = PUpRight   + Forward;
    voxel_position FwdUpLeft    = PUpLeft    + Forward;
    voxel_position FwdDownRight = PDownRight + Forward;
    voxel_position FwdDownLeft  = PDownLeft  + Forward;

    // Backward diagonals
    voxel_position BackUpRight   = PUpRight   - Forward;
    voxel_position BackUpLeft    = PUpLeft    - Forward;
    voxel_position BackDownRight = PDownRight - Forward;
    voxel_position BackDownLeft  = PDownLeft  - Forward;

    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PUp);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PDown);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PLeft);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PForward);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PBack);

    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PUpRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PUpLeft);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PDownRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, PDownLeft);

    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, FwdUpRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, FwdUpLeft);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, FwdDownRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, FwdDownLeft);

    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, BackUpRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, BackUpLeft);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, BackDownRight);
    CheckAndIncrementCurrentP(Chunk, World->ChunkDim, &CurrentP, &CurrentClosestDistanceSq, TargetP, BackDownLeft);

    if (LoopStartingP == CurrentP)
      break;
  }

  return CurrentP;
}

// NOTE(Jesse): Pretty sure this function can be deleted
#if 0
link_internal world_chunk* GetWorldChunk( world *World, world_position P, chunk_dimension VisibleRegion);

b32
CanBuildWorldChunkBoundary(world *World, world_chunk *Chunk, chunk_dimension VisibleRegion)
{
  // FIXME(Jesse): This is _real_ bad for the cache!
  b32 Result = True;

  world_position ChunkP = Chunk->WorldP;


  world_position Left  = ChunkP - Voxel_Position(1, 0, 0);
  world_position Right = ChunkP + Voxel_Position(1, 0, 0);
  world_position Top   = ChunkP + Voxel_Position(0, 1, 0);
  world_position Bot   = ChunkP - Voxel_Position(0, 1, 0);
  world_position Front = ChunkP + Voxel_Position(0, 0, 1);
  world_position Back  = ChunkP - Voxel_Position(0, 0, 1);

  // We could try bailing early to save the cache sometimes
  world_chunk *TestChunk = GetWorldChunk( World, Left , VisibleRegion);
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  TestChunk = GetWorldChunk( World, Right, VisibleRegion );
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  TestChunk = GetWorldChunk( World, Top, VisibleRegion );
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  TestChunk = GetWorldChunk( World, Bot, VisibleRegion );
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  TestChunk = GetWorldChunk( World, Front, VisibleRegion );
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  TestChunk = GetWorldChunk( World, Back, VisibleRegion );
  Result &= TestChunk && IsSet(TestChunk, Chunk_VoxelsInitialized);

  return Result;
}
#endif

void
DrawFolie(untextured_3d_geometry_buffer *Mesh, aabb *AABB)
{
  v3 RenderP = AABB->Center;
  DrawVoxel( Mesh, RenderP, GREY, AABB->Radius*2);

  return;
}

void
DrawParticle(untextured_3d_geometry_buffer *Source, untextured_3d_geometry_buffer *Dest, u8 ColorIndex)
{
  v4 FaceColors[VERTS_PER_FACE];
  FillColorArray(ColorIndex, FaceColors, DefaultPalette, VERTS_PER_FACE);
  BufferVertsChecked( Source, Dest );
  return;
}

void
BufferEntity( untextured_3d_geometry_buffer* Dest, entity *Entity, animation *Animation, graphics *Graphics, chunk_dimension WorldChunkDim, r32 dt)
{
  TIMED_FUNCTION();
  // Debug light code
  /* v3 LightP = GetRenderP(world, Entity->P + Entity->Model.Dim/2); */
  /* glUniform3fv(RG->LightPID, 1, &LightP[0]); */
  //

  if (Spawned(Entity))
  {
#if DEBUG_DRAW_COLLISION_VOLUMES
    aabb AABB = GetRenderSpaceAABB(WorldChunkDim, Entity, Graphics->Camera);
    auto CopyDest = ReserveBufferSpace(Dest, VERTS_PER_AABB);
    DEBUG_DrawAABB(&CopyDest, AABB, PINK);
#endif

    v3 AnimationOffset = {};
    if (Animation)
    {
      Animation->t += dt;
      AnimationOffset = GetInterpolatedPosition(Animation);
    }

    BufferChunkMesh(Graphics, Dest, &Entity->Model.Mesh, WorldChunkDim, Entity->P.WorldP, Entity->Scale, Entity->P.Offset + AnimationOffset, Entity->Rotation);
  }

  return;
}

untextured_3d_geometry_buffer
ReserveBufferSpace(untextured_3d_geometry_buffer* Reservation, u32 ElementsToReserve)
{
  TIMED_FUNCTION();
  Assert(ElementsToReserve);

  untextured_3d_geometry_buffer Result = {};

  for (;;)
  {
    umm ReservationAt = Reservation->At;
    umm ReservationRequest = ReservationAt + ElementsToReserve;
    if (ReservationRequest < Reservation->End)
    {
      if ( AtomicCompareExchange(&Reservation->At, (u32)ReservationRequest, (u32)ReservationAt) )
      {
        Result.Verts = Reservation->Verts + ReservationAt;
        Result.Colors = Reservation->Colors + ReservationAt;
        Result.Normals = Reservation->Normals + ReservationAt;
        Result.End = ElementsToReserve;

        break;
      }
    }
    else
    {
      Warn("Failed to reserve buffer space");
      break;
    }
  }

  return Result;
}

link_internal void
BufferEntities( entity **EntityTable, untextured_3d_geometry_buffer* Dest,
                graphics *Graphics, world *World, r32 dt)
{
  TIMED_FUNCTION();
  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex)
  {
    entity *Entity = EntityTable[EntityIndex];
    BufferEntity( Dest, Entity, 0, Graphics, World->ChunkDim, dt);
  }

  return;
}

link_internal void
Render_BufferGameGeometry(engine_resources *Resources)
{
  platform                  *Plat         = Resources->Plat;
  graphics                  *Graphics     = Resources->Graphics;
  world                     *World        = Resources->World;
  memory_arena              *Memory       = Resources->Memory;
  heap_allocator            *Heap         = &Resources->Heap;
  entity                   **EntityTable  = Resources->EntityTable;
  gpu_mapped_element_buffer *GpuMap       = GetCurrentGpuMap(Graphics);

  BufferWorld(Plat, &GpuMap->Buffer, World, Graphics, Resources->World->VisibleRegion, Heap);
  BufferEntities( EntityTable, &GpuMap->Buffer, Graphics, World, Plat->dt);
}
