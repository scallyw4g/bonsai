// src/engine/world_chunk.cpp:2:0


link_internal world_chunk_ptr_block*
Allocate_world_chunk_ptr_block(memory_arena *Memory)
{
  world_chunk_ptr_block *Result = Allocate(world_chunk_ptr_block, Memory, 1);
  Result->Elements = Allocate(world_chunk_ptr, Memory, 32);
  return Result;
}

link_internal cs
CS(world_chunk_ptr_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index Index)
{
  world_chunk_ptr_block_array_index LastI = LastIndex(Array);

  world_chunk_ptr *Element = GetPtr(Array, Index);
  world_chunk_ptr *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    world_chunk_ptr_block *Current = Array->First;
    world_chunk_ptr_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal world_chunk_ptr *
Push(world_chunk_ptr_block_array *Array, world_chunk_ptr *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_world_chunk_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 32)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      world_chunk_ptr_block *Next = Allocate_world_chunk_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  world_chunk_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

