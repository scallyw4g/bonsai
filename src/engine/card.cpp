link_internal card_block_array
CardBlockArray(memory_arena *Memory)
{
  card_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal card_block *
NewBlock(card_block_array *Arr)
{
  card_block  *NewBlock     = Allocate(card_block, Arr->Memory, 1);
  card_block **NewBlockPtrs = Allocate(card_block*, Arr->Memory, Arr->BlockCount + 1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;

  return NewBlock;
}

link_internal b32
InitCardRenderGroup(card_render_group *Group, gpu_heap_allocator *Heap, memory_arena *Memory, m4 *ViewProjection, texture *SpriteTextureArray)
{
  auto GL = GetGL();

  b32 Result = CompileNewShader( &Group->Shader,
                                  CSz("Cards"),
                                   CSzs(BONSAI_SHADER_PATH "Card.vertexshader"),
                                   CSzs(BONSAI_SHADER_PATH "Card.fragmentshader"),
                                   Memory
                                 );

  Group->BlockArray = CardBlockArray(Memory);
  Group->FreeList = 0;
  Group->Lock = {};

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

link_internal card_block *
ReserveCardBlock(card_render_group *Group)
{
  AcquireFutex(&Group->Lock);

  card_block *Result = 0;
  if (Group->FreeList)
  {
    Result = Group->FreeList;
    Group->FreeList = Result->Next;
  }
  else
  {
    Result = NewBlock(&Group->BlockArray);
  }

  Assert(Result);
  *Result = {};
  Result->Active = True;

  RangeIterator_t(u32, CardIndex, CARDS_PER_BLOCK)
  {
    card *C = Result->Elements + CardIndex;
    C->Tint = V3(1.f);
    C->AlphaCutoff = 0.5f;
  }

  ReleaseFutex(&Group->Lock);
  return Result;
}

link_internal card_block *
ReserveCardBlock(engine_resources *Engine)
{
  return ReserveCardBlock(&Engine->Graphics.Cards);
}

link_internal card_block *
ReserveCardBlock(graphics *Graphics)
{
  return ReserveCardBlock(&Graphics->Cards);
}

link_internal void
ReleaseCardBlock(card_render_group *Group, card_block *Block)
{
  if (Block)
  {
    AcquireFutex(&Group->Lock);
    Assert(Block->Active);
    *Block = {};
    Block->Active = False;
    Block->Next = Group->FreeList;
    Group->FreeList = Block;
    ReleaseFutex(&Group->Lock);
  }
}

link_internal void
ReleaseCardBlock(engine_resources *Engine, card_block *Block)
{
  ReleaseCardBlock(&Engine->Graphics.Cards, Block);
}

link_internal void
ReleaseCardBlock(graphics *Graphics, card_block *Block)
{
  ReleaseCardBlock(&Graphics->Cards, Block);
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

  texture *Sprites = &Engine->Stdlib.Ui.SpriteTextureArray;

  if ( Group->Quad.SizeInElements &&
       Group->Shader.ID != INVALID_SHADER &&
       Sprites->ID != INVALID_TEXTURE_HANDLE )
  {
    AcquireFutex(&Group->Lock);

    u32 TotalCardsToDraw = 0;
    RangeIterator_t(u32, BlockIndex, Group->BlockArray.BlockCount)
    {
      card_block *Block = Group->BlockArray.BlockPtrs[BlockIndex];
      if (Block && Block->Active)
      {
        TotalCardsToDraw += Block->Count;
      }
    }

    if (TotalCardsToDraw > 0)
    {
      auto GL = GetGL();
      camera *Camera = Graphics->Camera;

      card_instance *Instances = Allocate(card_instance, GetTranArena(), TotalCardsToDraw);

      u32 InstanceIndex = 0;
      RangeIterator_t(u32, BlockIndex, Group->BlockArray.BlockCount)
      {
        card_block *Block = Group->BlockArray.BlockPtrs[BlockIndex];
        if (Block && Block->Active)
        {
          RangeIterator_t(u32, CardIndex, Block->Count)
          {
            card *Card = Block->Elements + CardIndex;
            /* if (Card->Dim.x > 0.f && Card->Dim.y > 0.f) */
            {
              card_instance *I = Instances + InstanceIndex++;

              v3 Right, Up;
              ComputeCardBasis(Card, Camera, &Right, &Up);

              v3 Center = GetRenderP(Engine, Card->P)*GLOBAL_RENDER_SCALE_FACTOR;

              v3 Tint = LengthSq(Card->Tint) > 0.f ? Card->Tint : V3(1.f);
              r32 AlphaCutoff = Card->AlphaCutoff > 0.f ? Card->AlphaCutoff : 0.5f;

              I->Center[0] = Center.x; I->Center[1] = Center.y; I->Center[2] = Center.z;
              I->SpriteIndex = r32(Card->SpriteIndex);
              I->Right[0] = Right.x; I->Right[1] = Right.y; I->Right[2] = Right.z;
              I->HalfWidth = Card->Dim.x*0.5f*GLOBAL_RENDER_SCALE_FACTOR;
              I->Up[0] = Up.x; I->Up[1] = Up.y; I->Up[2] = Up.z;
              I->HalfHeight = Card->Dim.y*0.5f*GLOBAL_RENDER_SCALE_FACTOR;
              I->Tint[0] = Tint.x; I->Tint[1] = Tint.y; I->Tint[2] = Tint.z;
              I->AlphaCutoff = AlphaCutoff;
            }
          }
        }
      }
      Assert(InstanceIndex == TotalCardsToDraw);

      ReleaseFutex(&Group->Lock);

      GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, Group->InstanceBuffer);
      GL->BufferData(GL_SHADER_STORAGE_BUFFER, Cast(GLsizeiptr, sizeof(card_instance)*TotalCardsToDraw), Instances, GL_DYNAMIC_DRAW);
      GL->BindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Group->InstanceBuffer);

      GL->BindFramebuffer(GL_FRAMEBUFFER, Graphics->gBuffer->FBO.ID);
      SetViewport(GetApplicationResolution(&Engine->Settings));
      UseShader(&Group->Shader);

      // Cards are two-sided
      GL->Disable(GL_CULL_FACE);

      GL->BindVertexArray(Graphics->GpuHeap.Storage.Handles.VAO);
      GL->DrawArraysInstanced(GL_TRIANGLES, s32(Group->Quad.BaseOffsetInElements), s32(Group->Quad.SizeInElements), s32(TotalCardsToDraw));
      GL->BindVertexArray(0);

      CleanupTextureBindings(&Group->Shader);
      GL->BindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
      GL->Enable(GL_CULL_FACE);

      AssertNoGlErrors;
    }
    else
    {
      ReleaseFutex(&Group->Lock);
    }
  }
}
