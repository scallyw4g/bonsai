link_internal void
poof(@async)
FinalizeNoiseValues( work_queue_job *Job,
                gpu_readback_buffer  PBOBuf,
                                u32 *NoiseData,
                                v3i  NoiseDim,
                        octree_node *DestNode )
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

  // NOTE(Jesse): NoiseValues is a per-chunk copy of the batch readback made by
  // CheckNoiseReadbackJob, which has already unmapped and released the PBO.
  Assert(PBOBuf.PBO == INVALID_PBO_HANDLE);
  HeapDeallocate(&Engine->Heap, NoiseData);

  Assert(Graphics->NoiseFinalizeJobsPending);
  AtomicDecrement(&Graphics->NoiseFinalizeJobsPending);
}

link_internal void
poof(@async @render)
CheckNoiseReadbackJob( work_queue_job *Job,
                  gpu_readback_buffer  PBOBuf,
                                  v3i  NoiseDim,
                          octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch )
{
  TIMED_FUNCTION();

  u32 BatchCount = (u32)AtElements(&Batch);
  Assert(BatchCount);
  Assert(Batch.Start[0]->Flags & Chunk_Queued);

  auto Plat = GetPlatform();
  auto Engine = GetEngineResources();

  u32 SyncStatus = GetGL()->ClientWaitSync(PBOBuf.Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
  AssertNoGlErrors;

  switch(SyncStatus)
  {
    case GL_ALREADY_SIGNALED: // { RuntimeBreak(); } break;
    case GL_CONDITION_SATISFIED:
    {
      TIMED_NAMED_BLOCK(MapBuffer);

      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, PBOBuf.PBO);
      AssertNoGlErrors;
      u32 *BatchNoiseValues = Cast(u32*, GetGL()->MapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
      AssertNoGlErrors;

      // The readback texture holds the chunks side-by-side along x, so we copy
      // each chunk out into its own contiguous buffer.
      //
      u32 ChunkStride  = u32(NoiseDim.x);
      u32 RowCount     = u32(NoiseDim.y*NoiseDim.z);
      u32 BatchRowSize = ChunkStride*TERRAIN_BATCH_CHUNK_COUNT;

      umm ChunkBytes = sizeof(u32)*ChunkStride*RowCount;

      octree_node *Nodes[TERRAIN_BATCH_CHUNK_COUNT] = {};
      u32 *ChunkNoise[TERRAIN_BATCH_CHUNK_COUNT] = {};
      u32 NodeCount = 0;

      RangeIterator_t(u32, BatchNodeIndex, BatchCount)
      {
        octree_node *Node = Batch.Start[BatchNodeIndex];
        Assert(Node->Flags & Chunk_Queued);
        u32 *Dest = Cast(u32*, HeapAllocate(&Engine->Heap, ChunkBytes));
        RangeIterator_t(u32, RowIndex, RowCount)
        {
          u32 *Src = BatchNoiseValues + (RowIndex*BatchRowSize) + (NodeCount*ChunkStride);
          MemCopy(Cast(u8*, Src), Cast(u8*, Dest + (RowIndex*ChunkStride)), sizeof(u32)*ChunkStride);
        }

        Nodes[NodeCount] = Node;
        ChunkNoise[NodeCount] = Dest;
        ++NodeCount;
      }

      GetGL()->UnmapBuffer(GL_PIXEL_PACK_BUFFER);
      AssertNoGlErrors;
      GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
      GetGL()->DeleteBuffers(1, &PBOBuf.PBO);
      AssertNoGlErrors;
      GetGL()->DeleteSync(PBOBuf.Fence);
      AssertNoGlErrors;

      RangeIterator_t(u32, NodeIndex, NodeCount)
      {
        auto Task = FinalizeNoiseValues_Task(&Plat->LowPriority, Job, {}, ChunkNoise[NodeIndex], NoiseDim, Nodes[NodeIndex] );
        PushTask(Job, &Task);
      }

      // Tasks in a job run in order, so this runs after every node in the batch has been processed
      auto FinalizeTask = FinalizeShitAndFuckinDoStuff_Task(&Plat->LoRenderQ, Batch);
      PushTask(Job, &FinalizeTask);
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
InitializeNoiseBuffer(octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch, work_queue_job *Job)
{
  TIMED_FUNCTION();

  engine_resources *Engine = GetEngineResources();
  UNPACK_ENGINE_RESOURCES(Engine);

  // NOTE(Jesse): Batch holds up to TERRAIN_BATCH_CHUNK_COUNT nodes, which are
  // all processed together.
  //
  world_chunk *BatchChunks[TERRAIN_BATCH_CHUNK_COUNT] = {};
  u32 BatchCount = (u32)AtElements(&Batch);
  Assert(BatchCount);
  RangeIterator_t(u32, BatchNodeIndex, BatchCount)
  {
    octree_node *BatchNode = Batch.Start[BatchNodeIndex];
    Assert(BatchNode->Flags & Chunk_Queued);
    BatchChunks[BatchNodeIndex] = BatchNode->Chunk;
  }
  RangeIterator_t(u32, PendingIndex, BatchCount) { AtomicIncrement(&Graphics->NoiseFinalizeJobsPending); }

  world_edit_render_context *WorldEditRC = &Graphics->WorldEditRC;

  GetGL()->ClearColor(-10000000000.f, -10000000000.f, -10000000000.f, -10000000000.f);
  ClearFramebuffer(WorldEditRC->Framebuffers + 0);
  ClearFramebuffer(WorldEditRC->Framebuffers + 1);
  ClearFramebuffer(WorldEditRC->Framebuffers + 2);

  DispatchTerrainShaders(Graphics, BatchChunks, BatchCount);

  //
  // Apply edits
  //

  rtt_framebuffer *Read  = WorldEditRC->Framebuffers;
  rtt_framebuffer *Write = WorldEditRC->Framebuffers + 1;

  RangeIterator_t(u32, BatchIndex, BatchCount)
  {
  octree_node *Node = Batch.Start[BatchIndex];
  world_chunk *Chunk = Node->Chunk;

  /* rtt_framebuffer Accum = WorldEditRC->Framebuffers[CurrentAccumulationTextureIndex]; */
  {
    AcquireFutex(&Node->Lock);
    if (TotalElements(&Node->Edits))
    {
      AssertNoGlErrors;

      // The world edit shader only knows about one chunk at a time
      Graphics->TerrainShapingRC.WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
      Graphics->TerrainShapingRC.ChunkResolution      = V3(Chunk->DimInChunks);

      UseShader(WorldEditRC);
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

      world_edit_op *Ops = Allocate(world_edit_op, GetTranArena(), TotalOps);
      RangeIterator(KeyIndex, EditCount)
      {
        TIMED_NAMED_BLOCK(WorldEditDrawCall);

        auto Program = &WorldEditRC->Program;
        world_edit *Edit = Cast(world_edit*, Keys[KeyIndex].Index);
        if (Edit->Brush) // NOTE(Jesse): Don't necessarily have to have a brush if we created the edit before we created a brush.
        {
          world_edit_brush BrushInstance = *Edit->Brush;
          ApplyInstanceEdits(&BrushInstance, &Edit->InstanceEdits);


          // Buffer up all layer ops in brush
          //
          s32 AtOpIndex = 0;
          u32 TexUnit = 0;
          BindUniformByName(Program, "InputTex", &Read->DestTexture, TexUnit++);

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

            Ops[AtOpIndex++] = Op;
            Assert(AtOpIndex <= TotalOps);
          }
#endif


          // NOTE(Jesse): We pass this blend mode in because we want to take the
          // layers blend mode, not the blend mode for the brush.
          BindUniformByName(Program, "BrushBlendMode", BrushInstance.BrushBlendMode);

          AssertNoGlErrors;

          auto GL = GetGL();

          s32 MaxFragShaderTexUnits = 0;
          GL->GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &MaxFragShaderTexUnits);

          local_persist u32 OpStorageBuffer = 0;
          if (OpStorageBuffer == 0) { GL->GenBuffers(1, &OpStorageBuffer); }

          GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, OpStorageBuffer);
          AssertNoGlErrors;

          GL->BufferData(GL_SHADER_STORAGE_BUFFER, Cast(u32, sizeof(world_edit_op))*Cast(u32, AtOpIndex), Ops, GL_DYNAMIC_DRAW);
          AssertNoGlErrors;

          GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, OpStorageBuffer);

          BindUniformByName(Program, "OpCount", AtOpIndex);
          BindUniformByName(Program, "ChunkIndex", s32(BatchIndex));

          v3 SimChunkMin = GetSimSpaceP(World, Chunk->WorldP);
          v3 ChunkRelEditMin = (SimEditBounds_Transformed.Min - SimChunkMin);
          v3 ChunkRelEditMax = (SimEditBounds_Transformed.Max - SimChunkMin);

          BindUniformByName(Program, "ChunkRelEditMin", &ChunkRelEditMin);
          BindUniformByName(Program, "ChunkRelEditMax", &ChunkRelEditMax);


          BindFramebuffer(Write);
          RenderQuad();

#define Swap(a, b) do { auto tmp = b; b = a; a = tmp; } while (false)
          Swap(Read, Write);
#undef Swap
        }

        AssertNoGlErrors;
      }
    }
    ReleaseFutex(&Node->Lock);
  }
  } // for each node in batch

  // We always swap textures, so we read from the Read texture
  texture *CurrentAccumulationTexture = &Read->DestTexture;

  //
  // Terrain Finalize
  //
  {
    TIMED_NAMED_BLOCK(TerrainFinalizeDrawCall);
    GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->TerrainFinalizeRC.FBO.ID);

    UseShader(&Graphics->TerrainFinalizeRC);

    BindUniformByName(&Graphics->TerrainFinalizeRC.Program, "InputTex", CurrentAccumulationTexture, 0);

    /* gpu_timer Timer = StartGpuTimer(); */
    RenderQuad();
    /* EndGpuTimer(&Timer); */
    /* Push(&Graphics->GpuTimers, &Timer); */

    AssertNoGlErrors;
  }

  /* Assert(Chunk1->Dim == V3i(64)); */
  /* Assert(NoiseDim == V3(66)); */
  v3i NoiseDim = V3i(66);

  s32 NoiseElementCount = s32(Volume(CurrentAccumulationTexture->Dim));
  s32 NoiseByteCount = NoiseElementCount*s32(sizeof(u32));

  {
    TIMED_NAMED_BLOCK(GenPboAndInitTransfer);
    u32 PBO;
    GetGL()->GenBuffers(1, &PBO);
    AssertNoGlErrors;

    /* Info("(%d) Allocated PBO (%u)", ThreadLocal_ThreadIndex, PBO); */
    {
      TIMED_NAMED_BLOCK(BindAndBuffer);
      GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
      GetGL()->BufferData(GL_PIXEL_PACK_BUFFER, NoiseByteCount, 0, GL_STREAM_READ);
    }
    {
      TIMED_NAMED_BLOCK(ReadPixels);
      AssertNoGlErrors;
      GetGL()->ReadPixels(0, 0, CurrentAccumulationTexture->Dim.x, CurrentAccumulationTexture->Dim.y, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    }
    AssertNoGlErrors;
    GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    gl_fence Fence = GetGL()->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    auto NextTask = CheckNoiseReadbackJob_Task( &Plat->HiRenderQ, Job, {PBO,Fence}, NoiseDim, Batch );
    NextTask.Timeout = 1.0;
    PushTask(Job, &NextTask);
  }
}
