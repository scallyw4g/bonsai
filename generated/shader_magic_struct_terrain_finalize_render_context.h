// src/engine/graphics.h:87:0

link_internal b32
InitializeTerrainFinalizeRenderContext(
  terrain_finalize_render_context *Struct
    , u32 Ignored

)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/TerrainFinalize.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->Ignored = Ignored;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Ignored, "Ignored");



    u32 Expected =  1 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseShader( terrain_finalize_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
  {
    GetGL()->UseProgram(Struct->Program.ID);

    s32 TextureUnit = 0;
    s32 UniformIndex = 0;
            BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;



    if (UniformIndex !=  1  )
    {
      Error("Shader (terrain_finalize_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/TerrainFinalize.fragmentshader\")");
  }
}

