// src/engine/graphics.h:49:0

link_internal void
InitializeTerrainDecorationRenderContext( terrain_decoration_render_context *Struct
    , texture *DerivsTex
  , v3 ChunkDim
  , v3 WorldspaceChunkBasis
  , v3 ChunkResolution
)
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/decoration/terrain_decoration.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->DerivsTex = DerivsTex;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->DerivsTex, "DerivsTex");

    Struct->ChunkDim = ChunkDim;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkDim, "ChunkDim");

    Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

    Struct->ChunkResolution = ChunkResolution;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkResolution, "ChunkResolution");

  u32 Expected =  4 ;
  if (UniformIndex != Expected )
  {
    Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
  }



  AssertNoGlErrors;

  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_decoration_render_context *Struct )
{
  GetStdlib()->GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex !=  4  )
  {
    Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots!");
  }
}

