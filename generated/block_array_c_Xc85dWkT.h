// callsite
// external/bonsai_stdlib/src/primitive_containers.cpp:12:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2519:0




link_internal cs
CS( primitive_value_changed_record_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal primitive_value_changed_record *
Set( primitive_value_changed_record_block_array *Arr,
  primitive_value_changed_record *Element,
  primitive_value_changed_record_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  primitive_value_changed_record_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( primitive_value_changed_record_block_array *Arr )
{
  primitive_value_changed_record_block  *NewBlock     = Allocate( primitive_value_changed_record_block , Arr->Memory,                 1);
  primitive_value_changed_record_block **NewBlockPtrs = Allocate( primitive_value_changed_record_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( primitive_value_changed_record_block_array *Array, primitive_value_changed_record_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( primitive_value_changed_record_block_array *Array, primitive_value_changed_record_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  primitive_value_changed_record *Prev = {};

  primitive_value_changed_record_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    primitive_value_changed_record *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( primitive_value_changed_record_block_array *Array, primitive_value_changed_record *Element )
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

link_internal primitive_value_changed_record_block_array_index
Find( primitive_value_changed_record_block_array *Array, primitive_value_changed_record *Query)
{
  primitive_value_changed_record_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(primitive_value_changed_record_block_array_index *Index)
{
  primitive_value_changed_record_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal primitive_value_changed_record *
Push( primitive_value_changed_record_block_array *Array, primitive_value_changed_record *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  primitive_value_changed_record *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal primitive_value_changed_record *
Push( primitive_value_changed_record_block_array *Array )
{
  primitive_value_changed_record Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( primitive_value_changed_record_block_array *Array, primitive_value_changed_record_block_array_index Index, primitive_value_changed_record *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  primitive_value_changed_record *Prev = Push(Array);

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
Insert( primitive_value_changed_record_block_array *Array, u32 Index, primitive_value_changed_record *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( primitive_value_changed_record_block_array *Array, primitive_value_changed_record *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal primitive_value_changed_record *
Pop( primitive_value_changed_record_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


