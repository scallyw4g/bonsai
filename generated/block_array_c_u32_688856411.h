// external/bonsai_stdlib/src/primitive_containers.cpp:2:0





link_internal cs
CS( u32_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal u32 *
Set( u32_block_array *Arr,
  u32 *Element,
  u32_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  u32_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( u32_block_array *Arr )
{
  u32_block  *NewBlock     = Allocate( u32_block , Arr->Memory,                 1);
  u32_block **NewBlockPtrs = Allocate( u32_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( u32_block_array *Array, u32_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( u32_block_array *Array, u32_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  u32 *Prev = {};

  u32_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    u32 *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( u32_block_array *Array, u32 *Element )
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

link_internal u32_block_array_index
Find( u32_block_array *Array, u32 *Query)
{
  u32_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(u32_block_array_index *Index)
{
  u32_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal u32 *
Push( u32_block_array *Array, u32 *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  u32 *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal u32 *
Push( u32_block_array *Array )
{
  u32 Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( u32_block_array *Array, u32 *Element )
{
  Assert(Array->Memory);
  u32 *Prev = {};

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



