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
  glDrawArrays(GL_TRIANGLES, 0, (s32)VertexCount);  \
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
  glGenBuffers(1, &Global_QuadVertexBuffer);
  Assert(Global_QuadVertexBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
RenderQuad()
{
  if (!Global_QuadVertexBuffer)
  {
    Init_Global_QuadVertexBuffer();
  }

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  Draw(6);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);
}

void
DrawTexturedQuad(shader *SimpleTextureShader)
{
  r32 Scale = 0.5f;

  glDepthFunc(GL_LEQUAL);

  texture *Texture = SimpleTextureShader->FirstUniform->Texture;
  SetViewport( V2(Texture->Dim.x, Texture->Dim.y)*Scale );

  glUseProgram(SimpleTextureShader->ID);

  BindShaderUniforms(SimpleTextureShader);

  RenderQuad();
  AssertNoGlErrors;

  return;
}
