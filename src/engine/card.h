// Cards are textured quads that live in the 3D world.  They're depth tested,
// lit, and ambient-occluded like any other geometry because they're written
// to the gBuffer.
//
// Usage (call any time before Bonsai_Render, from any thread):
//
//   card *C = DrawCard(Engine, P, V2(8, 12), &MyTexture);
//   C->Flags = CardFlag_BillboardZ;          // optional
//   C->UvMin = V2(0.5, 0); C->UvMax = V2(1); // optional, sub-rect of an atlas
//
// Cards are immediate-mode; they're consumed and cleared every frame.
// The texture must outlive the frame.  UV (0,0) is the bottom-left corner, so
// the first row of texel data is the bottom of the card.  Texels with
// alpha < AlphaCutoff are discarded.

#define MAX_CARDS_PER_FRAME (4096)

enum card_flags
{
  CardFlag_None       = 0,
  CardFlag_Billboard  = (1 << 0), // Always faces the camera
  CardFlag_BillboardZ = (1 << 1), // Faces the camera, rotating only about the world Z axis
};

struct card
{
  cp  P;      // Center of the card
  v3  Right;  // Unit vectors spanning the card plane.  Facing is Cross(Right, Up).
  v3  Up;     // Ignored if a Billboard flag is set.
  v2  Dim;    // Full width/height in world units

  v2  UvMin = V2(0);
  v2  UvMax = V2(1);

  v3  Tint = V3(1);
  r32 AlphaCutoff = 0.5f;

  texture *Texture;
  u32      Flags;
};

struct card_render_group
{
  shader Shader;

  card         *Cards;
  volatile u32  Count;
  volatile u32  Dropped; // Cards submitted past MAX_CARDS_PER_FRAME
  card          Scratch; // Handed out when the budget is exhausted

  // NOTE(Jesse): Backing storage for the per-card shader uniforms.
  v3      Center;
  v3      Right;
  v3      Up;
  v2      HalfDim;
  v2      UvMin;
  v2      UvMax;
  v3      Tint;
  r32     AlphaCutoff;
  texture Texture;
};

struct engine_resources;

link_internal b32  InitCardRenderGroup(card_render_group *Group, memory_arena *Memory, m4 *ViewProjection);
link_internal void RenderCardsToGBuffer(engine_resources *Engine);
