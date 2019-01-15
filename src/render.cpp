#include <render_init.cpp>
#include <render_utils.cpp>

inline m4
GetShadowMapMVP(light *GlobalLight)
{
  // Compute the MVP matrix from the light's point of view
  /* v3 Translate = GetRenderP(Camera->Target, Camera); */
  m4 depthProjectionMatrix = Orthographic(SHADOW_MAP_X,
                                          SHADOW_MAP_Y,
                                          SHADOW_MAP_Z_MIN,
                                          SHADOW_MAP_Z_MAX);

  v3 Front = Normalize(GlobalLight->Position);
  v3 Right = Cross( Front, V3(0,1,0) );
  v3 Up = Cross(Right, Front);

  v3 Target = V3(0);
  m4 depthViewMatrix =  LookAt(GlobalLight->Position, Target, Up);

  return depthProjectionMatrix * depthViewMatrix;
}

void
RenderAoTexture(ao_render_group *AoGroup)
{
  glBindFramebuffer(GL_FRAMEBUFFER, AoGroup->FBO.ID);
  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  glUseProgram(AoGroup->Shader.ID);

  glUniform3fv(AoGroup->SsaoKernelUniform, SSAO_KERNEL_SIZE, (r32*)AoGroup->SsaoKernel);

  BindShaderUniforms(&AoGroup->Shader);

  RenderQuad();

  AssertNoGlErrors;

  return;
}

void
UpdateLightingTextures(game_lights *Lights)
{
  // TODO(Jesse): Allocate lights such that this swizzle is unneeded
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

  glBindTexture(Type, Lights->PositionTex->ID);
  glTexImage2D( Type, 0, GL_RGB32F,
                Lights->PositionTex->Dim.x, Lights->PositionTex->Dim.y,
                0,  GL_RGB, GL_FLOAT, PosData);
  AssertNoGlErrors;

  glBindTexture(Type, Lights->ColorTex->ID);
  glTexImage2D( Type, 0, GL_RGB32F,
                Lights->ColorTex->Dim.x, Lights->ColorTex->Dim.y,
                0,  GL_RGB, GL_FLOAT, ColorData);
  AssertNoGlErrors;

  return;
}

void
DrawGBufferToFullscreenQuad( platform *Plat, graphics *Graphics)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));

  glUseProgram(Graphics->gBuffer->LightingShader.ID);

  UpdateLightingTextures(Graphics->Lights);
  Graphics->gBuffer->ShadowMVP = NdcToScreenSpace * GetShadowMapMVP(&Graphics->Lights->Lights[0]);

  BindShaderUniforms(&Graphics->gBuffer->LightingShader);

  RenderQuad();

  AssertNoGlErrors;

  return;
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
RenderShadowMap(untextured_3d_geometry_buffer *Mesh, graphics *Graphics)
{
  TIMED_FUNCTION();
  SetViewport(V2(SHADOW_MAP_RESOLUTION_X, SHADOW_MAP_RESOLUTION_Y));

  m4 MVP = GetShadowMapMVP(Camera, &SG->GameLights[0]);

  glBindFramebuffer(GL_FRAMEBUFFER, SG->FramebufferName);

  glUseProgram(SG->DepthShader.ID);
  glUniformMatrix4fv(SG->MVP_ID, 1, GL_FALSE, &MVP.E[0].E[0]);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, RG->vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, Mesh->At*sizeof(v3), Mesh->VertexData, GL_STATIC_DRAW);
  glVertexAttribPointer(
    0,                  // The attribute we want to configure
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  Draw(Mesh->At);

  glDisableVertexAttribArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return;
}
#endif

void
RenderWorldToGBuffer(gpu_mapped_element_buffer* GpuMap, g_buffer_render_group *RG)
{
  TIMED_FUNCTION();
  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO.ID);
  glUseProgram(RG->gBufferShader.ID);

  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  BindShaderUniforms(&RG->gBufferShader);

  FlushBuffersToCard(GpuMap);
  Draw(GpuMap->Buffer.At);
  GpuMap->Buffer.At = 0;

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  return;
}

inline void
RenderGBuffer(gpu_mapped_element_buffer *GpuMap, graphics *Graphics)
{
  TIMED_FUNCTION();

  RenderWorldToGBuffer(GpuMap, Graphics->gBuffer);

  AssertNoGlErrors;

  return;
}

inline void
RenderPostBuffer(post_processing_group *PostGroup, untextured_3d_geometry_buffer *Mesh)
{
  glBindFramebuffer(GL_FRAMEBUFFER, PostGroup->FBO.ID);
  glUseProgram(PostGroup->Shader.ID);

  SetViewport( V2(SCR_WIDTH, SCR_HEIGHT) );

  BindShaderUniforms(&PostGroup->Shader);

  TIMED_BLOCK("PostBuffer - Bind and buffer data");
    u32 AttributeIndex = 0;
    BufferVertsToCard(PostGroup->VertexBuffer, Mesh, &AttributeIndex);
    BufferColorsToCard(PostGroup->ColorBuffer, Mesh, &AttributeIndex);
  END_BLOCK("PostBuffer - Bind and buffer data");

  Draw(Mesh->At);
  Mesh->At = 0;

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
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

inline r32
GetTheta(v3 P1, v3 P2)
{
  r32 DotP1P2 = Dot(P1,P2);

  r32 LP1, LP2;

  LP1 = Length(P1);
  LP2 = Length(P2);

  Assert(LP1 != 0);
  Assert(LP2 != 0);

  r32 cosTheta = DotP1P2 / (LP1*LP2);
  cosTheta = ClampBilateral(cosTheta);
  r32 Theta = (r32)acos( cosTheta );

  Assert(Theta >= -1 || Theta <= 1);
  return Theta;
}

inline Quaternion
RotatePoint(v3 P1, v3 P2)
{
  P1 = Normalize(P1);
  P2 = Normalize(P2);
  v3 Axis = Normalize(Cross(P1, P2));

  r32 Theta = GetTheta(P1, P2);

  Quaternion Result = Quaternion((Axis*(r32)sin(Theta/2.0f)), (r32)cos(Theta/2.0f));

  if (Length(Result.xyz) == 0)  // The resulting rotation was inconsequential
    Result = Quaternion();

  return Result;
}

inline v3
Rotate(v3 P, Quaternion Rotation)
{
  v3 Result = ((Rotation * Quaternion(P,0)) * Conjugate(Rotation)).xyz;
  return Result;
}

inline line
Rotate(line Line, Quaternion Rotation)
{
  line Result;

  Result.MinP = Rotate(Line.MinP, Rotation);
  Result.MaxP = Rotate(Line.MaxP, Rotation);

  return Result;
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
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClearDepth(1.0f);

#if BONSAI_INTERNAL
  debug_state* DebugState = GetDebugState();

  debug_text_render_group *TextRG = &DebugState->TextRenderGroup;
  glBindFramebuffer(GL_FRAMEBUFFER, TextRG->FBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

  // FIXME(Jesse): This is taking _forever_ on Linux (GLES) .. does it take
  // forever on other Linux systems?
  glBindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* glBindFramebuffer(GL_FRAMEBUFFER, Graphics->SG->FramebufferName); */
  /* glClear(GL_DEPTH_BUFFER_BIT); */

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return;
}

void
BufferChunkMesh(
    untextured_3d_geometry_buffer *Dest,
    untextured_3d_geometry_buffer *Src,
    chunk_dimension WorldChunkDim,
    world_position WorldP,
    graphics *Graphics,
    r32 Scale = 1.0f,
    v3 Offset = V3(0)
  )
{
  /* TIMED_FUNCTION(); */

  if (!Src || Src->At == 0)
    return;

#if DEBUG_CHUNK_AABB
  DEBUG_DrawChunkAABB(Dest, Graphics, WorldP, WorldChunkDim, PINK, 0.1f);
#endif

  v3 ModelBasisP =
    GetRenderP( WorldChunkDim, Canonical_Position(Offset, WorldP), Graphics->Camera);

  BufferVertsChecked(Dest, Src->At,
      Src->Verts, Src->Normals, Src->Colors,
      ModelBasisP, V3(Scale));

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

    if ( Neighbor && IsSet( Neighbor, Chunk_Initialized) )
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

inline v3
GetSign(v3 P)
{
  v3 Result =
    V3( GetSign(P.x), GetSign(P.y), GetSign(P.z));

  return Result;
}

inline voxel_position
GetSign(voxel_position P)
{
  voxel_position Result =
    Voxel_Position( GetSign(P.x), GetSign(P.y), GetSign(P.z));

  return Result;
}

voxel_position
RayTraceCollision(chunk_data *Chunk, chunk_dimension Dim, v3 StartingP, v3 Ray, v3 CenteringRay)
{
  Assert(LengthSq(Ray) == 1);
  v3 Result = V3(-1,-1,-1);

  // Clamp magnitude of this ray to 1 in each axis
  CenteringRay = GetSign(CenteringRay);

  v3 CurrentP = StartingP;
  while ( IsInsideDim(Dim, CurrentP) )
  {
    v3 CachedP = CurrentP;

    while ( IsInsideDim(Dim, CurrentP) )
    {
      if ( IsFilledInChunk(Chunk, Voxel_Position(CurrentP), Dim) )
      {
        Result = CurrentP;
        goto finished;
      }

      CurrentP += Ray;
    }

    CurrentP = CachedP;
    CurrentP += CenteringRay;
  }

  finished:

  return Voxel_Position(Result);
}

function edge*
Edge(voxel_position P0, voxel_position P1, memory_arena* Memory)
{
  edge* Result = Allocate(edge, Memory, 1);
  Result->P0 = P0;
  Result->P1 = P1;
  Result->LengthSq = LengthSq(P1 - P0);

  return Result;
}

#if 0
function void
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
      r32 ShortestDistance = FLT_MAX;
      u32 ShortestDistanceIndex = UINT_MAX;
      v3 ShortestP = V3(FLT_MAX);
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

      r32 LeastEdgeSum = FLT_MAX;
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
IsBoundaryVoxel(chunk_data *Chunk, voxel_position Offset, chunk_dimension Dim)
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
CheckAndIncrementCurrentP(
    chunk_data *Chunk,
    chunk_dimension Dim,
    voxel_position *CurrentP,
    s32 *CurrentClosestDistanceSq,
    voxel_position TargetP,
    voxel_position TestP
  )
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
TraverseSurfaceToBoundary(
    world *World,
    chunk_data *Chunk,
    voxel_position StartingP,
    voxel_position IterDir
  )
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


b32
CanBuildWorldChunkBoundary(world *World, world_chunk *Chunk)
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
  world_chunk *TestChunk = GetWorldChunk( World, Left );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  TestChunk = GetWorldChunk( World, Right );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  TestChunk = GetWorldChunk( World, Top );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  TestChunk = GetWorldChunk( World, Bot );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  TestChunk = GetWorldChunk( World, Front );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  TestChunk = GetWorldChunk( World, Back );
  Result &= TestChunk && IsSet(TestChunk, Chunk_Initialized);

  return Result;
}

void
DrawFolie(untextured_3d_geometry_buffer *Mesh, aabb *AABB)
{
  v3 RenderP = AABB->Center;
  DrawVoxel( Mesh, RenderP, GREY, AABB->Radius*2);

  return;
}

#if 1
void
DrawParticle(
    untextured_3d_geometry_buffer *Source,
    untextured_3d_geometry_buffer *Dest,
    u8 ColorIndex
  )
{
  v4 FaceColors[FACE_VERT_COUNT];
  FillColorArray(ColorIndex, FaceColors, FACE_VERT_COUNT);;
#if 1
  BufferVertsChecked( Source, Dest );
#else
  RightFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, RightFaceNormalData, FaceColors);

  LeftFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, LeftFaceNormalData, FaceColors);

  BottomFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, BottomFaceNormalData, FaceColors);

  TopFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, TopFaceNormalData, FaceColors);

  FrontFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, FrontFaceNormalData, FaceColors);

  BackFaceVertexData( MinP, V3(Diameter), VertexData);
  BufferVerts(Mesh, gBuffer, SG, Camera, 6, VertexData, BackFaceNormalData, FaceColors);
#endif

  return;
}
#endif

void
BufferEntity(
    untextured_3d_geometry_buffer* Dest,
    entity *Entity,
    animation *Animation,
    graphics *Graphics,
    chunk_dimension WorldChunkDim,
    r32 dt
  )
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
    DEBUG_DrawAABB(Dest, Graphics, AABB, PINK);
#endif

    v3 AnimationOffset = {};
    if (Animation)
    {
      Animation->t += dt;
      AnimationOffset = GetInterpolatedPosition(Animation);
    }

    BufferChunkMesh(Dest, &Entity->Model.Mesh, WorldChunkDim, Entity->P.WorldP, Graphics, Entity->Scale, Entity->P.Offset + AnimationOffset);
  }

  return;
}

untextured_3d_geometry_buffer
ReserveBufferSpace(untextured_3d_geometry_buffer* Reservation, u32 ElementsToReserve)
{
  TIMED_FUNCTION();
  Assert(ElementsToReserve);

  untextured_3d_geometry_buffer Result = {};
  if (Reservation->At + ElementsToReserve < Reservation->End)
  {
    Result.Verts = Reservation->Verts + Reservation->At;
    Result.Colors = Reservation->Colors + Reservation->At;
    Result.Normals = Reservation->Normals + Reservation->At;
    Result.End = ElementsToReserve;

    Reservation->At += ElementsToReserve;
  }
  else
  {
    Warn("Failed to reserve buffer space");
  }

  return Result;
}

void
BufferWorldChunk(
    untextured_3d_geometry_buffer *Dest,
    world_chunk *Chunk,
    graphics *Graphics,
    work_queue* Queue
    /* hotkeys* Hotkeys */
  )
{
  if (!Chunk || !Chunk->Mesh)
    return;

  chunk_data *ChunkData = Chunk->Data;
  if (ChunkData->Flags == Chunk_MeshComplete)
  {

#if 0
    {
      untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Dest, Chunk->Mesh->At);
      v3 ModelBasisP = GetRenderP( WORLD_CHUNK_DIM, Canonical_Position(V3(0), Chunk->WorldP), Graphics->Camera);

      work_queue_entry Entry = {};
      Entry.Type = WorkEntryType_CopyBuffer;
      Entry.GpuCopyParams.Src = Chunk->Mesh;
      Entry.GpuCopyParams.Dest = CopyDest;
      Entry.GpuCopyParams.Basis = ModelBasisP;

      PushWorkQueueEntry(Queue, &Entry);
    }
#endif

#if 0
    current_triangles *CurrentTriangles = Chunk->CurrentTriangles;
    if ( (Hotkeys->Debug_TriangulateIncrement || Hotkeys->Debug_TriangulateDecrement) &&
         (CurrentTriangles->SurfacePoints->End > 0))
    {

      if (Hotkeys->Debug_TriangulateIncrement)
      {
        if (CurrentTriangles->CurrentPointIndex+1 < CurrentTriangles->SurfacePoints->End)
        {
          ++CurrentTriangles->CurrentPointIndex;
        }
        /* TriangulateUntilIndex(Chunk->LodMesh, Chunk->CurrentTriangles, TranArena, CurrentTriangles->CurrentPointIndex); */
      }

      if (Hotkeys->Debug_TriangulateDecrement)
      {
        if (CurrentTriangles->CurrentPointIndex > 0)
        {
          --CurrentTriangles->CurrentPointIndex;
        }
        /* TriangulateUntilIndex(Chunk->LodMesh, Chunk->CurrentTriangles, TranArena, CurrentTriangles->CurrentPointIndex); */
      }
    }
#endif

#if 1
    if (Chunk->LodMesh_Complete && Chunk->LodMesh->At)
    {
      untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(Dest, Chunk->LodMesh->At);
      v3 ModelBasisP = GetRenderP(WORLD_CHUNK_DIM, Chunk->WorldP, Graphics->Camera);

      work_queue_entry Entry = {};
      Entry.Type = WorkEntryType_CopyBuffer;
      Entry.GpuCopyParams.Src = Chunk->LodMesh;
      Entry.GpuCopyParams.Dest = CopyDest;
      Entry.GpuCopyParams.Basis = ModelBasisP;

      Assert(CopyDest.At == 0);
      Assert(CopyDest.End == Chunk->LodMesh->At);

      PushWorkQueueEntry(Queue, &Entry);
    }
#endif
  }
  else if (IsSet(ChunkData, Chunk_Queued))
  {
    // Note(Jesse): This is really slow so use judiciously
    /* DEBUG_DrawChunkAABB(&World->Mesh, Graphics, Chunk, WORLD_CHUNK_DIM, TEAL, 0.1f); */
  }
  else
  {
    /* DEBUG_DrawChunkAABB(&World->Mesh, Graphics, Chunk, WORLD_CHUNK_DIM, RED, 0.1f); */
  }

  return;
}

inline void
QueueChunkForInit(game_state *GameState, work_queue *Queue, world_chunk *Chunk);

#define MAX_PICKED_WORLD_CHUNKS 32
void
BufferWorld(game_state* GameState, untextured_3d_geometry_buffer* Dest, world *World, graphics *Graphics, world_position VisibleRadius, hotkeys* Hotkeys, ray PickRay, world_chunk** PickedChunks, u32* PickedChunkCount)
{
  TIMED_FUNCTION();

  world_position Min = World->Center - VisibleRadius;
  world_position Max = World->Center + VisibleRadius + 1;

  for (s32 z = Min.z; z < Max.z; ++ z)
  {
    for (s32 y = Min.y; y < Max.y; ++ y)
    {
      for (s32 x = Min.x; x < Max.x; ++ x)
      {
        world_position P = World_Position(x,y,z);
        world_chunk *Chunk = GetWorldChunk( World, P );

        if (Chunk && Chunk->Mesh)
        {
          TIMED_BLOCK("Mouse Ray Intersection");
          v3 MinP = GetRenderP(WORLD_CHUNK_DIM, Canonical_Position(V3(0,0,0), Chunk->WorldP), Graphics->Camera);
          v3 MaxP = GetRenderP(WORLD_CHUNK_DIM, Canonical_Position(WORLD_CHUNK_DIM, Chunk->WorldP), Graphics->Camera);
          aabb ChunkAABB = MinMaxAABB(MinP, MaxP);

          if (Hotkeys->Debug_MousePick && *PickedChunkCount < MAX_PICKED_WORLD_CHUNKS && Intersect(ChunkAABB, PickRay) )
          {
            PickedChunks[*PickedChunkCount] = Chunk;
            *PickedChunkCount = *PickedChunkCount+1;
          }

          BufferWorldChunk(Dest, Chunk, Graphics, &GameState->Plat->HighPriority/*, Hotkeys*/);
          END_BLOCK();
        }
        else if (!Chunk)
        {
          Chunk = GetWorldChunkFor(GameState->Memory, World, P);
          QueueChunkForInit(GameState, &GameState->Plat->LowPriority, Chunk);
        }
      }
    }
  }

  return;
}

void
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
