// callsite
// src/engine/editor.cpp:2:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2434:0




link_internal cs
CS( simple_brush_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal simple_brush *
Set( simple_brush_block_array *Arr,
  simple_brush *Element,
  simple_brush_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  simple_brush_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( simple_brush_block_array *Arr )
{
  simple_brush_block  *NewBlock     = Allocate( simple_brush_block , Arr->Memory,                 1);
  simple_brush_block **NewBlockPtrs = Allocate( simple_brush_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( simple_brush_block_array *Array, simple_brush_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( simple_brush_block_array *Array, simple_brush_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  simple_brush *Prev = {};

  simple_brush_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    simple_brush *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( simple_brush_block_array *Array, simple_brush *Element )
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

link_internal simple_brush_block_array_index
Find( simple_brush_block_array *Array, simple_brush *Query)
{
  simple_brush_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(simple_brush_block_array_index *Index)
{
  simple_brush_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal simple_brush *
Push( simple_brush_block_array *Array, simple_brush *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  simple_brush *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal simple_brush *
Push( simple_brush_block_array *Array )
{
  simple_brush Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( simple_brush_block_array *Array, simple_brush_block_array_index Index, simple_brush *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  simple_brush *Prev = Push(Array);

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
Insert( simple_brush_block_array *Array, u32 Index, simple_brush *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( simple_brush_block_array *Array, simple_brush *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


