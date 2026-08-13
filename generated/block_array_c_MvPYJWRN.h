// callsite
// external/bonsai_stdlib/src/heap_allocator.cpp:1:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2519:0




link_internal cs
CS( heap_allocation_block_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal heap_allocation_block *
Set( heap_allocation_block_block_array *Arr,
  heap_allocation_block *Element,
  heap_allocation_block_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  heap_allocation_block_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 256;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( heap_allocation_block_block_array *Arr )
{
  heap_allocation_block_block  *NewBlock     = Allocate( heap_allocation_block_block , Arr->Memory,                 1);
  heap_allocation_block_block **NewBlockPtrs = Allocate( heap_allocation_block_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( heap_allocation_block_block_array *Array, heap_allocation_block_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( heap_allocation_block_block_array *Array, heap_allocation_block_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  heap_allocation_block *Prev = {};

  heap_allocation_block_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    heap_allocation_block *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( heap_allocation_block_block_array *Array, heap_allocation_block *Element )
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

link_internal heap_allocation_block_block_array_index
Find( heap_allocation_block_block_array *Array, heap_allocation_block *Query)
{
  heap_allocation_block_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(heap_allocation_block_block_array_index *Index)
{
  heap_allocation_block_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal heap_allocation_block *
Push( heap_allocation_block_block_array *Array, heap_allocation_block *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  heap_allocation_block *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal heap_allocation_block *
Push( heap_allocation_block_block_array *Array )
{
  heap_allocation_block Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( heap_allocation_block_block_array *Array, heap_allocation_block_block_array_index Index, heap_allocation_block *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  heap_allocation_block *Prev = Push(Array);

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
Insert( heap_allocation_block_block_array *Array, u32 Index, heap_allocation_block *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( heap_allocation_block_block_array *Array, heap_allocation_block *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal heap_allocation_block *
Pop( heap_allocation_block_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


