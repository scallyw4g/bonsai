// src/engine/world_chunk.cpp:4:0





link_internal world_chunk_ptr_block *
Allocate_world_chunk_ptr_block(memory_arena *Memory)
{
  world_chunk_ptr_block *Result = Allocate( world_chunk_ptr_block, Memory, 1);
  Result->Elements = Allocate( world_chunk_ptr, Memory, 32);
  return Result;
}

link_internal cs
CS( world_chunk_ptr_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal world_chunk_ptr 
Set( world_chunk_ptr_block_array *Arr,
  world_chunk_ptr Element,
  world_chunk_ptr_block_array_index Index )
{
  world_chunk_ptr Result = {};
  if (Index.Block)
  {
    world_chunk_ptr *Slot = &Index.Block->Elements[Index.ElementIndex];
    *Slot = Element;

    Result = *Slot;
  }

  return Result;
}

link_internal void
RemoveOrdered( world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index Index)
{
  NotImplemented;
}

link_internal void
RemoveOrdered( world_chunk_ptr_block_array *Array, world_chunk_ptr *Element )
{
  NotImplemented;
}

link_internal void
RemoveUnordered( world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index Index)
{
  world_chunk_ptr_block_array_index LastI = LastIndex(Array);

  world_chunk_ptr Element = GetPtr(Array, Index);
  world_chunk_ptr LastElement = GetPtr(Array, LastI);

  Set(Array, LastElement, Index);

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // TODO(Jesse): There's obviously a way better way to do this ..
    auto AtE = AtElements(Array);
    s32 Count = s32(GetIndex(&AtE));

    if (Count == 0)
    {
      // Nothing to be done, we've popping the last thing off the array
      Assert(Index.Block == Array->First);
      Assert(Index.Block == Array->Current);
      Assert(Index.BlockIndex == 0);
      Assert(Index.ElementIndex == 0);
    }
    else
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
}

link_internal world_chunk_ptr_block_array_index
Find( world_chunk_ptr_block_array *Array, world_chunk_ptr Query)
{
  world_chunk_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
  IterateOver(Array, E, Index)
  {
    if ( E == Query)
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

link_internal b32
IsValid(world_chunk_ptr_block_array_index *Index)
{
  world_chunk_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal world_chunk_ptr *
Push( world_chunk_ptr_block_array *Array, world_chunk_ptr *Element)
{
  Assert(Array->Memory);

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

link_internal world_chunk_ptr *
Push( world_chunk_ptr_block_array *Array )
{
  world_chunk_ptr Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

