// src/engine/graphics.h:71:0

link_internal b32
InitializeWorldEditRenderContext(
  world_edit_render_context *Struct
    , v3 *ChunkDim
  , v3 *WorldspaceChunkBasis
  , v3 *ChunkResolution
  , s32 Type
)
{
      b32 Result = CompileShaderPair(&Struct->Program, CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/world_edit.fragmentshader"));

  if (Result)
  {
    Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

    u32 UniformIndex = 0;

            Struct->ChunkDim = ChunkDim;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ChunkDim, "ChunkDim");

        Struct->WorldspaceChunkBasis = WorldspaceChunkBasis;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->WorldspaceChunkBasis, "WorldspaceChunkBasis");

        Struct->ChunkResolution = ChunkResolution;
    SetShaderUniform(&Struct->Program, UniformIndex++, Struct->ChunkResolution, "ChunkResolution");

        Struct->Type = Type;
    SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->Type, "Type");

    u32 Expected =  4 ;
    if (UniformIndex != Expected )
    {
      Error("Shader (world_edit_render_context) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
    }
  }



  AssertNoGlErrors;
  return Result;
}

link_internal void
UseShader( world_edit_render_context *Struct )
{
  if (Struct->Program.ID != INVALID_SHADER)
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

        BindUniformById(Struct->Uniforms+UniformIndex, &TextureUnit);
    ++UniformIndex;

    if (UniformIndex !=  4  )
    {
      Error("Shader (world_edit_render_context) had an incorrect number of uniform slots!");
    }
  }
  else
  {
    SoftError("Attempted to bind uncompiled Shader (\"external/bonsai_stdlib/shaders/Passthrough.vertexshader\") | (\"shaders/terrain/world_edit.fragmentshader\")");
  }
}

