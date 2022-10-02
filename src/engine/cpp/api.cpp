#if PLATFORM_GL_IMPLEMENTATIONS

#define UNPACK_ENGINE_RESOURCES(Res)                                      \
  platform                  *Plat          =  Resources->Plat;            \
  world                     *World         =  Resources->World;           \
  game_state                *GameState     =  Resources->GameState;       \
  memory_arena              *Memory        =  Resources->Memory;          \
  heap_allocator            *Heap          = &Resources->Heap;            \
  entity                   **EntityTable   =  Resources->EntityTable;     \
  hotkeys                   *Hotkeys       =  Resources->Hotkeys;         \
  mesh_freelist             *MeshFreelist  = &Resources->MeshFreelist;    \
  graphics                  *Graphics      =  Resources->Graphics;        \
  gpu_mapped_element_buffer *GpuMap        =  GetCurrentGpuMap(Graphics); \
  g_buffer_render_group     *gBuffer       =  Graphics->gBuffer;;         \
  camera                    *Camera        =  Graphics->Camera;




link_export b32
Bonsai_OnLibraryLoad(engine_resources *Resources)
{
  b32 Result = InitializeOpenglFunctions();
  Global_DebugStatePointer = Resources->DebugState;
  return Result;
}

link_export b32
Bonsai_Init(engine_resources *Resources)
{
  TIMED_FUNCTION();

  b32 Result = True;

  DEBUG_REGISTER_ARENA(TranArena, 0);

  memory_arena *BonsaiInitArena = AllocateArena();;
  DEBUG_REGISTER_ARENA(BonsaiInitArena, 0);

  Resources->Memory = BonsaiInitArena;

  Init_Global_QuadVertexBuffer();

  Resources->World = AllocateWorld();
  if (!Resources->World) { Error("Initializing World"); return False; }

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

  graphics *G = Resources->Graphics;
  G->GpuBufferWriteIndex = (G->GpuBufferWriteIndex + 1) % 2;

  UNPACK_ENGINE_RESOURCES();

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

#if 0 // BONSAI_DEBUG_SYSTEM_API
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

  UNPACK_ENGINE_RESOURCES(Resources);

  Bonsai_SimulateEntitiesAndWorld(Resources);
  Render_BufferGameGeometry(Resources);

  Resources->Graphics->gBuffer->ViewProjection =
    ProjectionMatrix(Camera, Plat->WindowWidth, Plat->WindowHeight) *
    ViewMatrix(World->ChunkDim, Camera);

#if 0 // BONSAI_DEBUG_SYSTEM_API
  if (GetDebugState)
  {
    DEBUG_COMPUTE_PICK_RAY(&gBuffer->ViewProjection);
    for (u32 ChunkIndex = 0;
        ChunkIndex < GetDebugState()->PickedChunkCount;
        ++ChunkIndex)
    {
      world_chunk *Chunk = GetDebugState()->PickedChunks[ChunkIndex];
      untextured_3d_geometry_buffer CopyDest = ReserveBufferSpace(&GpuMap->Buffer, VERTS_PER_AABB);
      u8 Color = GREEN;

      if (Chunk == GetDebugState()->HotChunk)
      {
        Color = PINK;
      }

      DEBUG_DrawChunkAABB(&CopyDest, Graphics, Chunk, World->ChunkDim, Color, 0.35f);
    }
  }
#endif

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
  gpu_mapped_element_buffer *GpuMap = GetCurrentGpuMap(Graphics);

  RenderGBuffer(GpuMap, Graphics);
  RenderAoTexture(AoGroup);
  DrawGBufferToFullscreenQuad(Plat, Graphics);

  BonsaiSwapBuffers(Resources->Os);

  b32 Result = True;
  return Result;
}

#endif
