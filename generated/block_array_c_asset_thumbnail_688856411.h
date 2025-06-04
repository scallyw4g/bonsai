// src/engine/editor.cpp:121:0





/* link_internal block_t * */
/* Allocate_(element_t.name)_block(memory_arena *Memory) */
/* { */
/*   block_t *Result = Allocate( block_t, Memory, 1); */
/*   return Result; */
/* } */

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
  auto LastElement = GetPtr(Array, LastIndex(Array));
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
  asset_thumbnail_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(asset_thumbnail_block_array_index *Index)
{
  asset_thumbnail_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
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
Shift( asset_thumbnail_block_array *Array, asset_thumbnail *Element )
{
  Assert(Array->Memory);
  asset_thumbnail *Prev = {};

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

