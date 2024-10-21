// src/engine/render/shadow_map.h:21:0

link_internal void
InitializeShadowMapShader( shadow_map_shader *Struct  , m4 MVP    , m4 ModelMatrix    , v3*MinClipP_worldspace    , v3*MaxClipP_worldspace  )
{
  Struct->Program = LoadShaders(CSz("shaders/DepthRTT.vertexshader"), CSz("shaders/DepthRTT.fragmentshader"));

  u32 UniformIndex = 0;

  Struct->MVP = MVP;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->MVP, "MVP");
  ++UniformIndex;

  Struct->ModelMatrix = ModelMatrix;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program,&Struct->ModelMatrix, "ModelMatrix");
  ++UniformIndex;

  Struct->MinClipP_worldspace = MinClipP_worldspace;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, Struct->MinClipP_worldspace, "MinClipP_worldspace");
  ++UniformIndex;

  Struct->MaxClipP_worldspace = MaxClipP_worldspace;
  Struct->Uniforms[UniformIndex] = ShaderUniform(&Struct->Program, Struct->MaxClipP_worldspace, "MaxClipP_worldspace");
  ++UniformIndex;

  if (UniformIndex != 4  )
  {
    Error("Shader (shadow_map_shader) had an incorrect number of uniform slots!");
  }



  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( shadow_map_shader *Struct )
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

  if (UniformIndex != 4  )
  {
    Error("Shader (shadow_map_shader) had an incorrect number of uniform slots!");
  }
}

