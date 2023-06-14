

inline void
DEBUG_DrawLine_Aligned( untextured_3d_geometry_buffer *Mesh,
                        v3 P1, v3 P2, u32 ColorIndex, r32 Thickness )
{
  v3 Radius = (P2-P1)/2.f;
  v3 Center = P1 + Radius;

  v3 DrawDim = Max(Abs(Radius*2.f), V3(Thickness));
  DrawVoxel(Mesh, Center, ColorIndex, DrawDim);
}

inline void
DEBUG_DrawLine( untextured_3d_geometry_buffer *Mesh,
                v3 P1, v3 P2, u32 ColorIndex, r32 Thickness )
{
  // 2 verts per line, 3 r32s per vert

  u32 MeshStartingAt = Mesh->At;

  v3 localNormalData[] =
  {
    {{0, 0, 0}},
    {{0, 0, 0}},
    {{0, 0, 0}},

    {{0, 0, 0}},
    {{0, 0, 0}},
    {{0, 0, 0}},
  };

  /* P1.x = P1.x - (Thickness/2.0f); */
  /* P2.x = P2.x - (Thickness/2.0f); */

  v4 FaceColors[VERTS_PER_FACE];
  FillColorArray(ColorIndex, FaceColors, DefaultPalette, VERTS_PER_FACE);

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


    BufferVertsChecked(Mesh,
        6,
        localVertexData,
        localNormalData,
        FaceColors);
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


    BufferVertsChecked(Mesh,
        6,
        localVertexData,
        localNormalData,
        FaceColors);
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


    BufferVertsChecked(Mesh,
        6,
        localVertexData,
        localNormalData,
        FaceColors);
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
DEBUG_DrawVectorAt(untextured_3d_geometry_buffer *Mesh, v3 Offset, v3 Vector, u32 ColorIndex, r32 Thickness )
{
  DEBUG_DrawLine(Mesh, Offset, Vector + Offset, ColorIndex, Thickness );
}

inline void
DEBUG_DrawLine(untextured_3d_geometry_buffer *Mesh, line Line, u32 ColorIndex, r32 Thickness )
{
  DEBUG_DrawLine(Mesh, Line.MinP, Line.MaxP, ColorIndex, Thickness);
  return;
}

#define DEFAULT_LINE_THICKNESS (0.1f)
void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, v3 MinP, v3 MaxP, u32 ColorIndex, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  TIMED_FUNCTION();

  /* v3 HalfDim = (GetRenderP(world, MaxCP) - GetRenderP(world, MinCP)) / 2; */

  // Start in model space
  /* v3 MinP = HalfDim * -1; */
  /* v3 MaxP = HalfDim; */

  // Compute verticies
  v3 TopRL = V3(MinP.x, MaxP.y, MinP.z);
  v3 TopRR = V3(MaxP.x, MaxP.y, MinP.z);
  v3 TopFL = V3(MinP.x, MaxP.y, MaxP.z);
  v3 TopFR = V3(MaxP.x, MaxP.y, MaxP.z);
  v3 BotRL = V3(MinP.x, MinP.y, MinP.z);
  v3 BotRR = V3(MaxP.x, MinP.y, MinP.z);
  v3 BotFL = V3(MinP.x, MinP.y, MaxP.z);
  v3 BotFR = V3(MaxP.x, MinP.y, MaxP.z);

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
  DEBUG_DrawLine_Aligned(Mesh, TopRL, TopRR, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, TopFL, TopFR, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, TopFL, TopRL, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, TopFR, TopRR, ColorIndex, Thickness);

  // Right
  DEBUG_DrawLine_Aligned(Mesh, TopFR, BotFR, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, TopRR, BotRR, ColorIndex, Thickness);

  // Left
  DEBUG_DrawLine_Aligned(Mesh, TopFL, BotFL, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, TopRL, BotRL, ColorIndex, Thickness);

  // Bottom
  DEBUG_DrawLine_Aligned(Mesh, BotRL, BotRR, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, BotFL, BotFR, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, BotFL, BotRL, ColorIndex, Thickness);
  DEBUG_DrawLine_Aligned(Mesh, BotFR, BotRR, ColorIndex, Thickness);


  return;
}

link_internal void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, aabb Rect, u32 ColorIndex, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  v3 MinP = Rect.Center - Rect.Radius;
  v3 MaxP = Rect.Center + Rect.Radius;
  DEBUG_DrawAABB( Mesh, MinP, MaxP, ColorIndex, Thickness );
  return;
}

link_internal void
DEBUG_DrawAABB(untextured_3d_geometry_buffer *Mesh, rect3i Rect, u32 ColorIndex, r32 Thickness = DEFAULT_LINE_THICKNESS )
{
  DEBUG_DrawAABB( Mesh, V3(Rect.Min), V3(Rect.Max), ColorIndex, Thickness );
}


link_internal void
DEBUG_DrawChunkAABB( untextured_3d_geometry_buffer *Mesh, graphics *Graphics, world_position WorldP,
                     chunk_dimension WorldChunkDim, u32 ColorIndex , r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  v3 MinP = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), WorldP), Graphics->Camera);
  v3 MaxP = GetRenderP(WorldChunkDim, Canonical_Position(WorldChunkDim, WorldP), Graphics->Camera);

  DEBUG_DrawAABB(Mesh, MinP, MaxP, ColorIndex, Thickness);
  return;
}

link_internal void
DEBUG_DrawChunkAABB( untextured_3d_geometry_buffer *Mesh, graphics *Graphics, world_chunk *Chunk,
                     chunk_dimension WorldChunkDim, u32 ColorIndex, r32 Thickness = DEFAULT_LINE_THICKNESS)
{
  v3 MinP = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), Chunk->WorldP), Graphics->Camera);
  v3 MaxP = GetRenderP(WorldChunkDim, Canonical_Position(WorldChunkDim, Chunk->WorldP), Graphics->Camera);


  DEBUG_DrawAABB(Mesh, MinP, MaxP, ColorIndex, Thickness);
  return;
}

