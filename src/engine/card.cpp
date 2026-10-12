link_internal b32
InitCardRenderGroup(card_render_group *Group, gpu_heap_allocator *Heap, memory_arena *Memory, m4 *ViewProjection, texture *SpriteTextureArray)
{
  auto GL = GetGL();

  b32 Result = CompileShaderPair( &Group->Shader,
                                   CSz(BONSAI_SHADER_PATH "Card.vertexshader"),
                                   CSz(BONSAI_SHADER_PATH "Card.fragmentshader") );

  Group->Cards = Allocate(card, Memory, MAX_CARDS_PER_FRAME);

  Group->Shader.Uniforms = ShaderUniformBuffer(2, Memory);
  InitShaderUniform(&Group->Shader, 0, ViewProjection,      "ViewProjection");
  InitShaderUniform(&Group->Shader, 1, SpriteTextureArray,  "SpriteTextureArray");

  GL->GenBuffers(1, &Group->InstanceBuffer);

  // Unit quad, 2 triangles.  Positions are signed bytes (GL_BYTE) on the heap.
  Group->Quad = GpuHeapAllocate(Heap, 6);
  if (Group->Quad.SizeInElements == 6)
  {
    s8 Verts[6*3] =
    {
      -1, -1, 0,
       1, -1, 0,
      -1,  1, 0,
      -1,  1, 0,
       1, -1, 0,
       1,  1, 0,
    };

    s8 Normals[6*3] = {};
    matl Mat[6] = {};

    s64 v3u8Offset = s64(Group->Quad.BaseOffsetInElements*sizeof(v3_u8));
    s64 matlOffset = s64(Group->Quad.BaseOffsetInElements*sizeof(matl));

    GL->BindBuffer(GL_ARRAY_BUFFER, Heap->Storage.Handles.Handles[mesh_VertexHandle]);
    GL->BufferSubData(GL_ARRAY_BUFFER, v3u8Offset, sizeof(Verts), Verts);

    GL->BindBuffer(GL_ARRAY_BUFFER, Heap->Storage.Handles.Handles[mesh_NormalHandle]);
    GL->BufferSubData(GL_ARRAY_BUFFER, v3u8Offset, sizeof(Normals), Normals);

    GL->BindBuffer(GL_ARRAY_BUFFER, Heap->Storage.Handles.Handles[mesh_MatHandle]);
    GL->BufferSubData(GL_ARRAY_BUFFER, matlOffset, sizeof(Mat), Mat);

    GL->BindBuffer(GL_ARRAY_BUFFER, 0);
  }
  else
  {
    Error("Allocating card quad on the gpu heap");
    Result = False;
  }

  AssertNoGlErrors;
  return Result;
}

// Reserves a card for this frame.  Safe to call from any thread.  If the
// per-frame budget is exhausted a scratch card is returned, which is never drawn.
link_internal card *
DrawCard( engine_resources *Engine,
          cp P,
          v2 Dim,
          u32 SpriteIndex,
          u32 Flags = CardFlag_Billboard,
          v3 Right = WORLD_X,
          v3 Up = WORLD_Z )
{
  card_render_group *Group = &Engine->Graphics.Cards;

  u32 Index = 0;
  do
  {
    Index = Group->Count;
    if (Index >= MAX_CARDS_PER_FRAME)
    {
      AtomicIncrement(&Group->Dropped);
      Group->Scratch = {};
      return &Group->Scratch;
    }
  }
  while (!AtomicCompareExchange(&Group->Count, Index+1, Index));

  card *Result = Group->Cards + Index;
  *Result = {};
  Result->P           = P;
  Result->Right       = Right;
  Result->Up          = Up;
  Result->Dim         = Dim;
  Result->SpriteIndex = SpriteIndex;
  Result->Flags       = Flags;
  return Result;
}

link_internal void
ComputeCardBasis(card *Card, camera *Camera, v3 *Right, v3 *Up)
{
  *Right = Card->Right;
  *Up    = Card->Up;

  // Cards always face the camera
  {
    v3 Front = Camera->Front;

    v3 R = Cross(Front, WORLD_Z);
    if (Dot(R, R) > 0.0001f) { R = Normalize(R); }
    else                     { R = -1.f*Camera->Right; } // Looking straight up/down

    *Right = R;
    *Up = (Card->Flags & CardFlag_BillboardZ) ? WORLD_Z : Cross(R, Front);
  }
}

link_internal void
RenderCardsToGBuffer(engine_resources *Engine)
{
  TIMED_FUNCTION();

  graphics          *Graphics = &Engine->Graphics;
  card_render_group *Group    = &Graphics->Cards;

  u32 Count = Group->Count < MAX_CARDS_PER_FRAME ? Group->Count : MAX_CARDS_PER_FRAME;

  if (Group->Dropped)
  {
    SoftError("Dropped (%u) cards; MAX_CARDS_PER_FRAME (%u) exceeded.", Group->Dropped, MAX_CARDS_PER_FRAME);
    Group->Dropped = 0;
  }

  texture *Sprites = &Engine->Stdlib.Ui.SpriteTextureArray;

  if ( Count &&
       Group->Quad.SizeInElements &&
       Group->Shader.ID != INVALID_SHADER &&
       Sprites->ID != INVALID_TEXTURE_HANDLE )
  {
    auto GL = GetGL();
    camera *Camera = Graphics->Camera;

    card_instance *Instances = Allocate(card_instance, GetTranArena(), Count);

    RangeIterator_t(u32, CardIndex, Count)
    {
      card *Card = Group->Cards + CardIndex;
      card_instance *I = Instances + CardIndex;

      v3 Right, Up;
      ComputeCardBasis(Card, Camera, &Right, &Up);

      v3 Center = GetRenderP(Engine, Card->P)*GLOBAL_RENDER_SCALE_FACTOR;

      I->Center[0] = Center.x; I->Center[1] = Center.y; I->Center[2] = Center.z;
      I->SpriteIndex = r32(Card->SpriteIndex);
      I->Right[0] = Right.x; I->Right[1] = Right.y; I->Right[2] = Right.z;
      I->HalfWidth = Card->Dim.x*0.5f*GLOBAL_RENDER_SCALE_FACTOR;
      I->Up[0] = Up.x; I->Up[1] = Up.y; I->Up[2] = Up.z;
      I->HalfHeight = Card->Dim.y*0.5f*GLOBAL_RENDER_SCALE_FACTOR;
      I->Tint[0] = Card->Tint.x; I->Tint[1] = Card->Tint.y; I->Tint[2] = Card->Tint.z;
      I->AlphaCutoff = Card->AlphaCutoff;
    }

    GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, Group->InstanceBuffer);
    GL->BufferData(GL_SHADER_STORAGE_BUFFER, Cast(GLsizeiptr, sizeof(card_instance)*Count), Instances, GL_DYNAMIC_DRAW);
    GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Group->InstanceBuffer);

    GL->BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
    SetViewport(GetApplicationResolution(&Engine->Settings));
    UseShader(&Group->Shader);

    // Cards are two-sided
    GL->Disable(GL_CULL_FACE);

    GL->BindVertexArray(Graphics->GpuHeap.Storage.Handles.VAO);
    GL->DrawArraysInstanced(GL_TRIANGLES, s32(Group->Quad.BaseOffsetInElements), s32(Group->Quad.SizeInElements), s32(Count));
    GL->BindVertexArray(0);

    CleanupTextureBindings(&Group->Shader);
    GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    GL->Enable(GL_CULL_FACE);

    AssertNoGlErrors;
  }

  Group->Count = 0;
}
