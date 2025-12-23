// callsite
// src/engine/world_chunk.cpp:19:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2502:0




link_internal cs
CS( world_chunk_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal world_chunk_ptr 
Set( world_chunk_ptr_block_array *Arr,
  world_chunk_ptr Element,
  world_chunk_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  world_chunk_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 32;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( world_chunk_ptr_block_array *Arr )
{
  world_chunk_ptr_block  *NewBlock     = Allocate( world_chunk_ptr_block , Arr->Memory,                 1);
  world_chunk_ptr_block **NewBlockPtrs = Allocate( world_chunk_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  world_chunk_ptr Prev = {};

  world_chunk_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    world_chunk_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( world_chunk_ptr_block_array *Array, world_chunk_ptr Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
}

link_internal world_chunk_ptr_block_array_index
Find( world_chunk_ptr_block_array *Array, world_chunk_ptr Query)
{
  world_chunk_ptr_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if ( E == Query )
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
  world_chunk_ptr_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal world_chunk_ptr 
Push( world_chunk_ptr_block_array *Array, world_chunk_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  world_chunk_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal world_chunk_ptr 
Push( world_chunk_ptr_block_array *Array )
{
  world_chunk_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Insert( world_chunk_ptr_block_array *Array, world_chunk_ptr_block_array_index Index, world_chunk_ptr Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  world_chunk_ptr Prev = Push(Array);

  auto Last = LastIndex(Array);

  RangeIteratorReverseRange(I, s32(Last.Index), s32(Index.Index))
  {
    auto E = GetPtr(Array, umm(I));
    *Prev = *E;
    Prev = E;
  }

  *Prev = *Element;
}

link_internal void
Insert( world_chunk_ptr_block_array *Array, u32 Index, world_chunk_ptr Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( world_chunk_ptr_block_array *Array, world_chunk_ptr Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal world_chunk_ptr 
Pop( world_chunk_ptr_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


