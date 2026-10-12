link_internal b32
InitCardRenderGroup(card_render_group *Group, memory_arena *Memory, m4 *ViewProjection)
{
  b32 Result = CompileShaderPair( &Group->Shader,
                                   CSz(BONSAI_SHADER_PATH "Card.vertexshader"),
                                   CSz(BONSAI_SHADER_PATH "Card.fragmentshader") );

  Group->Cards = Allocate(card, Memory, MAX_CARDS_PER_FRAME);

  Group->Shader.Uniforms = ShaderUniformBuffer(10, Memory);

  InitShaderUniform(&Group->Shader, 0, ViewProjection,       "ViewProjection");
  InitShaderUniform(&Group->Shader, 1, &Group->Center,       "CardCenter");
  InitShaderUniform(&Group->Shader, 2, &Group->Right,        "CardRight");
  InitShaderUniform(&Group->Shader, 3, &Group->Up,           "CardUp");
  InitShaderUniform(&Group->Shader, 4, &Group->HalfDim,      "CardHalfDim");
  InitShaderUniform(&Group->Shader, 5, &Group->UvMin,        "CardUvMin");
  InitShaderUniform(&Group->Shader, 6, &Group->UvMax,        "CardUvMax");
  InitShaderUniform(&Group->Shader, 7, &Group->Texture,      "CardTexture");
  InitShaderUniform(&Group->Shader, 8, &Group->Tint,         "CardTint");
  InitShaderUniform(&Group->Shader, 9, &Group->AlphaCutoff,  "CardAlphaCutoff");

  return Result;
}

// Reserves a card for this frame.  Safe to call from any thread.  If the
// per-frame budget is exhausted a scratch card is returned, which is never drawn.
link_internal card *
DrawCard( engine_resources *Engine,
          cp P,
          v2 Dim,
          texture *Texture,
          u32 Flags = CardFlag_None,
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
  Result->P       = P;
  Result->Right   = Right;
  Result->Up      = Up;
  Result->Dim     = Dim;
  Result->Texture = Texture;
  Result->Flags   = Flags;
  return Result;
}

link_internal void
ComputeCardBasis(card *Card, camera *Camera, v3 *Right, v3 *Up)
{
  *Right = Card->Right;
  *Up    = Card->Up;

  if (Card->Flags & (CardFlag_Billboard|CardFlag_BillboardZ))
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

  if (Count && Group->Shader.ID != INVALID_SHADER)
  {
    auto GL = GetGL();
    camera *Camera = Graphics->Camera;

    GL->BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
    SetViewport(GetApplicationResolution(&Engine->Settings));
    GL->UseProgram(Group->Shader.ID);

    // Cards are two-sided
    GL->Disable(GL_CULL_FACE);

    RangeIterator_t(u32, CardIndex, Count)
    {
      card *Card = Group->Cards + CardIndex;

      if (Card->Texture == 0 || Card->Texture->ID == INVALID_TEXTURE_HANDLE) { continue; }

      v3 Right, Up;
      ComputeCardBasis(Card, Camera, &Right, &Up);

      Group->Center      = GetRenderP(Engine, Card->P);
      Group->Right       = Right;
      Group->Up          = Up;
      Group->HalfDim     = V2(Card->Dim.x*0.5f, Card->Dim.y*0.5f);
      Group->UvMin       = Card->UvMin;
      Group->UvMax       = Card->UvMax;
      Group->Tint        = Card->Tint;
      Group->AlphaCutoff = Card->AlphaCutoff;
      Group->Texture     = *Card->Texture;

      BindShaderUniforms(&Group->Shader);
      RenderQuad();
    }

    CleanupTextureBindings(&Group->Shader);
    GL->Enable(GL_CULL_FACE);

    AssertNoGlErrors;
  }

  Group->Count = 0;
}
