// external/bonsai_stdlib/src/ansi_stream.cpp:3:0





link_internal cs
CS( u8_cursor_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal u8_cursor *
Set( u8_cursor_block_array *Arr,
  u8_cursor *Element,
  u8_cursor_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  u8_cursor_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( u8_cursor_block_array *Arr )
{
  u8_cursor_block  *NewBlock     = Allocate( u8_cursor_block , Arr->Memory,                 1);
  u8_cursor_block **NewBlockPtrs = Allocate( u8_cursor_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( u8_cursor_block_array *Array, u8_cursor_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( u8_cursor_block_array *Array, u8_cursor_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  u8_cursor *Prev = {};

  u8_cursor_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    u8_cursor *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( u8_cursor_block_array *Array, u8_cursor *Element )
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

link_internal u8_cursor_block_array_index
Find( u8_cursor_block_array *Array, u8_cursor *Query)
{
  u8_cursor_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(u8_cursor_block_array_index *Index)
{
  u8_cursor_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal u8_cursor *
Push( u8_cursor_block_array *Array, u8_cursor *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  u8_cursor *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal u8_cursor *
Push( u8_cursor_block_array *Array )
{
  u8_cursor Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( u8_cursor_block_array *Array, u8_cursor *Element )
{
  Assert(Array->Memory);
  u8_cursor *Prev = {};

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

