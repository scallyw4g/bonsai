// src/engine/world_update.h:18:0





struct voxel_stack_element_block
{
  /* u32 Index; */
  umm At;
  voxel_stack_element Elements[8];
};

struct voxel_stack_element_block_array_index
{
  umm Index; 
  /* block_t *Block; */
  /* u32 BlockIndex; */
  /* u32 ElementIndex; */
};

struct voxel_stack_element_block_array
{
  voxel_stack_element_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal voxel_stack_element_block_array
VoxelStackElementBlockArray(memory_arena *Memory)
{
  voxel_stack_element_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(voxel_stack_element_block_array_index *Thing1, voxel_stack_element_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( voxel_stack_element_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(voxel_stack_element_block_array_index Thing1, voxel_stack_element_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( voxel_stack_element_block_array_index ) );

  return Result;
}


typedef voxel_stack_element_block_array voxel_stack_element_paged_list;

link_internal voxel_stack_element_block_array_index
operator++( voxel_stack_element_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( voxel_stack_element_block_array_index I0, voxel_stack_element_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( voxel_stack_element_block_array_index I0, voxel_stack_element_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( voxel_stack_element_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal voxel_stack_element_block_array_index
ZerothIndex( voxel_stack_element_block_array *Arr )
{
  return {};
}

link_internal voxel_stack_element_block_array_index
Capacity( voxel_stack_element_block_array *Arr )
{
  voxel_stack_element_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal voxel_stack_element_block_array_index
AtElements( voxel_stack_element_block_array *Arr )
{
  voxel_stack_element_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( voxel_stack_element_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal voxel_stack_element_block_array_index
LastIndex( voxel_stack_element_block_array *Arr )
{
  voxel_stack_element_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( voxel_stack_element_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal voxel_stack_element_block *
GetBlock( voxel_stack_element_block_array *Arr, voxel_stack_element_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  voxel_stack_element_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal voxel_stack_element *
GetPtr( voxel_stack_element_block_array *Arr, voxel_stack_element_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  voxel_stack_element_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  voxel_stack_element *Result = (Block->Elements + ElementIndex);
  return Result;
}

link_internal voxel_stack_element *
TryGetPtr(voxel_stack_element_block_array *Arr, voxel_stack_element_block_array_index Index)
{
  voxel_stack_element * Result = {};
  if (Arr->BlockPtrs && Index.Index < Capacity(Arr).Index)
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}


link_internal voxel_stack_element *
GetPtr( voxel_stack_element_block_array *Arr, umm Index )
{
  voxel_stack_element_block_array_index I = {Index};
  return GetPtr(Arr, I);
}

link_internal voxel_stack_element *
TryGetPtr(voxel_stack_element_block_array *Arr, umm Index)
{
  voxel_stack_element * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr).Index)
  {
    voxel_stack_element_block_array_index I = {Index};
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
CS( voxel_stack_element_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal voxel_stack_element *
Set( voxel_stack_element_block_array *Arr,
  voxel_stack_element *Element,
  voxel_stack_element_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  voxel_stack_element_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( voxel_stack_element_block_array *Arr )
{
  voxel_stack_element_block  *NewBlock     = Allocate( voxel_stack_element_block , Arr->Memory,                 1);
  voxel_stack_element_block **NewBlockPtrs = Allocate( voxel_stack_element_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( voxel_stack_element_block_array *Array, voxel_stack_element_block_array_index Index)
{
  auto LastElement = GetPtr(Array, LastIndex(Array));
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( voxel_stack_element_block_array *Array, voxel_stack_element_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  voxel_stack_element *Prev = {};

  voxel_stack_element_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    voxel_stack_element *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( voxel_stack_element_block_array *Array, voxel_stack_element *Element )
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

link_internal voxel_stack_element_block_array_index
Find( voxel_stack_element_block_array *Array, voxel_stack_element *Query)
{
  voxel_stack_element_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(voxel_stack_element_block_array_index *Index)
{
  voxel_stack_element_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal voxel_stack_element *
Push( voxel_stack_element_block_array *Array, voxel_stack_element *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  voxel_stack_element *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal voxel_stack_element *
Push( voxel_stack_element_block_array *Array )
{
  voxel_stack_element Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( voxel_stack_element_block_array *Array, voxel_stack_element *Element )
{
  Assert(Array->Memory);
  voxel_stack_element *Prev = {};

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


