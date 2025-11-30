// callsite
// external/bonsai_stdlib/src/texture.h:51:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct texture_ptr_block
{
  /* u32 Index; */
  umm At;
  texture_ptr Elements[8];
};


struct texture_ptr_block_array_index
{
  umm Index; 
};

struct texture_ptr_block_array
poof(
  @collection
  
  
)
{
  texture_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal texture_ptr_block_array
TexturePtrBlockArray(memory_arena *Memory)
{
  texture_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(texture_ptr_block_array_index *Thing1, texture_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( texture_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(texture_ptr_block_array_index Thing1, texture_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( texture_ptr_block_array_index ) );

  return Result;
}


typedef texture_ptr_block_array texture_ptr_paged_list;

link_internal texture_ptr_block_array_index
operator++( texture_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( texture_ptr_block_array_index I0, texture_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( texture_ptr_block_array_index I0, texture_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( texture_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal texture_ptr_block_array_index
ZerothIndex( texture_ptr_block_array *Arr )
{
  return {};
}

link_internal texture_ptr_block_array_index
Capacity( texture_ptr_block_array *Arr )
{
  texture_ptr_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal texture_ptr_block_array_index
AtElements( texture_ptr_block_array *Arr )
{
  texture_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( texture_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal texture_ptr_block_array_index
LastIndex( texture_ptr_block_array *Arr )
{
  texture_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( texture_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal texture_ptr_block *
GetBlock( texture_ptr_block_array *Arr, texture_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  texture_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal texture_ptr 
GetPtr( texture_ptr_block_array *Arr, texture_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  texture_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  texture_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}


link_internal texture_ptr 
GetPtr( texture_ptr_block_array *Arr, umm Index )
{
  texture_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal texture_ptr 
TryGetPtr( texture_ptr_block_array *Arr, texture_ptr_block_array_index Index)
{
  texture_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal texture_ptr 
TryGetPtr( texture_ptr_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, texture_ptr_block_array_index{Index});
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

