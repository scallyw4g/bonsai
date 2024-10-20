link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Chunk->DEBUG_OwnedByThread == 0);
  Chunk->DEBUG_OwnedByThread = ThreadLocal_ThreadIndex;

  Assert( (Chunk->Flags & Chunk_Queued) == 0);
  Assert( Chunk->Flags & (Chunk_Deallocate|Chunk_VoxelsInitialized));

  DeallocateMeshes(&Chunk->Meshes, MeshFreelist);

  /* DeallocateGpuBuffers(RenderQueue, Chunk); */
  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    auto Handles = Chunk->Meshes.GpuBufferHandles+MeshIndex;
    // @vertex_handle_primal
    //
    /* if (Handles->VertexHandle) { GL.DeleteBuffers(3, &Handles->VertexHandle); } */
    PushDeallocateBuffersCommand(RenderQ, Handles);
  }

  ClearWorldChunk(Chunk);

  Assert(Chunk->DEBUG_OwnedByThread == 0);
  Assert(Chunk->Flags == 0);
  Assert(Chunk->Next == 0);

  FullBarrier;
}

link_internal void
RenderOctree(engine_resources *Engine, shader *Shader)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  b32     Continue = True;

  octree_node_ptr_stack Stack = OctreeNodePtrStack(1024, &World->OctreeMemory);
  Push(&Stack, &World->Root);

  /* RuntimeBreak(); */
  while (CurrentCount(&Stack))
  {
    octree_node *Node = Pop(&Stack);

    if (Node->Type == OctreeNodeType_Leaf)
    {
      SyncGpuBuffersImmediate(Engine, &Node->Chunk->Meshes);
      DrawLod(Engine, Shader, &Node->Chunk->Meshes, 0, {}, Quaternion(), V3(1));
    }

    if (Node->Children[0]) { Push(&Stack, Node->Children[0]); }
    if (Node->Children[1]) { Push(&Stack, Node->Children[1]); }
    if (Node->Children[2]) { Push(&Stack, Node->Children[2]); }
    if (Node->Children[3]) { Push(&Stack, Node->Children[3]); }
    if (Node->Children[4]) { Push(&Stack, Node->Children[4]); }
    if (Node->Children[5]) { Push(&Stack, Node->Children[5]); }
    if (Node->Children[6]) { Push(&Stack, Node->Children[6]); }
    if (Node->Children[7]) { Push(&Stack, Node->Children[7]); }
  }
}


link_internal void
RenderLoop(thread_startup_params *ThreadParams, engine_resources *Engine)
{
  // Map immediate GPU buffers for first frame
  MapGpuBuffer_untextured_3d_geometry_buffer(&Engine->Graphics.GpuBuffers[0]);
  MapGpuBuffer_untextured_3d_geometry_buffer(&Engine->Graphics.Transparency.GpuBuffer);

  os *Os         = &Engine->Stdlib.Os;
  /* platform *Plat = &Engine->Stdlib.Plat; */
  while ( FutexNotSignaled(ThreadParams->WorkerThreadsExitFutex) )
  {
    UNPACK_ENGINE_RESOURCES(Engine);

    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();
    WorkerThread_BeforeJobStart(ThreadParams);

    while (work_queue_entry *Job = PopWorkQueueEntry(RenderQ))
    {
      /* TIMED_NAMED_BLOCK(RENDER_LOOP); */
      tswitch(Job)
      {
        case type_work_queue_entry_noop:
        case type_work_queue_entry_init_world_chunk:
        case type_work_queue_entry_copy_buffer_set:
        case type_work_queue_entry_copy_buffer_ref:
        case type_work_queue_entry_init_asset:
        case type_work_queue_entry_update_world_region:
        case type_work_queue_entry_rebuild_mesh:
        case type_work_queue_entry_build_chunk_mesh:
        case type_work_queue_entry_sim_particle_system:
        case type_work_queue_entry__align_to_cache_line_helper:
        {
          InvalidCodePath();
        } break;


        { tmatch(work_queue_entry_async_function_call, Job, RPC)
          DispatchAsyncFunctionCall(RPC);
        } break;

        { tmatch(work_queue_entry__bonsai_render_command, Job, RC)
          tswitch(RC)
          {
            InvalidCase(type_work_queue_entry__bonsai_render_command_noop);

            { tmatch(bonsai_render_command_allocate_texture, RC, Command)

              texture *Texture = Command->Texture;
              switch (Texture->Channels)
              {
                case 3:
                {
                  *Texture = MakeTexture_RGB(Texture->Dim, (const v3*)Command->Data, Texture->DebugName, Texture->Slices, Texture->Format);
                } break;

                case 4:
                {
                  *Texture = MakeTexture_RGBA(Texture->Dim, Cast(u32*, Command->Data), Texture->DebugName, Texture->Slices, Texture->Format);
                } break;

                InvalidDefaultCase;
              }

            } break;


            { tmatch(bonsai_render_command_deallocate_texture, RC, Command)
              NotImplemented;
            } break;


            { tmatch(bonsai_render_command_allocate_buffers, RC, Command)
              NotImplemented;
            } break;

            { tmatch(bonsai_render_command_reallocate_buffers, RC, Command)
              auto *Handles = Command->Handles;
              auto *Mesh    = Command->Mesh;

              ReallocateAndSyncGpuBuffers(Handles, Mesh);
              DeallocateMesh(Mesh, &Engine->geo_u3d_MeshFreelist);
            } break;


            { tmatch(bonsai_render_command_deallocate_buffers, RC, Command)
              if (*Command->Buffers) { GL.DeleteBuffers(Command->Count, Command->Buffers); }
              RangeIterator(Index, Command->Count) { Command->Buffers[Index] = 0; }
            } break;


            { tmatch(bonsai_render_command_deallocate_world_chunk, RC, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_deallocate_world_chunk);
              world_chunk *Chunk = Command->Chunk;
              FreeWorldChunk(Engine, Chunk);
            } break;


            { tmatch(bonsai_render_command_clear_all_framebuffers, RC, Command)
              ClearFramebuffers(Graphics, &Engine->RTTGroup);
            } break;




            { tmatch(bonsai_render_command_setup_shader, RC, Command)
              switch (Command->ShaderId)
              {
                InvalidCase(BonsaiRenderCommand_ShaderId_noop);

                case BonsaiRenderCommand_ShaderId_gBuffer:
                {
                  SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), Editor->LayeredBrushEditor.SeedBrushWithSelection);
                } break;

                case BonsaiRenderCommand_ShaderId_ShadowMap:
                {
                  SetupShadowMapShader(Graphics, GetShadowMapResolution(&Engine->Settings), Editor->LayeredBrushEditor.SeedBrushWithSelection);
                } break;
              }
            } break;

            { tmatch(bonsai_render_command_teardown_shader, RC, Command)
              switch (Command->ShaderId)
              {
                InvalidCase(BonsaiRenderCommand_ShaderId_noop);

                case BonsaiRenderCommand_ShaderId_gBuffer:
                {
                  TeardownGBufferShader(Graphics);
                } break;

                case BonsaiRenderCommand_ShaderId_ShadowMap:
                {
                  TeardownShadowMapShader(Graphics);
                } break;
              }
            } break;

            { tmatch(bonsai_render_command_set_shader_uniform, RC, Command)
              shader *Shader = Command->Shader;
              shader_uniform *Uniform = &Command->Uniform;
              if (Uniform->ID >= 0)
              {
                BindUnifromById(Uniform, &Command->TextureUnit);
              }
              else
              {
                BindUniformByName(Shader, Uniform, &Command->TextureUnit);
              }
            } break;

            { tmatch(bonsai_render_command_draw_world_chunk_draw_list, RC, Command)
              RenderDrawList(Engine, Command->DrawList, Command->Shader);
            } break;

            { tmatch(bonsai_render_command_draw_all_entities, RC, Command)
              DrawEntities(Command->Shader, EntityTable, &GpuMap->Buffer, 0, Graphics, World, Plat->dt);
            } break;


            { tmatch(bonsai_render_command_initialize_noise_buffer, RC, _Command)
              /* Command = 0; */
              TIMED_NAMED_BLOCK(bonsai_render_command_initialize_noise_buffer);

              bonsai_render_command_initialize_noise_buffer C = RC->bonsai_render_command_initialize_noise_buffer;


              world_chunk **Chunk2 = &C.Chunk;
              world_chunk *Chunk1 = C.Chunk;
              world_chunk *Chunk = Chunk1;

              Info("(%llu)  (%llu)", u64(Chunk), u64(Chunk1));
              Assert(s64(Chunk) == s64(Chunk1));

              /* world_chunk *Chunk = C.Chunk; */
              auto *Shader = &Graphics->GpuNoise.GradientShader;

              v3i Apron = V3i(2, 2, 2);
              v3 NoiseDim = V3(Shader->ChunkDim);
              Assert(V3(Chunk1->Dim+Apron) == NoiseDim);

              v2i ViewportSize = V2i(s32(Chunk1->Dim.x), s32(Chunk1->Dim.y*Chunk1->Dim.z));

              {
                TIMED_NAMED_BLOCK(Draw);
                GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->GpuNoise.FBO.ID);

                SetViewport(ViewportSize);
                UseShader(Shader);
                RenderQuad();
                AssertNoGlErrors;
              }


              s32 NoiseElementCount = s32(Volume(NoiseDim));
              r32 *NoiseValues;
#if 1
              s32 NoiseByteCount = NoiseElementCount*s32(sizeof(f32));
              {
                TIMED_NAMED_BLOCK(GenPboAndInitTransfer);
                u32 PBO;
                GL.GenBuffers(1, &PBO);
                AssertNoGlErrors;
                GL.BindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
                GL.BufferData(GL_PIXEL_PACK_BUFFER, NoiseByteCount, 0, GL_STREAM_READ);
                AssertNoGlErrors;
                GL.ReadPixels(0, 0, ViewportSize.x, ViewportSize.y, GL_RED, GL_FLOAT, 0);
                AssertNoGlErrors;
              }

              {
                TIMED_NAMED_BLOCK(Fence);
                gl_fence Fence = GL.FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

                b32 Done = False;
                while (!Done)
                {
                u32 SyncStatus = GL.ClientWaitSync(Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
                  switch(SyncStatus)
                  {
                    case GL_ALREADY_SIGNALED:
                    case GL_CONDITION_SATISFIED:
                    {
                      Done = True;
                    } break;

                    case GL_TIMEOUT_EXPIRED:
                    {
                      SyncStatus = GL.ClientWaitSync(Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
                    } break;

                    case GL_WAIT_FAILED:
                    {
                      SoftError("Error waiting on gl sync object");
                      Done = True;
                    } break;
                  }
                }
                AssertNoGlErrors;
              }

              /* SleepMs(1); */

              {
                HISTOGRAM_FUNCTION();
                TIMED_NAMED_BLOCK(MapBuffer);
                NoiseValues = Cast(f32*, GL.MapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
                AssertNoGlErrors;
              }
#else
              NoiseValues = AllocateAligned(r32, GetTranArena(), NoiseElementCount, 32);
              {
                TIMED_NAMED_BLOCK(BindTexture);
                GL.BindTexture(GL_TEXTURE_2D, Shader->ChunkTexture.ID);
              }
              {
                HISTOGRAM_FUNCTION();
                TIMED_NAMED_BLOCK(GetTexImage);
                GL.GetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, NoiseValues);
              }
#endif
              Assert(NoiseValues);

              /* auto E = WorkQueueEntry(WorkQueueEntryBuildChunkMesh(C->ChunkSize, C->NoiseValues)); */
              /* PushWorkQueueEntry(&Plat->LowPriority, &E); */

              world_chunk *DestChunk = Chunk1;
              world_chunk *SynChunk = AllocateWorldChunk({}, Chunk1->Dim + V3i(0, 2, 2), V3i(1), GetTranArena());
              SynChunk->Flags = Chunk_Queued;
              v3i WorldBasis = {};
              v3i SynChunkDim = SynChunk->Dim;
              v3i SrcToDest = {};
              s64 zMin = 0;
              u16 PackedHSVColorValue = PackHSVColor(HSV_GREEN);
              b32 MakeExteriorFaces = False;

              u32 ChunkSum = FinalizeOccupancyMasksFromNoiseValues(SynChunk, WorldBasis, V3i(NoiseDim), NoiseValues, SrcToDest, zMin, PackedHSVColorValue);

              if (ChunkSum)
              {
                if (MakeExteriorFaces)
                {
                  MarkBoundaryVoxels_MakeExteriorFaces(SynChunk->Occupancy, SynChunk->Voxels, SynChunkDim, Global_ChunkApronMinDim, SynChunkDim-Global_ChunkApronMaxDim);
                }
                else
                {
                  MarkBoundaryVoxels_NoExteriorFaces(SynChunk->Occupancy, SynChunk->xOccupancyBorder, SynChunk->FaceMasks, SynChunk->Voxels, SynChunkDim, {}, SynChunkDim);
                }

                Assert(DestChunk->FilledCount == 0);
                Assert(DestChunk->Dim.x == 64);
                Assert(DestChunk->Dim.y == 64);
                Assert(DestChunk->Dim.z == 64);
                RangeIterator(z, 64)
                RangeIterator(y, 64)
                {
                  u64 Occ = DestChunk->Occupancy[y + z*64];
                  DestChunk->FilledCount += CountBitsSet_Kernighan(Occ);
                }

                Assert(DestChunk->FilledCount <= s32(Volume(DestChunk->Dim)));

                // NOTE(Jesse): The DestChunk is finalized at the end of the routine
                /* SetFlag(DestChunk, Chunk_VoxelsInitialized); */
                FinalizeChunkInitialization(SynChunk);

#if 0
                if (Flags & ChunkInitFlag_ComputeStandingSpots)
                {
                  NotImplemented;
                  ComputeStandingSpots( SynChunkDim, SynChunk, {{1,1,0}}, {{0,0,1}}, Global_StandingSpotDim,
                                        DestChunk->Dim, 0, &DestChunk->StandingSpots,
                                        Thread->TempMemory);
                }
#endif

                auto *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
                geo_u3d *TempMesh = AllocateTempMesh(Thread->TempMemory, DataType_v3_u8);

                RebuildWorldChunkMesh(Thread, SynChunk, {}, {}, MeshBit_Lod0, TempMesh, Thread->TempMemory);
                TempMesh->At = 0;

                /* Assert( (Flags & ChunkInitFlag_GenLODs) == 0); */

#define FINALIZE_MESH_FOR_CHUNK(Src, Dest, Bit)                               \
                {                                                                           \
                  auto *SrcMesh = (Src)->Meshes.E[ToIndex(Bit)];                            \
                  if (SrcMesh) {                                                            \
                    if (SrcMesh->At) {                                                      \
                      DestChunk->HasMesh = True;                                            \
                      AtomicReplaceMesh(&(Dest)->Meshes, Bit, SrcMesh, SrcMesh->Timestamp); \
                    } else {                                                                \
                      DeallocateMesh(EngineResources, SrcMesh);                             \
                    }                                                                       \
                  }                                                                         \
                }

                {
                  auto *EngineResources = GetEngineResources();
                  TIMED_NAMED_BLOCK(Chunk_Finalize);
                  FINALIZE_MESH_FOR_CHUNK(SynChunk, DestChunk, MeshBit_Lod0 );
                  /* FINALIZE_MESH_FOR_CHUNK(SynChunk, DestChunk, MeshBit_Lod1 ); */
                  /* FINALIZE_MESH_FOR_CHUNK(SynChunk, DestChunk, MeshBit_Lod2 ); */
                  /* FINALIZE_MESH_FOR_CHUNK(SynChunk, DestChunk, MeshBit_Lod3 ); */
                  /* FINALIZE_MESH_FOR_CHUNK(SynChunk, DestChunk, MeshBit_Lod4 ); */
#undef FINALIZE_MESH_FOR_CHUNK

                  Assert( (DestChunk->Flags & Chunk_VoxelsInitialized) == 0);
                  Assert( DestChunk->FilledCount <= s32(Volume(SynChunk)));

                  /* if (DestChunk->WorldP == V3i(0))  { RuntimeBreak(); } */

                  FinalizeChunkInitialization(Cast(world_chunk*, Cast(void*,DestChunk)));
                }
              }
            } break;


            { tmatch(bonsai_render_command_do_stuff, RC, Command)

              //
              // Render begin
              //

              ao_render_group     *AoGroup = Graphics->AoGroup;
              shadow_render_group *SG      = Graphics->SG;

              EngineDebug->Render.BytesSolidGeoLastFrame = GpuMap->Buffer.At;
              EngineDebug->Render.BytesTransGeoLastFrame = Graphics->Transparency.GpuBuffer.Buffer.At;

              // TODO(Jesse):  Make a render frame begin event to stuff this kinda thing onto?
              //
              // Update color texture, if necessary
              //
              s32 ColorCount = s32(AtElements(&Graphics->ColorPalette));
              if (ColorCount != Graphics->ColorPaletteTexture.Dim.x)
              {
                if (Graphics->ColorPaletteTexture.ID) { DeleteTexture(&Graphics->ColorPaletteTexture); }
                Graphics->ColorPaletteTexture =
                  MakeTexture_RGB( V2i(ColorCount, 1), Graphics->ColorPalette.Start, CSz("ColorPalette"));
              }
              //
              // Editor preview
              /* DrawStuffToGBufferTextures(Engine, GetApplicationResolution(&Engine->Settings)); */
              {
                v3i Radius = World->VisibleRegion/2;
                v3i Min = World->Center - Radius;
                v3i Max = World->Center + Radius;

                SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), False);
                shader *Shader = &Graphics->gBuffer->gBufferShader;
                DrawEditorPreview(Engine, Shader);
                TeardownGBufferShader(Graphics);

                SetupShadowMapShader(Graphics, GetShadowMapResolution(&Engine->Settings), False);
                Shader = &Graphics->SG->Shader.Program;
                DrawEditorPreview(Engine, Shader);
                TeardownShadowMapShader(Graphics);
              }

              /* DrawWorldAndEntitiesToShadowMap(GetShadowMapResolution(&Engine->Settings), Engine); */

              // TODO(Jesse): Move into engine debug
              DebugHighlightWorldChunkBasedOnState(Graphics, EngineDebug->PickedChunk, &GpuMap->Buffer);

              AssertNoGlErrors;

              Ensure( FlushBuffersToCard(GpuMap) ); // Unmaps buffer
              if (GpuMap->Buffer.At)
              {
                RenderImmediateGeometryToGBuffer(GetApplicationResolution(&Engine->Settings), GpuMap, Graphics);
                RenderImmediateGeometryToShadowMap(GpuMap, Graphics);
              }
              Clear(&GpuMap->Buffer);


              // NOTE(Jesse): I observed the AO lagging a frame behind if this is re-ordered
              // after the transparency/luminance textures.  I have literally 0 ideas as to
              // why that would be, but here we are.
              if (Graphics->Settings.UseSsao) { RenderAoTexture(GetApplicationResolution(&Engine->Settings), AoGroup); }

              {
                RenderTransparencyBuffers(GetApplicationResolution(&Engine->Settings), &Graphics->Settings, &Graphics->Transparency);
                RenderLuminanceTexture(GetApplicationResolution(&Engine->Settings), GpuMap, Lighting, Graphics);
              }

              if (Graphics->Settings.UseLightingBloom) { RunBloomRenderPass(Graphics); }
              /* if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, &Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomFBO); } */

              CompositeGameTexturesAndDisplay(Plat, Graphics);

              UiFrameEnd(&Engine->Ui);


              BonsaiSwapBuffers(&Engine->Stdlib.Os);


              HotReloadShaders(GetStdlib());


              /* GpuMap = GetNextGpuMap(Graphics); */

              // Map immediate GPU buffers for next frame
              MapGpuBuffer_untextured_3d_geometry_buffer(GpuMap);
              MapGpuBuffer_untextured_3d_geometry_buffer(&Graphics->Transparency.GpuBuffer);
              Assert(GpuMap->Buffer.At == 0);

              Graphics->RenderGate = False;

            } break;

            { tmatch(bonsai_render_command_gl_timer_init, RC, Command)
              AssertNoGlErrors;
              GL.GenQueries(1, Command->GlTimerObject);
              AssertNoGlErrors;
            } break;

            { tmatch(bonsai_render_command_gl_timer_start, RC, Command)
              AssertNoGlErrors;
              GL.BeginQuery(GL_TIME_ELAPSED, Command->GlTimerObject);
              AssertNoGlErrors;
            } break;

            { tmatch(bonsai_render_command_gl_timer_end, RC, Command)
              AssertNoGlErrors;
              GL.EndQuery(GL_TIME_ELAPSED);
              AssertNoGlErrors;
            } break;

            { tmatch(bonsai_render_command_gl_timer_read_value_and_histogram, RC, Command)
              AssertNoGlErrors;
              u64 TimerNs = 0;

              s32 Available = False;
              while (!Available)
              {
                GL.GetQueryObjectiv(Command->GlTimerObject, GL_QUERY_RESULT_AVAILABLE, &Available);
                /* if (Available == False) { Info("Waiting for query object to become available"); } */
              }


              GL.GetQueryObjectui64v(Command->GlTimerObject, GL_QUERY_RESULT, &TimerNs);
              /* Info("GL reported time of (%.2f)ms", f64(TimerNs)/1000000.0); */
              /* GetDebugState()->PushHistogramDataPoint(TimerNs); */
              AssertNoGlErrors;
            } break;

          }
        } break;
      }

      RewindArena(GetTranArena());
    }

    SleepMs(1);

    if (FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex)) break;

    if (FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex)) { WaitOnFutex(ThreadParams->WorkerThreadsSuspendFutex); }
  }

  Info("Exiting Render Thread (%d)", ThreadParams->ThreadIndex);
  WaitOnFutex(ThreadParams->WorkerThreadsExitFutex);
}

link_export THREAD_MAIN_RETURN
RenderThread_Main(void *ThreadStartupParams)
{
  b32 InitResult = True;
  thread_startup_params *ThreadParams = Cast(thread_startup_params*, ThreadStartupParams);

  Global_EngineResources = (engine_resources*)ThreadParams->EngineResources;
  Global_ThreadStates = Global_EngineResources->Stdlib.ThreadStates;
  Assert(Global_EngineResources);
  Assert(Global_ThreadStates);

  WorkerThread_BeforeJobStart(ThreadParams);

  /* Assert(ThreadParams->ThreadIndex > 0); */
  /* SetThreadLocal_ThreadIndex(ThreadParams->ThreadIndex); */

  engine_resources *Engine = GetEngineResources();
  os *Os = &Engine->Stdlib.Os;
  platform *Plat = &Engine->Stdlib.Plat;

  PlatformMakeRenderContextCurrent(Os);

  /* s32 VSyncFrames = 0; */
  /* InitResult &= OpenAndInitializeWindow(Os, Plat, VSyncFrames); */

  if (InitResult) { InitResult &= InitializeOpenglFunctions(); }

  if (InitResult) { InitResult &= GraphicsInit(&Engine->Graphics, &Engine->Settings, AllocateArena()); }

  if (InitResult)
  {
    memory_arena *UiMemory = AllocateArena();
    InitRenderer2D(&Engine->Ui, &Engine->Heap, UiMemory, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

    bitmap_block_array Bitmaps = {};
    Bitmaps.Memory = GetTranArena();
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64));
  }

  FullBarrier;

  Engine->Graphics.Initialized = True;

  if (InitResult)
  {
    RenderLoop(ThreadParams, Engine);
  }
  else
  {
    SoftError("Render thread initiailization failed.");
  }

  THREAD_MAIN_RETURN Result = ReinterpretCast(THREAD_MAIN_RETURN, InitResult);
  return Result;
}

link_internal b32
InitEngineResources(engine_resources *Engine)
{
  b32 Result = True;

  platform *Plat = &Engine->Stdlib.Plat;

  memory_arena *WorldAndEntityArena = AllocateArena(Megabytes(256));
  DEBUG_REGISTER_ARENA(WorldAndEntityArena, 0);

  Engine->GameMemory = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap                    = InitHeap(Gigabytes(2)); // TODO(Jesse): Is this actually used?
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1), True); // NOTE(Jesse): Asset system needs to be able to allocate from the render thread.

  Engine->World = Allocate(world, WorldAndEntityArena, 1);
  if (!Engine->World) { Error("Allocating World"); Result = False; }

  /* Assert(Global_ShaderHeaderCode.Start == 0); */
  /* LoadGlobalShaderHeaderCode(Engine->Settings.Graphics.ShaderLanguage); */

#if PLATFORM_WINDOW_IMPLEMENTATIONS
  /* PlatformCreateThread( RenderMain, Cast(void*, Engine), INVALID_THREAD_LOCAL_THREAD_INDEX ); */
#endif

  Engine->EntityTable = AllocateEntityTable(WorldAndEntityArena, TOTAL_ENTITY_COUNT);

  return Result;
}

link_internal b32
InitEngineDebug(engine_debug *Debug)
{
  b32 Result = True;

  Debug->Memory = AllocateArena();

  Debug->Textures.Memory = Debug->Memory;

  /* { */
  /*   Debug->WorldEditDebugThumb.Texture = MakeTexture_RGB(V2i(256), 0, CSz("WorldEditDebugTexture")); */
  /*   StandardCamera(&Debug->WorldEditDebugThumb.Camera, 10000.f, 1000.f, 30.f); */
  /*   AllocateMesh(&Debug->WorldEditDebugMesh,  u32(Kilobytes(64*16)), Debug->Memory); */
  /* } */

  return Result;
}

enum hard_reset_flags
{
  HardResetFlag_None = 0,
  HardResetFlag_NoResetCamera = (1 << 0),
};


link_internal void
AssertWorkerThreadsSuspended(engine_resources *Engine)
{
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.SignalValue != FUTEX_UNSIGNALLED_VALUE);
  Assert(Engine->Stdlib.Plat.WorkerThreadsSuspendFutex.ThreadsWaiting == GetWorkerThreadCount());
}

link_internal void
CancelAllWorkQueueJobs(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  AssertWorkerThreadsSuspended(Engine);

  CancelAllWorkQueueJobs(Plat, &Plat->HighPriority);
  CancelAllWorkQueueJobs(Plat, &Plat->LowPriority);
  CancelAllWorkQueueJobs(Plat, &Plat->RenderQ);
}

link_internal void
HardResetAssets(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  AssertWorkerThreadsSuspended(Engine);

  DeinitHeap(&Engine->AssetSystem.AssetMemory);

  Engine->AssetSystem = {};
  Engine->AssetSystem.AssetMemory = InitHeap(Gigabytes(1), True);
}

// NOTE(Jesse): This function soft-resets the engine to a state similar to that
// at which it was when the game init routine was called.  This is useful when
// resetting the game state.  For a more invasive 
link_internal void
SoftResetEngine(engine_resources *Engine, hard_reset_flags Flags = HardResetFlag_None)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  CancelAllWorkQueueJobs(Engine);

  // TODO(Jesse): Free octree here.
  NotImplemented;
#if 0
  u32 ChunksFreed = 0;
  RangeIterator(HashIndex, s32(World->HashSize))
  {
    if (world_chunk *Chunk = World->ChunkHash[HashIndex])
    {
      Chunk->Flags = Chunk_VoxelsInitialized;
      FreeWorldChunk(Engine, Chunk);
      World->ChunkHash[HashIndex] = 0;
      ++ChunksFreed;
    }
  }
#endif

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    if ( (Flags&HardResetFlag_NoResetCamera) && Graphics->GameCamera.GhostId.Index == EntityIndex ) { continue; }
    Unspawn(EntityTable[EntityIndex]);
  }

  HardResetAssets(Engine);
}

link_internal void
HardResetEngine(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  CancelAllWorkQueueJobs(Engine);

  RangeIterator_t(u32, EntityIndex, TOTAL_ENTITY_COUNT)
  {
    Unspawn(EntityTable[EntityIndex]);
  }

  VaporizeArena(Engine->GameMemory);
  Engine->GameMemory = AllocateArena();

  VaporizeArena(Engine->World->ChunkMemory);
  Engine->World->ChunkMemory = AllocateArena();

  HardResetEditor(&Engine->Editor);

  // TODO(Jesse)(leak): This leaks the texture handles; make a HardResetEngineDebug()
  VaporizeArena(Engine->EngineDebug.Memory);
  Engine->EngineDebug = {};
  Engine->EngineDebug.Memory = AllocateArena();

  HardResetAssets(Engine);
}

