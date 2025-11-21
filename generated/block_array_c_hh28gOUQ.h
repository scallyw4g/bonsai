// callsite
// external/bonsai_stdlib/src/framebuffer.cpp:3:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2446:0




link_internal cs
CS( rtt_framebuffer_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal rtt_framebuffer *
Set( rtt_framebuffer_block_array *Arr,
  rtt_framebuffer *Element,
  rtt_framebuffer_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  rtt_framebuffer_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( rtt_framebuffer_block_array *Arr )
{
  rtt_framebuffer_block  *NewBlock     = Allocate( rtt_framebuffer_block , Arr->Memory,                 1);
  rtt_framebuffer_block **NewBlockPtrs = Allocate( rtt_framebuffer_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( rtt_framebuffer_block_array *Array, rtt_framebuffer_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( rtt_framebuffer_block_array *Array, rtt_framebuffer_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  rtt_framebuffer *Prev = {};

  rtt_framebuffer_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    rtt_framebuffer *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( rtt_framebuffer_block_array *Array, rtt_framebuffer *Element )
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

link_internal rtt_framebuffer_block_array_index
Find( rtt_framebuffer_block_array *Array, rtt_framebuffer *Query)
{
  rtt_framebuffer_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(rtt_framebuffer_block_array_index *Index)
{
  rtt_framebuffer_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal rtt_framebuffer *
Push( rtt_framebuffer_block_array *Array, rtt_framebuffer *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  rtt_framebuffer *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal rtt_framebuffer *
Push( rtt_framebuffer_block_array *Array )
{
  rtt_framebuffer Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( rtt_framebuffer_block_array *Array, rtt_framebuffer_block_array_index Index, rtt_framebuffer *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  rtt_framebuffer *Prev = Push(Array);

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
Insert( rtt_framebuffer_block_array *Array, u32 Index, rtt_framebuffer *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( rtt_framebuffer_block_array *Array, rtt_framebuffer *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


