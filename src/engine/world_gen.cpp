
link_internal texture *
DispatchTerrainShaders(graphics *Graphics, terrain_gen_job_texture_pack *Pack, world_chunk **Chunks, u32 ChunkCount)
{
  Assert(ChunkCount > 0);
  Assert(ChunkCount <= TERRAIN_BATCH_CHUNK_COUNT);

  auto *RC = &Graphics->TerrainGenRC;

  // Point the shader's sampler bindings at this job's textures
  /* RC->ShapingTex = Pack->ShapingTex; */
  /* RC->DestTex    = &Pack->PingPong[0].DestTexture; */

  v3 BatchChunkBasis[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  v3 BatchChunkResolution[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  RangeIterator_t(u32, ChunkIndex, ChunkCount)
  {
    world_chunk *Chunk = Chunks[ChunkIndex];
    /* Assert(Chunk-> */
    BatchChunkBasis[ChunkIndex]      = V3(Chunk->WorldP) * V3(64);
    BatchChunkResolution[ChunkIndex] = V3(Chunk->DimInChunks);
  }

  // NOTE(Jesse): The world edit shader reads per-chunk values from its own
  // SSBO, but the terrain stages still consume these.
  RC->WorldspaceChunkBasis = BatchChunkBasis[0];
  RC->ChunkResolution      = BatchChunkResolution[0];

  auto GL = GetGL();


  UseShader(RC);

  BindUniformByName(&RC->Program, "ShapingTex", &Pack->ShapingTex, 0);
  /* BindUniformByName(&RC->Program, "DestTex", &Pack->PingPong[0].DestTexture, 2); */

  {
    s32 BasisLoc = GL->GetUniformLocation(RC->Program.ID, "BatchChunkBasis");
    s32 ResLoc   = GL->GetUniformLocation(RC->Program.ID, "BatchChunkResolution");
    if (BasisLoc != INVALID_SHADER_UNIFORM) { GL->Uniform3fv(BasisLoc, s32(ChunkCount), (r32*)BatchChunkBasis); }
    if (ResLoc   != INVALID_SHADER_UNIFORM) { GL->Uniform3fv(ResLoc,   s32(ChunkCount), (r32*)BatchChunkResolution); }
    AssertNoGlErrors;
  }

  /* BindUniformByName(&RC->Program, "ShapingTex", &RC->ShapingTex, 0); */


  //
  // Terrain shaping
  //
  {
    TIMED_NAMED_BLOCK(TerrainShaping);

    GL->BindFramebuffer(GL_FRAMEBUFFER, Pack->ShapingFBO.ID);
    BindUniformByName(&RC->Program, "Stage", 0);

    v2i DestTextureDim = Pack->ShapingTex.Dim;
    Assert(DestTextureDim == V2i(68*68, 68*TERRAIN_BATCH_CHUNK_COUNT));
    SetViewport(DestTextureDim);

    RenderQuad();
    AssertNoGlErrors;
  }

  //
  // Terrain decoration (computes normals from the shaping output inline)
  //
  {
    TIMED_NAMED_BLOCK(TerrainDecoration);

    GL->BindFramebuffer(GL_FRAMEBUFFER, Pack->PingPong[0].FBO.ID);
    BindUniformByName(&RC->Program, "Stage", 1);

    v2i DestTextureDim = Pack->PingPong[0].DestTexture.Dim;
    SetViewport(DestTextureDim);

    RenderQuad();
    AssertNoGlErrors;
  }

  return &Pack->PingPong[0].DestTexture;
}

link_internal void
poof(@async @render)
LaunchTerrainGenJob(terrain_gen_job_context *Context)
{
  TIMED_FUNCTION();

  auto GL = GetGL();
  auto Batch = Context->Batch;

  GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);


  RangeIterator_t(u32, TextureUnit, 9)
  {
    GL->ActiveTexture(GL_TEXTURE0 + TextureUnit);
    /* GL->BindTexture(GL_TEXTURE_2D_ARRAY, 0); */
    GL->BindTexture(GL_TEXTURE_2D, 0);
  }

  engine_resources *Engine = GetEngineResources();
  UNPACK_ENGINE_RESOURCES(Engine);

  // NOTE(Jesse): Batch holds up to TERRAIN_BATCH_CHUNK_COUNT nodes, which are
  // all processed together.
  //
  // TODO(Jesse): This is stupid, remove.
  world_chunk *BatchChunks[TERRAIN_BATCH_CHUNK_COUNT] = {};
  u32 BatchCount = (u32)AtElements(&Batch);
  Assert(BatchCount);
  RangeIterator_t(u32, BatchNodeIndex, BatchCount)
  {
    octree_node *BatchNode = Batch.Start[BatchNodeIndex];
    Assert(BatchNode->Flags & Chunk_Queued);
    BatchChunks[BatchNodeIndex] = BatchNode->Chunk;
  }

  terrain_gen_job_texture_pack *Pack = &Graphics->TerrainTextures;

  world_edit_render_context *WorldEditRC = &Graphics->WorldEditRC;

  GL->ClearColor(-10000000000.f, -10000000000.f, -10000000000.f, -10000000000.f);
  ClearFramebuffer(Pack->PingPong + 0);
  ClearFramebuffer(Pack->PingPong + 1);
  ClearFramebuffer(Pack->PingPong + 2);

  DispatchTerrainShaders(Graphics, Pack, BatchChunks, BatchCount);

  //
  // Apply edits
  //

  rtt_framebuffer *Read  = Pack->PingPong;
  rtt_framebuffer *Write = Pack->PingPong + 1;

  UseShader(WorldEditRC);
  AssertNoGlErrors;

  auto Program = &WorldEditRC->Program;

  // Texture unit 0 is the input texture, the rest are brush color textures
  u32 TexUnit = 0;
  BindUniformByName(Program, "InputTex", &Read->DestTexture, TexUnit++);

  // Every chunk's ops are packed back to back in one buffer, and each chunk
  // records the range of ops that apply to it.
  world_edit_chunk_range ChunkRanges[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  world_edit_op *NodeOps[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  s32 NodeOpCounts[TERRAIN_BATCH_MAX_CHUNK_COUNT] = {};
  s32 TotalOpCount = 0;

  RangeIterator_t(u32, BatchIndex, BatchCount)
  {
    octree_node *Node = Batch.Start[BatchIndex];
    world_chunk *Chunk = Node->Chunk;

    ChunkRanges[BatchIndex].WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
    ChunkRanges[BatchIndex].ChunkResolution      = V3(Chunk->DimInChunks);

    {
      AcquireFutex(&Node->Lock);
      if (TotalElements(&Node->Edits))
      {
        AssertNoGlErrors;

        // NOTE(Jesse): @duplicated_edit_ordinal_sort_code
        // Was too lazy to make a templated overload for the sort
        // function.. so here we are.  Hopefully I don't pay for this
        // in the future.
        // {
#if 1
        s32 EditCount = s32(TotalElements(&Node->Edits));
        sort_key *Keys = Allocate(sort_key, GetTranArena(), EditCount);


        // Count Ops and Sort them by Ordinal
        //
        s32 TotalOps = 0;
        IterateOver(&Node->Edits, Edit, EditIndex)
        {
          u32 KeyIndex = u32(GetIndex(&EditIndex));
          Keys[KeyIndex] = {u64(Edit), u64(Edit->Ordinal)};

          if (Edit->Brush)
          {
            TotalOps += Edit->Brush->LayerCount;
          }
        }

        BubbleSort_descending(Keys, u32(EditCount));
#endif
        // }

        world_edit_op *Ops = TotalOps ? Allocate(world_edit_op, GetTranArena(), TotalOps) : 0;
        s32 AtOpIndex = 0;
        RangeIterator(KeyIndex, EditCount)
        {
          world_edit *Edit = Cast(world_edit*, Keys[KeyIndex].Index);
          if (Edit->Brush) // NOTE(Jesse): Don't necessarily have to have a brush if we created the edit before we created a brush.
          {
            world_edit_brush BrushInstance = *Edit->Brush;
            ApplyInstanceEdits(&BrushInstance, &Edit->InstanceEdits);

#if 1
            // NOTE(Jesse): Here we compute AABBs for the layers after
            // applying transforms (atm: just rotation) and union them
            // all to get the extents of all the rotated primitives.
            //
            aabb SimEditBounds_Transformed = InvertedInfinityRectangle_rect3();
            RangeIterator(LayerIndex, BrushInstance.LayerCount)
            {
              brush_layer *Layer = BrushInstance.Layers + LayerIndex;
              aabb LayerBounds = ComputeEditBoundsFromLayerTransforms(Layer, Edit->Region, Edit->Rotation, Chunk->WorldP);
              SimEditBounds_Transformed = Union(&SimEditBounds_Transformed, &LayerBounds);
            }

            aabb SimEditBounds = GetSimSpaceRect(World, Edit->Region);

            v3 SimChunkMin = GetSimSpaceP(World, Chunk->WorldP);
            v3 ChunkRelEditMin = (SimEditBounds_Transformed.Min - SimChunkMin);
            v3 ChunkRelEditMax = (SimEditBounds_Transformed.Max - SimChunkMin);

            RangeIterator(LayerIndex, BrushInstance.LayerCount)
            {
              texture ColorTex = {};
              brush_layer *Layer = BrushInstance.Layers + LayerIndex;

              auto Op = WorldEditOpForBrushLayer(Layer, SimEditBounds, Edit->Rotation, Chunk->WorldP, &TexUnit, &ColorTex);
              if (Op.ColorTextureUnit)
              {
                Info("Binding Tex (%d) to unit (%d)", ColorTex.ID, Op.ColorTextureUnit);
                BindUniformByName(Program, "ColorTextureArray", &ColorTex, Op.ColorTextureUnit);
              }

              Op.ChunkRelEditMin = ChunkRelEditMin;
              Op.ChunkRelEditMax = ChunkRelEditMax;

              // NOTE(Jesse): We pass the brush blend mode in separately because
              // we want to take the layers blend mode, not the blend mode for the brush.
              Op.BrushBlendMode = BrushInstance.BrushBlendMode;

              Op.EditFlags = 0;
              if (LayerIndex == 0)                             { Op.EditFlags |= WorldEditOpEditFlag_FirstLayer; }
              if (LayerIndex == BrushInstance.LayerCount-1) { Op.EditFlags |= WorldEditOpEditFlag_LastLayer; }

              Ops[AtOpIndex++] = Op;
              Assert(AtOpIndex <= TotalOps);
            }
#endif
          }
        }

        NodeOps[BatchIndex]      = Ops;
        NodeOpCounts[BatchIndex] = AtOpIndex;
        TotalOpCount += AtOpIndex;
      }
      ReleaseFutex(&Node->Lock);
    }
  } // for each node in batch

  if (TotalOpCount)
  {
    TIMED_NAMED_BLOCK(WorldEditDrawCall);

    world_edit_op *AllOps = Allocate(world_edit_op, GetTranArena(), TotalOpCount);
    s32 AtOpIndex = 0;
    RangeIterator_t(u32, BatchIndex, BatchCount)
    {
      ChunkRanges[BatchIndex].OpStart = AtOpIndex;
      RangeIterator(OpIndex, NodeOpCounts[BatchIndex])
      {
        AllOps[AtOpIndex++] = NodeOps[BatchIndex][OpIndex];
      }
      ChunkRanges[BatchIndex].OpEnd = AtOpIndex;
    }
    Assert(AtOpIndex == TotalOpCount);

    local_persist u32 OpStorageBuffer = 0;
    local_persist u32 ChunkRangeStorageBuffer = 0;
    if (OpStorageBuffer == 0)         { GL->GenBuffers(1, &OpStorageBuffer); }
    if (ChunkRangeStorageBuffer == 0) { GL->GenBuffers(1, &ChunkRangeStorageBuffer); }

    GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, OpStorageBuffer);
    GL->BufferData(GL_SHADER_STORAGE_BUFFER, Cast(u32, sizeof(world_edit_op))*Cast(u32, TotalOpCount), AllOps, GL_DYNAMIC_DRAW);
    GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, OpStorageBuffer);
    AssertNoGlErrors;

    GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, ChunkRangeStorageBuffer);
    GL->BufferData(GL_SHADER_STORAGE_BUFFER, Cast(u32, sizeof(ChunkRanges)), ChunkRanges, GL_DYNAMIC_DRAW);
    GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ChunkRangeStorageBuffer);
    AssertNoGlErrors;

    BindFramebuffer(Write);
    RenderQuad();
    AssertNoGlErrors;

    GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

#define Swap(a, b) do { auto tmp = b; b = a; a = tmp; } while (false)
    Swap(Read, Write);
#undef Swap
  }

  // We always swap textures, so we read from the Read texture
  texture *CurrentAccumulationTexture = &Read->DestTexture;

  //
  // Terrain Finalize
  //
  {
    TIMED_NAMED_BLOCK(TerrainFinalizeDrawCall);
    GL->BindFramebuffer(GL_FRAMEBUFFER, Context->FinalizeFBO.ID);

    UseShader(&Graphics->TerrainGenRC);

    BindUniformByName(&Graphics->TerrainGenRC.Program, "Stage", 2);
    BindUniformByName(&Graphics->TerrainGenRC.Program, "InputTex", CurrentAccumulationTexture, 3);
    SetViewport(Context->FinalizeTex.Dim);

    /* gpu_timer Timer = StartGpuTimer(); */
    RenderQuad();
    /* EndGpuTimer(&Timer); */
    /* Push(&Graphics->GpuTimers, &Timer); */

    AssertNoGlErrors;
  }


  v3i NoiseDim = V3i(66);
  s32 NoiseElementCount = s32(Volume(CurrentAccumulationTexture->Dim));
  s32 NoiseByteCount = NoiseElementCount*s32(sizeof(u32));


  auto Job = Context->Job;

  // Clear binding points.  Debug code, should remove and make systemic
  {
    GL->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    GL->BindFramebuffer(GL_FRAMEBUFFER, 0);

    RangeIterator_t(u32, TextureUnit, 9)
    {
      GL->ActiveTexture(GL_TEXTURE0 + TextureUnit);
      GL->BindTexture(GL_TEXTURE_2D, 0);
    }
  }

  {
    Context->RenderFence = GL->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    auto Task = WaitForFence_Task(&Plat->LoRenderQ, Job, &Context->RenderFence);
    Task.Timeout = 1;
    PushTask(Job, &Task);
  }

#if 0
  {
    // Initializes Context->ReadbackHandles.Fence for the subsequent WaitForFence_Task
    auto Task = RequestTerrainReadback_Task(&Plat->LoRenderQ, Context, CurrentAccumulationTexture->Dim);
    PushTask(Job, &Task);
  }

  {
    auto Task = WaitForFence_Task(&Plat->LoRenderQ, Job, &Context->ReadbackHandles.Fence);
    Task.Timeout = 1;
    PushTask(Job, &Task);
  }

  {
    auto Task = RecieveTerrainReadback_Task(&Plat->LoRenderQ, Context, NoiseDim);
    PushTask(Job, &Task);
  }

  {
    auto Task = FinalizeNoiseValues_Task(&Plat->LowPriority, Context, NoiseDim);
    PushTask(Job, &Task);
  }

  {
    auto Task = FinalizeShitAndFuckinDoStuff_Task(&Plat->HiRenderQ, Context);
    PushTask(Job, &Task);
  }
#endif

}

link_internal void
FinalizeNoiseValuesForNode( octree_node *DestNode,
                                    u32 *NoiseData,
                                    v3i  NoiseDim )
{
  TIMED_FUNCTION();

  auto Engine = GetEngineResources();
  UNPACK_ENGINE_RESOURCES(Engine);

  auto Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);

  octree_node *Node = DestNode;
  auto Chunk = Node->Chunk;

  Assert(Node->Flags & Chunk_Queued);

  u32 *NoiseValues = NoiseData;
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

      AllocateMesh(&GenChunk->Buffer, DataType_v3_u8, u32(FacesRequired*VERTS_PER_FACE), &Engine->Heap);

      BuildWorldChunkMeshFromMarkedVoxels_Naieve( GenChunk->Voxels, SynChunk->FaceMasks, SynChunk->Dim, {}, {}, &GenChunk->Buffer, 0);
    }
  }

  Assert(Node->Flags & Chunk_Queued);

  // NOTE(Jesse): Whether or not we have a mesh, the node is finalized by
  // FinalizeShitAndFuckinDoStuff, which runs once for the whole batch.
  //
  if (Continued == False)
  {
    Assert(GenChunk->Buffer.End == 0);
    ClearGenChunk( GenChunk );
    Free(&GetEngineResources()->GenChunkFreelist, GenChunk);
    Node->PendingGenChunk = 0;
  }
  else
  {
    Node->PendingGenChunk = GenChunk;
  }
}

// NOTE(Jesse): BatchNoiseData is the mapped PBO for the whole batch readback.
// Chunk k's noise is the contiguous block starting at k*Volume(NoiseDim), so
// we hand each node a pointer into it without copying.  The PBO stays mapped
// until FinalizeShitAndFuckinDoStuff (on the render thread) unmaps and releases it.
//
link_internal void
poof(@async)
FinalizeNoiseValues( terrain_gen_job_context *Context, v3i NoiseDim )
{
  TIMED_FUNCTION();

  auto Batch = Context->Batch;
  u32 BatchCount = (u32)AtElements(&Batch);
  Assert(BatchCount);

  Assert(NoiseDim == V3i(66));
  u32 ChunkNoiseCount = u32(Volume(NoiseDim));

  RangeIterator_t(u32, BatchNodeIndex, BatchCount)
  {
    octree_node *Node = Batch.Start[BatchNodeIndex];
    s64 ElementOffset = (BatchNodeIndex*ChunkNoiseCount);
    Assert(ElementOffset < Context->ReadbackBufferElementLen);
    FinalizeNoiseValuesForNode(Node, Context->ReadbackBuffer + ElementOffset, NoiseDim);
  }
}

// NOTE(Jesse): This takes a pointer to a fence instead of a copied value such
// that we can fire off jobs that initialize fences throughout and still
// initialize the whole job up-front.
//
link_internal void
poof(@async @render)
WaitForFence(work_queue_job *Job, GLsync *Fence)
{
  TIMED_FUNCTION();

  Assert(Fence && *Fence != INVALID_GL_FENCE);

  auto GL = GetGL();
  u32 SyncStatus = GL->ClientWaitSync(*Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
  AssertNoGlErrors;
  switch(SyncStatus)
  {
    case GL_ALREADY_SIGNALED:
    case GL_CONDITION_SATISFIED:
    {
      GL->DeleteSync(*Fence);
      *Fence = INVALID_GL_FENCE;
    } break;

    case GL_TIMEOUT_EXPIRED:
    {
      // The runtime will resubmit the job for us if we decrement NextTaskIndex
      Assert(Job->NextTaskIndex > 0);
      Job->NextTaskIndex -= 1;
    } break;

    // NOTE(Jesse): Made this a hard error because I've never seen it happen,
    // and I'm not sure what the conditions would have to be for it to fire
    case GL_WAIT_FAILED:
    {
      Error("Error waiting on gl sync object");
    } break;
  }
}

link_internal void
poof(@async @render)
RequestTerrainReadback( terrain_gen_job_context *Context, v2i TextureDim )
{
  TIMED_FUNCTION();
  Assert(Context->ReadbackHandles.PBO != INVALID_PBO_HANDLE);

  // NOTE(Jesse): We don't call GetGL() here because it asserts we're the render
  // thread, which this job is not.  Should probably make that better someday..
  auto Stdlib = GetStdlib();
  auto GL = &Stdlib->GL;

  auto Batch = Context->Batch;


  {
    Assert(s64(Volume(TextureDim))*s64(sizeof(u32)) == Context->ReadbackBufferByteLen);
  }

  u32 PBO = Context->ReadbackHandles.PBO;

  // NOTE(Jesse): This runs on the transfer thread, which owns the secondary GL
  // context.  Buffers, textures and sync objects are shared between contexts,
  // but framebuffers are not, so we build a temporary one around the shared
  // finalize texture.
  framebuffer ReadFBO = GenFramebuffer(GL);
  GL->BindFramebuffer(GL_FRAMEBUFFER, ReadFBO.ID);
  FramebufferTexture(GL, &ReadFBO, &Context->FinalizeTex);
  GL->BindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
  AssertNoGlErrors;
  {
    TIMED_NAMED_BLOCK(ReadPixels);
    AssertNoGlErrors;
    GL->ReadPixels(0, 0, TextureDim.x, TextureDim.y, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    AssertNoGlErrors;
  }
  GL->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  GL->BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL->DeleteFramebuffers(1, &ReadFBO.ID);
  AssertNoGlErrors;

  Context->ReadbackHandles.Fence = GL->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

  // The fence is waited on from the render thread's context, so it has to be
  // flushed to the GPU from this one.
  GL->Flush();
}

link_internal void
poof(@async @render)
RecieveTerrainReadback( terrain_gen_job_context *Context, v3i NoiseDim )
{
  TIMED_FUNCTION();

  auto Batch = Context->Batch;

  u32 BatchCount = (u32)AtElements(&Batch);
  Assert(BatchCount);
  Assert(Batch.Start[0]->Flags & Chunk_Queued);

  auto GL = GetGL();
  auto Plat = GetPlatform();
  auto Engine = GetEngineResources();

  auto PBOBuf = Context->ReadbackHandles;
  auto    Job = Context->Job;

  {
    TIMED_NAMED_BLOCK(MapBufferAndCopy);
    AssertNoGlErrors;

    GL->BindBuffer(GL_PIXEL_PACK_BUFFER, PBOBuf.PBO);
    AssertNoGlErrors;
    u32 *ReadbackBuffer = Cast(u32*, GL->MapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
    AssertNoGlErrors;

    CopyMemory(ReadbackBuffer, Context->ReadbackBuffer, u64(Context->ReadbackBufferByteLen));

    GL->UnmapBuffer(GL_PIXEL_PACK_BUFFER);
    GL->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }
}

link_internal void
poof(@async @render)
FinalizeShitAndFuckinDoStuff( terrain_gen_job_context *Context )
{
  TIMED_FUNCTION();

  auto GL = GetGL();
  graphics *Graphics = GetGraphics();

  auto Job = Context->Job;
  auto PBOBuf = Context->ReadbackHandles;

  // NOTE(Jesse): Nodes that FinalizeNoiseValues found no mesh for have no
  // PendingGenChunk.
  //
  // NOTE(Jesse): A node can be requeued as soon as it's finalized, so we never
  // touch a node after finalizing it.
  //
  auto Batch = Context->Batch;
  u32 BatchCount = (u32)AtElements(&Batch);
  RangeIterator_t(u32, BatchNodeIndex, BatchCount)
  {
    octree_node *DestNode = Batch.Start[BatchNodeIndex];
    Assert(DestNode->Flags & Chunk_Queued);

    gen_chunk *GenChunk = DestNode->PendingGenChunk;
    DestNode->PendingGenChunk = 0;

    world_chunk *DestChunk = DestNode->Chunk;

    if (GenChunk == 0)
    {
      FinalizeNodeInitializaion(DestNode);

      // Deallocate the stale mesh if the new chunk didn't have a mesh
      if (DestChunk && HasGpuMesh(DestChunk))
      {
        GpuHeapDeallocate(&Graphics->GpuHeap, &DestChunk->Mesh);
      }
    }
    else
    {
      Assert(HasGpuMesh(&GenChunk->Chunk) == False);

      gpu_heap_allocation NewGpuAllocation = {};

      if (GenChunk->Buffer.At)
      {
        TIMED_NAMED_BLOCK(GPUCopy);
        // CopyDataToGpuHeapAllocation
        {
          gpu_heap_allocation Allocation = GpuHeapAllocate(&Graphics->GpuHeap, GenChunk->Buffer.At);
          NewGpuAllocation = Allocation;

          s64 v3u8Offset = s64(Allocation.BaseOffsetInElements*sizeof(v3_u8));
          s64 matlOffset = s64(Allocation.BaseOffsetInElements*sizeof(matl));

          s64 v3u8Size = s64(GenChunk->Buffer.At*sizeof(v3_u8));
          s64 matlSize = s64(GenChunk->Buffer.At*sizeof(matl));

          GL->BindBuffer(GL_ARRAY_BUFFER, Graphics->GpuHeap.Storage.Handles.Handles[mesh_VertexHandle]);
          GL->BufferSubData(GL_ARRAY_BUFFER, v3u8Offset, v3u8Size, GenChunk->Buffer.Verts);

          GL->BindBuffer(GL_ARRAY_BUFFER, Graphics->GpuHeap.Storage.Handles.Handles[mesh_NormalHandle]);
          GL->BufferSubData(GL_ARRAY_BUFFER, v3u8Offset, v3u8Size, GenChunk->Buffer.Normals);

          GL->BindBuffer(GL_ARRAY_BUFFER, Graphics->GpuHeap.Storage.Handles.Handles[mesh_MatHandle]);
          GL->BufferSubData(GL_ARRAY_BUFFER, matlOffset, matlSize, GenChunk->Buffer.Mat);
        }
        DeallocateMesh(&GetEngineResources()->Heap, &GenChunk->Buffer);
      }

      // @dest_chunk_can_have_mesh
      /* Assert(HasGpuMesh(DestChunk)       == False); */
      if (IsGpuHeapAllocated(&Graphics->GpuHeap, &DestChunk->Mesh)) { GpuHeapDeallocate(&Graphics->GpuHeap, &DestChunk->Mesh); }
      if (IsGpuHeapAllocated(&Graphics->GpuHeap, &NewGpuAllocation)) { DestChunk->Mesh = NewGpuAllocation; }

      Assert(GenChunk->Chunk.Dim == V3i(64,66,66));
      Assert(DestChunk->Dim == V3i(64));

      RangeIterator(zIndex, 64)
      RangeIterator(yIndex, 64)
      {
        s32 DstIndex = GetIndex(yIndex, zIndex, V2i(64));
        s32 SrcIndex = GetIndex(yIndex+1, zIndex+1, V2i(66));

        Assert(DestChunk->Occupancy[DstIndex] == GenChunk->Chunk.Occupancy[SrcIndex]);
      }

      FinalizeNodeInitializaion(DestNode);

      ClearGenChunk( GenChunk );
      Free(&GetEngineResources()->GenChunkFreelist, GenChunk);
    }
  }

  // This job is done, so we unmap the readback PBO that FinalizeNoiseValues
  // read from in place, and free up our slot for a new chunk init job.  The PBO
  // itself is kept in the job's terrain_gen_job_context for the next job in this slot.
#if 0
  {
    TIMED_NAMED_BLOCK(PBOUnmap);
    GL->BindBuffer(GL_PIXEL_PACK_BUFFER, PBOBuf.PBO);
    GL->UnmapBuffer(GL_PIXEL_PACK_BUFFER);
    GL->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    GL->DeleteSync(PBOBuf.Fence);
    AssertNoGlErrors;
  }
#endif

  // NOTE(Jesse): The job is awaited, so it can't retire itself.  The slot is
  // released by DispatchTerrainJobs once the job reaches the Await state.
  ClearList(&Context->Batch);
}

