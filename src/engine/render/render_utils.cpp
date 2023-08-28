global_variable u32 Global_QuadVertexBuffer;
global_variable v3 GlobalLightPosition = {{0.20f, 1.0f, 1.0f}};

global_variable m4 NdcToScreenSpace =
{
  V4(0.5, 0.0, 0.0, 0.0),
  V4(0.0, 0.5, 0.0, 0.0),
  V4(0.0, 0.0, 0.5, 0.0),
  V4(0.5, 0.5, 0.5, 1.0)
};

void
Init_Global_QuadVertexBuffer()
{
  GL.GenBuffers(1, &Global_QuadVertexBuffer);
  Assert(Global_QuadVertexBuffer);

  GL.BindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  GL.BufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
RenderQuad()
{
  if (!Global_QuadVertexBuffer) { Init_Global_QuadVertexBuffer(); }

  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, Global_QuadVertexBuffer);
  GL.VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  Draw(6);

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);
  GL.DisableVertexAttribArray(0);
}

void
DrawTexturedQuad(shader *SimpleTextureShader)
{
  r32 Scale = 0.5f;

  GL.DepthFunc(GL_LEQUAL);

  texture *Texture = SimpleTextureShader->FirstUniform->Texture;
  SetViewport( V2(Texture->Dim.x, Texture->Dim.y)*Scale );

  GL.UseProgram(SimpleTextureShader->ID);

  BindShaderUniforms(SimpleTextureShader);

  RenderQuad();
  AssertNoGlErrors;

  return;
}

