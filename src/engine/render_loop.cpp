link_internal void
RenderLoop(thread_startup_params *ThreadParams, engine_resources *Engine)
{
  // Map immediate GPU buffers for first frame
  MapGpuBuffer_untextured_3d_geometry_buffer(&Engine->Graphics.GpuBuffers[0]);
  MapGpuBuffer_untextured_3d_geometry_buffer(&Engine->Graphics.Transparency.GpuBuffer);

  auto LowPriorityQ = &Engine->Stdlib.Plat.LowPriority;

  os *Os         = &Engine->Stdlib.Os;
  /* platform *Plat = &Engine->Stdlib.Plat; */
  while ( FutexNotSignaled(ThreadParams->WorkerThreadsExitFutex) )
  {
    UNPACK_ENGINE_RESOURCES(Engine);

    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();
    WorkerThread_BeforeJobStart(ThreadParams);

    AssertNoGlErrors;

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
          TIMED_NAMED_BLOCK(work_queue_entry_async_function_call);
          DispatchAsyncFunctionCall(RPC);
        } break;

        { tmatch(work_queue_entry__bonsai_render_command, Job, RenderCommand)
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
                  u32 SyncStatus = GL.ClientWaitSync(PBOJob->PBOBuf.Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
                  switch(SyncStatus)
                  {
                    case GL_ALREADY_SIGNALED:
                    case GL_CONDITION_SATISFIED:
                    {
                      AtomicDecrement(&Graphics->NoiseFinalizeJobsPending);
                      TIMED_NAMED_BLOCK(MapBuffer);
                      AssertNoGlErrors;
                      GL.DeleteBuffers(1, &PBOJob->PBOBuf.PBO);
                      GL.DeleteSync(PBOJob->PBOBuf.Fence);
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

              /* Assert(HasGpuMesh(Command->Dest) == 0); */
              /* Assert(HasGpuMesh(&Command->DestChunk->Mesh) == 0); */
              /* Command->Dest[0] = AllocateAndMapGpuBuffer(Command->Type, Command->ElementCount); */

              ReallocateGpuBuffers(&Command->Dest->Handles, Command->Type, Command->ElementCount);
              MapGpuBuffer_untextured_3d_geometry_buffer(Command->Dest);

              Assert(HasGpuMesh(Command->Dest) == 1);
              /* Assert(HasGpuMesh(&Command->DestChunk->Mesh) == 1); */

              auto Next = WorkQueueEntry(WorkQueueEntryBuildWorldChunkMesh(Command->SynChunk, Command->DestChunk));
              PushWorkQueueEntry(LowPriorityQ, &Next);
            } break;

            { tmatch(bonsai_render_command_unmap_gpu_element_buffer, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_unmap_gpu_element_buffer);
              FlushBuffersToCard(Command->Buf);

              FinalizeChunkInitialization(Cast(world_chunk*, Cast(void*, Command->Chunk)));
            } break;

            { tmatch(bonsai_render_command_unmap_and_deallocate_buffer, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_unmap_and_deallocate_buffer);

              gpu_readback_buffer PBOBuf = Command->PBOBuf;
              GL.BindBuffer(GL_PIXEL_PACK_BUFFER, PBOBuf.PBO);
              AssertNoGlErrors;
              GL.UnmapBuffer(GL_PIXEL_PACK_BUFFER);
              AssertNoGlErrors;
              GL.DeleteBuffers(1, &PBOBuf.PBO);
              GL.DeleteSync(PBOBuf.Fence);
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


            { tmatch(bonsai_render_command_allocate_buffers, RenderCommand, Command)
              NotImplemented;
            } break;

            { tmatch(bonsai_render_command_reallocate_buffers, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_reallocate_buffers);
              auto *Handles = Command->Handles;
              auto *Mesh    = Command->Mesh;

              ReallocateAndSyncGpuBuffers(Handles, Mesh);
              DeallocateMesh(Mesh, &Engine->geo_u3d_MeshFreelist);
            } break;


            { tmatch(bonsai_render_command_deallocate_buffers, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_deallocate_buffers);
              if (*Command->Buffers) { GL.DeleteBuffers(Command->Count, Command->Buffers); }
              RangeIterator(Index, Command->Count) { Command->Buffers[Index] = 0; }
            } break;


            { tmatch(bonsai_render_command_deallocate_world_chunk, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_deallocate_world_chunk);
              world_chunk *Chunk = Command->Chunk;
              FreeWorldChunk(Engine, Chunk);
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
                  SetupGBufferShader(Graphics, GetApplicationResolution(&Engine->Settings), Editor->MaskSelection);
                } break;

                case BonsaiRenderCommand_ShaderId_ShadowMap:
                {
                  SetupShadowMapShader(Graphics, GetShadowMapResolution(&Engine->Settings), Editor->MaskSelection);
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
              RenderDrawList(Engine, Command->DrawList, Command->Shader);
            } break;

            { tmatch(bonsai_render_command_draw_all_entities, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_draw_all_entities);
              DrawEntities(Command->Shader, EntityTable, &GpuMap->Buffer, 0, Graphics, World, Plat->dt);
            } break;


            { tmatch(bonsai_render_command_initialize_noise_buffer, RenderCommand, _Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_initialize_noise_buffer);
              /* Command = 0; */


              AtomicIncrement(&Graphics->NoiseFinalizeJobsPending);

              bonsai_render_command_initialize_noise_buffer C = RenderCommand->bonsai_render_command_initialize_noise_buffer;

              octree_node *Node = C.Node;
              world_chunk **Chunk2 = &C.Node->Chunk;
              world_chunk *Chunk1 = C.Node->Chunk;
              world_chunk *Chunk = Chunk1;

              Assert(s64(Chunk) == s64(Chunk1));

              texture *InputTex = 0;

              //
              // Launch terrain shaping shader
              //
              {
                auto *RC = &Graphics->TerrainShapingRC;

                RC->WorldspaceChunkBasis = V3(Chunk->WorldP) * V3(64);
                RC->ChunkResolution = V3(Chunk->DimInChunks);

                TIMED_NAMED_BLOCK(TerrainDrawCall);
                GL.BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO.ID);
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

                TIMED_NAMED_BLOCK(TerrainDrawCall);
                GL.BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO.ID);
                UseShader(RC);
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

                TIMED_NAMED_BLOCK(TerrainDrawCall);
                GL.BindFramebuffer(GL_FRAMEBUFFER, RC->DestFBO->ID);
                UseShader(RC);
                // Using texture unit 1 because the DerivsTex is automatically bound in
                // UseShader to unit 0
                BindUniformByName(&RC->Program, "InputTex", InputTex, 1);

                RenderQuad();

                AssertNoGlErrors;

                InputTex = RC->DestTex;
              }

              s32 PingPongIndex = 1;

#if 1
              //
              // Apply edits
              //

              {
                auto WorldEditRC = &Graphics->WorldEditRC;
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
                  s32 EditCount = s32(TotalElements(&Node->Edits));
                  sort_key *Keys = Allocate(sort_key, GetTranArena(), EditCount);


                  IterateOver(&Node->Edits, Edit, EditIndex)
                  {
                    u32 KeyIndex = u32(GetIndex(&EditIndex));
                    Keys[KeyIndex] = {u64(Edit), u64(Edit->Ordinal)};
                  }

                  BubbleSort_descending(Keys, u32(EditCount));
                  // }

                  RangeIterator(KeyIndex, EditCount)
                  {
                    TIMED_NAMED_BLOCK(WorldEditDrawCall);

                    world_edit *Edit = Cast(world_edit*, Keys[KeyIndex].Index);
                    if (Edit->Brush) // NOTE(Jesse): Don't necessarily have to have a brush if we created the edit before we created a brush.
                    {
                      layered_brush *Brush = &Edit->Brush->Layered;
                      RangeIterator(LayerIndex, Brush->LayerCount)
                      {
                        GL.BindFramebuffer(GL_FRAMEBUFFER, WorldEditRC->PingPongFBOs[PingPongIndex].ID);

                        BindUniformByName(&WorldEditRC->Program, "InputTex", InputTex, 1);

                        brush_layer *Layer = Brush->Layers + LayerIndex;

                        {
                          v3 RGBColor = HSVtoRGB(Layer->Settings.HSVColor);
                          BindUniformByName(&WorldEditRC->Program, "RGBColor", &RGBColor);
                        }

                        BindUniformByName(&WorldEditRC->Program, "BrushType", Layer->Settings.Type);
                        BindUniformByName(&WorldEditRC->Program, "BlendMode", Layer->Settings.Mode);
                        BindUniformByName(&WorldEditRC->Program, "Modifiers", Layer->Settings.Modifier);
                        BindUniformByName(&WorldEditRC->Program, "ColorMode", Layer->Settings.ColorMode);
                        BindUniformByName(&WorldEditRC->Program, "Invert",    Layer->Settings.Invert);

                        rect3 SimEditRect = GetSimSpaceRect(World, Edit->Region);
                           v3 SimChunkMin = GetSimSpaceP(World, Chunk->WorldP);
                           v3 EditRectRad = GetRadius(&SimEditRect);

                        // NOTE(Jesse): Must call bind explicitly because the
                        // driver doesn't cache these values otherwise .. it
                        // just reads them whenever it wants through the pointer..
                        v3 ChunkRelEditMin = SimEditRect.Min - SimChunkMin;
                        BindUniformByName(&WorldEditRC->Program, "ChunkRelEditMin", &ChunkRelEditMin);
                        AssertNoGlErrors;

                        v3 Mx = SimEditRect.Max - SimChunkMin;
                        BindUniformByName(&WorldEditRC->Program, "ChunkRelEditMax", &Mx);
                        AssertNoGlErrors;

                        switch (Layer->Settings.Type)
                        {
                          case BrushLayerType_Noise:
                          {
                            noise_layer *Noise = &Layer->Settings.Noise;
                            BindUniformByName(&WorldEditRC->Program, "NoiseType", Noise->Type);

                            switch (Noise->Type)
                            {
                              case NoiseType_Perlin:
                              {
                                auto *Perlin = &Noise->Perlin;
                                BindUniformByName(&WorldEditRC->Program, "Threshold", Perlin->Threshold);
                                BindUniformByName(&WorldEditRC->Program, "Period",   &Perlin->Period);
                                BindUniformByName(&WorldEditRC->Program, "Power", Perlin->Amplitude);
                              } break;

                              case NoiseType_Voronoi:
                              {
                                auto *Voronoi = &Noise->Voronoi;
                                BindUniformByName(&WorldEditRC->Program, "Squareness", Voronoi->Squareness);
                                BindUniformByName(&WorldEditRC->Program, "Threshold", Voronoi->Threshold);
                                BindUniformByName(&WorldEditRC->Program, "Period",   &Voronoi->Period);
                                BindUniformByName(&WorldEditRC->Program, "Power", Voronoi->Amplitude);
                              } break;

                              case NoiseType_White:
                              {} break;
                            }

                          } break;

                          case BrushLayerType_Shape:
                          {
                            shape_layer *Shape = &Layer->Settings.Shape;
                            BindUniformByName(&WorldEditRC->Program, "ShapeType", Shape->Type);
                            BindUniformByName(&WorldEditRC->Program, "Threshold", Shape->Threshold);
                            BindUniformByName(&WorldEditRC->Program, "Power",     Shape->Power);
                            switch(Shape->Type)
                            {
                              case ShapeType_Rect:     { /* No special uniforms needed for Rect .. */ } break;

                              case ShapeType_Sphere:
                              {
                                auto Sphere = &Shape->Sphere;

                                v3 SimSphereOrigin = GetSimSpaceP(World, Edit->Region.Min + EditRectRad);
                                v3 ChunkRelLocation = SimSphereOrigin - SimChunkMin;

                                BindUniformByName(&WorldEditRC->Program, "ChunkRelLocation", &ChunkRelLocation);
                                BindUniformByName(&WorldEditRC->Program, "Radius", Sphere->Radius);
                              } break;

                              case ShapeType_Line:
                              {
                                auto Line = &Shape->Line;
                                BindUniformByName(&WorldEditRC->Program, "Radius", Line->Radius);
                              } break;

                              case ShapeType_Cylinder:
                              {
                                auto Cylinder = &Shape->Cylinder;
                                BindUniformByName(&WorldEditRC->Program, "Axis", Cylinder->Axis);
                                BindUniformByName(&WorldEditRC->Program, "Radius", Cylinder->Radius);
                              } break;

                              // @sdf_shape_step(5): Calculate values and bind uniform variables for the new shape
                              //
                            }
                          } break;
                        }


                        /* gpu_timer Timer = StartGpuTimer(); */
                        RenderQuad();
                        /* EndGpuTimer(&Timer); */
                        /* Push(&Graphics->GpuTimers, &Timer); */
                        InputTex = &WorldEditRC->PingPongTextures[PingPongIndex];
                        PingPongIndex = (PingPongIndex + 1) & 1;
                      }
                    }

                    AssertNoGlErrors;
                  }
                }
                ReleaseFutex(&Node->Lock);
              }
#endif


              //
              // Terrain Finalize
              //
              {
                TIMED_NAMED_BLOCK(TerrainFinalizeDrawCall);
                GL.BindFramebuffer(GL_FRAMEBUFFER, Graphics->TerrainFinalizeRC.FBO.ID);

                UseShader(&Graphics->TerrainFinalizeRC);

                BindUniformByName(&Graphics->TerrainFinalizeRC.Program, "InputTex", InputTex, 0);

                /* gpu_timer Timer = StartGpuTimer(); */
                RenderQuad();
                /* EndGpuTimer(&Timer); */
                /* Push(&Graphics->GpuTimers, &Timer); */

                AssertNoGlErrors;
              }

              /* Assert(Chunk1->Dim == V3i(64)); */
              /* Assert(NoiseDim == V3(66)); */
              v3i NoiseDim = V3i(66);

              s32 NoiseElementCount = s32(Volume(InputTex->Dim));
              s32 NoiseByteCount = NoiseElementCount*s32(sizeof(u16));

              {
                TIMED_NAMED_BLOCK(GenPboAndInitTransfer);
                u32 PBO;
                GL.GenBuffers(1, &PBO);
                AssertNoGlErrors;
                GL.BindBuffer(GL_PIXEL_PACK_BUFFER, PBO);
                GL.BufferData(GL_PIXEL_PACK_BUFFER, NoiseByteCount, 0, GL_STREAM_READ);
                AssertNoGlErrors;
                GL.ReadPixels(0, 0, InputTex->Dim.x, InputTex->Dim.y, GL_RED_INTEGER, GL_UNSIGNED_SHORT, 0);
                AssertNoGlErrors;
                GL.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

                gl_fence Fence = GL.FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

                dummy_work_queue_entry_build_chunk_mesh Readback = { {PBO,Fence}, NoiseDim, Chunk};
                Push(&Graphics->NoiseReadbackJobs, &Readback);
              }

            } break;


            { tmatch(bonsai_render_command_do_stuff, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_do_stuff);

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


#if 0
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

              Ensure( FlushBuffersToCard(GpuMap) ); // Unmaps buffer
              if (GpuMap->Buffer.At)
              {
                RenderImmediateGeometryToGBuffer(GetApplicationResolution(&Engine->Settings), GpuMap, Graphics);
                /* RenderImmediateGeometryToShadowMap(GpuMap, Graphics); */
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
              TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_init);
              AssertNoGlErrors;
              GL.GenQueries(1, Command->GlTimerObject);
              AssertNoGlErrors;
            } break;

            { tmatch(bonsai_render_command_gl_timer_start, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_start);
              AssertNoGlErrors;
              GL.BeginQuery(GL_TIME_ELAPSED, Command->GlTimerObject);
              AssertNoGlErrors;
            } break;

            { tmatch(bonsai_render_command_gl_timer_end, RenderCommand, Command)
              TIMED_NAMED_BLOCK(bonsai_render_command_gl_timer_end);
              AssertNoGlErrors;
              GL.EndQuery(GL_TIME_ELAPSED);
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
                GL.GetQueryObjectiv(Command->GlTimerObject, GL_QUERY_RESULT_AVAILABLE, &Available);
                /* if (Available == False) { Info("Waiting for query object to become available"); } */
              }


              GL.GetQueryObjectui64v(Command->GlTimerObject, GL_QUERY_RESULT, &TimerNs);
              /* Info("GL reported time of (%.2f)ms", f64(TimerNs)/1000000.0); */
              /* GetDebugState()->PushHistogramDataPoint(TimerNs); */
              AssertNoGlErrors;
#endif
            } break;

          }
        } break;
      }

      RewindArena(GetTranArena());
    }

    {
      TIMED_NAMED_BLOCK(CheckReadbackJobs);
      IterateOver(&Graphics->NoiseReadbackJobs, PBOJob, JobIndex)
      {
        /* Info("0x%x 0x%x", PBOJob->PBOBuf.PBO, PBOJob->PBOBuf.Fence); */
        u32 SyncStatus = GL.ClientWaitSync(PBOJob->PBOBuf.Fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
        AssertNoGlErrors;
        switch(SyncStatus)
        {
          case GL_ALREADY_SIGNALED:
          case GL_CONDITION_SATISFIED:
          {
            TIMED_NAMED_BLOCK(MapBuffer);
            AssertNoGlErrors;
            GL.BindBuffer(GL_PIXEL_PACK_BUFFER, PBOJob->PBOBuf.PBO);
            AssertNoGlErrors;
            u16 *NoiseValues = Cast(u16*, GL.MapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
            AssertNoGlErrors;

            auto BuildMeshJob = WorkQueueEntry(WorkQueueEntryFinalizeNoiseValues(PBOJob->PBOBuf, NoiseValues, PBOJob->NoiseDim, PBOJob->Chunk));
            PushWorkQueueEntry(&Plat->LowPriority, &BuildMeshJob);

            // TODO(Jesse): This actually makes the loop skip a job because we
            // shorten the array, but never update the index we're looking at.
            //
            // It doesn't matter in this case because this runs every frame, so
            // the skipped job is just a frame late.  But, it would be nice if
            // this was better.
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
      }
    }

    if (FutexIsSignaled(ThreadParams->WorkerThreadsExitFutex)) break;

    if (FutexIsSignaled(ThreadParams->WorkerThreadsSuspendFutex)) { WaitOnFutex(ThreadParams->WorkerThreadsSuspendFutex); }

    SleepMs(1);
  }

  Info("Exiting Render Thread (%d)", ThreadParams->ThreadIndex);
  WaitOnFutex(ThreadParams->WorkerThreadsExitFutex);
}

link_export THREAD_MAIN_RETURN
RenderThread_Main(void *ThreadStartupParams)
{
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

  // NOTE(Jesse): This now happens in stdlib startup
  /* s32 VSyncFrames = 0; */
  /* InitResult &= OpenAndInitializeWindow(Os, Plat, VSyncFrames); */
  /* if (InitResult) { InitResult &= InitializeOpenglFunctions(); } */

  b32 InitResult = True;
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
    Error("Render thread initiailization failed.");
  }

  THREAD_MAIN_RETURN Result = ReinterpretCast(THREAD_MAIN_RETURN, InitResult);
  return Result;
}
