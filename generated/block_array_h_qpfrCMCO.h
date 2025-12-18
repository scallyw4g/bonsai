// callsite
// src/engine/world_chunk.h:379:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct gen_chunk_ptr_block
{
  /* u32 Index; */
  umm At;
  gen_chunk_ptr Elements[32];
};


struct gen_chunk_ptr_block_array_index
{
  umm Index; 
};

struct gen_chunk_ptr_block_array
poof(
  @collection
  
  
)
{
  gen_chunk_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal gen_chunk_ptr_block_array
GenChunkPtrBlockArray(memory_arena *Memory)
{
  gen_chunk_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(gen_chunk_ptr_block_array_index *Thing1, gen_chunk_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( gen_chunk_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(gen_chunk_ptr_block_array_index Thing1, gen_chunk_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( gen_chunk_ptr_block_array_index ) );

  return Result;
}


typedef gen_chunk_ptr_block_array gen_chunk_ptr_paged_list;

link_internal gen_chunk_ptr_block_array_index
operator++( gen_chunk_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( gen_chunk_ptr_block_array_index I0, gen_chunk_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( gen_chunk_ptr_block_array_index I0, gen_chunk_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( gen_chunk_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal gen_chunk_ptr_block_array_index
ZerothIndex( gen_chunk_ptr_block_array *Arr )
{
  return {};
}

link_internal gen_chunk_ptr_block_array_index
Capacity( gen_chunk_ptr_block_array *Arr )
{
  gen_chunk_ptr_block_array_index Result = {Arr->BlockCount * 32};
  return Result;
}

link_internal gen_chunk_ptr_block_array_index
AtElements( gen_chunk_ptr_block_array *Arr )
{
  gen_chunk_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( gen_chunk_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal gen_chunk_ptr_block_array_index
LastIndex( gen_chunk_ptr_block_array *Arr )
{
  gen_chunk_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( gen_chunk_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal gen_chunk_ptr_block *
GetBlock( gen_chunk_ptr_block_array *Arr, gen_chunk_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 32;
  Assert(BlockIndex < Arr->BlockCount);
  gen_chunk_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal gen_chunk_ptr 
GetPtr( gen_chunk_ptr_block_array *Arr, gen_chunk_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  gen_chunk_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 32;
  gen_chunk_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}


link_internal gen_chunk_ptr 
GetPtr( gen_chunk_ptr_block_array *Arr, umm Index )
{
  gen_chunk_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal gen_chunk_ptr 
TryGetPtr( gen_chunk_ptr_block_array *Arr, gen_chunk_ptr_block_array_index Index)
{
  gen_chunk_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal gen_chunk_ptr 
TryGetPtr( gen_chunk_ptr_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, gen_chunk_ptr_block_array_index{Index});
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

