// callsite
// external/bonsai_stdlib/src/file.cpp:14:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2571:0




link_internal cs
CS( hot_reloadable_file_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal b32
ValidateBlocksMatchCapacity( hot_reloadable_file_block_array *Array )
{
  auto Cap = Capacity(Array).Index;
  auto Elements = AtElements(Array).Index;
  b32 Result = Elements <= Cap;
  Assert(Result);
  return Result;
}

link_internal hot_reloadable_file *
Set( hot_reloadable_file_block_array *Arr,
  hot_reloadable_file *Element,
  hot_reloadable_file_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  hot_reloadable_file_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( hot_reloadable_file_block_array *Arr )
{
  hot_reloadable_file_block  *NewBlock     = Allocate( hot_reloadable_file_block , Arr->Memory,                 1);
  hot_reloadable_file_block **NewBlockPtrs = Allocate( hot_reloadable_file_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  hot_reloadable_file *Prev = {};

  hot_reloadable_file_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    hot_reloadable_file *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
RemoveOrdered( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal hot_reloadable_file_block_array_index
Find( hot_reloadable_file_block_array *Array, hot_reloadable_file *Query)
{
  hot_reloadable_file_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(hot_reloadable_file_block_array_index *Index)
{
  hot_reloadable_file_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal hot_reloadable_file *
Push( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  hot_reloadable_file *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal hot_reloadable_file *
Push( hot_reloadable_file_block_array *Array )
{
  hot_reloadable_file Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index Index, hot_reloadable_file *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  hot_reloadable_file *Prev = Push(Array);

  auto Last = LastIndex(Array);

  RangeIteratorReverseRange(I, s32(Last.Index), s32(Index.Index))
  {
    auto E = GetPtr(Array, umm(I));
    *Prev = *E;
    Prev = E;
  }

  *Prev = *Element;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
Insert( hot_reloadable_file_block_array *Array, u32 Index, hot_reloadable_file *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */





link_internal hot_reloadable_file *
Pop( hot_reloadable_file_block_array *Array )
{
  hot_reloadable_file * Result = {};

  if (auto P = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    Result = P;
  }
  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal void
ClearList( hot_reloadable_file_block_array *Array )
{
  IterateOver(Array, Element, ElementIndex)
  {
    *Element = {};
  }

  Array->ElementCount = 0;
}


