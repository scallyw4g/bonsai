// src/engine/graphics.h:46:0

link_internal void
InitializeTerrainShapingRenderContext( terrain_shaping_render_context *Struct
    , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution
)
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/shaping/default.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->ChunkDim = ChunkDim;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkDim, "ChunkDim");

    Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

    Struct->ChunkResolution = ChunkResolution;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkResolution, "ChunkResolution");

  u32 Expected =  3 ;
  if (UniformIndex != Expected )
  {
    Error("Shader (terrain_shaping_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
  }



  AssertNoGlErrors;

  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_shaping_render_context *Struct )
{
  GetGL()->UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex !=  3  )
  {
    Error("Shader (terrain_shaping_render_context) had an incorrect number of uniform slots!");
  }
}

