//
// TODO(Jesse)(high-priority, getrenderp): This is complete garbage and needs to be removed asap.
//
#if 1

link_internal cp
ComputeTarget(camera *Camera)
{
  cp Result = Camera->CurrentP + Camera->Front*Camera->DistanceFromTarget;
  return Result;
}

inline v3
GetRenderP(canonical_position P, camera *Camera, world_position WorldChunkDim)
{
  cp ViewingTarget = ComputeTarget(Camera);
  v3 CameraOffset = ViewingTarget.Offset + (ViewingTarget.WorldP * WorldChunkDim);
  v3 Result = P.Offset + (P.WorldP * WorldChunkDim) - CameraOffset;
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, canonical_position P, camera *Camera)
{
  v3 Result = GetRenderP(P, Camera, WorldChunkDim);
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, v3 Offset, camera *Camera)
{
  v3 Result = GetRenderP(WorldChunkDim, Canonical_Position(Offset, World_Position(0)), Camera);
  return Result;
}

inline v3
GetRenderP( engine_resources *Engine, cp P )
{
  v3 Result = GetRenderP(Engine->World->ChunkDim, P, Engine->Graphics.Camera);
  return Result;
}

inline v3
GetRenderP( engine_resources *Engine, v3i WorldP )
{
  v3 Result = GetRenderP(Engine->World->ChunkDim, Canonical_Position(V3(0), WorldP), Engine->Graphics.Camera);
  return Result;
}

inline v3
GetRenderP( engine_resources *Engine, v3 Offset )
{
  v3 Result = GetRenderP(Engine->World->ChunkDim, Offset, Engine->Graphics.Camera);
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, world_position WorldP, camera *Camera)
{
  v3 Result = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), WorldP), Camera);
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, standing_spot *Spot, camera *Camera)
{
  v3 Result = Spot->P.Offset + GetRenderP(WorldChunkDim, Spot->P.WorldP, Camera);
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, entity *entity, camera *Camera)
{
  v3 Result = GetRenderP(WorldChunkDim, entity->P, Camera);
  return Result;
}

inline aabb
GetRenderSpaceAABB(chunk_dimension WorldChunkDim, entity *Entity, camera *Camera)
{
  v3 Radius = Entity->_CollisionVolumeRadius;
  v3 Center = GetRenderP(WorldChunkDim, Entity->P, Camera) + Radius;

  aabb Result = RectCenterRad(Center, Radius);
  return Result;
}
#endif

inline m4
LookAt(v3 P, v3 Target, v3 Up)
{
  v3 Front = Normalize(Target - P);
  v3 Right = Normalize(Cross(Front, Up));

  // FIXME(Jesse): I believe this is backwards!  Furthermore, shouldn't we be
  // able to re-use the camera computed vectors?
  Up       = Normalize(Cross(Right, Front));

  m4 Result = {
    V4(Right.x       , Up.x       , -Front.x      , 0) ,
    V4(Right.y       , Up.y       , -Front.y      , 0) ,
    V4(Right.z       , Up.z       , -Front.z      , 0) ,
    V4(-Dot(Right,P) , -Dot(Up,P) ,  Dot(Front,P) , 1)
  };

  return Result;
}

inline m4
ViewMatrix(chunk_dimension WorldChunkDim, camera *Camera)
{
  m4 Result = LookAt(
    GetRenderP(WorldChunkDim, Camera->CurrentP, Camera),
    GetRenderP(WorldChunkDim, ComputeTarget(Camera), Camera),
    Camera->Up
  );

  return Result;
}

// Note(Jesse): Lifted from the MESA implementation of the GLU library
link_internal b32
Inverse(r32 m[16], r32 invOut[16])
{
  TIMED_FUNCTION();

  // TODO(Jesse): Does changing these to r32 have a material effect on this routine?

  r64 inv[16];
  r64 det;

  inv[0] = r64(m[5]  * m[10] * m[15] -
               m[5]  * m[11] * m[14] -
               m[9]  * m[6]  * m[15] +
               m[9]  * m[7]  * m[14] +
               m[13] * m[6]  * m[11] -
               m[13] * m[7]  * m[10]);

  inv[4] = r64(-m[4]  * m[10] * m[15] +
                m[4]  * m[11] * m[14] +
                m[8]  * m[6]  * m[15] -
                m[8]  * m[7]  * m[14] -
                m[12] * m[6]  * m[11] +
                m[12] * m[7]  * m[10]);

  inv[8] = r64(m[4]  * m[9] * m[15] -
               m[4]  * m[11] * m[13] -
               m[8]  * m[5] * m[15] +
               m[8]  * m[7] * m[13] +
               m[12] * m[5] * m[11] -
               m[12] * m[7] * m[9]);

  inv[12] = r64(-m[4]  * m[9] * m[14] +
                 m[4]  * m[10] * m[13] +
                 m[8]  * m[5] * m[14] -
                 m[8]  * m[6] * m[13] -
                 m[12] * m[5] * m[10] +
                 m[12] * m[6] * m[9]);

  det = r64(m[0] * r32(inv[0]) + m[1] * r32(inv[4]) + m[2] * r32(inv[8]) + m[3] * r32(inv[12]));

  if (det == 0)
      return False;

  det = 1.0 / det;


  inv[1] = r64( -m[1]  * m[10] * m[15] +
                m[1]  * m[11] * m[14] +
                m[9]  * m[2] * m[15] -
                m[9]  * m[3] * m[14] -
                m[13] * m[2] * m[11] +
                m[13] * m[3] * m[10]);

  inv[5] = r64( m[0]  * m[10] * m[15] -
               m[0]  * m[11] * m[14] -
               m[8]  * m[2] * m[15] +
               m[8]  * m[3] * m[14] +
               m[12] * m[2] * m[11] -
               m[12] * m[3] * m[10]);

  inv[9] = r64( -m[0]  * m[9] * m[15] +
                m[0]  * m[11] * m[13] +
                m[8]  * m[1] * m[15] -
                m[8]  * m[3] * m[13] -
                m[12] * m[1] * m[11] +
                m[12] * m[3] * m[9]);

  inv[13] = r64( m[0]  * m[9] * m[14] -
                m[0]  * m[10] * m[13] -
                m[8]  * m[1] * m[14] +
                m[8]  * m[2] * m[13] +
                m[12] * m[1] * m[10] -
                m[12] * m[2] * m[9]);

  inv[2] = r64( m[1]  * m[6] * m[15] -
               m[1]  * m[7] * m[14] -
               m[5]  * m[2] * m[15] +
               m[5]  * m[3] * m[14] +
               m[13] * m[2] * m[7] -
               m[13] * m[3] * m[6]);

  inv[6] = r64( -m[0]  * m[6] * m[15] +
                m[0]  * m[7] * m[14] +
                m[4]  * m[2] * m[15] -
                m[4]  * m[3] * m[14] -
                m[12] * m[2] * m[7] +
                m[12] * m[3] * m[6]);

  inv[10] = r64( m[0]  * m[5] * m[15] -
                m[0]  * m[7] * m[13] -
                m[4]  * m[1] * m[15] +
                m[4]  * m[3] * m[13] +
                m[12] * m[1] * m[7] -
                m[12] * m[3] * m[5]);

  inv[14] = r64( -m[0]  * m[5] * m[14] +
                 m[0]  * m[6] * m[13] +
                 m[4]  * m[1] * m[14] -
                 m[4]  * m[2] * m[13] -
                 m[12] * m[1] * m[6] +
                 m[12] * m[2] * m[5]);

  inv[3] = r64( -m[1] * m[6] * m[11] +
                m[1] * m[7] * m[10] +
                m[5] * m[2] * m[11] -
                m[5] * m[3] * m[10] -
                m[9] * m[2] * m[7] +
                m[9] * m[3] * m[6]);

  inv[7] = r64( m[0] * m[6] * m[11] -
               m[0] * m[7] * m[10] -
               m[4] * m[2] * m[11] +
               m[4] * m[3] * m[10] +
               m[8] * m[2] * m[7] -
               m[8] * m[3] * m[6]);

  inv[11] = r64( -m[0] * m[5] * m[11] +
                 m[0] * m[7] * m[9] +
                 m[4] * m[1] * m[11] -
                 m[4] * m[3] * m[9] -
                 m[8] * m[1] * m[7] +
                 m[8] * m[3] * m[5]);

  inv[15] = r64( m[0] * m[5] * m[10] -
                m[0] * m[6] * m[9] -
                m[4] * m[1] * m[10] +
                m[4] * m[2] * m[9] +
                m[8] * m[1] * m[6] -
                m[8] * m[2] * m[5]);

  for (s32 i = 0; i < 16; i++)
      invOut[i] = (r32)(inv[i] * det);

  return True;
}

link_internal m4
Inverse(m4 M)
{
  m4 Result;

  if (Inverse(M.E[0].E, Result.E[0].E))
  {
  }
  else
  {
    SoftError("Unable to invert matrix");
  }

  return Result;
}

link_internal m4
Transpose(m4 M)
{
  m4 Result = {
    V4(M.E[0].E[0], M.E[1].E[0], M.E[2].E[0], M.E[3].E[0] ),
    V4(M.E[0].E[1], M.E[1].E[1], M.E[2].E[1], M.E[3].E[1] ),
    V4(M.E[0].E[2], M.E[1].E[2], M.E[2].E[2], M.E[3].E[2] ),
    V4(M.E[0].E[3], M.E[1].E[3], M.E[2].E[3], M.E[3].E[3] ),
  };

  return Result;
}
