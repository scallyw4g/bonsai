// callsite
// external/bonsai_stdlib/src/counted_string.cpp:930:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2502:0




link_internal cs
CS( counted_string_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal counted_string *
Set( counted_string_block_array *Arr,
  counted_string *Element,
  counted_string_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  counted_string_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 32;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( counted_string_block_array *Arr )
{
  counted_string_block  *NewBlock     = Allocate( counted_string_block , Arr->Memory,                 1);
  counted_string_block **NewBlockPtrs = Allocate( counted_string_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( counted_string_block_array *Array, counted_string_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( counted_string_block_array *Array, counted_string_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  counted_string *Prev = {};

  counted_string_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    counted_string *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( counted_string_block_array *Array, counted_string *Element )
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

link_internal counted_string_block_array_index
Find( counted_string_block_array *Array, counted_string *Query)
{
  counted_string_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(counted_string_block_array_index *Index)
{
  counted_string_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal counted_string *
Push( counted_string_block_array *Array, counted_string *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  counted_string *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal counted_string *
Push( counted_string_block_array *Array )
{
  counted_string Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( counted_string_block_array *Array, counted_string_block_array_index Index, counted_string *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  counted_string *Prev = Push(Array);

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
Insert( counted_string_block_array *Array, u32 Index, counted_string *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( counted_string_block_array *Array, counted_string *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal counted_string *
Pop( counted_string_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


