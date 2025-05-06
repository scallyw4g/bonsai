// src/engine/graphics.h:71:0

link_internal void
InitializeWorldEditRenderContext( world_edit_render_context *Struct    , v3 *ChunkDim     , v3 *WorldspaceChunkBasis     , v3 *ChunkResolution     , s32 Type  )
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/world_edit.fragmentshader"));
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

  if (UniformIndex !=  4  )
  {
    Error("Shader (world_edit_render_context) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( world_edit_render_context *Struct )
{
  GL.UseProgram(Struct->Program.ID);

  s32 TextureUnit = 0;
  s32 UniformIndex = 0;
      BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

    BindUnifromById(Struct->Uniforms+UniformIndex, &TextureUnit);
  ++UniformIndex;

  if (UniformIndex !=  4  )
  {
    Error("Shader (world_edit_render_context) had an incorrect number of uniform slots!");
  }
}

