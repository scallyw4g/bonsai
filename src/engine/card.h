// Cards are sprites that live in the 3D world.  They're depth tested, lit, and
// ambient-occluded like any other geometry because they're written to the
// gBuffer.
//
// All cards share a single unit quad that lives on the gpu heap, and are drawn
// with one instanced draw call.  Per-card data is uploaded to a storage buffer
// once per frame.  The sprite is selected out of the UI's SpriteTextureArray.
//
// Usage (call any time before Bonsai_Render, from any thread):
//
//   card *C = DrawCard(Engine, P, V2(8, 8), SpriteIndex);
//   C->Flags = CardFlag_BillboardZ;  // optional; stay upright
//   C->Tint  = V3(1, 0.8, 0.8);      // optional
//
// Cards are immediate-mode; they're consumed and cleared every frame.  Texels
// with alpha < AlphaCutoff are discarded.

#define MAX_CARDS_PER_FRAME (4096)

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

struct card_render_group
{
  shader Shader;

  card         *Cards;
  volatile u32  Count;
  volatile u32  Dropped; // Cards submitted past MAX_CARDS_PER_FRAME
  card          Scratch; // Handed out when the budget is exhausted

  gpu_heap_allocation Quad; // 6 verts, lives on Graphics->GpuHeap
  u32                 InstanceBuffer;
};

struct engine_resources;

link_internal b32  InitCardRenderGroup(card_render_group *Group, gpu_heap_allocator *Heap, memory_arena *Memory, m4 *ViewProjection, texture *SpriteTextureArray);
link_internal void RenderCardsToGBuffer(engine_resources *Engine);
