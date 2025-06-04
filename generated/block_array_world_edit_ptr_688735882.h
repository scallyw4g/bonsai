// src/engine/editor.h:1253:0





struct world_edit_ptr_block
{
  /* u32 Index; */
  umm At;
  world_edit_ptr Elements[8];
};

struct world_edit_ptr_block_array_index
{
  umm Index; 
  /* block_t *Block; */
  /* u32 BlockIndex; */
  /* u32 ElementIndex; */
};

struct world_edit_ptr_block_array
{
  world_edit_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal world_edit_ptr_block_array
WorldEditPtrBlockArray(memory_arena *Memory)
{
  world_edit_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(world_edit_ptr_block_array_index *Thing1, world_edit_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( world_edit_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(world_edit_ptr_block_array_index Thing1, world_edit_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( world_edit_ptr_block_array_index ) );

  return Result;
}


typedef world_edit_ptr_block_array world_edit_ptr_paged_list;

link_internal world_edit_ptr_block_array_index
operator++( world_edit_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( world_edit_ptr_block_array_index I0, world_edit_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( world_edit_ptr_block_array_index I0, world_edit_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( world_edit_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal world_edit_ptr_block_array_index
ZerothIndex( world_edit_ptr_block_array *Arr )
{
  return {};
}

link_internal world_edit_ptr_block_array_index
Capacity( world_edit_ptr_block_array *Arr )
{
  world_edit_ptr_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal world_edit_ptr_block_array_index
AtElements( world_edit_ptr_block_array *Arr )
{
  world_edit_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( world_edit_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal world_edit_ptr_block_array_index
LastIndex( world_edit_ptr_block_array *Arr )
{
  world_edit_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( world_edit_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal world_edit_ptr_block *
GetBlock( world_edit_ptr_block_array *Arr, world_edit_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  world_edit_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal world_edit_ptr 
GetPtr( world_edit_ptr_block_array *Arr, world_edit_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  world_edit_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  world_edit_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}

link_internal world_edit_ptr 
TryGetPtr(world_edit_ptr_block_array *Arr, world_edit_ptr_block_array_index Index)
{
  world_edit_ptr  Result = {};
  if (Arr->BlockPtrs && Index.Index < Capacity(Arr).Index)
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}


link_internal world_edit_ptr 
GetPtr( world_edit_ptr_block_array *Arr, umm Index )
{
  world_edit_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}

link_internal world_edit_ptr 
TryGetPtr(world_edit_ptr_block_array *Arr, umm Index)
{
  world_edit_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr).Index)
  {
    world_edit_ptr_block_array_index I = {Index};
    Result = GetPtr(Arr, I);
  }
  return Result;
}





/* link_internal block_t * */
/* Allocate_(element_t.name)_block(memory_arena *Memory) */
/* { */
/*   block_t *Result = Allocate( block_t, Memory, 1); */
/*   return Result; */
/* } */

link_internal cs
CS( world_edit_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal world_edit_ptr 
Set( world_edit_ptr_block_array *Arr,
  world_edit_ptr Element,
  world_edit_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  world_edit_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( world_edit_ptr_block_array *Arr )
{
  world_edit_ptr_block  *NewBlock     = Allocate( world_edit_ptr_block , Arr->Memory,                 1);
  world_edit_ptr_block **NewBlockPtrs = Allocate( world_edit_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( world_edit_ptr_block_array *Array, world_edit_ptr_block_array_index Index)
{
  auto LastElement = GetPtr(Array, LastIndex(Array));
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( world_edit_ptr_block_array *Array, world_edit_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  world_edit_ptr Prev = {};

  world_edit_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    world_edit_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( world_edit_ptr_block_array *Array, world_edit_ptr Element )
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

link_internal world_edit_ptr_block_array_index
Find( world_edit_ptr_block_array *Array, world_edit_ptr Query)
{
  world_edit_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(world_edit_ptr_block_array_index *Index)
{
  world_edit_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal world_edit_ptr 
Push( world_edit_ptr_block_array *Array, world_edit_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  world_edit_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal world_edit_ptr 
Push( world_edit_ptr_block_array *Array )
{
  world_edit_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Shift( world_edit_ptr_block_array *Array, world_edit_ptr Element )
{
  Assert(Array->Memory);
  world_edit_ptr Prev = {};

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


