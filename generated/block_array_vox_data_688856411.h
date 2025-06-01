// src/engine/model.h:88:0





struct vox_data_block
{
  /* u32 Index; */
  umm At;
  vox_data Elements[8];
};

struct vox_data_block_array_index
{
  umm Index; 
  /* block_t *Block; */
  /* u32 BlockIndex; */
  /* u32 ElementIndex; */
};

struct vox_data_block_array
{
  vox_data_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal vox_data_block_array
VoxDataBlockArray(memory_arena *Memory)
{
  vox_data_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(vox_data_block_array_index *Thing1, vox_data_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( vox_data_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(vox_data_block_array_index Thing1, vox_data_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( vox_data_block_array_index ) );

  return Result;
}


typedef vox_data_block_array vox_data_paged_list;

link_internal vox_data_block_array_index
operator++( vox_data_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( vox_data_block_array_index I0, vox_data_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( vox_data_block_array_index I0, vox_data_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( vox_data_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal vox_data_block_array_index
ZerothIndex( vox_data_block_array *Arr )
{
  return {};
}

link_internal vox_data_block_array_index
Capacity( vox_data_block_array *Arr )
{
  vox_data_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal vox_data_block_array_index
AtElements( vox_data_block_array *Arr )
{
  vox_data_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( vox_data_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal vox_data_block_array_index
LastIndex( vox_data_block_array *Arr )
{
  vox_data_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( vox_data_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal vox_data_block *
GetBlock( vox_data_block_array *Arr, vox_data_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  vox_data_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal vox_data *
GetPtr( vox_data_block_array *Arr, vox_data_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  vox_data_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  vox_data *Result = (Block->Elements + ElementIndex);
  return Result;
}

link_internal vox_data *
TryGetPtr(vox_data_block_array *Arr, vox_data_block_array_index Index)
{
  vox_data * Result = {};
  if (Arr->BlockPtrs && Index.Index < Capacity(Arr).Index)
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}


link_internal vox_data *
GetPtr( vox_data_block_array *Arr, umm Index )
{
  vox_data_block_array_index I = {Index};
  return GetPtr(Arr, I);
}

link_internal vox_data *
TryGetPtr(vox_data_block_array *Arr, umm Index)
{
  vox_data * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr).Index)
  {
    vox_data_block_array_index I = {Index};
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
CS( vox_data_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal vox_data *
Set( vox_data_block_array *Arr,
  vox_data *Element,
  vox_data_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  vox_data_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( vox_data_block_array *Arr )
{
  vox_data_block  *NewBlock     = Allocate( vox_data_block , Arr->Memory,                 1);
  vox_data_block **NewBlockPtrs = Allocate( vox_data_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( vox_data_block_array *Array, vox_data_block_array_index Index)
{
  auto LastElement = GetPtr(Array, LastIndex(Array));
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( vox_data_block_array *Array, vox_data_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  vox_data *Prev = {};

  vox_data_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    vox_data *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( vox_data_block_array *Array, vox_data *Element )
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

link_internal vox_data_block_array_index
Find( vox_data_block_array *Array, vox_data *Query)
{
  vox_data_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(vox_data_block_array_index *Index)
{
  vox_data_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal vox_data *
Push( vox_data_block_array *Array, vox_data *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  vox_data *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal vox_data *
Push( vox_data_block_array *Array )
{
  vox_data Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Shift( vox_data_block_array *Array, vox_data *Element )
{
  Assert(Array->Memory);
  vox_data *Prev = {};

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


