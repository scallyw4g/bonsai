link_internal void
FinalizeNoiseValues(   engine_resources *Engine,
  work_queue_entry_finalize_noise_values *Task,
                      thread_local_state *Thread,
                          work_queue_job *Job
  )
{
  UNPACK_ENGINE_RESOURCES(Engine);

  octree_node *Node = Task->DestNode;
  auto Chunk = Node->Chunk;

  u32 *NoiseValues = Task->NoiseData;
  v3i  NoiseDim    = Task->NoiseDim;
  Assert(NoiseValues);
  Assert(Chunk);

  world_chunk *DestChunk = Node->Chunk;

  // NOTE(Jesse): This is valid when we resubmit a chunk because we change the edits
  // We need to keep these intact so that we don't flicker and hence have to wait
  // till the very end to reallocate
  // @dest_chunk_can_have_mesh
  /* Assert(HasGpuMesh(&DestChunk->Handles) == False); */

  gen_chunk *GenChunk = GetOrAllocate(&Engine->GenChunkFreelist, {}, Chunk->Dim + V3i(0, 2, 2), Chunk->DimInChunks, Thread->PermMemory);
  world_chunk *SynChunk = &GenChunk->Chunk;

  Assert(GenChunk->Buffer.End == 0);
  Assert(HasGpuMesh(SynChunk) == False);

  voxel *Voxels = GenChunk->Voxels;

  Assert(NoiseDim == V3i(66, 66, 66));
  Assert(SynChunk->Dim == V3i(64, 66, 66));

  v3i WorldBasis = {};
  v3i SrcToDest = {};
  s64 zMin = 0;

  if ( (Node->Flags & Chunk_SpawnTriggersRun) == 0)
  {
    SetFlag(&Node->Flags, Chunk_SpawnTriggersRun);

    b32 SpawnedStuff = False;
    IterateOver(&Engine->ChunkCompletionCallbacks, CP, CompletionCallbackIndex)
    {
      chunk_completion_callback Callback = *CP;
      SpawnedStuff |= Callback(Engine, NoiseDim, NoiseValues, Node);
    }
  }

  u32 ChunkSum = FinalizeOccupancyMasksFromNoiseValues(SynChunk, Voxels, WorldBasis, NoiseDim, NoiseValues, SrcToDest, zMin);

  b32 Continued = False;
  if (ChunkSum && ChunkSum < u32(Volume(SynChunk->Dim)))
  {
    MakeFaceMasks_NoExteriorFaces(SynChunk->Occupancy,
                                  SynChunk->xOccupancyBorder,
                                  SynChunk->FaceMasks,
                                  Voxels,
                                  SynChunk->Dim,
                                  {},
                                  SynChunk->Dim);

    Assert(SynChunk->Dim.x == 64);
    Assert(SynChunk->Dim.y == 66);
    Assert(SynChunk->Dim.z == 66);

    Assert(DestChunk->FilledCount == 0);
    Assert(DestChunk->Dim.x == 64);
    Assert(DestChunk->Dim.y == 64);
    Assert(DestChunk->Dim.z == 64);
    RangeIterator(z, 64)
    RangeIterator(y, 64)
    {
      s32 OI = (y+1) + ((z+1)*66);
      u64 Occ = SynChunk->Occupancy[OI];
      DestChunk->FilledCount += CountBitsSet_Kernighan(Occ);

      DestChunk->Occupancy[y + (z*64)] = Occ;
      DestChunk->FaceMasks[(y + (z*64))+0] = SynChunk->FaceMasks[OI+0];
      DestChunk->FaceMasks[(y + (z*64))+1] = SynChunk->FaceMasks[OI+1];
      DestChunk->FaceMasks[(y + (z*64))+2] = SynChunk->FaceMasks[OI+2];
      DestChunk->FaceMasks[(y + (z*64))+3] = SynChunk->FaceMasks[OI+3];
      DestChunk->FaceMasks[(y + (z*64))+4] = SynChunk->FaceMasks[OI+4];
      DestChunk->FaceMasks[(y + (z*64))+5] = SynChunk->FaceMasks[OI+5];
    }


    RangeIterator(zIndex, 64)
    RangeIterator(yIndex, 64)
    {
      s32 DstIndex = GetIndex(yIndex, zIndex, V2i(64));
      s32 SrcIndex = GetIndex(yIndex+1, zIndex+1, V2i(66));

      Assert(DestChunk->Occupancy[DstIndex] == GenChunk->Chunk.Occupancy[SrcIndex]);
    }

    Assert(DestChunk->FilledCount <= s32(Volume(DestChunk->Dim)));

    /* FinalizeChunkInitialization(SynChunk); */

    s32 FacesRequired = CountRequiredFacesForMesh_Naieve(SynChunk->FaceMasks, SynChunk->Dim, V3i(0,1,1));
    if (FacesRequired)
    {
      Continued = True;

      /* Info("Chunk had faces (%d)", FacesRequired); */
      Assert(Node->Flags & Chunk_Queued);
      /* PushBonsaiRenderCommandAllocateAndMapGpuElementBuffer( */
      /*     LoRenderQ, DataType_v3_u8, u32(FacesRequired*VERTS_PER_FACE), &GenChunk->Mesh, */
      /*     GenChunk, Node); // NOTE(Jesse): These should go away once we can specify the next job here.. */

      AllocateMesh(&GenChunk->Buffer, DataType_v3_u8, u32(FacesRequired*VERTS_PER_FACE), &Engine->Heap);

      BuildWorldChunkMeshFromMarkedVoxels_Naieve( GenChunk->Voxels, SynChunk->FaceMasks, SynChunk->Dim, {}, {}, &GenChunk->Buffer, 0);

      /* FinalizeShitAndFuckinDoStuff(GenChunk, DestModel->Node); */
      auto Params = FinalizeShitAndFuckinDoStuff_AsyncParams(LoRenderQ, GenChunk, Node);
      work_queue_entry Next = WorkQueueEntryAsyncFunction(LoRenderQ, &Params);
      PushTask(Job, &Next);
    }
  }

  Assert(Node->Flags & Chunk_Queued);

  // If we didn't continue, we're done, free the resources
  //
  if (Continued == False)
  {
    Assert(GenChunk->Buffer.End == 0);
    /* DeallocateHandles(LoRenderQ, &GenChunk->Mesh.Handles); */

    ClearGenChunk( GenChunk );
    Free(&GetEngineResources()->GenChunkFreelist, GenChunk);
    FinalizeNodeInitializaion(Node);

    // Deallocate the stale mesh if the new chunk didn't have a mesh
    if (Node->Chunk && HasGpuMesh(Node->Chunk) )
    {
      GpuHeapDeallocate(&GetGraphics()->GpuHeap, &Node->Chunk->Mesh);
    }
  }

  // NOTE(Jesse): The CPU initializer obviously doesn't need to deallocate
  // a PBO, so it sets the PBO handle to -1
  Assert(Task->PBOBuf.PBO != INVALID_PBO_HANDLE);
  PushBonsaiRenderCommandUnmapAndDeallocatePbo(LoRenderQ, Task->PBOBuf);

  Assert(Graphics->NoiseFinalizeJobsPending);
  AtomicDecrement(&Graphics->NoiseFinalizeJobsPending);
  AtomicDecrement(&Graphics->TotalChunkJobsActive);
}


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
