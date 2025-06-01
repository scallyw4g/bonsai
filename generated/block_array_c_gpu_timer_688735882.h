// src/engine/render/gpu_timer.cpp:1:0





/* link_internal block_t * */
/* Allocate_(element_t.name)_block(memory_arena *Memory) */
/* { */
/*   block_t *Result = Allocate( block_t, Memory, 1); */
/*   return Result; */
/* } */

link_internal cs
CS( gpu_timer_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal gpu_timer *
Set( gpu_timer_block_array *Arr,
  gpu_timer *Element,
  gpu_timer_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  gpu_timer_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 128;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( gpu_timer_block_array *Arr )
{
  gpu_timer_block  *NewBlock     = Allocate( gpu_timer_block , Arr->Memory,                 1);
  gpu_timer_block **NewBlockPtrs = Allocate( gpu_timer_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( gpu_timer_block_array *Array, gpu_timer_block_array_index Index)
{
  auto LastElement = GetPtr(Array, LastIndex(Array));
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( gpu_timer_block_array *Array, gpu_timer_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  gpu_timer *Prev = {};

  gpu_timer_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    gpu_timer *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( gpu_timer_block_array *Array, gpu_timer *Element )
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

link_internal gpu_timer_block_array_index
Find( gpu_timer_block_array *Array, gpu_timer *Query)
{
  gpu_timer_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(gpu_timer_block_array_index *Index)
{
  gpu_timer_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal gpu_timer *
Push( gpu_timer_block_array *Array, gpu_timer *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  gpu_timer *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal gpu_timer *
Push( gpu_timer_block_array *Array )
{
  gpu_timer Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( gpu_timer_block_array *Array, gpu_timer *Element )
{
  Assert(Array->Memory);
  gpu_timer *Prev = {};

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

