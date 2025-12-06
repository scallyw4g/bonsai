// callsite
// src/engine/editor.cpp:203:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2502:0




link_internal cs
CS( asset_thumbnail_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal asset_thumbnail *
Set( asset_thumbnail_block_array *Arr,
  asset_thumbnail *Element,
  asset_thumbnail_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  asset_thumbnail_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( asset_thumbnail_block_array *Arr )
{
  asset_thumbnail_block  *NewBlock     = Allocate( asset_thumbnail_block , Arr->Memory,                 1);
  asset_thumbnail_block **NewBlockPtrs = Allocate( asset_thumbnail_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( asset_thumbnail_block_array *Array, asset_thumbnail_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( asset_thumbnail_block_array *Array, asset_thumbnail_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  asset_thumbnail *Prev = {};

  asset_thumbnail_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    asset_thumbnail *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( asset_thumbnail_block_array *Array, asset_thumbnail *Element )
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

link_internal asset_thumbnail_block_array_index
Find( asset_thumbnail_block_array *Array, asset_thumbnail *Query)
{
  asset_thumbnail_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(asset_thumbnail_block_array_index *Index)
{
  asset_thumbnail_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal asset_thumbnail *
Push( asset_thumbnail_block_array *Array, asset_thumbnail *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  asset_thumbnail *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal asset_thumbnail *
Push( asset_thumbnail_block_array *Array )
{
  asset_thumbnail Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( asset_thumbnail_block_array *Array, asset_thumbnail_block_array_index Index, asset_thumbnail *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  asset_thumbnail *Prev = Push(Array);

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
Insert( asset_thumbnail_block_array *Array, u32 Index, asset_thumbnail *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( asset_thumbnail_block_array *Array, asset_thumbnail *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal asset_thumbnail *
Pop( asset_thumbnail_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}


