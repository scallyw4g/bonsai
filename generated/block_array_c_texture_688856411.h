// callsite
// external/bonsai_stdlib/src/texture.cpp:5:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2502:0




link_internal cs
CS( texture_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal texture *
Set( texture_block_array *Arr,
  texture *Element,
  texture_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  texture_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( texture_block_array *Arr )
{
  texture_block  *NewBlock     = Allocate( texture_block , Arr->Memory,                 1);
  texture_block **NewBlockPtrs = Allocate( texture_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( texture_block_array *Array, texture_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( texture_block_array *Array, texture_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  texture *Prev = {};

  texture_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    texture *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( texture_block_array *Array, texture *Element )
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

link_internal texture_block_array_index
Find( texture_block_array *Array, texture *Query)
{
  texture_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(texture_block_array_index *Index)
{
  texture_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal texture *
Push( texture_block_array *Array, texture *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  texture *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal texture *
Push( texture_block_array *Array )
{
  texture Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( texture_block_array *Array, texture_block_array_index Index, texture *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  texture *Prev = Push(Array);

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
Insert( texture_block_array *Array, u32 Index, texture *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( texture_block_array *Array, texture *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal texture *
Pop( texture_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


