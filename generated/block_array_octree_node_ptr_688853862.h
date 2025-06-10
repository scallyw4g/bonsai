// src/engine/world.h:85:0





struct octree_node_ptr_block
{
  /* u32 Index; */
  umm At;
  octree_node_ptr Elements[8];
};

struct octree_node_ptr_block_array_index
{
  umm Index; 
};

struct octree_node_ptr_block_array
{
  octree_node_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal octree_node_ptr_block_array
OctreeNodePtrBlockArray(memory_arena *Memory)
{
  octree_node_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(octree_node_ptr_block_array_index *Thing1, octree_node_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( octree_node_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(octree_node_ptr_block_array_index Thing1, octree_node_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( octree_node_ptr_block_array_index ) );

  return Result;
}


typedef octree_node_ptr_block_array octree_node_ptr_paged_list;

link_internal octree_node_ptr_block_array_index
operator++( octree_node_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( octree_node_ptr_block_array_index I0, octree_node_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( octree_node_ptr_block_array_index I0, octree_node_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( octree_node_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal octree_node_ptr_block_array_index
ZerothIndex( octree_node_ptr_block_array *Arr )
{
  return {};
}

link_internal octree_node_ptr_block_array_index
Capacity( octree_node_ptr_block_array *Arr )
{
  octree_node_ptr_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal octree_node_ptr_block_array_index
AtElements( octree_node_ptr_block_array *Arr )
{
  octree_node_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( octree_node_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal octree_node_ptr_block_array_index
LastIndex( octree_node_ptr_block_array *Arr )
{
  octree_node_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( octree_node_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal octree_node_ptr_block *
GetBlock( octree_node_ptr_block_array *Arr, octree_node_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  octree_node_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal octree_node_ptr 
GetPtr( octree_node_ptr_block_array *Arr, octree_node_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  octree_node_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  octree_node_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}

link_internal octree_node_ptr 
TryGetPtr(octree_node_ptr_block_array *Arr, octree_node_ptr_block_array_index Index)
{
  octree_node_ptr  Result = {};
  if (Arr->BlockPtrs && Index.Index < Capacity(Arr).Index)
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}


link_internal octree_node_ptr 
GetPtr( octree_node_ptr_block_array *Arr, umm Index )
{
  octree_node_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}

link_internal octree_node_ptr 
TryGetPtr(octree_node_ptr_block_array *Arr, umm Index)
{
  octree_node_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr).Index)
  {
    octree_node_ptr_block_array_index I = {Index};
    Result = GetPtr(Arr, I);
  }
  return Result;
}





link_internal cs
CS( octree_node_ptr_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal octree_node_ptr 
Set( octree_node_ptr_block_array *Arr,
  octree_node_ptr Element,
  octree_node_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  octree_node_ptr_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = Element;
  return *Slot;
}

link_internal void
NewBlock( octree_node_ptr_block_array *Arr )
{
  octree_node_ptr_block  *NewBlock     = Allocate( octree_node_ptr_block , Arr->Memory,                 1);
  octree_node_ptr_block **NewBlockPtrs = Allocate( octree_node_ptr_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( octree_node_ptr_block_array *Array, octree_node_ptr_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( octree_node_ptr_block_array *Array, octree_node_ptr_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  octree_node_ptr Prev = {};

  octree_node_ptr_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    octree_node_ptr E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( octree_node_ptr_block_array *Array, octree_node_ptr Element )
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

link_internal octree_node_ptr_block_array_index
Find( octree_node_ptr_block_array *Array, octree_node_ptr Query)
{
  octree_node_ptr_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(octree_node_ptr_block_array_index *Index)
{
  octree_node_ptr_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal octree_node_ptr 
Push( octree_node_ptr_block_array *Array, octree_node_ptr Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  octree_node_ptr Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal octree_node_ptr 
Push( octree_node_ptr_block_array *Array )
{
  octree_node_ptr Element = {};
  auto Result = Push(Array, Element);
  return Result;
}

link_internal void
Shift( octree_node_ptr_block_array *Array, octree_node_ptr Element )
{
  Assert(Array->Memory);
  octree_node_ptr Prev = {};

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


