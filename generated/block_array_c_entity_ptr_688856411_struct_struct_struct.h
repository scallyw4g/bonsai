// src/engine/world_chunk.cpp:1:0





/* link_internal block_t * */
/* Allocate_(element_t.name)_block(memory_arena *Memory) */
/* { */
/*   block_t *Result = Allocate( block_t, Memory, 1); */
/*   return Result; */
/* } */

link_internal cs
CS( entity_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal entity_ptr 
Set( entity_ptr_block_array *Arr,
  entity_ptr Element,
  entity_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  entity_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( entity_ptr_block_array *Arr )
{
  entity_ptr_block  *NewBlock     = Allocate( entity_ptr_block , Arr->Memory,                 1);
  entity_ptr_block **NewBlockPtrs = Allocate( entity_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( entity_ptr_block_array *Array, entity_ptr_block_array_index Index)
{
  auto LastElement = GetPtr(Array, LastIndex(Array));
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( entity_ptr_block_array *Array, entity_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  entity_ptr Prev = {};

  entity_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    entity_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( entity_ptr_block_array *Array, entity_ptr Element )
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

link_internal entity_ptr_block_array_index
Find( entity_ptr_block_array *Array, entity_ptr Query)
{
  entity_ptr_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(entity_ptr_block_array_index *Index)
{
  entity_ptr_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal entity_ptr 
Push( entity_ptr_block_array *Array, entity_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  entity_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal entity_ptr 
Push( entity_ptr_block_array *Array )
{
  entity_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Shift( entity_ptr_block_array *Array, entity_ptr Element )
{
  Assert(Array->Memory);
  entity_ptr Prev = {};

  // Alocate a new thingy
  Push(Array);

  auto End = AtElements(Array);
  RangeIteratorReverse(Index, s32(End.Index))
  {
    auto E = GetPtr(Array, umm(Index));
    if (Prev) { *Prev = *E; }
    Prev = E;
  }

  *Prev = *Element;
}

