
void
RenderAoTexture(v2i ApplicationResolution, ao_render_group *AoGroup)
{
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);
  SetViewport(ApplicationResolution/2);

  GetGL()->UseProgram(AoGroup->Shader.ID);

  GetGL()->Uniform3fv(AoGroup->SsaoKernelUniform, SSAO_KERNEL_SIZE, (r32*)AoGroup->SsaoKernel);

  BindShaderUniforms(&AoGroup->Shader);

  RenderQuad();

  CleanupTextureBindings(&AoGroup->Shader);

  AssertNoGlErrors;
}

void
UpdateLightingTextures(game_lights *Lights)
{
  // TODO(Jesse, id: 120, tags: allocation, speed): Allocate lights such that this swizzle is unneeded
  v3 *PosData = AllocateProtection(v3, GetTranArena(), MAX_LIGHTS, False);
  v3 *ColorData = AllocateProtection(v3, GetTranArena(), MAX_LIGHTS, False);

  for (s32 LightIndex = 0;
      LightIndex < Lights->Count;
      ++LightIndex)
  {
    PosData[LightIndex] = Lights->Lights[LightIndex].Position;
    ColorData[LightIndex] = Lights->Lights[LightIndex].Color;
  }

  u32 Type = GL_TEXTURE_2D;

  GetGL()->BindTexture(Type, Lights->PositionTex.ID);
  GetGL()->TexImage2D( Type, 0, GL_RGB32F,
                Lights->PositionTex.Dim.x, Lights->PositionTex.Dim.y,
                0,  GL_RGB, GL_FLOAT, PosData);
  AssertNoGlErrors;

  GetGL()->BindTexture(Type, Lights->ColorTex.ID);
  GetGL()->TexImage2D( Type, 0, GL_RGB32F,
                Lights->ColorTex.Dim.x, Lights->ColorTex.Dim.y,
                0,  GL_RGB, GL_FLOAT, ColorData);
  AssertNoGlErrors;

  return;
}

#if 0
link_internal void
Debug_DrawTextureToDebugQuad( shader *DebugShader )
{
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(256));

  GetGL()->UseProgram(DebugShader->ID);
  BindShaderUniforms(DebugShader);

  RenderQuad();

  CleanupTextureBindings(&AoGroup->Shader);

  AssertNoGlErrors;
}
#endif

inline m4
GetShadowMapMVP(world *World, camera *Camera, v3 SunP)
{
  m4 depthProjectionMatrix = Orthographic(SHADOW_MAP_X,
                                          SHADOW_MAP_Y,
                                          SHADOW_MAP_Z_MIN,
                                          SHADOW_MAP_Z_MAX);

  v3 Target = {};
  v3 Up = V3(0.f, 1.f, 0.f);
  m4 depthViewMatrix =  LookAt(SunP, Target, Up);

  return depthProjectionMatrix * depthViewMatrix;
}

link_internal void
RenderImmediateGeometryToShadowMap(world *World, graphics *Graphics, gpu_mapped_element_buffer *GpuMap)
{
  TIMED_FUNCTION();

  shadow_render_group *SG = Graphics->SG;

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  SetViewport(GetShadowMapResolution(&GetEngineResources()->Settings));

  UseShader(&SG->Shader);

  Draw(GpuMap->Buffer.At);

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);

  return;
}

link_internal void
RenderImmediateGeometryToGBuffer(v2i ApplicationResolution, triple_buffered_gpu_mapped_element_buffer *ImmediateGeometry, graphics *Graphics)
{
  TIMED_FUNCTION();


  auto Handles = CurrentHandles(ImmediateGeometry);
  Assert(Handles->Mapped == False);

  auto GBufferRenderGroup = Graphics->gBuffer;

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, GBufferRenderGroup->FBO.ID);
  GetGL()->UseProgram(GBufferRenderGroup->gBufferShader.ID);

  SetViewport(ApplicationResolution);

  BindShaderUniforms(&GBufferRenderGroup->gBufferShader);

  // TODO(Jesse): Hoist this check out of here
  GetGL()->Disable(GL_CULL_FACE);

  /* Draw(GpuMap->Buffer.At); */

  DrawGpuBufferImmediate(Handles, ImmediateGeometry->Buffer.At);
  GetGL()->Enable(GL_CULL_FACE);

  CleanupTextureBindings(&GBufferRenderGroup->gBufferShader);

  AssertNoGlErrors;
}

link_internal void
CompositeGameTexturesAndDisplay( platform *Plat, graphics *Graphics )
{
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(Plat->ScreenDim);

  GetGL()->Enable(GL_BLEND);
  GetGL()->BlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

  UseShader(&Graphics->CompositeGroup.Shader);

  GetGL()->Disable(GL_BLEND);

  RenderQuad();

  AssertNoGlErrors;

  return;
}

// Does lighting on gBuffer textures.  Also composites transparent surfaces
link_internal void
RenderLuminanceTexture(v2i ApplicationResolution, lighting_render_group *Lighting, graphics *Graphics)
{
  SetViewport(ApplicationResolution);

  UpdateLightingTextures(&Graphics->Lighting.Lights);

  // TODO(Jesse): Explain this.
  Graphics->SG->Shader.ViewProjection = NdcToScreenSpace * Graphics->SG->Shader.ViewProjection;

  /* GetGL()->Enable(GL_BLEND); */
  /* GetGL()->BlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); */

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);

/*   GetGL()->Disable(GL_BLEND); */

  {
    UseShader(&Lighting->Program);
    RenderQuad();
  }

  /* { */
  /*   UseShader(&Bloom->Shader); */
    /* FlushBuffersToCard(&Bloom->GpuBuffer); */
  /*   Draw(Bloom->GpuBuffer.Buffer.At); */
  /* } */

  AssertNoGlErrors;
}

link_internal void
GaussianBlurTexture(gaussian_render_group *Group, texture *TexIn, framebuffer *DestFBO)
{
  bool horizontal = true, first_iteration = true;
  u32 amount = 10;
  /* u32 amount = 25; */
  /* u32 amount = 1; */

  AssertNoGlErrors;
  UseShader(&Group->Shader);
  AssertNoGlErrors;

  for (u32 i = 0; i < amount; i++)
  {
    AssertNoGlErrors;
    b32 last_iteration = (i == amount-1);

    if (last_iteration)
    {
      GetGL()->BindFramebuffer(GL_FRAMEBUFFER, DestFBO->ID);
    }
    else
    {
      GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Group->FBOs[horizontal].ID);
    }

    AssertNoGlErrors;
    BindUniformByName(&Group->Shader, "horizontal", horizontal);

    texture *Tex;
    if (first_iteration)
    {
      Tex = TexIn;
    }
    else
    {
      Tex = &Group->Textures[!horizontal];
    }

    /* GetGL()->BindTexture( GL_TEXTURE_2D, Tex->ID ); */
    BindUniformByName(&Group->Shader, "SrcImage", Tex, 0);

    AssertNoGlErrors;
    RenderQuad();

    horizontal = !horizontal;
    if (first_iteration) first_iteration = false;
  }

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* link_internal gpu_mapped_element_buffer * */
/* GetNextGpuMap(graphics *Graphics) */
/* { */
/*   gpu_mapped_element_buffer* GpuMap = Graphics->GpuBuffers + ((Graphics->GpuBufferWriteIndex+1)%2); */
/*   return GpuMap; */
/* } */

/* link_internal gpu_mapped_element_buffer * */
/* GetCurrentGpuMap(graphics *Graphics) */
/* { */
/*   gpu_mapped_element_buffer* GpuMap = Graphics->GpuBuffers + Graphics->GpuBufferWriteIndex; */
/*   return GpuMap; */
/* } */

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

#endif

inline void
ClearFramebuffers(graphics *Graphics, render_to_texture_group *RTTGroup)
{
  TIMED_FUNCTION();

  GetGL()->ClearColor(Graphics->SkyColor.r, Graphics->SkyColor.g, Graphics->SkyColor.b, 1.f);
  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
  GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SetDefaultFramebufferClearColors();

  /* GetGL()->BindFramebuffer(GL_FRAMEBUFFER, RTTGroup->FBO.ID); */
  /* GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName);
  GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->Lighting.FBO.ID);
  GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO(Jesse): Why exactly would this not be necessary?
  /* glBindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName); */
  /* glClear(GL_DEPTH_BUFFER_BIT); */

  for (s32 Index = 0; Index < 2; ++Index)
  {
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->Gaussian.FBOs[Index].ID);
    GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, 0);
  GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  if (Graphics->Settings.BravoilMcGuireOIT)
  {
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->Transparency.FBO.ID);

#if 1
    GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
    {
      u32 Attachments = GL_COLOR_ATTACHMENT0;
      GetGL()->DrawBuffers(1, &Attachments);
      GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    {
      u32 Attachments = GL_COLOR_ATTACHMENT0 + 1;
      GetGL()->DrawBuffers(1, &Attachments);
      GetGL()->ClearColor(1.f, 1.f, 1.f, 1.f);
      GetGL()->Clear(GL_COLOR_BUFFER_BIT);
    }

    {
      // Reset draw buffers
      SetDrawBuffers(&Graphics->Transparency.FBO);
    }
#endif
  }
  else
  {
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->Transparency.FBO.ID);
    GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }


  return;
}

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
  b32 Result = False;

  NotImplemented;
#if 0
  s32 VoxelIndex = GetIndex(Offset, Dim);
  voxel *V = &Chunk->Voxels[VoxelIndex];

  Result |= IsSet( V, Voxel_BackFace);
  Result |= IsSet( V, Voxel_FrontFace);
  Result |= IsSet( V, Voxel_TopFace);
  Result |= IsSet( V, Voxel_BottomFace);
  Result |= IsSet( V, Voxel_LeftFace);
  Result |= IsSet( V, Voxel_RightFace);
#endif

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

void
DrawParticle(untextured_3d_geometry_buffer *Source, untextured_3d_geometry_buffer *Dest, u16 MCV_ColorIndex)
{
  v3 FaceColors[VERTS_PER_FACE];
  FillColorArray(GetColorPalette(), MCV_ColorIndex, FaceColors, VERTS_PER_FACE);
  BufferVertsChecked( Source, Dest );
  return;
}

link_internal void
DrawEntityCollisionVolume(entity *Entity, untextured_3d_geometry_buffer *Dest, graphics *Graphics, v3i WorldChunkDim, v3 RGBColor = RGB_PINK, f32 Thickness = DEFAULT_LINE_THICKNESS)
{
  aabb AABB = GetRenderSpaceAABB(WorldChunkDim, Entity, Graphics->Camera);
  auto CopyDest = ReserveBufferSpace(Dest, VERTS_PER_AABB);
  DEBUG_DrawAABB(&CopyDest, AABB, RGBColor, Thickness);
}

link_internal void
HighlightEntity(engine_resources *Engine, entity *Entity)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  DrawEntityCollisionVolume(Entity, &GpuMap->Buffer, Graphics, World->ChunkDim, RGB_YELLOW);
}


link_internal void
DrawFrustum(world *World, graphics *Graphics, camera *Camera)
{
  NotImplemented;
  /* auto *GpuBuffer = &Graphics->ImmediateGeometry; */
  /* auto Dest = ReserveBufferSpace(GpuBuffer, VERTS_PER_LINE*4); */

  /* v3 SimSpaceP = GetSimSpaceP(World, Camera->CurrentP); */
  /* DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Top.Normal*5.f),    RGB_RED, 0.2f ); */
  /* DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Bottom.Normal*5.f), RGB_BLUE, 0.2f ); */
  /* DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Left.Normal*5.f),   RGB_GREEN, 0.2f ); */
  /* DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Right.Normal*5.f),  RGB_YELLOW, 0.2f ); */
}

link_internal void
RenderTransparencyBuffers(v2i ApplicationResolution, render_settings *Settings, transparency_render_group *Group)
{
  FlushBuffersToCard_gpu_mapped_element_buffer(&Group->GpuBuffer.Handles);

  if (Group->GpuBuffer.Buffer.At)
  {
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

    UseShader(&Group->Shader);

    if (Settings->BravoilMcGuireOIT)
    {
      SetViewport(ApplicationResolution);
      GetGL()->Disable(GL_CULL_FACE);

      GetGL()->Enable(GL_BLEND);
      /* GetGL()->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

      // TODO(Jesse): The portable version requires changing the shader a bit
      /* GetGL()->BlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA); */

      GetGL()->BlendFunci(0, GL_ONE, GL_ONE);
      GetGL()->BlendFunci(1, GL_ONE, GL_ONE);
      /* GetGL()->BlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA); */
      /* GetGL()->BlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

      Draw(Group->GpuBuffer.Buffer.At);

      GetGL()->Disable(GL_BLEND);

      GetGL()->Enable(GL_CULL_FACE);
    }
    else
    {
      GetGL()->Enable(GL_BLEND);
      GetGL()->BlendFunc(GL_ONE, GL_ONE);
      GetGL()->Disable(GL_CULL_FACE);
      /* GetGL()->DepthFunc(GL_LEQUAL); */
      /* GetGL()->DepthFunc(GL_ALWAYS); */

      SetViewport(ApplicationResolution);

      Draw(Group->GpuBuffer.Buffer.At);

      GetGL()->Disable(GL_BLEND);
      GetGL()->Enable(GL_CULL_FACE);
      /* GetGL()->DepthFunc(GL_LEQUAL); */
    }

    Group->GpuBuffer.Buffer.At = 0;
  }
}

link_internal b32
SetupRenderToTextureShader(engine_resources *Engine, texture *Texture, camera *Camera)
{
  b32 Result = (Texture->ID != 0);
  if (Result)
  {
    auto RTTGroup = &Engine->RTTGroup;

    // GL stuff
    {
      GetGL()->BindFramebuffer(GL_FRAMEBUFFER, RTTGroup->FBO.ID);
      GetGL()->BindTexture(GL_TEXTURE_2D, Texture->ID);

      RTTGroup->FBO.Attachments = 0;
      FramebufferTexture(&RTTGroup->FBO, Texture);
      SetDrawBuffers(&RTTGroup->FBO);

      RTTGroup->ViewProjection =
        ProjectionMatrix(Camera, V2(Texture->Dim)) *
        ViewMatrix(Engine->World->ChunkDim, Camera) ;

      // Must come after RTTGroup->ViewProjection is set
      UseShader(&RTTGroup->Shader);

      SetViewport(V2(Texture->Dim));

      GetGL()->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      GetGL()->Enable(GL_DEPTH_TEST);
    }
  }
  return Result;
}

link_internal void
SetupVertexAttribsFor_world_chunk_element_buffer(gpu_element_buffer_handles *Handles)
{
  AssertNoGlErrors;

  GetGL()->EnableVertexAttribArray(VERTEX_POSITION_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_NORMAL_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_COLOR_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_TRANS_EMISS_LAYOUT_LOCATION);
  AssertNoGlErrors;

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->Handles[mesh_VertexHandle]);
  GetGL()->VertexAttribPointer(VERTEX_POSITION_LAYOUT_LOCATION, 3, GL_BYTE, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->Handles[mesh_NormalHandle]);
  GetGL()->VertexAttribPointer(VERTEX_NORMAL_LAYOUT_LOCATION, 3, GL_BYTE, GL_TRUE, 0, (void*)0);
  AssertNoGlErrors;


  // NOTE(Jesse): This is just here to break when the size of these changes,
  // serving as a reminder to update this code.
  const u32 MtlFloatElements = sizeof(matl)/sizeof(u8);
  CAssert(MtlFloatElements == 4);

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->Handles[mesh_MatHandle]);
  /* GetGL()->VertexAttribIPointer(VERTEX_COLOR_LAYOUT_LOCATION, 1, GL_UNSIGNED_INT, 0, 0); */
  GetGL()->VertexAttribIPointer(VERTEX_COLOR_LAYOUT_LOCATION, 1, GL_SHORT, sizeof(matl), Cast(void*, OffsetOf(ColorIndex, matl)));
  GetGL()->VertexAttribIPointer(VERTEX_TRANS_EMISS_LAYOUT_LOCATION, 2, GL_BYTE, sizeof(matl), Cast(void*, OffsetOf(Transparency, matl)) ); // @vertex_attrib_I_pointer_transparency_offsetof
  AssertNoGlErrors;
}


link_internal void
DrawGpuBufferImmediate(gpu_element_buffer_handles *Handles, u32 Count)
{
  AssertNoGlErrors;
  Assert(Handles->Mapped == False);
  Assert(Handles->ElementCount);
  Assert(Handles->VAO);

  auto GL = GetGL();

  GL->BindVertexArray(Handles->VAO);
  /* SetupVertexAttribsFor_u3d_geo_element_buffer(Handles); */
  Draw(Count);
  GL->BindBuffer(GL_ARRAY_BUFFER, 0);
}

link_internal void
DrawGpuBufferImmediate(gpu_element_buffer_handles *Handles)
{
  DrawGpuBufferImmediate(Handles, Handles->ElementCount);
}

poof(
  func gpu_buffer(container_t, buffer_t)
  {

    link_internal void
    CopyBufferIntoBuffer( (buffer_t.name) *Src, (buffer_t.name) *Dest)
    {
      BufferVertsChecked(Src, Dest, {}, V3(1.0f));
    }

    link_internal void
    CopyToGpuBuffer( (buffer_t.name) *Mesh, gpu_mapped_(buffer_t.name) *GpuBuffer)
    {
      gpu_mapped_(buffer_t.name) Dest = MapGpuBuffer_(container_t.name)(&GpuBuffer->Handles);
      CopyBufferIntoBuffer(Mesh, &Dest.Buffer);
      FlushBuffersToCard(&Dest);
    }

    link_internal void
    CopyToGpuBuffer( (buffer_t.name) *Mesh, gpu_element_buffer_handles *Handles)
    {
      gpu_mapped_(buffer_t.name) Dest = MapGpuBuffer_(container_t.name)(Handles);
      CopyBufferIntoBuffer(Mesh, &Dest.Buffer);
      FlushBuffersToCard(&Dest);
    }

    link_internal b32
    SyncGpuBuffersAsync(engine_resources *Engine, (container_t.name) *Meshes)
    {
      b32 Result = False;
      Assert(ThreadLocal_ThreadIndex != 1);

      RangeIterator(MeshIndex, MeshIndex_Count)
      {
        world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
        if (HasMesh(Meshes, MeshBit))
        {
          gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

          (buffer_t.name) *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
          if (Mesh && Mesh->At)
          {
            PushReallocateBuffersCommand(&Engine->Stdlib.Plat.RenderQ, Handles, Mesh);
            Result = True;
          }
        }
      }

      // TODO(Jesse): Is this actually a thing??
      FullBarrier;

      return Result;
    }

    link_internal b32
    SyncGpuBuffersImmediate(engine_resources *Engine, (container_t.name) *Meshes)
    {
      b32 Result = False;
      Assert(ThreadLocal_ThreadIndex == 1);

      RangeIterator(MeshIndex, MeshIndex_Count)
      {
        world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
        if (HasMesh(Meshes, MeshBit))
        {
          gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

          (buffer_t.name) *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
          if (Mesh)
          {
            if (Mesh->At)
            {
              ReallocateAndSyncGpuBuffers(Handles, Mesh);
              Result = True;
            }
            else
            {
              DeallocateGpuBuffer(Handles);
              AssertNoGlErrors;
            }

            DeallocateMesh(Engine, Mesh);
          }
        }
      }

      AssertNoGlErrors;

      // TODO(Jesse): Is this actually a thing??
      FullBarrier;

      return Result;
    }
  }
)

link_internal void
ReallocateAndSyncGpuBuffers(gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh);

/* poof(gpu_buffer(lod_element_buffer, untextured_3d_geometry_buffer)) */
/* #include <generated/gpu_buffer_lod_element_buffer_untextured_3d_geometry_buffer.h> */

link_internal void
DeleteGpuBuffer(gpu_element_buffer_handles *Handles)
{
  Assert(Handles->VAO);
  Assert(Handles->Handles[0]);
  Assert(Handles->Handles[1]);
  Assert(Handles->Handles[2]);
  GetGL()->DeleteVertexArrays(1, &Handles->VAO);
  GetGL()->DeleteBuffers(3, &Handles->Handles[mesh_VertexHandle]);
}

link_internal void
ReallocateGpuBuffers(gpu_element_buffer_handles *Handles, data_type Type, u32 ElementCount)
{
  Assert(False);
  Assert(Handles->Mapped == False);
  if (Handles->VAO)
  {
    DeleteGpuBuffer(Handles);
  }
  Clear(Handles);

  AllocateGpuBuffer_gpu_mapped_element_buffer(Handles, Type, ElementCount);
}

link_internal void
ReallocateAndSyncGpuBuffers(gpu_element_buffer_handles *Handles, untextured_3d_geometry_buffer *Mesh)
{
  ReallocateGpuBuffers(Handles, Mesh->Type, Mesh->At);
  /* CopyToGpuBuffer(Mesh, Handles); */
  NotImplemented;
}

link_internal gpu_mapped_element_buffer
AllocateAndMapGpuBuffer(data_type Type, u32 ElementCount)
{
  gpu_mapped_element_buffer Buf = {};
  AllocateGpuBuffer(&Buf, Type, ElementCount);
  MapGpuBuffer(&Buf);
  return Buf;
}

/* poof(gpu_buffer(world_chunk_lod_element_buffer, world_chunk_geometry_buffer)) */
/* #include <generated/gpu_buffer_world_chunk_lod_element_buffer_world_chunk_geometry_buffer.h> */

link_internal m4
GetTransformMatrix(v3 Basis, v3 Scale, Quaternion Rotation)
{
  m4 Result = Translate(Basis) * ScaleTransform(Scale) * RotateTransform(Rotation) * IdentityMatrix; // TODO(Jesse): wtf are we multiplying by IdentityMatrix for?
  return Result;
}

link_internal m4
GetTransformMatrix(entity *Entity)
{
  v3 Basis = GetRenderP(GetEngineResources(), Entity->P);
  m4 Result = GetTransformMatrix(Basis, V3(Entity->Scale), FromEuler(Entity->EulerAngles));
  return Result;
}

#if 0
link_internal void
DrawLod_world_chunk(engine_resources *Engine, shader *Shader, world_chunk_lod_element_buffer *Meshes, r32 DistanceSquared, v3 Basis, Quaternion Rotation = Quaternion(), v3 Scale = V3(1.f))
{
  UNPACK_ENGINE_RESOURCES(Engine);

  AssertNoGlErrors;
  auto MeshBit = MeshBit_None;

#if 1
  if (DistanceSquared > Square(400*32))
  {
    if (HasGpuMesh(Meshes, MeshBit_Lod4)) { MeshBit = MeshBit_Lod4; }
  }
  else if (DistanceSquared > Square(250*32))
  {
    if (HasGpuMesh(Meshes, MeshBit_Lod3)) { MeshBit = MeshBit_Lod3; }
  }
  else if (DistanceSquared > Square(150*32))
  {
    if (HasGpuMesh(Meshes, MeshBit_Lod2)) { MeshBit = MeshBit_Lod2; }
  }
  else if (DistanceSquared > Square(70*32))
  {
    if (HasGpuMesh(Meshes, MeshBit_Lod1)) { MeshBit = MeshBit_Lod1; }
  }
  else
  {
   if (HasGpuMesh(Meshes, MeshBit_Lod0)) { MeshBit = MeshBit_Lod0; }
  }
#else
  /* if (HasGpuMesh(Meshes, MeshBit_Lod4)) { MeshBit = MeshBit_Lod4; } */
  /* if (HasGpuMesh(Meshes, MeshBit_Lod3)) { MeshBit = MeshBit_Lod3; } */
  /* if (HasGpuMesh(Meshes, MeshBit_Lod2)) { MeshBit = MeshBit_Lod2; } */
  /* if (HasGpuMesh(Meshes, MeshBit_Lod1)) { MeshBit = MeshBit_Lod1; } */
  if (HasGpuMesh(Meshes, MeshBit_Lod0)) { MeshBit = MeshBit_Lod0; }
#endif

  if (MeshBit != MeshBit_None)
  {
    m4 LocalTransform = GetTransformMatrix(Basis, Scale, Rotation);
    AssertNoGlErrors;

    m4 NormalMatrix = Transpose(Inverse(LocalTransform));
    AssertNoGlErrors;

    Ensure(TryBindUniform(Shader, "ModelMatrix", &LocalTransform));
    AssertNoGlErrors;
    TryBindUniform(Shader, "NormalMatrix", &NormalMatrix); // NOTE(Jesse): Not all shaders that use this path draw normals (namely, DepthRTT)
    AssertNoGlErrors;

    auto Handles = &Meshes->GpuBufferHandles[ToIndex(MeshBit)];

    SetupVertexAttribsFor_world_chunk_element_buffer(Handles);
    DrawGpuBufferImmediate(Handles);
    AssertNoGlErrors;
  }
}
#endif

#if 1
link_internal void
poof(@async @render)
DrawLod( engine_resources *Engine,
         shader *Shader,
         gpu_element_buffer_handles *Handles,
         v3 Basis,
         Quaternion Rotation,
         v3 Scale )
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Engine);

  AssertNoGlErrors;
  Assert(Handles->Mapped == False);

  if (HasGpuMesh(Handles))
  {
    m4 ModelMatrix = GetTransformMatrix(Basis*GLOBAL_RENDER_SCALE_FACTOR, Scale*GLOBAL_RENDER_SCALE_FACTOR, Rotation);
    TryBindUniform(Shader, "ModelMatrix", &ModelMatrix);

    m4 NormalMatrix = Transpose(Inverse(ModelMatrix));
    TryBindUniform(Shader, "NormalMatrix", &NormalMatrix); // NOTE(Jesse): Not all shaders that use this path draw normals (namely, DepthRTT)

    DrawGpuBufferImmediate(Handles);
    AssertNoGlErrors;
  }
}
#endif

#if 0
link_internal void
DrawLod(engine_resources *Engine, shader *Shader, lod_element_buffer *Meshes, r32 DistanceSquared, v3 Basis, Quaternion Rotation, v3 Scale )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  AssertNoGlErrors;
  auto MeshBit = MeshBit_None;

  if (HasGpuMesh(Meshes, MeshBit_Lod0))
  {
    m4 LocalTransform = GetTransformMatrix(Basis*GLOBAL_RENDER_SCALE_FACTOR, Scale*GLOBAL_RENDER_SCALE_FACTOR, Rotation);
    m4 NormalMatrix = Transpose(Inverse(LocalTransform));

    // @janky_model_matrix_bs
    Ensure(TryBindUniform(Shader, "ModelMatrix", &LocalTransform));
    AssertNoGlErrors;
    TryBindUniform(Shader, "NormalMatrix", &NormalMatrix); // NOTE(Jesse): Not all shaders that use this path draw normals (namely, DepthRTT)
    AssertNoGlErrors;

    auto Handles = &Meshes->GpuBufferHandles[ToIndex(MeshBit_Lod0)];

    SetupVertexAttribsFor_u3d_geo_element_buffer(Handles);
    DrawGpuBufferImmediate(Handles);
    AssertNoGlErrors;
  }
}
#endif

#if 0
link_internal void
RenderToTexture_world_chunk(engine_resources *Engine, asset_thumbnail *Thumb, world_chunk_lod_element_buffer *Meshes, v3 Offset, camera *Camera = 0)
{
  if (Camera == 0) { Camera = &Thumb->Camera; }
  if (SetupRenderToTextureShader(Engine, &Thumb->Texture, Camera))
  {
    DrawLod_world_chunk(Engine, &Engine->RTTGroup.Shader, Meshes, 0.f, Offset);
  }
  else
  {
    Warn("Attempted to render to an unallocated texture.");
  }
}
#endif

/* link_internal void */
/* RenderToTexture_Async(engine_resources *Engine, asset_thumbnail *Thumb, world_chunk_lod_element_buffer *Meshes, v3 Offset, camera *Camera = 0) */
/* { */
/*   NotImplemented; */
/* } */

link_internal void
RenderToTexture(engine_resources *Engine, asset_thumbnail *Thumb, gpu_mapped_element_buffer *Mesh, v3 Offset, camera *Camera = 0)
{
  if (Camera == 0) { Camera = &Thumb->Camera; }
  if (SetupRenderToTextureShader(Engine, &Thumb->Texture, Camera))
  {
    DrawLod(Engine, &Engine->RTTGroup.Shader, &Mesh->Handles, Offset);
  }
  else
  {
    Warn("Attempted to render to an unallocated texture.");
  }
}

link_internal void
RenderToTexture(engine_resources *Engine, asset_thumbnail *Thumb, model *Model, v3 Offset, camera *Camera = 0)
{
  RenderToTexture(Engine, Thumb, &Model->Mesh, Offset, Camera);
}

link_internal void
RenderToTexture(engine_resources *Engine, asset_thumbnail *Thumb, untextured_3d_geometry_buffer *Src, v3 Offset, camera *Camera = 0)
{
  if (Camera == 0) { Camera = &Thumb->Camera; }
  if (SetupRenderToTextureShader(Engine, &Thumb->Texture, Camera))
  {
    auto RTTGroup = &Engine->RTTGroup;

    // Geometry stuff
    {
      MapGpuBuffer(&RTTGroup->GeoBuffer);
      untextured_3d_geometry_buffer* Dest = &RTTGroup->GeoBuffer.Buffer;

      BufferVertsChecked(Src, Dest, Offset, V3(1.0f));
      FlushBuffersToCard_gpu_mapped_element_buffer(&RTTGroup->GeoBuffer.Handles);
    }

    GetGL()->Enable(GL_DEPTH_TEST);
    Draw(RTTGroup->GeoBuffer.Buffer.At);
    RTTGroup->GeoBuffer.Buffer.At = 0;
  }
  else
  {
    Warn("Attempted to render to an unallocated texture.");
  }
}

link_internal void
DrawEntity(              shader *Shader,
  untextured_3d_geometry_buffer *Dest,
  untextured_3d_geometry_buffer *TransparentDest,
                         entity *Entity,
                      animation *Animation,
                       graphics *Graphics, 
                chunk_dimension  WorldChunkDim,
                r32 dt )
{
  TIMED_FUNCTION();

  if (Spawned(Entity))
  {
    // gBuffer, once for shadow) so we should move this to the simulation loop
    // TODO(Jesse): This function gets called twice per frame (once for
    v3 AnimationOffset = {};
    {
      if (GetEngineDebug()->DrawEntityCollisionVolumes)
      {
        DrawEntityCollisionVolume(Entity, Dest, Graphics, WorldChunkDim);
      }

      if (Animation)
      {
        Animation->t += dt;
        AnimationOffset = GetInterpolatedPosition(Animation);
      }
    }

    maybe_asset_ptr MaybeAsset = GetAssetPtr(GetEngineResources(), &Entity->AssetId);
    if ( MaybeAsset.Tag )
    {

      if (MaybeAsset.Value->LoadState == AssetLoadState_Allocated)
      {
        Assert(MaybeAsset.Value->Id.Index != INVALID_ASSET_INDEX);
        QueueAssetForLoad(&GetEngineResources()->Stdlib.Plat.LowPriority, MaybeAsset.Value);
      }

      if (MaybeAsset.Value->LoadState == AssetLoadState_Loaded )
      {
        model *Model = GetModel(MaybeAsset.Value, &Entity->AssetId, Entity->ModelIndex);

        if (Entity->Behavior & EntityBehaviorFlags_FitCollisionVolumeToModel)
        {
          Entity->Behavior = entity_behavior_flags(Entity->Behavior & ~EntityBehaviorFlags_FitCollisionVolumeToModel);
          Entity->_CollisionVolumeRadius = Model->Dim/2.f;
        }

        // TODO(Jesse): Do we still do this here?
        NotImplemented;
        /* SyncGpuBuffersImmediate(GetEngineResources(), &Model->Mesh); */
        AssertNoGlErrors;

        v3 Offset = AnimationOffset + Entity->Scale*(V3(Model->Dim)/2.f);
        v3 Basis = GetRenderP(GetEngineResources(), Entity->P) + Offset;
        AssertNoGlErrors;

        DrawLod(GetEngineResources(), Shader, &Model->Mesh.Handles, Basis, FromEuler(Entity->EulerAngles), V3(Entity->Scale));
      }
    }
  }
}

link_internal void
SetupGBufferShader(graphics *Graphics, v2i ApplicationResolution, b32 DoSelectionMasking)
{
  g_buffer_render_group *GBufferRenderGroup = Graphics->gBuffer;

  if (DoSelectionMasking)
  {
    auto SelectionRegion = GetLevelEditor()->Selection.Region;
    SelectionRegion.Min.Offset += V3(0.0001f);
    SelectionRegion.Max.Offset -= V3(0.0001f);
    Graphics->MinClipP_worldspace = GetRenderP(GetEngineResources(), SelectionRegion.Min);
    Graphics->MaxClipP_worldspace = GetRenderP(GetEngineResources(), SelectionRegion.Max);
  }
  else
  {
    Graphics->MinClipP_worldspace = {};
    Graphics->MaxClipP_worldspace = {};
  }

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, GBufferRenderGroup->FBO.ID);
  GetGL()->UseProgram(GBufferRenderGroup->gBufferShader.ID);

  SetViewport(ApplicationResolution);

  BindShaderUniforms(&GBufferRenderGroup->gBufferShader);

  GetGL()->Disable(GL_CULL_FACE);

  AssertNoGlErrors;
}

link_internal void
TeardownGBufferShader(graphics *Graphics)
{
  auto GBufferRenderGroup = Graphics->gBuffer;
  CleanupTextureBindings(&GBufferRenderGroup->gBufferShader);
  GetGL()->Enable(GL_CULL_FACE);
}




link_internal void
SetupShadowMapShader(world *World, graphics *Graphics, v2i ShadowMapResolution, b32 DoSelectionMasking)
{
  shadow_render_group *SG = Graphics->SG;

  if (DoSelectionMasking)
  {
    auto SelectionRegion = GetLevelEditor()->Selection.Region;
    SelectionRegion.Min.Offset += V3(0.0001f);
    SelectionRegion.Max.Offset -= V3(0.0001f);
    Graphics->MinClipP_worldspace = GetRenderP(GetEngineResources(), SelectionRegion.Min);
    Graphics->MaxClipP_worldspace = GetRenderP(GetEngineResources(), SelectionRegion.Max);
  }
  else
  {
    Graphics->MinClipP_worldspace = {};
    Graphics->MaxClipP_worldspace = {};
  }

  GetGL()->BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  SetViewport(ShadowMapResolution);

  UseShader(&SG->Shader);

  GetGL()->Disable(GL_CULL_FACE);

  AssertNoGlErrors;
}

link_internal void
TeardownShadowMapShader(graphics *Graphics)
{
  GetGL()->Enable(GL_CULL_FACE);
  AssertNoGlErrors;
}




link_internal void
DrawEntities( shader *Shader,
              entity **EntityTable,
              untextured_3d_geometry_buffer* Dest,
              untextured_3d_geometry_buffer* TransparencyDest,
              graphics *Graphics, world *World, r32 dt)
{
  TIMED_FUNCTION();

  RangeIterator(EntityIndex, TOTAL_ENTITY_COUNT)
  {
    entity *Entity = EntityTable[EntityIndex];
    DrawEntity(Shader,  Dest, TransparencyDest, Entity, 0, Graphics, World->ChunkDim, dt);
  }
}

link_internal void
DrawEntitiesToGBuffer( v2i ApplicationResolution,
                       entity **EntityTable,
                       untextured_3d_geometry_buffer* Dest,
                       untextured_3d_geometry_buffer* TransparencyDest,
                       graphics *Graphics, world *World, r32 dt)
{
  TIMED_FUNCTION();

  NotImplemented; // NOTE: Unused

  b32 OldMajorGrid = Graphics->Settings.DrawMajorGrid;
  b32 OldMinorGrid = Graphics->Settings.DrawMinorGrid;

  Graphics->Settings.DrawMajorGrid = False;
  Graphics->Settings.DrawMinorGrid = False;

  SetupGBufferShader(Graphics, ApplicationResolution, False);

  DrawEntities(&Graphics->gBuffer->gBufferShader, EntityTable, Dest, TransparencyDest, Graphics, World, dt);

  Graphics->Settings.DrawMajorGrid = OldMajorGrid;
  Graphics->Settings.DrawMinorGrid = OldMinorGrid;

  TeardownGBufferShader(Graphics);
}

link_internal void
ComputeDrawListsAndQueueUnallocatedChunks(engine_resources *Engine)
{
  NotImplemented;
#if 0
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Engine);

  world_chunk_ptr_paged_list *MainDrawList = &Graphics->MainDrawList;
  world_chunk_ptr_paged_list *ShadowMapDrawList = &Graphics->ShadowMapDrawList;

  // Reset world_chunk draw lists
  {
    Clear(MainDrawList);
    Clear(ShadowMapDrawList);

    MainDrawList->Memory = GetTranArena();
    ShadowMapDrawList->Memory = GetTranArena();
  }

  v3i Radius = World->VisibleRegion/2;
  v3i Min = World->Center - Radius;
  v3i Max = World->Center + Radius;

  // NOTE(Jesse): Made this z-major iteration as a convienience when
  // initializing large, flat-ish worlds.  If it's z-major there aren't large
  // pauses for big underground regions/air regions.
  for (s32 x = Min.x; x < Max.x; ++ x)
  for (s32 y = Min.y; y < Max.y; ++ y)
  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    world_position  P     = World_Position(x,y,z);
    world_chunk    *Chunk = GetWorldChunkFromHashtable( World, P );
    if (Chunk)
    {
      if (HasCpuMesh(&Chunk->Meshes))
      {
        SyncGpuBuffersAsync(Engine, &Chunk->Meshes);
      }

      if (HasGpuMesh(&Chunk->Meshes))
      {
        Push(ShadowMapDrawList, &Chunk);
        if (IsInFrustum(World, Camera, Chunk))
        {
          Push(MainDrawList, &Chunk);
        }
      }
    }
    else
    {
      Chunk = GetAndInsertFreeWorldChunk(World, P);
      if (Chunk)
      { QueueChunkForInit(&Plat->LowPriority, Chunk, MeshBit_Lod0);  }
      else
      { InvalidCodePath(); }
    }
  }
#endif
}

link_internal void
poof(@async @render)
CheckOcclusionQuery(world_chunk *Chunk)
{
  auto GL = GetGL();

  if (Chunk->OcclusionFrames == 0 && Chunk->QueryActive)
  {
    s32 Available = 0;
    GL->GetQueryObjectiv(Chunk->OcclusionQueryId, GL_QUERY_RESULT_AVAILABLE, &Available);
    if (Available)
    {
      Chunk->QueryActive = False;

      s32 FragmentsWritten = 0;
      GL->GetQueryObjectiv(Chunk->OcclusionQueryId, GL_QUERY_RESULT, &FragmentsWritten);

      if (FragmentsWritten == 0)
      {
        local_persist random_series OcclusionEntropy = {43654747355143};
        Chunk->OcclusionFrames = RandomBetween(0u, &OcclusionEntropy, 20u);
        Info("Occluded");
      }
      else
      {
        Info("Visible");
      }
    }
  }
}

link_internal void
RenderDrawList(engine_resources *Engine, octree_node_ptr_paged_list *DrawList, shader *Shader, camera *Camera)
{
  world *World = Engine->World;;

  sort_key_f32 *Keys = Allocate(sort_key_f32, GetTranArena(), DrawList->ElementCount);

  {
    IterateOver(DrawList, Node, KeyIndex)
    {
      sort_key_f32 *Key = Keys + KeyIndex.Index;

      Key->Index = u64(Node);
      if (Camera)
      {
        Key->Value = Distance(GetSimSpaceP(Engine->World, Node->WorldP), GetSimSpaceP(Engine->World, Camera->CurrentP));
      }
    }
  }

  if (Camera)
  {
    BubbleSort_descending(Keys, DrawList->ElementCount);
  }

  RangeIterator_t(u32, KeyIndex, DrawList->ElementCount)
  {
    sort_key_f32 *Key = Keys + KeyIndex;
    octree_node *Node = Cast(octree_node *, Key->Index);

    world_chunk *Chunk = Node->Chunk;
    Assert(Chunk);

    // In case gpu meshes got deallocated after the chunk was added to the draw list
    if (HasGpuMesh(Chunk))
    {
      if (Chunk->OcclusionFrames) { Chunk->OcclusionFrames--; continue; }

      v3 Offset = V3(Node->Resolution);
      /* v3 Offset = V3(Node->Resolution*0.5f); */
      /* v3 Offset = -1.f*V3(Node->Resolution*0.5f); */
      /* v3 Offset = {}; */
      v3 Basis = Offset;
      if (Camera)
      {
        Basis += GetRenderP(World->ChunkDim, Chunk->WorldP, Camera);
      }
      else
      {
        Basis += GetSimSpaceP(World, Chunk->WorldP);
      }

      if (Camera)
      {
        auto GL = GetGL();
        if (Chunk->OcclusionQueryId == 0)
        {
          GL->GenQueries(1, &Chunk->OcclusionQueryId);
          Assert(Chunk->OcclusionQueryId);
        }

        if (Chunk->QueryActive == False)
        {
          Chunk->QueryActive = True;
          GL->BeginQuery(GL_SAMPLES_PASSED, Chunk->OcclusionQueryId);
          DrawLod(Engine, Shader, &Chunk->Handles, Basis, Quaternion(), V3(Chunk->DimInChunks));
          GL->EndQuery(GL_SAMPLES_PASSED);
        }
        else
        {
          DrawLod(Engine, Shader, &Chunk->Handles, Basis, Quaternion(), V3(Chunk->DimInChunks));
        }

        /* CheckOcclusionQuery(Chunk); */
      }


      AssertNoGlErrors;
    }
  }
}

link_internal void
RenderDrawList(engine_resources *Engine, world_chunk_ptr_paged_list *DrawList, shader *Shader, camera *Camera)
{
  world *World = Engine->World;;

  IterateOver(DrawList, Chunk, ChunkIndex)
  {
    // In case gpu meshes got deallocated after the chunk was added to the draw list
    if (HasGpuMesh(Chunk))
    {
      v3 Basis;
      if (Camera)
      {
        Basis = GetRenderP(World->ChunkDim, Chunk->WorldP, Camera);
      }
      else
      {
        Basis = GetSimSpaceP(World, Chunk->WorldP);
      }
      DrawLod(Engine, Shader, &Chunk->Handles, Basis, Quaternion(), V3(Chunk->DimInChunks));
      AssertNoGlErrors;
    }
  }
}

#if 0
link_internal void
DrawWorld(engine_resources *Engine, v2i ApplicationResolution)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  RangeIterator_t(u32, ChunkIndex, World->HashSize)
  {
    world_chunk *Chunk = World->ChunkHash[ChunkIndex];
    if (Chunk)
    {
      if (IsInFrustum(World, Camera, Chunk))
      {
        v3 CameraP = GetSimSpaceP(World, Camera->CurrentP);
        v3 ChunkP  = GetSimSpaceP(World, Chunk->WorldP);

        // This is kinda barf.. we'd rather free these when the chunk gets
        // rebuilt but can't do it from another thread.  Maybe when the chunk
        // update job gets pushed is more straight-forward?

        SyncGpuBuffersImmediate(Engine, &Chunk->Meshes);
        v3 Basis = GetRenderP(Engine, Chunk->WorldP);
        DrawLod(Engine, &Graphics->gBuffer->gBufferShader, &Chunk->Meshes, 0.f, Basis);

#if 0
        umm StandingSpotCount = AtElements(&Chunk->StandingSpots);
        /* DebugLine("drawing (%u) standing spots", StandingSpotCount); */
        for (u32 SpotIndex = 0; SpotIndex < StandingSpotCount; ++SpotIndex)
        {
          v3i *Spot = Chunk->StandingSpots.Start + SpotIndex;
          v3 RenderSpot = GetRenderP(World->ChunkDim, Canonical_Position(*Spot, Chunk->WorldP), Graphics->Camera);
          /* DrawStandingSpot(&Graphics->Transparency.GeoBuffer.Buffer, RenderSpot, V3(Global_StandingSpotDim)); */
          DrawStandingSpot(&GpuMap->Buffer, RenderSpot, V3(Global_StandingSpotDim));
        }
#endif

      }
    }
  }
}
#endif

link_internal void
DrawStuffToGBufferTextures(engine_resources *Engine, v2i ApplicationResolution)
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Engine);


}

link_internal void
DrawWorldAndEntitiesToShadowMap(v2i ShadowMapResolution, engine_resources *Engine)
{
  TIMED_FUNCTION();

  UNPACK_ENGINE_RESOURCES(Engine);

  v3i Radius = V3i(World->VisibleRegionSize/2);
  v3i Min = World->Center - Radius;
  v3i Max = World->Center + Radius;

  shadow_render_group *SG = Graphics->SG;

  b32 MaskSelection = False;
  SetupShadowMapShader(World, Graphics, ShadowMapResolution, MaskSelection);

  // NOTE(Jesse): So there's a visual distinction between preview and instantiated
  /* DrawEditorPreview(Engine, &SG->DepthShader); */

  /* DrawEntities( &SG->DepthShader, EntityTable, &GpuMap->Buffer, &Graphics->Transparency.GpuBuffer.Buffer, Graphics, World, Plat->dt); */

  AssertNoGlErrors;

  /* RenderDrawList(Engine, &Graphics->ShadowMapDrawList, &SG->DepthShader); */

  TeardownShadowMapShader(Graphics);
}

link_internal void
UpdateKeyLight(graphics *Graphics, r32 tDay)
{
  auto SG = Graphics->SG;
  r32 tDaytime = Cos(tDay);
  r32 tPostApex = Sin(tDay);

  lighting_settings *Lighting = &Graphics->Settings.Lighting;

  v3 DawnColor = HSVtoRGB(Lighting->DawnHSV) * Lighting->DawnIntensity;
  v3 SunColor  = HSVtoRGB(Lighting->SunHSV ) * Lighting->SunIntensity;
  v3 DuskColor = HSVtoRGB(Lighting->DuskHSV) * Lighting->DuskIntensity;
  v3 MoonColor = HSVtoRGB(Lighting->MoonHSV) * Lighting->MoonIntensity;

  Lighting->SunP.x = Sin(((Graphics->SunBasis.x*PI32)) + tDay);
  Lighting->SunP.y = Cos(((Graphics->SunBasis.y*PI32))+ tDay);
  Lighting->SunP.z = (1.3f+Cos(((Graphics->SunBasis.z*PI32)) + tDay))/2.f;

  if (tDaytime > 0.f)
  {
    if (tPostApex > 0.f)
    {
      Lighting->CurrentSunColor = Lerp(tDaytime, DuskColor, SunColor);
    }
    else
    {
      Lighting->CurrentSunColor = Lerp(tDaytime, DawnColor, SunColor);
    }
  }
  else
  {
    if (tPostApex > 0.f)
    {
      Lighting->CurrentSunColor = Lerp(Abs(tDaytime), DuskColor, MoonColor);
    }
    else
    {
      Lighting->CurrentSunColor = Lerp(Abs(tDaytime), DawnColor, MoonColor);
    }
  }

  switch (Graphics->Settings.ToneMappingType)
  {
    case ToneMappingType_None:
    case ToneMappingType_Reinhard:
    case ToneMappingType_Exposure:
      { } break;

    case ToneMappingType_AGX:
    case ToneMappingType_AGX_Sepia:
    case ToneMappingType_AGX_Punchy:
    {
      if (LengthSq(Lighting->CurrentSunColor) > 1.f)
      {
        Lighting->CurrentSunColor = Normalize(Lighting->CurrentSunColor);
      }
    } break;
  }
}


link_internal void
poof(@async @render)
FinalizeShitAndFuckinDoStuff(gen_chunk *GenChunk, octree_node *DestNode)
{
  world_chunk *DestChunk = DestNode->Chunk;
  world_chunk *SynChunk = &GenChunk->Chunk;
  Assert(HasGpuMesh(&GenChunk->Mesh) == True);
  Assert(HasGpuMesh( SynChunk)       == False);

  // @dest_chunk_can_have_mesh
  /* Assert(HasGpuMesh(DestChunk)       == False); */

  FlushBuffersToCard_gpu_mapped_element_buffer(&GenChunk->Mesh.Handles);

  auto DestHandles = DestChunk->Handles;

  DestChunk->Handles = GenChunk->Mesh.Handles;

  if (HasGpuMesh(&DestHandles))
  {
    DeleteGpuBuffer(&DestHandles);
  }

  GenChunk->Mesh = {};
  Free(&GetEngineResources()->GenChunkFreelist, GenChunk);

  FinalizeNodeInitializaion(DestNode);
}

