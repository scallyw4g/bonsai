inline v3
GetRenderP(canonical_position P, camera *Camera, world_position WorldChunkDim)
{
  v3 CameraOffset = Camera->ViewingTarget.Offset + (Camera->ViewingTarget.WorldP * WorldChunkDim);
  v3 Result = P.Offset + (P.WorldP * WORLD_CHUNK_DIM) - CameraOffset;
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, canonical_position P, camera *Camera)
{
  TIMED_FUNCTION();
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
GetRenderP( chunk_dimension WorldChunkDim, world_position WorldP, camera *Camera)
{
  v3 Result = GetRenderP(WorldChunkDim, Canonical_Position(V3(0,0,0), WorldP), Camera);
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
  v3 Radius = Entity->CollisionVolumeRadius;
  v3 Center = GetRenderP(WorldChunkDim, Entity->P, Camera) + Radius;

  aabb Result(Center, Radius);
  return Result;
}

inline m4
LookAt(v3 P, v3 Target, v3 Up)
{
  v3 Front = Normalize(Target - P);
  v3 Right = Normalize(Cross(Front, Up));
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
GetViewMatrix(chunk_dimension WorldChunkDim, camera *Camera)
{
  m4 Result = LookAt(
    GetRenderP(WorldChunkDim, Camera->CurrentP, Camera),
    GetRenderP(WorldChunkDim, Camera->ViewingTarget, Camera),
    Camera->Up
  );

  return Result;
}
