// callsite
// src/engine/world_chunk.h:465:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct world_chunk_ptr_block
{
  /* u32 Index; */
  umm At;
  world_chunk_ptr Elements[32];
};


struct world_chunk_ptr_block_array_index
{
  umm Index; 
};

struct world_chunk_ptr_block_array
poof(
  @collection
  
  
)
{
  world_chunk_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal world_chunk_ptr_block_array
WorldChunkPtrBlockArray(memory_arena *Memory)
{
  world_chunk_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(world_chunk_ptr_block_array_index *Thing1, world_chunk_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( world_chunk_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(world_chunk_ptr_block_array_index Thing1, world_chunk_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( world_chunk_ptr_block_array_index ) );

  return Result;
}


typedef world_chunk_ptr_block_array world_chunk_ptr_paged_list;

link_internal world_chunk_ptr_block_array_index
operator++( world_chunk_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( world_chunk_ptr_block_array_index I0, world_chunk_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( world_chunk_ptr_block_array_index I0, world_chunk_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( world_chunk_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal world_chunk_ptr_block_array_index
ZerothIndex( world_chunk_ptr_block_array *Arr )
{
  return {};
}

link_internal world_chunk_ptr_block_array_index
Capacity( world_chunk_ptr_block_array *Arr )
{
  world_chunk_ptr_block_array_index Result = {Arr->BlockCount * 32};
  return Result;
}

link_internal world_chunk_ptr_block_array_index
AtElements( world_chunk_ptr_block_array *Arr )
{
  world_chunk_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( world_chunk_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal world_chunk_ptr_block_array_index
LastIndex( world_chunk_ptr_block_array *Arr )
{
  world_chunk_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( world_chunk_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal world_chunk_ptr_block *
GetBlock( world_chunk_ptr_block_array *Arr, world_chunk_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 32;
  Assert(BlockIndex < Arr->BlockCount);
  world_chunk_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal world_chunk_ptr 
GetPtr( world_chunk_ptr_block_array *Arr, world_chunk_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  world_chunk_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 32;
  world_chunk_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}


link_internal world_chunk_ptr 
GetPtr( world_chunk_ptr_block_array *Arr, umm Index )
{
  world_chunk_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal world_chunk_ptr 
TryGetPtr( world_chunk_ptr_block_array *Arr, world_chunk_ptr_block_array_index Index)
{
  world_chunk_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal world_chunk_ptr 
TryGetPtr( world_chunk_ptr_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, world_chunk_ptr_block_array_index{Index});
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

