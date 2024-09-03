link_internal void
DeallocateAndClearWorldChunk(engine_resources *Engine, world_chunk *Chunk)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(Chunk->DEBUG_OwnedByThread == 0);
  Chunk->DEBUG_OwnedByThread = ThreadLocal_ThreadIndex;

  Assert(Chunk->Flags & Chunk_Deallocate|Chunk_VoxelsInitialized);

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
                  SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), True);
                } break;

                case BonsaiRenderCommand_ShaderId_ShadowMap:
                {
                  SetupShadowMapShader(Graphics, GetShadowMapResolution(&Engine->Settings));
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

              // Editor preview
              /* DrawStuffToGBufferTextures(Engine, GetApplicationResolution(&Engine->Settings)); */
              {
                v3i Radius = World->VisibleRegion/2;
                v3i Min = World->Center - Radius;
                v3i Max = World->Center + Radius;

                SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), False);

                /* RenderDrawList(Engine, &Graphics->MainDrawList); */

                shader *Shader = &Graphics->gBuffer->gBufferShader;
                DrawEditorPreview(Engine, Shader);

              /*   { // NOTE(Jesse): Don't draw the grid on entities; it looks fucky if they're rotated. */
              /*     BindUniformByName(Shader, "DrawMajorGrid", False); */
              /*     BindUniformByName(Shader, "DrawMinorGrid", False); */
              /*     r32 dt = Plat->dt; */
              /*     DrawEntities(Shader, EntityTable, &GpuMap->Buffer, 0, Graphics, World, dt); */
              /*   } */

                TeardownGBufferShader(Graphics);
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

    bitmap_block_array Bitmaps = { .Memory = GetTranArena() };
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

