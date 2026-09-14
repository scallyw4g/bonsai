// callsite
// src/engine/work_queue.h:250:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2519:0




link_internal cs
CS( work_queue_entry_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal work_queue_entry *
Set( work_queue_entry_block_array *Arr,
  work_queue_entry *Element,
  work_queue_entry_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  work_queue_entry_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( work_queue_entry_block_array *Arr )
{
  work_queue_entry_block  *NewBlock     = Allocate( work_queue_entry_block , Arr->Memory,                 1);
  work_queue_entry_block **NewBlockPtrs = Allocate( work_queue_entry_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( work_queue_entry_block_array *Array, work_queue_entry_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( work_queue_entry_block_array *Array, work_queue_entry_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  work_queue_entry *Prev = {};

  work_queue_entry_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    work_queue_entry *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( work_queue_entry_block_array *Array, work_queue_entry *Element )
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

link_internal work_queue_entry_block_array_index
Find( work_queue_entry_block_array *Array, work_queue_entry *Query)
{
  work_queue_entry_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if ( AreEqual(E, Query) )
    {
      Result = Index;
      break;
    }
  }
  return Result;
}



link_internal b32
IsValid(work_queue_entry_block_array_index *Index)
{
  work_queue_entry_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal work_queue_entry *
Push( work_queue_entry_block_array *Array, work_queue_entry *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  work_queue_entry *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal work_queue_entry *
Push( work_queue_entry_block_array *Array )
{
  work_queue_entry Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( work_queue_entry_block_array *Array, work_queue_entry_block_array_index Index, work_queue_entry *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  work_queue_entry *Prev = Push(Array);

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
Insert( work_queue_entry_block_array *Array, u32 Index, work_queue_entry *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( work_queue_entry_block_array *Array, work_queue_entry *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal work_queue_entry *
Pop( work_queue_entry_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


