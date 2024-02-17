
link_internal b32
InitEngineResources(engine_resources *Engine)
{
  b32 Result = True;

  platform *Plat = &Engine->Stdlib.Plat;

  memory_arena *WorldAndEntityArena = AllocateArena(Megabytes(256));
  DEBUG_REGISTER_ARENA(WorldAndEntityArena, 0);

  Engine->GameMemory = AllocateArena();
  Engine->WorldUpdateMemory = AllocateArena();

  Engine->Heap = InitHeap(Gigabytes(2)); // TODO(Jesse): Is this actually used?
  Engine->AssetMemory = InitHeap(Gigabytes(1));

  Init_Global_QuadVertexBuffer();

  Engine->World = Allocate(world, WorldAndEntityArena, 1);
  if (!Engine->World) { Error("Allocating World"); Result = False; }

  Engine->Graphics = GraphicsInit(AllocateArena());
  if (!Engine->Graphics) { Error("Initializing Graphics"); Result = False; }

  {
    memory_arena *UiMemory = AllocateArena();
    InitRenderer2D(&Engine->Ui, &Engine->Heap, UiMemory, &Plat->MouseP, &Plat->MouseDP, &Plat->ScreenDim, &Plat->Input);
    /* Engine->UiSpriteTexture = LoadBitmap("assets/mystic_rpg_icon_pack/Sprites/300%/Tool_6.bmp", UiMemory); */
    /* Engine->UiSpriteTexture = LoadBitmap("assets/mystic_rpg_icon_pack/Sprites/300%/Tool_13.bmp", UiMemory); */

    bitmap_block_array Bitmaps = {};
    LoadBitmapsFromFolder(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps);
    LoadBitmapsFromFolder(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps);
    Engine->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapArray(&Bitmaps, V2i(64,64), UiMemory);

    /* Engine->UiSpriteTexture = LoadBitmap("assets/mystic_rpg_icon_pack/Sprites/300%/Tool_20.bmp", UiMemory); */
    /* Engine->UiSpriteTexture = LoadBitmap("assets/mystic_rpg_icon_pack/Sprites/300%/Inventory_0.bmp", UiMemory); */
    /* Engine->UiSpriteTexture = LoadBitmap("assets/test.bmp", UiMemory); */
    /* Engine->UiSpriteTexture = LoadBitmap("assets/test_1.bmp", UiMemory); */
    /* Engine->UiSpriteTexture = LoadBitmap("assets/test_2.bmp", UiMemory); */
  }

  Engine->EntityTable = AllocateEntityTable(WorldAndEntityArena, TOTAL_ENTITY_COUNT);
  return Result;
}

link_internal b32
InitEngineDebug(engine_debug *Debug)
{
  b32 Result = True;

  Debug->Memory = AllocateArena();

  {
    Debug->WorldEditDebugThumb.Texture = MakeTexture_RGB(V2i(1024,1024), 0, Debug->Memory, CSz("WorldEditDebugTexture"));
    StandardCamera(&Debug->WorldEditDebugThumb.Camera, 10000.f, 1000.f, 30.f, {});
    AllocateMesh(&Debug->WorldEditDebugMesh,  u32(Kilobytes(64*16)), Debug->Memory);
  }

  return Result;
}
