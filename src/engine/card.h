// Cards are sprites that live in the 3D world.  They're depth tested, lit, and
// ambient-occluded like any other geometry because they're written to the
// gBuffer.
//
// All cards share a single unit quad that lives on the gpu heap, and are drawn
// with instanced drawing.  Per-card data is uploaded to a storage buffer.
// The sprite is selected out of the UI's SpriteTextureArray.
//
// Retained mode usage:
//
//   card_block *Block = ReserveCardBlock(Engine);
//   card *C = Block->Elements + Block->Count++;
//   C->P           = P;
//   C->Dim         = V2(8, 8);
//   C->SpriteIndex = SpriteIndex;
//   C->Flags       = CardFlag_BillboardZ;  // optional; stay upright
//   C->Tint        = V3(1, 0.8, 0.8);      // optional
//
// Releasing blocks:
//
//   ReleaseCardBlock(Engine, Block);
//
// Texels with alpha < AlphaCutoff are discarded.

#define CARDS_PER_BLOCK (4096)

enum card_flags
{
  CardFlag_None       = 0,
  CardFlag_Billboard  = (1 << 0), // Faces the camera (always applied)
  CardFlag_BillboardZ = (1 << 1), // Keep the card upright; rotate only about the world Z axis
};

struct card
{
  cp  P;      // Center of the card
  v3  Right;  // Unused; cards always face the camera.
  v3  Up;     // Unused; cards always face the camera.
  v2  Dim;    // Full width/height in world units

  u32 SpriteIndex; // Slice of Ui.SpriteTextureArray to draw

  v3  Tint = V3(1);
  r32 AlphaCutoff = 0.5f;

  u32 Flags;
};

// NOTE(Jesse): Must match card_instance in Card.vertexshader (std430)
struct card_instance
{
  r32 Center[3];
  r32 SpriteIndex;
  r32 Right[3];
  r32 HalfWidth;
  r32 Up[3];
  r32 HalfHeight;
  r32 Tint[3];
  r32 AlphaCutoff;
};
CAssert(sizeof(card_instance) == 64);

struct card_block
{
  card        Elements[CARDS_PER_BLOCK];
  u32         Count;
  card_block *Next;
  b32         Active;
};

struct card_block_array
{
  card_block **BlockPtrs;
  u32          BlockCount;
  u32          ElementCount;
  memory_arena *Memory;
};

struct card_render_group
{
  shader Shader;

  bonsai_futex     Lock;
  card_block_array BlockArray;
  card_block      *FreeList;

  gpu_heap_allocation Quad; // 6 verts, lives on Graphics->GpuHeap
  u32                 InstanceBuffer;
};

struct engine_resources;
struct graphics;

link_internal b32         InitCardRenderGroup(card_render_group *Group, gpu_heap_allocator *Heap, memory_arena *Memory, m4 *ViewProjection, texture *SpriteTextureArray);
link_internal card_block* ReserveCardBlock(card_render_group *Group);
link_internal card_block* ReserveCardBlock(engine_resources *Engine);
link_internal card_block* ReserveCardBlock(graphics *Graphics);

link_internal void        ReleaseCardBlock(card_render_group *Group, card_block *Block);
link_internal void        ReleaseCardBlock(engine_resources *Engine, card_block *Block);
link_internal void        ReleaseCardBlock(graphics *Graphics, card_block *Block);

link_inline void
FreeCardBlock(card_render_group *Group, card_block *Block)
{
  ReleaseCardBlock(Group, Block);
}

link_inline void
FreeCardBlock(engine_resources *Engine, card_block *Block)
{
  ReleaseCardBlock(Engine, Block);
}

link_inline void
FreeCardBlock(graphics *Graphics, card_block *Block)
{
  ReleaseCardBlock(Graphics, Block);
}

link_internal void RenderCardsToGBuffer(engine_resources *Engine);
