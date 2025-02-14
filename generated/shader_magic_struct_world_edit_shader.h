// src/engine/graphics.h:39:0

link_internal void
InitializeWorldEditRenderContext( world_edit_render_context *Struct    , v3 *ChunkDim     , v3 *WorldspaceBasis     , v3 *ChunkResolution     , s32 Type  )
{
      Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/world_edit.fragmentshader"));

  u32 UniformIndex = 0;

      Struct->ChunkDim = ChunkDim;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, Struct->ChunkDim, "ChunkDim");
  ++UniformIndex;

    Struct->WorldspaceBasis = WorldspaceBasis;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, Struct->WorldspaceBasis, "WorldspaceBasis");
  ++UniformIndex;

    Struct->ChunkResolution = ChunkResolution;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, Struct->ChunkResolution, "ChunkResolution");
  ++UniformIndex;

    Struct->Type = Type;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, &Struct->Type, "Type");
  ++UniformIndex;

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

