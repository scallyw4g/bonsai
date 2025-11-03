

link_internal texture *
DispatchTerrainShaders(graphics *Graphics, world_chunk *Chunk)
{
  texture *InputTex = 0;
  //
  // Launch terrain shaping shader
  //
  {
    auto *RC = &Graphics->TerrainShapingRC;

    RC->WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
    RC->ChunkResolution = V3(Chunk->DimInChunks);

    TIMED_NAMED_BLOCK(TerrainShaping);

    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO.ID);
    UseShader(RC);

    v2i DestTextureDim = RC->DestTex.Dim;
    SetViewport(DestTextureDim);
    Assert(DestTextureDim == V2i(68, 68*68));

    RenderQuad();

    AssertNoGlErrors;
    InputTex = &RC->DestTex;
  }


  //
  // Calculate derivs of terrain shaping step
  //
  {
    auto *RC = &Graphics->TerrainDerivsRC;

    TIMED_NAMED_BLOCK(TerrainDerivs);
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO.ID);
    UseShader(RC);

    // @derivs_texture_binding_to_shader_unit_0
    BindUniformByName(&RC->Program, "InputTex", InputTex, 0);

    v2i DestTextureDim = RC->DestTex.Dim;
    Assert(DestTextureDim == V2i(66, 66*66));
    SetViewport(DestTextureDim);

    RenderQuad();

    AssertNoGlErrors;

    /* InputTex = &RC->DestTex; */
  }

  //
  // Launch terrain decoration shader
  //
  {
    auto *RC = &Graphics->TerrainDecorationRC;

    RC->WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
    RC->ChunkResolution = V3(Chunk->DimInChunks);

    TIMED_NAMED_BLOCK(TerrainDecoration);
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO->ID);
    UseShader(RC);
    // Using texture unit 1 because the DerivsTex is automatically bound in UseShader to unit 0
    // @derivs_texture_binding_to_shader_unit_0
    BindUniformByName(&RC->Program, "InputTex", InputTex, 1);

    RenderQuad();

    AssertNoGlErrors;

    InputTex = RC->DestTex;
  }

  return InputTex;
}
