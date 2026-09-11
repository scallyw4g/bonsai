// callsite
// src/engine/editor.cpp:1:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2519:0




link_internal cs
CS( undo_record_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal undo_record *
Set( undo_record_block_array *Arr,
  undo_record *Element,
  undo_record_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  undo_record_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 64;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( undo_record_block_array *Arr )
{
  undo_record_block  *NewBlock     = Allocate( undo_record_block , Arr->Memory,                 1);
  undo_record_block **NewBlockPtrs = Allocate( undo_record_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( undo_record_block_array *Array, undo_record_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( undo_record_block_array *Array, undo_record_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  undo_record *Prev = {};

  undo_record_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    undo_record *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( undo_record_block_array *Array, undo_record *Element )
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

link_internal undo_record_block_array_index
Find( undo_record_block_array *Array, undo_record *Query)
{
  undo_record_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(undo_record_block_array_index *Index)
{
  undo_record_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal undo_record *
Push( undo_record_block_array *Array, undo_record *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  undo_record *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal undo_record *
Push( undo_record_block_array *Array )
{
  undo_record Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( undo_record_block_array *Array, undo_record_block_array_index Index, undo_record *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  undo_record *Prev = Push(Array);

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
Insert( undo_record_block_array *Array, u32 Index, undo_record *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( undo_record_block_array *Array, undo_record *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal undo_record *
Pop( undo_record_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


