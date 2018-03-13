#ifndef RENDER_H
#define RENDER_H

// FIXME(Jesse): This should not do draw call tracking in release mode
//

#define Draw(VertexCount) \
  Draw_(VertexCount, __FUNCTION__);

void Draw_(u64 N, const char * Caller)
{
  TIMED_FUNCTION();
  u64 Index = ((u64)Caller) % Global_DrawCallArrayLength;

  debug_draw_call *DrawCall = &Global_DrawCalls[Index];

  if ( DrawCall->Caller )
  {
    // If this assert fires, we need to implement hash collisions here
    Assert(StringsMatch(DrawCall->Caller, Caller));
  }
  else
  {
    DrawCall->Caller = Caller;
  }

  DrawCall->Count++;
  glDrawArrays(GL_TRIANGLES, 0, N);
}

inline void
SetViewport(v2 Dim)
{
  glViewport(0, 0, Dim.x, Dim.y);
  return;
}

inline m4
Orthographic( r32 X, r32 Y, r32 Zmin, r32 Zmax, v3 Translate )
{

#if 1
  m4 Result = GLM4(glm::ortho<r32>(-X+Translate.x, X+Translate.x,
                                   -Y+Translate.y, Y+Translate.y,
                                   Zmin +Translate.z, Zmax +Translate.z));
#else
  m4 Result = IdentityMatrix;
  Assert(False);
#endif

  return Result;
}

inline m4
Perspective(radians FOV, r32 AspectRatio, r32 NearClip, r32 FarClip)
{

#if 1
  glm::mat4 Projection = glm::perspective(FOV, AspectRatio, NearClip, FarClip);
  m4 Result = GLM4(Projection);
#else
  // Scale
  r32 S = (1/(tan((FOV/2) * (PIf/180))));

  // Remap z to 0-1
  r32 Z = (-FarClip)/(FarClip-NearClip);
  r32 ZZ = -FarClip * NearClip / (FarClip - NearClip);

  m4 Result = {
    V4(S, 0, 0, 0),
    V4(0, S, 0, 0),
    V4(0, 0, Z, -1),
    V4(0, 0, ZZ, 0),
  };
#endif



  return Result;
}

inline radians
Rads(degrees Degrees)
{
  radians Result = (Degrees/180);
  return Result;
}

inline m4
GetProjectionMatrix(camera *Camera, int WindowWidth, int WindowHeight)
{
  m4 Projection = Perspective(
      Rads(Camera->Frust.FOV),
      (float)WindowWidth/(float)WindowHeight,
      Camera->Frust.nearClip,
      Camera->Frust.farClip);

  return Projection;
}

inline v3
GetRenderP(canonical_position P, camera *Camera)
{
  v3 CameraOffset = Camera->Target.Offset + (Camera->Target.WorldP * Global_WorldChunkDim);
  v3 Result = P.Offset + (P.WorldP * Global_WorldChunkDim) - CameraOffset;
  return Result;
}

inline v3
GetRenderP( chunk_dimension WorldChunkDim, canonical_position P, camera *Camera)
{
  TIMED_FUNCTION();
  v3 CameraOffset = Camera->Target.Offset + (Camera->Target.WorldP * WorldChunkDim);
  v3 Result = P.Offset + (P.WorldP * WorldChunkDim) - CameraOffset;
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
  glm::mat4 M = glm::lookAt( glm::vec3(P.x, P.y, P.z),
                             glm::vec3(Target.x, Target.y, Target.z),
                             glm::vec3(Up.x, Up.y, Up.z) );

  m4 Result = GLM4(M);

  return Result;
}

inline m4
GetViewMatrix(chunk_dimension WorldChunkDim, camera *Camera)
{
  m4 Result = LookAt(
    GetRenderP(WorldChunkDim, Camera->P, Camera),
    GetRenderP(WorldChunkDim, Camera->Target, Camera),
    Camera->Up
  );

  return Result;
}

#define BEGIN_CARD_BUFFERING() { u32 AttributeIndex = 0;
#define END_CARD_BUFFERING()   }

#define BUFFER_VERTS_TO_CARD(BufferId, Mesh)                                                            \
  GL_Global->glEnableVertexAttribArray(AttributeIndex);                                                 \
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, BufferId);                                                   \
  GL_Global->glBufferData(GL_ARRAY_BUFFER, Mesh->CurrentIndex*sizeof(v3), Mesh->Verts, GL_STATIC_DRAW); \
  GL_Global->glVertexAttribPointer( AttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);                \
  ++AttributeIndex;

#define BUFFER_COLORS_TO_CARD(BufferId, Mesh)                                                            \
  GL_Global->glEnableVertexAttribArray(AttributeIndex);                                                  \
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, BufferId);                                                    \
  GL_Global->glBufferData(GL_ARRAY_BUFFER, Mesh->CurrentIndex*sizeof(v3), Mesh->Colors, GL_STATIC_DRAW); \
  GL_Global->glVertexAttribPointer(AttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);                  \
  ++AttributeIndex;

#define BUFFER_NORMALS_TO_CARD(BufferId, Mesh)                                                            \
  GL_Global->glEnableVertexAttribArray(AttributeIndex);                                                   \
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, BufferId);                                                     \
  GL_Global->glBufferData(GL_ARRAY_BUFFER, Mesh->CurrentIndex*sizeof(v3), Mesh->Normals, GL_STATIC_DRAW); \
  GL_Global->glVertexAttribPointer(AttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);                   \
  ++AttributeIndex;

#define BUFFER_UVS_TO_CARD(BufferId, Mesh)                                                           \
  GL_Global->glEnableVertexAttribArray(AttributeIndex);                                              \
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);                                            \
  GL_Global->glBufferData(GL_ARRAY_BUFFER, Mesh->CurrentIndex*sizeof(v2), Geo->UVs, GL_STATIC_DRAW); \
  GL_Global->glVertexAttribPointer(AttributeIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );             \
  ++AttributeIndex;



#endif
