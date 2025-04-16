inline void
DEBUG_DrawLine( untextured_3d_geometry_buffer *Mesh, v3 P1, v3 P2, v3 RGBColor, r32 Thickness )
{
  // 2 verts per line, 3 r32s per vert

  u32 MeshStartingAt = Mesh->At;

  v3 localNormalData[] =
  {
    {{1, 0, 0}},
    {{1, 0, 0}},
    {{1, 0, 0}},

    {{1, 0, 0}},
    {{1, 0, 0}},
    {{1, 0, 0}},
  };

  /* P1.x = P1.x - (Thickness/2.0f); */
  /* P2.x = P2.x - (Thickness/2.0f); */

  /* v3 Color = GetColorData(Color); */

  vertex_material Materials[VERTS_PER_FACE];
#if 1
  FillArray(VertexMaterial(RGBtoPackedHSV(RGBColor), 0.f, 0.f), Materials, VERTS_PER_FACE);
#else

  Materials[0] = VertexMaterial(RGBtoPackedHSV(RGBColor), 0.f, 0.f);
  Materials[1] = {};
  Materials[2] = VertexMaterial(RGBtoPackedHSV(RGBColor), 0.f, 0.f);

  Materials[3] = VertexMaterial(RGBtoPackedHSV(RGBColor), 0.f, 0.f);
  Materials[5] = {};
  Materials[4] = {};

#endif

  v2 TransEmiss[VERTS_PER_FACE] = {};

  r32 HalfThickness = Thickness/2.f;
  {
    v3 localVertexData[] =
    {
      {{P1.x - HalfThickness , P1.y , P1.z}} ,
      {{P2.x - HalfThickness , P2.y , P2.z}} ,
      {{P1.x + HalfThickness , P1.y , P1.z}} ,

      {{P1.x + HalfThickness , P1.y , P1.z}} ,
      {{P2.x - HalfThickness , P2.y , P2.z}} ,
      {{P2.x + HalfThickness , P2.y , P2.z}} ,
    };


    BufferVertsChecked(Mesh, 6, localVertexData, localNormalData, Materials);
  }

  {
    v3 localVertexData[] =
    {
      {{P1.x , P1.y - HalfThickness , P1.z}} ,
      {{P2.x , P2.y - HalfThickness , P2.z}} ,
      {{P1.x , P1.y + HalfThickness , P1.z}} ,

      {{P1.x , P1.y + HalfThickness , P1.z}} ,
      {{P2.x , P2.y - HalfThickness , P2.z}} ,
      {{P2.x , P2.y + HalfThickness , P2.z}} ,
    };


    BufferVertsChecked(Mesh, 6, localVertexData, localNormalData, Materials);
  }

  {
    v3 localVertexData[] =
    {
      {{P1.x , P1.y , P1.z - HalfThickness }} ,
      {{P2.x , P2.y , P2.z - HalfThickness }} ,
      {{P1.x , P1.y , P1.z + HalfThickness }} ,

      {{P1.x , P1.y , P1.z + HalfThickness }} ,
      {{P2.x , P2.y , P2.z - HalfThickness }} ,
      {{P2.x , P2.y , P2.z + HalfThickness }} ,
    };


    BufferVertsChecked(Mesh, 6, localVertexData, localNormalData, Materials);
  }

  u32 VertsBuffered = Mesh->At - MeshStartingAt;
  Assert(VertsBuffered == VERTS_PER_LINE);

#if 0
  // This is for anti-aliasing the lines; it draws extra triangles along the edges which can be set to alpha 0
  {
    r32 localVertexData[] =
    {
      P1.x + Thickness, P1.y + Thickness, P1.z + Thickness,
      P2.x, P2.y, P2.z,
      P2.x + Thickness, P2.y + Thickness, P2.z + Thickness,

      P2.x + Thickness, P2.y + Thickness, P2.z + Thickness,
      P1.x + Thickness, P1.y + Thickness, P1.z + Thickness,
      P1.x, P1.y, P1.z,
    };

    BufferVerts(world, gBuffer, SG, Camera,
        6,
        localVertexData,
        sizeof(localVertexData),
        localNormalData,
        sizeof(localVertexData),
        FaceColors);
  }
#endif

  return;
}

inline void
DEBUG_DrawSimSpaceVectorAt(engine_resources *Engine, v3 SimP, v3 Vector, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_LINE);

  v3 Offset = GetRenderP(Engine, SimSpaceToCanonical(World, SimP));
  DEBUG_DrawLine(&Mesh, Offset, Vector + Offset, RGBColor, Thickness );
}

inline void
DEBUG_DrawVectorAt(untextured_3d_geometry_buffer *Mesh, v3 Offset, v3 Vector, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  DEBUG_DrawLine(Mesh, Offset, Vector + Offset, RGBColor, Thickness );
}

inline void
DEBUG_DrawLine(untextured_3d_geometry_buffer *Mesh, line Line, v3 RGBColor, r32 Thickness )
{
  DEBUG_DrawLine(Mesh, Line.MinP, Line.MaxP, RGBColor, Thickness);
}

void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, v3 MinP, v3 MaxP, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  TIMED_FUNCTION();

  /* v3 HalfDim = (GetRenderP(world, MaxCP) - GetRenderP(world, MinCP)) / 2; */

  // Start in model space
  /* v3 MinP = HalfDim * -1; */
  /* v3 MaxP = HalfDim; */

  // Compute verticies
  v3 TopRL = V3(MinP.x, MaxP.y, MinP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 TopRR = V3(MaxP.x, MaxP.y, MinP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 TopFL = V3(MinP.x, MaxP.y, MaxP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 TopFR = V3(MaxP.x, MaxP.y, MaxP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 BotRL = V3(MinP.x, MinP.y, MinP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 BotRR = V3(MaxP.x, MinP.y, MinP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 BotFL = V3(MinP.x, MinP.y, MaxP.z) * GLOBAL_RENDER_SCALE_FACTOR;
  v3 BotFR = V3(MaxP.x, MinP.y, MaxP.z) * GLOBAL_RENDER_SCALE_FACTOR;

  /* // Apply rotation to verts */
  /* TopRL = ((Rotation * Quaternion(TopRL, 1)) * Conjugate(Rotation)).xyz; */
  /* TopRR = ((Rotation * Quaternion(TopRR, 1)) * Conjugate(Rotation)).xyz; */
  /* TopFL = ((Rotation * Quaternion(TopFL, 1)) * Conjugate(Rotation)).xyz; */
  /* TopFR = ((Rotation * Quaternion(TopFR, 1)) * Conjugate(Rotation)).xyz; */
  /* BotRL = ((Rotation * Quaternion(BotRL, 1)) * Conjugate(Rotation)).xyz; */
  /* BotRR = ((Rotation * Quaternion(BotRR, 1)) * Conjugate(Rotation)).xyz; */
  /* BotFL = ((Rotation * Quaternion(BotFL, 1)) * Conjugate(Rotation)).xyz; */
  /* BotFR = ((Rotation * Quaternion(BotFR, 1)) * Conjugate(Rotation)).xyz; */

  /* // Translate into world space */
  /* TopRL += HalfDim + MinCP.Offset; */
  /* TopRR += HalfDim + MinCP.Offset; */
  /* TopFL += HalfDim + MinCP.Offset; */
  /* TopFR += HalfDim + MinCP.Offset; */
  /* BotRL += HalfDim + MinCP.Offset; */
  /* BotRR += HalfDim + MinCP.Offset; */
  /* BotFL += HalfDim + MinCP.Offset; */
  /* BotFR += HalfDim + MinCP.Offset; */

  /* // Translate into render space */
  /* TopRL = GetRenderP(world, Canonical_Position(TopRL, MinCP.WorldP)); */
  /* TopRR = GetRenderP(world, Canonical_Position(TopRR, MinCP.WorldP)); */
  /* TopFL = GetRenderP(world, Canonical_Position(TopFL, MinCP.WorldP)); */
  /* TopFR = GetRenderP(world, Canonical_Position(TopFR, MinCP.WorldP)); */
  /* BotRL = GetRenderP(world, Canonical_Position(BotRL, MinCP.WorldP)); */
  /* BotRR = GetRenderP(world, Canonical_Position(BotRR, MinCP.WorldP)); */
  /* BotFL = GetRenderP(world, Canonical_Position(BotFL, MinCP.WorldP)); */
  /* BotFR = GetRenderP(world, Canonical_Position(BotFR, MinCP.WorldP)); */

  // Render
  //
  // Top
#define LINES_PER_AABB (12)
#define VERTS_PER_AABB (VERTS_PER_VOXEL*LINES_PER_AABB)
#if 0
  DEBUG_DrawLine_Aligned(Mesh, TopRL, TopRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, TopFL, TopFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, TopFL, TopRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, TopFR, TopRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Right
  DEBUG_DrawLine_Aligned(Mesh, TopFR, BotFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, TopRR, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Left
  DEBUG_DrawLine_Aligned(Mesh, TopFL, BotFL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, TopRL, BotRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Bottom
  DEBUG_DrawLine_Aligned(Mesh, BotRL, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, BotFL, BotFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, BotFL, BotRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine_Aligned(Mesh, BotFR, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
#else
  DEBUG_DrawLine(Mesh, TopRL, TopRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, TopFL, TopFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, TopFL, TopRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, TopFR, TopRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Right
  DEBUG_DrawLine(Mesh, TopFR, BotFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, TopRR, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Left
  DEBUG_DrawLine(Mesh, TopFL, BotFL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, TopRL, BotRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);

  // Bottom
  DEBUG_DrawLine(Mesh, BotRL, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, BotFL, BotFR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, BotFL, BotRL, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
  DEBUG_DrawLine(Mesh, BotFR, BotRR, RGBColor, Thickness * GLOBAL_RENDER_SCALE_FACTOR);
#endif
}

link_internal void
DEBUG_DrawSimSpaceAABB(engine_resources *Engine, v3 MinP, v3 MaxP, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  v3 P0 = GetRenderP(World->ChunkDim, SimSpaceToCanonical(World, MinP), Camera);
  v3 P1 = GetRenderP(World->ChunkDim, SimSpaceToCanonical(World, MaxP), Camera);

  untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
  DEBUG_DrawAABB(&Mesh, P0, P1, RGBColor, Thickness);
}

void
DEBUG_DrawSimSpaceAABB(engine_resources *Engine, aabb *AABB, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  v3 P0 = GetMin(AABB);
  v3 P1 = GetMax(AABB);
  DEBUG_DrawSimSpaceAABB(Engine, P0, P1, RGBColor, Thickness);
}

void
DEBUG_DrawAABB(engine_resources *Engine, v3 MinP, v3 MaxP, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  UNPACK_ENGINE_RESOURCES(Engine);
  untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
  DEBUG_DrawAABB(&Mesh, MinP, MaxP, RGBColor, Thickness);
}

link_internal void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, aabb Rect, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  v3 MinP = Rect.Min;
  v3 MaxP = Rect.Max;
  DEBUG_DrawAABB( Mesh, MinP, MaxP, RGBColor, Thickness );
}

link_internal void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, rect3i *Rect, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  DEBUG_DrawAABB( Mesh, V3(Rect->Min), V3(Rect->Max), RGBColor, Thickness );
}

void
DEBUG_DrawAABB(engine_resources *Engine, rect3i *Rect, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  UNPACK_ENGINE_RESOURCES(Engine);
  untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
  DEBUG_DrawAABB(&Mesh, Rect, RGBColor, Thickness);
}


link_internal void
DEBUG_HighlightVoxel(untextured_3d_geometry_buffer *Mesh, world *World, camera *Camera, cp P, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  v3 P0 = GetRenderP(World->ChunkDim, P, Camera) - Thickness;
  DEBUG_DrawAABB( Mesh, P0, P0 + V3(1) + Thickness, RGBColor, Thickness );
}

link_internal void
DEBUG_HighlightVoxel(engine_resources *Engine, cp P, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  UNPACK_ENGINE_RESOURCES(Engine);
  untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
  DEBUG_HighlightVoxel( &Mesh, World, Camera, P, RGBColor, Thickness );

}

link_internal void
DEBUG_HighlightVoxel(engine_resources *Engine, v3 SimP, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  cp P = SimSpaceToCanonical(Engine->World, SimP);
  DEBUG_HighlightVoxel(Engine, P, RGBColor, Thickness);
}

link_internal void
DEBUG_DrawChunkAABB( untextured_3d_geometry_buffer *Mesh, graphics *Graphics, world_position WorldP,
                     chunk_dimension WorldChunkDim, v3 RGBColor , r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  v3 MinP = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), WorldP), Graphics->Camera);
  v3 MaxP = GetRenderP(WorldChunkDim, Canonical_Position(WorldChunkDim, WorldP), Graphics->Camera);

  DEBUG_DrawAABB(Mesh, MinP, MaxP, RGBColor, Thickness);
}

link_internal void
DEBUG_DrawChunkAABB( untextured_3d_geometry_buffer *Mesh, graphics *Graphics, world_chunk *Chunk,
                     chunk_dimension WorldChunkDim, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  v3 MinP = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), Chunk->WorldP), Graphics->Camera);
  v3 MaxP = GetRenderP(WorldChunkDim, Canonical_Position(WorldChunkDim*Chunk->DimInChunks, Chunk->WorldP), Graphics->Camera);


  DEBUG_DrawAABB(Mesh, MinP, MaxP, RGBColor, Thickness);
}

link_internal void
DEBUG_HighlightChunk(world_chunk *Chunk, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  UNPACK_ENGINE_RESOURCES(GetEngineResources());
  auto Buf = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
  DEBUG_DrawChunkAABB( &Buf, Graphics, Chunk, World->ChunkDim, RGBColor, Thickness);
}

link_internal void
DEBUG_HighlightRegion(rect3cp Range, v3 RGBColor, r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  UNPACK_ENGINE_RESOURCES(GetEngineResources());
  auto SSRegion = GetSimSpaceAABB(World, Range);
  DEBUG_DrawSimSpaceAABB(GetEngineResources(), &SSRegion, RGBColor, Thickness );
}


