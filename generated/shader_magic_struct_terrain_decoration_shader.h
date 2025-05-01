// src/engine/graphics.h:51:0

link_internal void
InitializeTerrainDecorationRenderContext( terrain_decoration_render_context *Struct    , texture *DerivsTex     , v3 ChunkDim     , v3 WorldspaceBasis     , v3 ChunkResolution  )
{
      Struct->Program = CompileShaderPair(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/decoration/terrain_decoration.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->DerivsTex = DerivsTex;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->DerivsTex, "DerivsTex");

    Struct->ChunkDim = ChunkDim;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkDim, "ChunkDim");

    Struct->WorldspaceBasis = WorldspaceBasis;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->WorldspaceBasis, "WorldspaceBasis");

    Struct->ChunkResolution = ChunkResolution;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ChunkResolution, "ChunkResolution");

  if (UniformIndex !=  4  )
  {
    Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_decoration_render_context *Struct )
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
    Error("Shader (terrain_decoration_render_context) had an incorrect number of uniform slots!");
  }
}

