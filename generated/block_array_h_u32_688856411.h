// callsite
// external/bonsai_stdlib/src/primitive_containers.h:14:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct u32_block
{
  /* u32 Index; */
  umm At;
  u32 Elements[8];
};


struct u32_block_array_index
{
  umm Index; 
};

struct u32_block_array
poof(
  @collection
  
  
)
{
  u32_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal u32_block_array
U32BlockArray(memory_arena *Memory)
{
  u32_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(u32_block_array_index *Thing1, u32_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( u32_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(u32_block_array_index Thing1, u32_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( u32_block_array_index ) );

  return Result;
}


typedef u32_block_array u32_paged_list;

link_internal u32_block_array_index
operator++( u32_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( u32_block_array_index I0, u32_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( u32_block_array_index I0, u32_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( u32_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal u32_block_array_index
ZerothIndex( u32_block_array *Arr )
{
  return {};
}

link_internal u32_block_array_index
Capacity( u32_block_array *Arr )
{
  u32_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal u32_block_array_index
AtElements( u32_block_array *Arr )
{
  u32_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( u32_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal u32_block_array_index
LastIndex( u32_block_array *Arr )
{
  u32_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( u32_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal u32_block *
GetBlock( u32_block_array *Arr, u32_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  u32_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal u32 *
GetPtr( u32_block_array *Arr, u32_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  u32_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  u32 *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal u32 *
GetPtr( u32_block_array *Arr, umm Index )
{
  u32_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal u32 *
TryGetPtr( u32_block_array *Arr, u32_block_array_index Index)
{
  u32 * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal u32 *
TryGetPtr( u32_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, u32_block_array_index{Index});
  return Result;
}

/* link_internal void */
/* Swap( (element_t.name)_block_array *Arr, umm I0, umm I1) */
/* { */
/*   Assert(I0 < AtElements(Arr).Index); */
/*   Assert(I1 < AtElements(Arr).Index); */

/*   auto P0 = GetPtr(Arr, I0); */
/*   auto P1 = GetPtr(Arr, I1); */

/*   auto Tmp = *P0; */
/*   *P0 = *P1; */

/*   *P1 = Tmp; */
/* } */

