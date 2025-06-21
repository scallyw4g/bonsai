// src/engine/render/shadow_map.h:19:0

link_internal void
InitializeShadowMapShader( shadow_map_shader *Struct
    , m4 ModelMatrix
  , m4 ViewProjection
  , v3 *MinClipP_worldspace
  , v3 *MaxClipP_worldspace
)
{
      Struct->Program = CompileShaderPair(CSz("shaders/DepthRTT.vertexshader"), CSz("shaders/DepthRTT.fragmentshader"));
  Struct->Program.Uniforms = ShaderUniformBuffer(Struct->Uniforms, ArrayCount(Struct->Uniforms));

  u32 UniformIndex = 0;

      Struct->ModelMatrix = ModelMatrix;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ModelMatrix, "ModelMatrix");

    Struct->ViewProjection = ViewProjection;
  SetShaderUniform(&Struct->Program, UniformIndex++, &Struct->ViewProjection, "ViewProjection");

    Struct->MinClipP_worldspace = MinClipP_worldspace;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->MinClipP_worldspace, "MinClipP_worldspace");

    Struct->MaxClipP_worldspace = MaxClipP_worldspace;
  SetShaderUniform(&Struct->Program, UniformIndex++, Struct->MaxClipP_worldspace, "MaxClipP_worldspace");

  u32 Expected =  4 ;
  if (UniformIndex != Expected )
  {
    Error("Shader (shadow_map_shader) had an incorrect number of uniform slots! Expected (%d), Got (%d)", Expected, UniformIndex);
  }



  AssertNoGlErrors;

  RegisterShaderForHotReload(GetStdlib(), &Struct->Program);
}

link_internal void
UseShader( shadow_map_shader *Struct )
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
    Error("Shader (shadow_map_shader) had an incorrect number of uniform slots!");
  }
}

