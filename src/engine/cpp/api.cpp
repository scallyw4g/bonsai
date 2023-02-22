#if PLATFORM_GL_IMPLEMENTATIONS


link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  b32 Result = InitializeOpenglFunctions();
#if DEBUG_SYSTEM_API
  Global_DebugStatePointer = Resources->DebugState;
#endif
  return Result;
}

link_export b32
Bonsai_Init(engine_resources *Resources)
{
  TIMED_FUNCTION();

  b32 Result = True;

  DEBUG_REGISTER_NAMED_ARENA(TranArena, 0, "game_lib TranArena");

  memory_arena *BonsaiInitArena = AllocateArena(Megabytes(256));;
  DEBUG_REGISTER_ARENA(BonsaiInitArena, 0);

  Resources->Memory = BonsaiInitArena;

  Init_Global_QuadVertexBuffer();

  Resources->World = Allocate(world, BonsaiInitArena, 1);
  if (!Resources->World) { Error("Allocating World"); return False; }

  Resources->Graphics = GraphicsInit(BonsaiInitArena);
  if (!Resources->Graphics) { Error("Initializing Graphics"); return False; }

  Resources->Heap = InitHeap(Gigabytes(4));
  Resources->EntityTable = AllocateEntityTable(BonsaiInitArena, TOTAL_ENTITY_COUNT);

  return Result;
}

link_export b32
Bonsai_FrameBegin(engine_resources *Resources)
{
  TIMED_FUNCTION();

  // Must come before we update the frame index
  CollectUnusedChunks(Resources, &Resources->MeshFreelist, Resources->World->Memory, Resources->World->VisibleRegion);

  Resources->FrameIndex += 1;

  graphics *G = Resources->Graphics;
  G->GpuBufferWriteIndex = (Resources->FrameIndex) % 2;

  UNPACK_ENGINE_RESOURCES();

  World->ChunkHash = CurrentWorldHashtable(Resources);

  MapGpuElementBuffer(GpuMap);

  /* DEBUG_VALUE(GpuMap); */
  /* MapGpuElementBuffer(GpuMap); */
  ClearFramebuffers(Graphics);

#if DEBUG_DRAW_WORLD_AXIES
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_LINE*3);
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED, 0.2f );
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.2f );
    DEBUG_DrawLine(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.2f );
  }
#endif

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  Graphics->Lights->Count = 0;

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_FrameEnd(engine_resources *Resources)
{
  TIMED_FUNCTION();

  SignalFutex(&Resources->Plat->HighPriorityModeFutex);

  UNPACK_ENGINE_RESOURCES(Resources);

  Bonsai_SimulateEntitiesAndWorld(Resources);

  Render_BufferGameGeometry(Resources);
  UnsignalFutex(&Resources->Plat->HighPriorityModeFutex);

  Resources->Graphics->gBuffer->ViewProjection =
    ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    ViewMatrix(World->ChunkDim, Camera);

#if DEBUG_SYSTEM_API
  Debug_DoWorldChunkPicking(Resources);
#endif

  RewindArena(TranArena);

  b32 Result = True;
  return Result;
}

link_export b32
Bonsai_Render(engine_resources *Resources)
{
  TIMED_FUNCTION();

  platform *Plat            = Resources->Plat;
  graphics        *Graphics = Resources->Graphics;
  ao_render_group *AoGroup  = Graphics->AoGroup;
  shadow_render_group *SG   = Graphics->SG;
  gpu_mapped_element_buffer *GpuMap = GetCurrentGpuMap(Graphics);

  r32 MappedGameTime = (r32)(Plat->GameTime / 18.0);

  SG->Sun.Position.x = Sin(MappedGameTime);
  SG->Sun.Position.y = Cos(MappedGameTime);

  RenderGBuffer(GpuMap, Graphics);
  RenderShadowMap(GpuMap, Graphics);
  RenderAoTexture(AoGroup);
  DrawGBufferToFullscreenQuad(Plat, Graphics);

  Debug_DrawTextureToDebugQuad( &Graphics->SG->DebugTextureShader );
  /* Debug_DrawTextureToDebugQuad(&AoGroup->DebugSsaoShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugColorShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugPositionShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugNormalShader); */

  GpuMap->Buffer.At = 0;
  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);

  b32 Result = True;
  return Result;
}

#endif
