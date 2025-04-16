// src/engine/graphics.h:53:0

link_internal void
InitializeTerrainDerivsRenderContext( terrain_derivs_render_context *Struct )
{
      Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/derivs.fragmentshader"));

  u32 UniformIndex = 0;

  

  if (UniformIndex !=  3  )
  {
    Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_derivs_render_context *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
  

  if (UniformIndex !=  3  )
  {
    Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots!");
  }
}

