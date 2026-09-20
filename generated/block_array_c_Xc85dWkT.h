// callsite
// external/bonsai_stdlib/src/primitive_containers.cpp:12:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2531:0




link_internal cs
CS( base_ptr_relative_edit_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal base_ptr_relative_edit *
Set( base_ptr_relative_edit_block_array *Arr,
  base_ptr_relative_edit *Element,
  base_ptr_relative_edit_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  base_ptr_relative_edit_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( base_ptr_relative_edit_block_array *Arr )
{
  base_ptr_relative_edit_block  *NewBlock     = Allocate( base_ptr_relative_edit_block , Arr->Memory,                 1);
  base_ptr_relative_edit_block **NewBlockPtrs = Allocate( base_ptr_relative_edit_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  base_ptr_relative_edit *Prev = {};

  base_ptr_relative_edit_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    base_ptr_relative_edit *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit *Element )
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

link_internal base_ptr_relative_edit_block_array_index
Find( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit *Query)
{
  base_ptr_relative_edit_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(base_ptr_relative_edit_block_array_index *Index)
{
  base_ptr_relative_edit_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal base_ptr_relative_edit *
Push( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  base_ptr_relative_edit *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal base_ptr_relative_edit *
Push( base_ptr_relative_edit_block_array *Array )
{
  base_ptr_relative_edit Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit_block_array_index Index, base_ptr_relative_edit *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  base_ptr_relative_edit *Prev = Push(Array);

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
Insert( base_ptr_relative_edit_block_array *Array, u32 Index, base_ptr_relative_edit *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( base_ptr_relative_edit_block_array *Array, base_ptr_relative_edit *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal base_ptr_relative_edit *
Pop( base_ptr_relative_edit_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


