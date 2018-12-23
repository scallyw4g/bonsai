#define Draw(VertexCount) do {                      \
  TIMED_BLOCK("Draw");                              \
  DEBUG_TRACK_DRAW_CALL(__FUNCTION__, VertexCount); \
  glDrawArrays(GL_TRIANGLES, 0, (s32)VertexCount);  \
  END_BLOCK(); } while (0)

inline void
SetViewport(v2 Dim)
{
  glViewport(0, 0, (s32)Dim.x, (s32)Dim.y);
  return;
}

// TODO(Jesse): This only gets used when computing the shadow map, so I'm not
// even sure if it works ATM
inline m4
Orthographic( r32 X, r32 Y, r32 Zmin, r32 Zmax)
{
  r32 r = X;
  r32 l = -X;

  r32 t = Y;
  r32 b = -Y;

  r32 f = Zmax;
  r32 n = Zmin;

  m4 Result = {
    V4(2/(r-l) , 0       , 0        , -1*((r+l)/(r-l)) ) ,
    V4(0       , 2/(t-b) , 0        , -1*((t+b)/(t-b)) ) ,
    V4(0       , 0       , -2/(f-n) , -1*((f+n)/(f-n)) ) ,
    V4(0       , 0       , 0        , 1)
  };

  return Result;
}

inline m4
Perspective(radians FOV, v2 WindowDim, r32 NearClip, r32 FarClip)
{
  r32 TanHalfFOV = (r32)tan(FOV / 2.0f);
  r32 Aspect = WindowDim.x/WindowDim.y;

  r32 Z = (-(FarClip+NearClip)) / (FarClip-NearClip);
  r32 ZZ = (-(2.0f*FarClip*NearClip)) / (FarClip - NearClip);

  m4 Result = {
    V4(1.0f/(Aspect*TanHalfFOV) , 0               , 0  , 0)     ,
    V4(0                        , 1.0f/TanHalfFOV , 0  , 0)     ,
    V4(0                        , 0               , Z  , -1.0f) ,
    V4(0                        , 0               , ZZ , 0)     ,
  };

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
      V2(WindowWidth, WindowHeight),
      Camera->Frust.nearClip,
      Camera->Frust.farClip);

  return Projection;
}

inline void
BufferDataToCard(u32 BufferId, u32 Stride, u32 ByteCount, void *Data, u32 *AttributeIndex)
{
#if BONSAI_INTERNAL
  debug_state *DebugState = GetDebugState();
  DebugState->BytesBufferedToCard += ByteCount;
#endif

  glEnableVertexAttribArray(*AttributeIndex);
  glBindBuffer(GL_ARRAY_BUFFER, BufferId);
  glBufferData(GL_ARRAY_BUFFER, ByteCount, Data, GL_STATIC_DRAW);
  glVertexAttribPointer(*AttributeIndex, (s32)Stride, GL_FLOAT, GL_FALSE, 0, (void*)0);
  *AttributeIndex += 1;

  return;
}

template <typename T> inline void
BufferVertsToCard(u32 BufferId, T *Mesh, u32 *AttributeIndex)
{
  TIMED_FUNCTION();
  u32 ByteCount = Mesh->At*sizeof(*Mesh->Verts);
  u32 Stride = sizeof(*Mesh->Verts)/sizeof(Mesh->Verts[0].E[0]);

  BufferDataToCard(BufferId, Stride, ByteCount, (void*)Mesh->Verts, AttributeIndex);

  return;
}

template <typename T> inline void
BufferColorsToCard(u32 BufferId, T *Mesh, u32* AttributeIndex)
{
  TIMED_FUNCTION();
  u32 Stride = sizeof(*Mesh->Colors)/sizeof(Mesh->Colors[0].E[0]);
  u32 ByteCount = Mesh->At*sizeof(*Mesh->Colors);

  BufferDataToCard(BufferId, Stride, ByteCount, (void*)Mesh->Colors, AttributeIndex);

  return;
}

template <typename T> inline void
BufferNormalsToCard(u32 BufferId, T *Mesh, u32 *AttributeIndex)
{
  TIMED_FUNCTION();
  u32 Stride = sizeof(*Mesh->Normals)/sizeof(Mesh->Normals[0].E[0]);
  u32 ByteCount = Mesh->At*sizeof(*Mesh->Normals);

  BufferDataToCard(BufferId, Stride, ByteCount, (void*)Mesh->Normals, AttributeIndex);

  return;
}

template <typename T> inline void
BufferUVsToCard(u32 BufferId, T *Mesh, u32 *AttributeIndex)
{
  TIMED_FUNCTION();
  u32 ByteCount = Mesh->At*sizeof(*Mesh->UVs);
  u32 Stride = sizeof(*Mesh->UVs)/sizeof(Mesh->UVs[0].x);

  BufferDataToCard(BufferId, Stride, ByteCount, (void*)Mesh->UVs, AttributeIndex);

  return;
}
