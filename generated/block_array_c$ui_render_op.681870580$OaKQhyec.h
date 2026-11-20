// callsite
// external/bonsai_stdlib/src/ui/ui.cpp:42:0

// def (block_array_c)
// external/bonsai_stdlib/src/poof_functions.h:2602:0




link_internal cs
CS( ui_render_op_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal b32
ValidateBlocksMatchCapacity( ui_render_op_block_array *Array )
{
  auto Cap = Capacity(Array).Index;
  auto Elements = AtElements(Array).Index;
  b32 Result = Elements <= Cap;
  Assert(Result);
  return Result;
}

link_internal ui_render_op *
Set( ui_render_op_block_array *Arr,
  ui_render_op *Element,
  ui_render_op_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  ui_render_op_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 1024;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( ui_render_op_block_array *Arr )
{
  ui_render_op_block  *NewBlock     = Allocate( ui_render_op_block , Arr->Memory,                 1);
  ui_render_op_block **NewBlockPtrs = Allocate( ui_render_op_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( ui_render_op_block_array *Array, ui_render_op_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( ui_render_op_block_array *Array, ui_render_op_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  ui_render_op *Prev = {};

  ui_render_op_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    ui_render_op *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
RemoveOrdered( ui_render_op_block_array *Array, ui_render_op *Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal ui_render_op_block_array_index
Find( ui_render_op_block_array *Array, ui_render_op *Query)
{
  ui_render_op_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(ui_render_op_block_array_index *Index)
{
  ui_render_op_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal ui_render_op *
Push( ui_render_op_block_array *Array, ui_render_op *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  ui_render_op *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal ui_render_op *
Push( ui_render_op_block_array *Array )
{
  ui_render_op Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( ui_render_op_block_array *Array, ui_render_op_block_array_index Index, ui_render_op *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  ui_render_op *Prev = Push(Array);

  auto Last = LastIndex(Array);

  RangeIteratorReverseRange(I, s32(Last.Index), s32(Index.Index))
  {
    auto E = GetPtr(Array, umm(I));
    *Prev = *E;
    Prev = E;
  }

  *Prev = *Element;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
Insert( ui_render_op_block_array *Array, u32 Index, ui_render_op *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( ui_render_op_block_array *Array, ui_render_op *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}




link_internal ui_render_op *
Pop( ui_render_op_block_array *Array )
{
  ui_render_op * Result = {};

  if (auto P = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    Result = P;
  }
  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal void
ClearList( ui_render_op_block_array *Array )
{
  IterateOver(Array, Element, ElementIndex)
  {
    *Element = {};
  }

  Array->ElementCount = 0;
}


