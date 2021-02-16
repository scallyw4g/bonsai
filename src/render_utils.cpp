global_variable u32 Global_QuadVertexBuffer;
global_variable v3 GlobalLightPosition = {{0.20f, 1.0f, 1.0f}};

global_variable m4 NdcToScreenSpace =
{
  V4(0.5, 0.0, 0.0, 0.0),
  V4(0.0, 0.5, 0.0, 0.0),
  V4(0.0, 0.0, 0.5, 0.0),
  V4(0.5, 0.5, 0.5, 1.0)
};

#define Draw(VertexCount) do {                      \
  TIMED_BLOCK("Draw");                              \
  DEBUG_TRACK_DRAW_CALL(__FUNCTION__, VertexCount); \
  GL->DrawArrays(GL_TRIANGLES, 0, (s32)VertexCount);  \
  END_BLOCK(); } while (0)

v3
Unproject(v2 ScreenP, r32 ClipZDepth, v2 ScreenDim, m4 *InvViewProj)
{
  TIMED_FUNCTION();

  Assert(ClipZDepth <= 1.0f);
  Assert(ClipZDepth >= 0.0f);

  v4 ClipCoords = (2.0f * V4(V3(ScreenP / ScreenDim, ClipZDepth), 1.0f)) -1.0f;
  ClipCoords.y *= -1;

  v4 WorldSpace = TransformColumnMajor(*InvViewProj, ClipCoords);
  v3 Result = WorldSpace.xyz / WorldSpace.w;

  return Result;
}

void
Init_Global_QuadVertexBuffer()
{
  GL->GenBuffers(1, &Global_QuadVertexBuffer);
  Assert(Global_QuadVertexBuffer);

  GL->BindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  GL->BufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
  GL->BindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
RenderQuad()
{
  if (!Global_QuadVertexBuffer)
  {
    Init_Global_QuadVertexBuffer();
  }

  GL->EnableVertexAttribArray(0);
  GL->BindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  GL->VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  Draw(6);

  GL->BindBuffer(GL_ARRAY_BUFFER, 0);
  GL->DisableVertexAttribArray(0);
}

void
DrawTexturedQuad(shader *SimpleTextureShader)
{
  r32 Scale = 0.5f;

  GL->DepthFunc(GL_LEQUAL);

  texture *Texture = SimpleTextureShader->FirstUniform->Texture;
  SetViewport( V2(Texture->Dim.x, Texture->Dim.y)*Scale );

  GL->UseProgram(SimpleTextureShader->ID);

  BindShaderUniforms(SimpleTextureShader);

  RenderQuad();
  AssertNoGlErrors;

  return;
}

inline void
BufferDataToCard(u32 BufferId, u32 Stride, u32 ByteCount, void *Data, u32 *AttributeIndex)
{
#if BONSAI_INTERNAL
  debug_state *DebugState = GetDebugState();
  DebugState->BytesBufferedToCard += ByteCount;
#endif

  GL->EnableVertexAttribArray(*AttributeIndex);
  GL->BindBuffer(GL_ARRAY_BUFFER, BufferId);
  GL->BufferData(GL_ARRAY_BUFFER, ByteCount, Data, GL_STATIC_DRAW);
  GL->VertexAttribPointer(*AttributeIndex, (s32)Stride, GL_FLOAT, GL_FALSE, 0, (void*)0);
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

