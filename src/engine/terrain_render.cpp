
link_internal texture *
DispatchTerrainShaders(graphics *Graphics, world_chunk **Chunks, u32 ChunkCount)
{
  Assert(ChunkCount > 0);
  Assert(ChunkCount <= TERRAIN_BATCH_CHUNK_COUNT);

  auto *RC = &Graphics->TerrainShapingRC;

  v3 BatchChunkBasis[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  v3 BatchChunkResolution[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  RangeIterator_t(u32, ChunkIndex, ChunkCount)
  {
    world_chunk *Chunk = Chunks[ChunkIndex];
    BatchChunkBasis[ChunkIndex]      = V3(Chunk->WorldP) * V3(64);
    BatchChunkResolution[ChunkIndex] = V3(Chunk->DimInChunks);
  }

  // NOTE(Jesse): These are the per-chunk values consumed by the world edit
  // shader, which processes one chunk at a time.  The caller sets them as it
  // iterates the batch.
  RC->WorldspaceChunkBasis = BatchChunkBasis[0];
  RC->ChunkResolution      = BatchChunkResolution[0];

  auto GL = GetGL();

  UseShader(RC);

  {
    s32 BasisLoc = GL->GetUniformLocation(RC->Program.ID, "BatchChunkBasis");
    s32 ResLoc   = GL->GetUniformLocation(RC->Program.ID, "BatchChunkResolution");
    if (BasisLoc != INVALID_SHADER_UNIFORM) { GL->Uniform3fv(BasisLoc, s32(ChunkCount), (r32*)BatchChunkBasis); }
    if (ResLoc   != INVALID_SHADER_UNIFORM) { GL->Uniform3fv(ResLoc,   s32(ChunkCount), (r32*)BatchChunkResolution); }
    AssertNoGlErrors;
  }

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
    Assert(DestTextureDim == V2i(68*TERRAIN_BATCH_CHUNK_COUNT, 68*68));
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
    Assert(DestTextureDim == V2i(66*TERRAIN_BATCH_CHUNK_COUNT, 66*66));
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

