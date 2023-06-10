#if PLATFORM_GL_IMPLEMENTATIONS


link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  b32 Result = InitializeOpenglFunctions();
#if DEBUG_SYSTEM_API
  Global_DebugStatePointer = Resources->DebugState;
#endif

  Global_ThreadStates = Resources->ThreadStates;

  // We should only ever call this from the main thread, and this sets our
  // thread index such that the game doesn't have to worry about doing it.
  if (ThreadLocal_ThreadIndex == -1) { SetThreadLocal_ThreadIndex(0); }
  else { Assert(ThreadLocal_ThreadIndex == 0); }

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
  Resources->Heap = InitHeap(Gigabytes(4));

  Init_Global_QuadVertexBuffer();

  Resources->World = Allocate(world, BonsaiInitArena, 1);
  if (!Resources->World) { Error("Allocating World"); return False; }

  memory_arena *GraphicsMemory = AllocateArena();
  Resources->Graphics = GraphicsInit(GraphicsMemory);
  if (!Resources->Graphics) { Error("Initializing Graphics"); return False; }

  memory_arena *GraphicsMemory2D = AllocateArena();
  InitRenderer2D(&Resources->GameUiRenderer, &Resources->Heap, GraphicsMemory2D);

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

  // Must come before UNPACK_ENGINE_RESOURCES such that we unpack the correct GpuMap
  graphics *G = Resources->Graphics;
  G->GpuBufferWriteIndex = (Resources->FrameIndex) % 2;

  UNPACK_ENGINE_RESOURCES(Resources);

  World->ChunkHash = CurrentWorldHashtable(Resources);

  MapGpuElementBuffer(GpuMap);

  ClearFramebuffers(Graphics);

#if DEBUG_DRAW_WORLD_AXIES
  {
    untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_VOXEL*3);
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(10000, 0, 0), RED, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 10000, 0), GREEN, 0.15f );
    DEBUG_DrawLine_Aligned(&CopyDest, V3(0,0,0), V3(0, 0, 10000), BLUE, 0.15f );
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

  // NOTE(Jesse): This has to come after the entities simulate, and before the draw
  // We have to 
  auto CameraTargetP = Resources->CameraTargetEntity ? Resources->CameraTargetEntity->P : Canonical_Position(0);
  {
    v2 MouseDelta = GetMouseDelta(Plat);
    input* GameInput = &Plat->Input;
    UpdateGameCamera(MouseDelta, GameInput, CameraTargetP, Camera, World->ChunkDim);
    Resources->Graphics->gBuffer->ViewProjection =
      ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
      ViewMatrix(World->ChunkDim, Camera);
  }

  if (World->Flags & WorldFlag_WorldCenterFollowsCameraTarget)
  {
    World->Center = CameraTargetP.WorldP;
  }

  Render_BufferGameGeometry(Resources);
  UnsignalFutex(&Resources->Plat->HighPriorityModeFutex);

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

  r32 MappedGameTime = Plat->GameTime / 18.0f;

  SG->Sun.Position.x = Sin(MappedGameTime);
  SG->Sun.Position.y = Cos(MappedGameTime);

  RenderGBuffer(GpuMap, Graphics);
  RenderShadowMap(GpuMap, Graphics);
  RenderAoTexture(AoGroup);
  DrawGBufferToFullscreenQuad(Plat, Graphics);

  /* Debug_DrawTextureToDebugQuad( &Graphics->SG->DebugTextureShader ); */
  /* Debug_DrawTextureToDebugQuad(&AoGroup->DebugSsaoShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugColorShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugPositionShader); */
  /* Debug_DrawTextureToDebugQuad(&Graphics->gBuffer->DebugNormalShader); */

  GpuMap->Buffer.At = 0;
  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);

  Resources->GameUiRenderer.ScreenDim = V2(Plat->WindowWidth, Plat->WindowHeight);
  FlushCommandBuffer(&Resources->GameUiRenderer, Resources->GameUiRenderer.CommandBuffer);

  b32 Result = True;
  return Result;
}

#endif
