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

inline m4
GetShadowMapMVP(light *Sun, v3 FrustumCenter)
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

  v3 Target = FrustumCenter;
  m4 depthViewMatrix =  LookAt(Sun->Position, Target, Up);

  return depthProjectionMatrix * depthViewMatrix;
}

link_internal void
RenderShadowMap(gpu_mapped_element_buffer *GpuMap, graphics *Graphics)
{
  TIMED_FUNCTION();

  shadow_render_group *SG = Graphics->SG;

  GL.BindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);
  GL.UseProgram(SG->DepthShader.ID);

  SetViewport(V2(SHADOW_MAP_RESOLUTION_X, SHADOW_MAP_RESOLUTION_Y));

  v3 FrustCenter = GetFrustumCenter(Graphics->Camera);
  SG->MVP = GetShadowMapMVP(&SG->Sun, FrustCenter);
  GL.UniformMatrix4fv(SG->MVP_ID, 1, GL_FALSE, &SG->MVP.E[0].E[0]);

  Draw(GpuMap->Buffer.At);

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

  return;
}

link_internal void
RenderGBuffer(gpu_mapped_element_buffer *GpuMap, graphics *Graphics)
{
  TIMED_FUNCTION();

  auto GBufferRenderGroup = Graphics->gBuffer;

  GL.BindFramebuffer(GL_FRAMEBUFFER, GBufferRenderGroup->FBO.ID);
  GL.UseProgram(GBufferRenderGroup->gBufferShader.ID);

  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  BindShaderUniforms(&GBufferRenderGroup->gBufferShader);

  FlushBuffersToCard(GpuMap);

  GL.Disable(GL_CULL_FACE);
  Draw(GpuMap->Buffer.At);
  GL.Enable(GL_CULL_FACE);

  AssertNoGlErrors;

  return;
}

link_internal void
CompositeAndDisplay( platform *Plat, graphics *Graphics )
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));

  GL.Enable(GL_BLEND);
  GL.BlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

  UseShader(&Graphics->CompositeGroup.Shader);

  GL.Disable(GL_BLEND);


  RenderQuad();

  AssertNoGlErrors;

  return;
}

// Does lighting on gBuffer textures.  Also composites transparent surfaces
link_internal void
RenderLuminanceTexture(gpu_mapped_element_buffer *GpuMap, lighting_render_group *Lighting, graphics *Graphics)
{
  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  UpdateLightingTextures(&Graphics->Lighting.Lights);

  // TODO(Jesse): Explain this.
  Graphics->SG->MVP = NdcToScreenSpace * Graphics->SG->MVP;

  /* GL.Enable(GL_BLEND); */
  /* GL.BlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA); */

  GL.BindFramebuffer(GL_FRAMEBUFFER, Lighting->FBO.ID);

/*   GL.Disable(GL_BLEND); */

  {
    UseShader(&Lighting->Shader);
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

  UseShader(&Group->Shader);

  for (u32 i = 0; i < amount; i++)
  {
    b32 last_iteration = (i == amount-1);

    if (last_iteration)
    {
      GL.BindFramebuffer(GL_FRAMEBUFFER, DestFBO->ID);
    }
    else
    {
      GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBOs[horizontal].ID);
    }

    BindUniform(&Group->Shader, CSz("horizontal"), horizontal);

    texture *Tex;
    if (first_iteration)
    {
      Tex = TexIn;
    }
    else
    {
      Tex = Group->Textures[!horizontal];
    }

    /* GL.BindTexture( GL_TEXTURE_2D, Tex->ID ); */
    BindUniform(&Group->Shader, CSz("SrcImage"), Tex, 0);

    RenderQuad();

    horizontal = !horizontal;
    if (first_iteration) first_iteration = false;
  }

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
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
ClearFramebuffers(graphics *Graphics, render_entity_to_texture_group *RTTGroup)
{
  TIMED_FUNCTION();

  SetDefaultFramebufferClearColors();

  GL.BindFramebuffer(GL_FRAMEBUFFER, RTTGroup->FBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->Lighting.FBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO(Jesse): Why exactly would this not be necessary?
  /* glBindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName); */
  /* glClear(GL_DEPTH_BUFFER_BIT); */

  for (s32 Index = 0; Index < 2; ++Index)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->Gaussian.FBOs[Index].ID);
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  if (Graphics->Settings.BravoilMcGuireOIT)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->Transparency.FBO.ID);

#if 1
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
    {
      u32 Attachments = GL_COLOR_ATTACHMENT0;
      GL.DrawBuffers(1, &Attachments);
      GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    {
      u32 Attachments = GL_COLOR_ATTACHMENT0 + 1;
      GL.DrawBuffers(1, &Attachments);
      GL.ClearColor(1.f, 1.f, 1.f, 1.f);
      GL.Clear(GL_COLOR_BUFFER_BIT);
    }

    {
      // Reset draw buffers
      SetDrawBuffers(&Graphics->Transparency.FBO);
    }
#endif
  }
  else
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->Transparency.FBO.ID);
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void
DrawParticle(untextured_3d_geometry_buffer *Source, untextured_3d_geometry_buffer *Dest, u8 ColorIndex)
{
  v3 FaceColors[VERTS_PER_FACE];
  FillColorArray(ColorIndex, FaceColors, DefaultPalette, VERTS_PER_FACE);
  BufferVertsChecked( Source, Dest );
  return;
}

link_internal void
DrawEntityCollisionVolume(entity *Entity, untextured_3d_geometry_buffer *Dest, graphics *Graphics, v3i WorldChunkDim, u8 ColorIndex = PINK, f32 Thickness = DEFAULT_LINE_THICKNESS)
{
  aabb AABB = GetRenderSpaceAABB(WorldChunkDim, Entity, Graphics->Camera);
  auto CopyDest = ReserveBufferSpace(Dest, VERTS_PER_AABB);
  DEBUG_DrawAABB(&CopyDest, AABB, ColorIndex, Thickness);
}

void
BufferEntity(
    untextured_3d_geometry_buffer* Dest,
    untextured_3d_geometry_buffer* TransparentDest,
    entity *Entity, animation *Animation, graphics *Graphics, chunk_dimension WorldChunkDim, r32 dt)
{
  TIMED_FUNCTION();

  if (Spawned(Entity))
  {
    if (GetEngineDebug()->DrawEntityCollisionVolumes)
    {
      DrawEntityCollisionVolume(Entity, Dest, Graphics, WorldChunkDim);
    }

    v3 AnimationOffset = {};
    if (Animation)
    {
      Animation->t += dt;
      AnimationOffset = GetInterpolatedPosition(Animation);
    }

    BufferChunkMesh(Graphics, Dest, &Entity->Model.Mesh, WorldChunkDim, Entity->P.WorldP, Entity->Scale, Entity->P.Offset + AnimationOffset, Entity->Rotation);
    BufferChunkMesh(Graphics, TransparentDest, &Entity->Model.TransparentMesh, WorldChunkDim, Entity->P.WorldP, Entity->Scale, Entity->P.Offset + AnimationOffset, Entity->Rotation);
  }

  return;
}

link_internal void
BufferEntities( entity **EntityTable, untextured_3d_geometry_buffer* Dest, untextured_3d_geometry_buffer* TransparencyDest,
                graphics *Graphics, world *World, r32 dt)
{
  TIMED_FUNCTION();
  for ( s32 EntityIndex = 0;
        EntityIndex < TOTAL_ENTITY_COUNT;
        ++EntityIndex)
  {
    entity *Entity = EntityTable[EntityIndex];
    BufferEntity( Dest, TransparencyDest, Entity, 0, Graphics, World->ChunkDim, dt);
  }

  return;
}

link_internal void
DrawFrustum(world *World, graphics *Graphics, camera *Camera)
{
  auto *GpuBuffer = &GetCurrentGpuMap(Graphics)->Buffer;
  auto Dest = ReserveBufferSpace(GpuBuffer, VERTS_PER_LINE*4);

  v3 SimSpaceP = GetSimSpaceP(World, Camera->CurrentP);
  DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Top.Normal*5.f), RED, 0.2f );
  DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Bot.Normal*5.f), BLUE, 0.2f );
  DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Left.Normal*5.f), GREEN, 0.2f );
  DEBUG_DrawLine(&Dest, line(SimSpaceP+Camera->Front*200.f, Camera->Frust.Right.Normal*5.f), YELLOW, 0.2f );
}

link_internal void
RenderTransparencyBuffers(render_settings *Settings, transparency_render_group *Group)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->FBO.ID);

  UseShader(&Group->Shader);

  if (Settings->BravoilMcGuireOIT)
  {
    FlushBuffersToCard(&Group->GpuBuffer);

    SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );
    GL.Disable(GL_CULL_FACE);

    GL.Enable(GL_BLEND);
    /* GL.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

    // TODO(Jesse): The portable version requires changing the shader a bit
    /* GL.BlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA); */

    GL.BlendFunci(0, GL_ONE, GL_ONE);
    GL.BlendFunci(1, GL_ONE, GL_ONE);
    /* GL.BlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA); */
    /* GL.BlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */

    Draw(Group->GpuBuffer.Buffer.At);

    GL.Disable(GL_BLEND);

    GL.Enable(GL_CULL_FACE);
  }
  else
  {
    GL.Enable(GL_BLEND);
    GL.BlendFunc(GL_ONE, GL_ONE);
    GL.Disable(GL_CULL_FACE);
    /* GL.DepthFunc(GL_LEQUAL); */
    /* GL.DepthFunc(GL_ALWAYS); */

    SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

    FlushBuffersToCard(&Group->GpuBuffer);
    Draw(Group->GpuBuffer.Buffer.At);

    GL.Disable(GL_BLEND);
    GL.Enable(GL_CULL_FACE);
    /* GL.DepthFunc(GL_LEQUAL); */
  }

  Group->GpuBuffer.Buffer.At = 0;
}

link_internal void
RenderToTexture(engine_resources *Engine, untextured_3d_geometry_buffer *Src, v3 Offset)
{
  auto World    = Engine->World;
  auto RTTGroup = &Engine->RTTGroup;

  // GL stuff
  {
    texture *Tex = RTTGroup->Texture;

    GL.BindFramebuffer(GL_FRAMEBUFFER, RTTGroup->FBO.ID);

    GL.UseProgram(RTTGroup->Shader.ID);

    SetViewport(V2(Tex->Dim));

#if 1
    auto Camera = RTTGroup->Camera;
#else
    camera *Camera = Engine->Graphics->Camera;
#endif

    RTTGroup->ViewProjection =
      /* Translate( GetRenderP(World->ChunkDim, Camera->CurrentP, Camera) ) * */
      /* Translate( GetSimSpaceP(World, CameraTarget) ) * */
      /* Translate( V3(-10) ) * */
      ProjectionMatrix(Camera, Tex->Dim.x, Tex->Dim.y) *
      ViewMatrix(World->ChunkDim, Camera)
      /* + Translate2(V3(-0.01f, 0.f, 0.f)) */
      /* * Translate( V3(-10) ) */
      /* Translate( GetSimSpaceP(World, Camera->CurrentP) ); */
      ;

    BindShaderUniforms(&RTTGroup->Shader);
  }

  // Geometry stuff
  {
    MapGpuElementBuffer(&RTTGroup->GeoBuffer);
    untextured_3d_geometry_buffer* Dest = &RTTGroup->GeoBuffer.Buffer;

    v3 Basis = Offset;
    BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
    FlushBuffersToCard(&RTTGroup->GeoBuffer);
  }

  GL.Enable(GL_DEPTH_TEST);
  Draw(RTTGroup->GeoBuffer.Buffer.At);
  RTTGroup->GeoBuffer.Buffer.At = 0;
}

