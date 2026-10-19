
link_internal texture *
DispatchTerrainShaders(graphics *Graphics, world_chunk *Chunk)
{
  auto *RC = &Graphics->TerrainShapingRC;

  RC->WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
  RC->ChunkResolution = V3(Chunk->DimInChunks);

  auto GL = GetGL();

  UseShader(RC);

  /* BindUniformByName(&RC->Program, "ShapingTex", &RC->ShapingTex, 0); */
  /* BindUniformByName(&RC->Program, "DerivsTex",  &RC->DerivsTex,  1); */


  //
  // Terrain shaping
  //
  {
    TIMED_NAMED_BLOCK(TerrainShaping);

    GL->BindFramebuffer(GL_FRAMEBUFFER, RC->ShapingFBO.ID);
    BindUniformByName(&RC->Program, "Stage", 0);

    v2i DestTextureDim = RC->ShapingTex.Dim;
    Assert(DestTextureDim == V2i(68, 68*68));
    SetViewport(DestTextureDim);

    RenderQuad();
    AssertNoGlErrors;
  }

  //
  // Calculate derivs of terrain shaping step
  //
  {
    TIMED_NAMED_BLOCK(TerrainDerivs);

    GL->BindFramebuffer(GL_FRAMEBUFFER, RC->DerivsFBO.ID);
    BindUniformByName(&RC->Program, "Stage", 1);

    v2i DestTextureDim = RC->DerivsTex.Dim;
    Assert(DestTextureDim == V2i(66, 66*66));
    SetViewport(DestTextureDim);

    RenderQuad();
    AssertNoGlErrors;
  }

  //
  // Terrain decoration
  //
  {
    TIMED_NAMED_BLOCK(TerrainDecoration);

    GL->BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO->ID);
    BindUniformByName(&RC->Program, "Stage", 2);

    v2i DestTextureDim = RC->DestTex->Dim;
    SetViewport(DestTextureDim);

    RenderQuad();
    AssertNoGlErrors;
  }

  return RC->DestTex;
}

