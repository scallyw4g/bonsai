// src/engine/graphics.h:86:0

link_internal void
InitializeTerrainFinalizeRenderContext( terrain_finalize_render_context *Struct    , texture *InputTex  )
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/TerrainFinalize.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->InputTex = InputTex;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->InputTex, "InputTex");



  if (UniformIndex !=  1  )
  {
    Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_finalize_render_context *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;



  if (UniformIndex !=  1  )
  {
    Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots!");
  }
}

