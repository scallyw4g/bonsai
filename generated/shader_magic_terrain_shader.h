// src/engine/graphics.h:16:0

link_internal void
InitializeTerrainShader( terrain_shader *Struct  , v3 ChunkDim    , v3 WorldspaceBasis    , v3 ChunkResolution  )
{
  Struct->Program = LoadShaders(CSz("external/bonsai_stdlib/shaders/Passthrough.vertexshader"), CSz("shaders/terrain/default.fragmentshader"));

  u32 UniformIndex = 0;

  Struct->ChunkDim = ChunkDim;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->ChunkDim, "ChunkDim");
  ++UniformIndex;

  Struct->WorldspaceBasis = WorldspaceBasis;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->WorldspaceBasis, "WorldspaceBasis");
  ++UniformIndex;

  Struct->ChunkResolution = ChunkResolution;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->ChunkResolution, "ChunkResolution");
  ++UniformIndex;

  if (UniformIndex != 3  )
  {
    Error("Shader (terrain_shader) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( terrain_shader *Struct )
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

  if (UniformIndex != 3  )
  {
    Error("Shader (terrain_shader) had an incorrect number of uniform slots!");
  }
}

