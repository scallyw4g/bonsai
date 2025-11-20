link_export void
DrainHiRenderQueue(engine_resources *Engine)
{
  /* TIMED_FUNCTION(); */

  Assert(RENDER_THREAD_ThreadLocal_ThreadIndex == ThreadLocal_ThreadIndex);

  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(EntityTable);

  /* RenderInfo("DrainRenderQueue"); */

  AssertNoGlErrors;
  while (work_queue_entry *Job = PopWorkQueueEntry(HiRenderQ))
  {
    /* RenderInfo("%S", ToString(Job->Type)); */
    /* TIMED_NAMED_BLOCK(RENDER_LOOP); */
    tswitch(Job)
    {
      case type_work_queue_entry_noop:
      case type_work_queue_entry_init_world_chunk:
      case type_work_queue_entry_copy_buffer_set:
      case type_work_queue_entry_copy_buffer_ref:
      case type_work_queue_entry_init_asset:
      /* case type_work_queue_entry_update_world_region: */
      case type_work_queue_entry_rebuild_mesh:
      case type_work_queue_entry_finalize_noise_values:
      case type_work_queue_entry_build_chunk_mesh:
      case type_work_queue_entry_sim_particle_system:
      case type_work_queue_entry__align_to_cache_line_helper:
      {
        InvalidCodePath();
      } break;


      { tmatch(work_queue_entry_async_function_call, Job, RPC)
        /* RenderInfo("%S", ToString(RPC->Type)); */
        TIMED_NAMED_BLOCK(work_queue_entry_async_function_call);
        DispatchAsyncFunctionCall(RPC);
      } break;

      { tmatch(work_queue_entry__bonsai_render_command, Job, RenderCommand)
        /* RenderInfo("%S", ToString(RenderCommand->Type)); */
        tswitch(RenderCommand)
        {
          InvalidCase(type_work_queue_entry__bonsai_render_command_noop);

          { case type_bonsai_render_command_cancel_all_noise_readback_jobs:
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_allocate_and_map_gpu_element_buffer, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_unmap_gpu_element_buffer, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_unmap_and_deallocate_pbo, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_allocate_texture, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_deallocate_texture, RenderCommand, Command)
            NotImplemented;
          } break;


          { tmatch(bonsai_render_command_allocate_handles, RenderCommand, Command)
            NotImplemented;
          } break;

          { tmatch(bonsai_render_command_deallocate_handles, RenderCommand, Command)
            NotImplemented;
          } break;

          { tmatch(bonsai_render_command_deallocate_world_chunk, RenderCommand, Command)
            InvalidCodePath();
          } break;


          { tmatch(bonsai_render_command_clear_all_framebuffers, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_clear_all_framebuffers);
            ClearFramebuffers(Graphics, &Engine->RTTGroup);
          } break;


          { tmatch(bonsai_render_command_setup_shader, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_setup_shader);

            switch (Command->ShaderId)
            {
              InvalidCase(BonsaiRenderCommand_ShaderId_noop);

              case BonsaiRenderCommand_ShaderId_gBuffer:
              {
                SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), False);
              } break;

              case BonsaiRenderCommand_ShaderId_ShadowMap:
              {
                SetupShadowMapShader(World, Graphics, GetShadowMapResolution(&Engine->Settings), False);
              } break;
            }
          } break;

          { tmatch(bonsai_render_command_teardown_shader, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_teardown_shader);
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

          { tmatch(bonsai_render_command_set_shader_uniform, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_set_shader_uniform);
            shader *Shader = Command->Shader;
            shader_uniform *Uniform = &Command->Uniform;
            if (Uniform->ID >= 0)
            {
              BindUniformById(Uniform, &Command->TextureUnit);
            }
            else
            {
              BindUniformByName(Shader, Uniform, &Command->TextureUnit);
            }
          } break;

          { tmatch(bonsai_render_command_draw_world_chunk_draw_list, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_draw_world_chunk_draw_list);
            if (FutexIsSignaled(&Graphics->RenderGate) == False) { SoftError("Called bonsai_render_command_draw_world_chunk_draw_list when RenderGate was not signalled!"); }
            RenderDrawList(Engine, Command->DrawList, Command->Shader, Command->Camera);
          } break;

          { tmatch(bonsai_render_command_draw_all_entities, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_draw_all_entities);
            if (FutexIsSignaled(&Graphics->RenderGate) == False) { SoftError("Called bonsai_render_command_draw_all_entities when RenderGate was not signalled!"); }
            DrawEntities(Command->Shader, EntityTable, &GpuMap->Buffer, 0, Graphics, World, Plat->dt);
          } break;


          { tmatch(bonsai_render_command_initialize_noise_buffer, RenderCommand, _Command)
            InvalidCodePath();
          } break;


          { tmatch(bonsai_render_command_do_stuff, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_do_stuff);

            //
            // Render begin
            //

            ao_render_group     *AoGroup = Graphics->AoGroup;

#if 0
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
#endif

#if 0
            //
            // Editor preview
            /* DrawStuffToGBufferTextures(Engine, GetApplicationResolution(&Engine->Settings)); */
            {
              shadow_render_group *SG      = Graphics->SG;
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
#endif

            /* DrawWorldAndEntitiesToShadowMap(GetShadowMapResolution(&Engine->Settings), Engine); */

            // TODO(Jesse): Move into engine debug
            world_chunk *C = EngineDebug->PickedNode ? EngineDebug->PickedNode->Chunk : 0;
            DebugHighlightWorldChunkBasedOnState(Graphics, C, &GpuMap->Buffer);

            AssertNoGlErrors;

            if (Graphics->Settings.DrawCameraGhost)
            {
              untextured_3d_geometry_buffer Mesh = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL);
              DrawVoxel(&Mesh, {}, V3(0.7f), V3(1)*(Graphics->GameCamera.DistanceFromTarget/1000.f*Graphics->Settings.CameraGhostSize));
            }

            Ensure( FlushBuffersToCard_gpu_mapped_element_buffer(CurrentHandles(GpuMap)) ); // Unmaps buffer

            if (GpuMap->Buffer.At)
            {
              RenderImmediateGeometryToGBuffer(GetApplicationResolution(&Engine->Settings), GpuMap, Graphics);
              /* RenderImmediateGeometryToShadowMap(World, Graphics, GpuMap); */
            }
            Clear(&GpuMap->Buffer);

            /* DrawBuffer(GpuMap, &Plat->ScreenDim); */


            // NOTE(Jesse): I observed the AO lagging a frame behind if this is re-ordered
            // after the transparency/luminance textures.  I have literally 0 ideas as to
            // why that would be, but here we are.
            if (Graphics->Settings.UseSsao) { RenderAoTexture(GetApplicationResolution(&Engine->Settings), AoGroup); }

            {
              /* RenderTransparencyBuffers(GetApplicationResolution(&Engine->Settings), &Graphics->Settings, &Graphics->Transparency); */
              RenderLuminanceTexture(GetApplicationResolution(&Engine->Settings), Lighting, Graphics);
            }

            if (Graphics->Settings.UseLightingBloom) { RunBloomRenderPass(Graphics); }
            /* if (Graphics->Settings.UseLightingBloom) { GaussianBlurTexture(&Graphics->Gaussian, &Graphics->Lighting.BloomTex, &Graphics->Lighting.BloomFBO); } */

            CompositeGameTexturesAndDisplay(Plat, Graphics);


            UiFrameEnd(&Engine->Ui);

            MapGpuBuffer(&Ui->SolidQuadGeometryBuffer);
            MapGpuBuffer(&Ui->TextGroup->Buf);

            BonsaiSwapBuffers(&Engine->Stdlib.Os);


            HotReloadShaders(GetStdlib());


            /* GpuMap = GetNextGpuMap(Graphics); */

            // Map GPU buffers for next frame
            MapGpuBuffer(GpuMap);
            /* MapGpuBuffer(&Graphics->Transparency.GpuBuffer); */
            Assert(GpuMap->Buffer.At == 0);

            UnsignalFutex(&Graphics->RenderGate, MAIN_THREAD_ThreadLocal_ThreadIndex);

            IterateOver(&Graphics->GpuTimers, Timer, TimerIndex)
            {
              if (Timer->Ns == 0)
              {
                if (QueryGpuTimer(Timer))
                {
#if BONSAI_DEBUG_SYSTEM_API
                  GetDebugState()->PushHistogramDataPoint(Timer->Ns);
                  // NOTE(Jesse): This skips the next timer, but it'll get
                  // hit on the next frame, so no worries ..
                  RemoveUnordered(&Graphics->GpuTimers, TimerIndex);
#endif
                }
              }
            }
          } break;

          { tmatch(bonsai_render_command_gl_timer_init, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_gl_timer_start, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_gl_timer_end, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_gl_timer_read_value_and_histogram, RenderCommand, Command)
            InvalidCodePath();
          } break;

        }
      } break;
    }

    RewindArena(GetTranArena());
  }
}

link_internal b32
AnyValidLayersRemaining(layered_brush *Brush, s32 At)
{
  b32 HasValidLayer = False;
  RangeIteratorRange(LayerIndex, Brush->LayerCount, At)
  {
    brush_layer *Layer = &Brush->Layers[LayerIndex];
    switch (Layer->Settings.Type)
    {
      case BrushLayerType_Brush:
      {
        if (Layer->Settings.Brush) { HasValidLayer = True; }
      } break;

      case BrushLayerType_Noise:
      case BrushLayerType_Shape:
      {
        HasValidLayer = True;
      } break;
    }
  }
  return HasValidLayer;
}

link_export void
DrainLoRenderQueue(engine_resources *Engine)
{
  /* TIMED_FUNCTION(); */

  UNPACK_ENGINE_RESOURCES(Engine);
  Assert(EntityTable);

  /* RenderInfo("DrainRenderQueue"); */

  if (FutexIsSignaled(&Engine->Graphics.RenderGate)) return;

  AssertNoGlErrors;
  while (work_queue_entry *Job = PopWorkQueueEntry(LoRenderQ))
  {
    /* RenderInfo("%S", ToString(Job->Type)); */
    /* TIMED_NAMED_BLOCK(RENDER_LOOP); */
    tswitch(Job)
    {
      case type_work_queue_entry_noop:
      case type_work_queue_entry_init_world_chunk:
      case type_work_queue_entry_copy_buffer_set:
      case type_work_queue_entry_copy_buffer_ref:
      case type_work_queue_entry_init_asset:
      /* case type_work_queue_entry_update_world_region: */
      case type_work_queue_entry_rebuild_mesh:
      case type_work_queue_entry_finalize_noise_values:
      case type_work_queue_entry_build_chunk_mesh:
      case type_work_queue_entry_sim_particle_system:
      case type_work_queue_entry__align_to_cache_line_helper:
      {
        InvalidCodePath();
      } break;


      { tmatch(work_queue_entry_async_function_call, Job, RPC)
        /* RenderInfo("%S", ToString(RPC->Type)); */
        TIMED_NAMED_BLOCK(work_queue_entry_async_function_call);
        DispatchAsyncFunctionCall(RPC);
      } break;

      { tmatch(work_queue_entry__bonsai_render_command, Job, RenderCommand)
        /* RenderInfo("%S", ToString(RenderCommand->Type)); */
        tswitch(RenderCommand)
        {
          InvalidCase(type_work_queue_entry__bonsai_render_command_noop);

          { case type_bonsai_render_command_cancel_all_noise_readback_jobs:
            TIMED_NAMED_BLOCK(CancelReadbackJobs);

            IterateOver(&Graphics->NoiseReadbackJobs, PBOJob, JobIndex)
            {
              b32 Done = False;
              while (!Done)
              {
                u32 SyncStatus = GetGL()->ClientWaitSync(PBOJob->PBOBuf.Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
                switch(SyncStatus)
                {

                  case GL_ALREADY_SIGNALED:
                  case GL_CONDITION_SATISFIED:
                  {
                    AtomicDecrement(&Graphics->NoiseFinalizeJobsPending);
                    TIMED_NAMED_BLOCK(MapBuffer);
                    AssertNoGlErrors;
                    GetGL()->DeleteBuffers(1, &PBOJob->PBOBuf.PBO);
                    GetGL()->DeleteSync(PBOJob->PBOBuf.Fence);
                    AssertNoGlErrors;
                    /* RemoveUnordered(&Graphics->NoiseReadbackJobs, JobIndex); */
                    Done = True;
                  } break;

                  case GL_WAIT_FAILED:
                  {
                    /* RemoveUnordered(&Graphics->NoiseReadbackJobs, JobIndex); */
                    SoftError("Error waiting on gl sync object");
                  } break;

                  case GL_TIMEOUT_EXPIRED:
                  {
                    SleepMs(1);
                  } break;
                }
              }
              AssertNoGlErrors;
            }

            // Clear the jobs
            umm C = Count(&Graphics->NoiseReadbackJobs);
            auto Z = ZerothIndex(&Graphics->NoiseReadbackJobs);
            RangeIterator_t(umm, Index, C) { RemoveUnordered(&Graphics->NoiseReadbackJobs, Z); }
            Assert(Count(&Graphics->NoiseReadbackJobs) == 0);

            /* Assert(Graphics->NoiseFinalizeJobsPending == 0); */
          } break;

          { tmatch(bonsai_render_command_allocate_and_map_gpu_element_buffer, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_allocate_and_map_gpu_element_buffer);

            AllocateGpuBuffer_gpu_mapped_element_buffer(&Command->Dest->Handles, Command->Type, Command->ElementCount);
            MapGpuBuffer(Command->Dest);
            Assert(HasGpuMesh(Command->Dest));

            Assert(Command->DestNode->Flags & Chunk_Queued);

            auto LowPriorityQ = &Engine->Stdlib.Plat.LowPriority;
            auto Next = WorkQueueEntry(WorkQueueEntryBuildWorldChunkMesh(Command->SynChunk, Command->DestNode));
            PushWorkQueueEntry(LowPriorityQ, &Next);
          } break;

          { tmatch(bonsai_render_command_unmap_gpu_element_buffer, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_unmap_gpu_element_buffer);

            // Supersceded by FinalizeShitAndFuckinDoStuff
            //
            NotImplemented;


/*             gpu_element_buffer_handles *Handles = Command->Handles; */
/*             FlushBuffersToCard_gpu_mapped_element_buffer(Handles); */
/*             Assert(Handles->Mapped == False); */


/*             Info("FlushBuffersToCard_gpu_mapped_element_buffer(%d)", Handles->ElementCount); */

/*             octree_node *Node = Command->DestNode; */
/*             Assert(&Node->Chunk->Handles == Handles); */
/*             Assert(HasGpuMesh(Node->Chunk)); */
/*             Assert(Node->Chunk->Handles.VAO == Handles->VAO); */
/*             Assert(Node->Chunk->Handles.Handles[0] == Handles->Handles[0]); */
/*             Assert(Node->Chunk->Handles.Handles[1] == Handles->Handles[1]); */
/*             Assert(Node->Chunk->Handles.Handles[2] == Handles->Handles[2]); */

            /* FinalizeNodeInitializaion(Cast(octree_node*, Cast(void*, Command->DestNode))); */
          } break;

          { tmatch(bonsai_render_command_unmap_and_deallocate_pbo, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_unmap_and_deallocate_pbo);

            gpu_readback_buffer PBOBuf = Command->PBOBuf;

            /* Info("(%d) Binding and Deallocating PBO (%u)", ThreadLocal_ThreadIndex, PBOBuf.PBO); */
            GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, PBOBuf.PBO);
            AssertNoGlErrors;
            GetGL()->UnmapBuffer(GL_PIXEL_PACK_BUFFER);
            AssertNoGlErrors;
            GetGL()->DeleteBuffers(1, &PBOBuf.PBO);
            AssertNoGlErrors;
            GetGL()->DeleteSync(PBOBuf.Fence);
            AssertNoGlErrors;

          } break;

          { tmatch(bonsai_render_command_allocate_texture, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_allocate_texture);

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


          { tmatch(bonsai_render_command_deallocate_texture, RenderCommand, Command)
            NotImplemented;
          } break;


          { tmatch(bonsai_render_command_allocate_handles, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_allocate_handles);
            NotImplemented;
            /* auto *Handles = Command->Handles; */
            /* auto *Mesh    = Command->Mesh; */

            /* ReallocateAndSyncGpuBuffers(Handles, Mesh); */
            /* DeallocateMesh(Mesh, &Engine->geo_u3d_MeshFreelist); */
          } break;


          { tmatch(bonsai_render_command_deallocate_handles, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_deallocate_handles);
            DeleteGpuBuffer(&Command->Handles);
          } break;


          { tmatch(bonsai_render_command_deallocate_world_chunk, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_deallocate_world_chunk);
            world_chunk *Chunk = Command->Chunk;
            FreeWorldChunk(Engine, Chunk);
          } break;


          { tmatch(bonsai_render_command_clear_all_framebuffers, RenderCommand, Command)
            InvalidCodePath();
          } break;




          { tmatch(bonsai_render_command_setup_shader, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_teardown_shader, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_set_shader_uniform, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_draw_world_chunk_draw_list, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_draw_all_entities, RenderCommand, Command)
            InvalidCodePath();
          } break;


          { tmatch(bonsai_render_command_initialize_noise_buffer, RenderCommand, _Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_initialize_noise_buffer);
            /* Command = 0; */

            AtomicIncrement(&Graphics->NoiseFinalizeJobsPending);

            bonsai_render_command_initialize_noise_buffer C = RenderCommand->bonsai_render_command_initialize_noise_buffer;

            octree_node *Node = C.DestNode;
            world_chunk **Chunk2 = &C.DestNode->Chunk;
            world_chunk *Chunk1 = C.DestNode->Chunk;
            world_chunk *Chunk = Chunk1;

            Assert(s64(Chunk) == s64(Chunk1));

            auto WorldEditRC = &Graphics->WorldEditRC;

            GetGL()->ClearColor(-10000000000.f, -10000000000.f, -10000000000.f, -10000000000.f);
            ClearFramebuffer(WorldEditRC->Framebuffers + 0);
            ClearFramebuffer(WorldEditRC->Framebuffers + 1);
            ClearFramebuffer(WorldEditRC->Framebuffers + 2);

            DispatchTerrainShaders(Graphics, Chunk);

            //
            // Apply edits
            //

            s32 CurrentAccumulationTextureIndex = 0;
            rtt_framebuffer *Accum = WorldEditRC->Framebuffers + CurrentAccumulationTextureIndex;
            /* rtt_framebuffer Accum = WorldEditRC->Framebuffers[CurrentAccumulationTextureIndex]; */
            {
              AcquireFutex(&Node->Lock);
              if (TotalElements(&Node->Edits))
              {
                AssertNoGlErrors;

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


                IterateOver(&Node->Edits, Edit, EditIndex)
                {
                  u32 KeyIndex = u32(GetIndex(&EditIndex));
                  Keys[KeyIndex] = {u64(Edit), u64(Edit->Ordinal)};
                }

                BubbleSort_descending(Keys, u32(EditCount));
#endif
                // }


                RangeIterator(KeyIndex, EditCount)
                {
                  TIMED_NAMED_BLOCK(WorldEditDrawCall);

                  world_edit *Edit = Cast(world_edit*, Keys[KeyIndex].Index);
                  if (Edit->Brush) // NOTE(Jesse): Don't necessarily have to have a brush if we created the edit before we created a brush.
                  {
                    world_edit_brush *Brush = Edit->Brush;

                    // TODO(Jesse): Can we swap these?
                    s32 CurrentReadTextureIndex = CurrentAccumulationTextureIndex + 2;
                    s32 CurrentWriteTextureIndex = CurrentAccumulationTextureIndex + 1;

                    rtt_framebuffer *Read  = WorldEditRC->Framebuffers + CurrentReadTextureIndex;
                    rtt_framebuffer *Write = WorldEditRC->Framebuffers + CurrentWriteTextureIndex;

                    Assert(Read->FBO.ID != Write->FBO.ID);
                    Assert(Read->FBO.ID != Accum->FBO.ID);
                    Assert(Write->FBO.ID != Accum->FBO.ID);

                    rtt_framebuffer Applied = ApplyBrush( WorldEditRC,
                                                          Edit->Region,
                                                          Edit->Rotation,
                                                          Brush,
                                                          Brush->BrushBlendMode,
                                                          Chunk,
                                                          Read, Write, Accum, False, False);

                    if (Applied.FBO.ID == Read->FBO.ID)
                    {
                      *Read = *Accum;
                    }
                    else
                    {
                      Assert(Applied.FBO.ID == Write->FBO.ID);
                      *Write = *Accum;
                    }

                    *Accum = Applied;

                    Assert(Read->FBO.ID != Write->FBO.ID);
                    Assert(Read->FBO.ID != Accum->FBO.ID);
                    Assert(Write->FBO.ID != Accum->FBO.ID);

                    GetGL()->ClearColor(-10000000000.f, -10000000000.f, -10000000000.f, -10000000000.f);
                    ClearFramebuffer(Write);
                    ClearFramebuffer(Read);
                  }

                  AssertNoGlErrors;
                }
              }
              ReleaseFutex(&Node->Lock);
            }

            /* DEBUG_DrawSimSpaceVectorAt(Engine, SimEditRect.Min + EditRectRad, xAxis*200.f, RGB_RED, DEFAULT_LINE_THICKNESS*4.f ); */
            /* DEBUG_DrawSimSpaceVectorAt(Engine, SimEditRect.Min + EditRectRad, yAxis*200.f, RGB_GREEN, DEFAULT_LINE_THICKNESS*4.f ); */
            /* DEBUG_DrawSimSpaceVectorAt(Engine, SimEditRect.Min + EditRectRad, zAxis*200.f, RGB_BLUE, DEFAULT_LINE_THICKNESS*4.f ); */
            /* DEBUG_DrawSimSpaceVectorAt(Engine, SimEditRect.Min + EditRectRad, PlaneNormal*400.f, RGB_PINK, DEFAULT_LINE_THICKNESS*2.f ); */

            texture *CurrentAccumulationTexture = &Accum->DestTexture;

            //
            // Terrain Finalize
            //
            {
              TIMED_NAMED_BLOCK(TerrainFinalizeDrawCall);
              GetGL()->BindFramebuffer(GL_FRAMEBUFFER, Graphics->TerrainFinalizeRC.FBO.ID);

              UseShader(&Graphics->TerrainFinalizeRC);

              // NOTE(Jesse): The other inputtex calls bind to texture unit 1 .. it is correct that
              // this bind to unit 0 because there's no derivs texture being bound here.
              //
              // @derivs_texture_binding_to_shader_unit_0
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
              GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
              GetGL()->BufferData(GL_PIXEL_PACK_BUFFER, NoiseByteCount, 0, GL_STREAM_READ);
              AssertNoGlErrors;
              GetGL()->ReadPixels(0, 0, CurrentAccumulationTexture->Dim.x, CurrentAccumulationTexture->Dim.y, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
              AssertNoGlErrors;
              GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

              gl_fence Fence = GetGL()->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

              dummy_work_queue_entry_build_chunk_mesh Readback = { {PBO,Fence}, NoiseDim, Node};
              Push(&Graphics->NoiseReadbackJobs, &Readback);
            }
          } break;


          { tmatch(bonsai_render_command_do_stuff, RenderCommand, Command)
            InvalidCodePath();
          } break;

          { tmatch(bonsai_render_command_gl_timer_init, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_init);
            AssertNoGlErrors;
            GetGL()->GenQueries(1, Command->GlTimerObject);
            AssertNoGlErrors;
          } break;

          { tmatch(bonsai_render_command_gl_timer_start, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_start);
            AssertNoGlErrors;
            GetGL()->BeginQuery(GL_TIME_ELAPSED, Command->GlTimerObject);
            AssertNoGlErrors;
          } break;

          { tmatch(bonsai_render_command_gl_timer_end, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_end);
            AssertNoGlErrors;
            GetGL()->EndQuery(GL_TIME_ELAPSED);
            AssertNoGlErrors;
          } break;

          { tmatch(bonsai_render_command_gl_timer_read_value_and_histogram, RenderCommand, Command)
            TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_read_value_and_histogram);
#if 0
            AssertNoGlErrors;
            u64 TimerNs = 0;

            s32 Available = False;
            while (!Available)
            {
              GetGL()->GetQueryObjectiv(Command->GlTimerObject, GL_QUERY_RESULT_AVAILABLE, &Available);
              /* if (Available == False) { Info("Waiting for query object to become available"); } */
            }


            GetGL()->GetQueryObjectui64v(Command->GlTimerObject, GL_QUERY_RESULT, &TimerNs);
            /* Info("GL reported time of (%.2f)ms", f64(TimerNs)/1000000.0); */
            /* GetDebugState()->PushHistogramDataPoint(TimerNs); */
            AssertNoGlErrors;
#endif
          } break;

        }
      } break;
    }

    RewindArena(GetTranArena());

    if (FutexIsSignaled(&Engine->Graphics.RenderGate)) return;
  }

  {
    TIMED_NAMED_BLOCK(CheckReadbackJobs);
    IterateOver(&Graphics->NoiseReadbackJobs, PBOJob, JobIndex)
    {
      TIMED_NAMED_BLOCK(CheckJob);
      Assert(PBOJob);

      /* Info("PBOJob(0x%x) JobIndex(%u)", PBOJob, JobIndex.Index); */
      /* Info("0x%x 0x%x", PBOJob->PBOBuf.PBO, PBOJob->PBOBuf.Fence); */

      u32 SyncStatus = GetGL()->ClientWaitSync(PBOJob->PBOBuf.Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
      AssertNoGlErrors;
      switch(SyncStatus)
      {
        case GL_ALREADY_SIGNALED:
        case GL_CONDITION_SATISFIED:
        {
          TIMED_NAMED_BLOCK(MapBuffer);

          AssertNoGlErrors;
          /* umm JobCount = AtElements(&Graphics->NoiseReadbackJobs).Index; */
          /* Info("(%d) Binding and Mapping PBOJob(0x%x) PBO(%u) JobCount(%d) JobIndex(%u)", ThreadLocal_ThreadIndex, PBOJob, PBOJob->PBOBuf.PBO, JobCount, JobIndex.Index); */
          GetGL()->BindBuffer(GL_PIXEL_PACK_BUFFER, PBOJob->PBOBuf.PBO);
          AssertNoGlErrors;
          u32 *NoiseValues = Cast(u32*, GetGL()->MapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
          AssertNoGlErrors;

          auto BuildMeshJob = WorkQueueEntry(WorkQueueEntryFinalizeNoiseValues(PBOJob->PBOBuf, NoiseValues, PBOJob->NoiseDim, PBOJob->DestNode));
          PushWorkQueueEntry(&Plat->LowPriority, &BuildMeshJob);

          // TODO(Jesse): This actually makes the loop skip a job because we
          // shorten the array, but never update the index we're looking at.
          //
          // It doesn't matter in this case because this runs every frame, so
          // the skipped job is just a frame late.  But, it would be nice if
          // this was better.
          // 
          // NOTE(Jesse): Must happen after we read the PBOJob values for the work queue entry
          RemoveUnordered(&Graphics->NoiseReadbackJobs, JobIndex);
        } break;

        case GL_TIMEOUT_EXPIRED:
        {
        } break;

        case GL_WAIT_FAILED:
        {
          SoftError("Error waiting on gl sync object");
        } break;
      }

      if (FutexIsSignaled(&Engine->Graphics.RenderGate)) return;
    }
  }
}

#define MillisecondsToNanoseconds(ms) (ms*1000.0)

link_internal void
SpinlockNs(s32 Nanoseconds)
{
  TIMED_FUNCTION();

  r64 StartMs = GetHighPrecisionClock();

  for (;;)
  {
    r64 CurrentMs = GetHighPrecisionClock();
    r64 Elapsed = CurrentMs - StartMs;

    s32 ElapsedNs = s32(MillisecondsToNanoseconds(Elapsed));
    if (ElapsedNs > Nanoseconds)
    {
      break;
    }
  }
}

link_export THREAD_MAIN_RETURN
RenderThread_Main(void *ThreadStartupParams)
{
  thread_local_state *Thread = Cast(thread_local_state*, ThreadStartupParams);
  WorkerThread_BeforeJobStart(Thread);

  Assert(GetStdlib()->ThreadStates);

  /* Assert(Thread->ThreadIndex > 0); */
  /* SetThreadLocal_ThreadIndex(Thread->ThreadIndex); */

  engine_resources *Engine    = GetEngineResources();
   application_api *AppApi    = &Thread->Stdlib->AppApi;
                os *Os        = &Engine->Stdlib.Os;
          platform *Plat      = &Engine->Stdlib.Plat;
        engine_api *EngineApi = &Engine->EngineApi;

  PlatformMakeRenderContextCurrent(Os);

  b32 InitResult = GraphicsInit(&Engine->Graphics, &Engine->Settings, AllocateArena());

  memory_arena *UiMemory = AllocateArena();
  if (InitResult)
  {
    InitRenderer2D(&Engine->Ui, &Engine->Heap, UiMemory, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);

    bitmap_block_array Bitmaps = BitmapBlockArray(GetTranArena());
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
    Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64));
  }

  // Map immediate GPU buffers for first frame
  MapGpuBuffer(&Engine->Graphics.ImmediateGeometry);
  MapGpuBuffer(&Engine->Graphics.Transparency.GpuBuffer);

  auto Ui = &Engine->Ui;
  MapGpuBuffer(&Ui->SolidQuadGeometryBuffer);
  MapGpuBuffer(&Ui->TextGroup->Buf);

  FullBarrier;
  SignalFutex(&Engine->Graphics.Initialized);

  if (InitResult)
  {
    // Wait for main thread to complete initialization
    while (FutexIsSignaled(&Engine->ReadyToStartMainLoop) == False) { SleepMs(1); };

    bonsai_futex *WorkerThreadsExitFutex = &Plat->WorkerThreadsExitFutex;
    while ( FutexNotSignaled(WorkerThreadsExitFutex) )
    {
      WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

      AppApi->WorkerBeforeJob(Thread);

      EngineApi->DrainHiRenderQueue(Engine);
      EngineApi->DrainLoRenderQueue(Engine);

      if (FutexIsSignaled(&Plat->WorkerThreadsSuspendFutex))
      {
        // Have to wait for all the worker threads to suspend cause they push
        // entries onto the render queues
        while (Plat->WorkerThreadsSuspendFutex.ThreadsWaiting != GetWorkerThreadCount()-1)
        {
          SleepMs(1);
        }

        // Even though we just called these we have to make sure they're actually
        // flushed; the main thread could have pushed stuff onto the Hi
        // queue while we were draining the Lo queue, for example.  Once we're
        // in this if we know the main thread is waiting for us
        EngineApi->DrainHiRenderQueue(Engine);
        EngineApi->DrainLoRenderQueue(Engine);
        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.HiRenderQ));
        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.LoRenderQ));

        SleepMs(3);

        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.HiRenderQ));
        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.LoRenderQ));

        WaitOnFutex(&Plat->WorkerThreadsSuspendFutex);
      }

      SpinlockNs(100);

      /* SleepMs(1); */
    }

    Info("Exiting Render Thread (%d)", Thread->ThreadIndex);
    WaitOnFutex(WorkerThreadsExitFutex);
  }
  else
  {
    Error("Render thread initiailization failed.");
  }

  THREAD_MAIN_RETURN Result = ReinterpretCast(THREAD_MAIN_RETURN, InitResult);
  return Result;
}
