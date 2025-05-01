// src/engine/graphics.h:54:0

link_internal void
InitializeTerrainDerivsRenderContext( terrain_derivs_render_context *Struct    , v3 Ignored  )
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/derivs.fragmentshader"));

  u32 UniformIndex = 0;

      Struct->Ignored = Ignored;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, &Struct->Ignored, "Ignored");
  ++UniformIndex;



  if (UniformIndex !=  1  )
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
      BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;



  if (UniformIndex !=  1  )
  {
    Error("Shader (terrain_derivs_render_context) had an incorrect number of uniform slots!");
  }
}

