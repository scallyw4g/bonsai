struct entity_block_index
{
  u32 BlockIndex;
  u32 ElementIndex;
};

struct entity_block
{
  u32 At;
  entity *Elements;
  entity_block *Next;
};

link_internal entity_block*
Allocate_entity_block(memory_arena *Memory)
{
  entity_block *Result = Allocate(entity_block, Memory, 1);
  Result->Elements = Allocate(entity, Memory, 4);
  return Result;
}

struct entity_block_array
{
  entity_block First;
  entity_block *Current;

  memory_arena *Memory;
};

link_internal void
Push(entity_block_array *Array, entity *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_entity_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 4)
  {
    entity_block *Next = Allocate_entity_block(Array->Memory);
    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  Array->Current->Elements[Array->Current->At++] = *Element;
}

link_internal entity *
GetPtr(entity_block *Block, umm Index)
{
  entity *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal u32
AtElements(entity_block *Block)
{
  return Block->At;
}


