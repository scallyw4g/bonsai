// callsite
// src/engine/world_chunk.h:316:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct entity_ptr_block
{
  /* u32 Index; */
  umm At;
  entity_ptr Elements[8];
};


struct entity_ptr_block_array_index
{
  umm Index; 
};

struct entity_ptr_block_array
poof(
  @collection
  
  
)
{
  entity_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal entity_ptr_block_array
EntityPtrBlockArray(memory_arena *Memory)
{
  entity_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(entity_ptr_block_array_index *Thing1, entity_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( entity_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(entity_ptr_block_array_index Thing1, entity_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( entity_ptr_block_array_index ) );

  return Result;
}


typedef entity_ptr_block_array entity_ptr_paged_list;

link_internal entity_ptr_block_array_index
operator++( entity_ptr_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( entity_ptr_block_array_index I0, entity_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( entity_ptr_block_array_index I0, entity_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( entity_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal entity_ptr_block_array_index
ZerothIndex( entity_ptr_block_array *Arr )
{
  return {};
}

link_internal entity_ptr_block_array_index
Capacity( entity_ptr_block_array *Arr )
{
  entity_ptr_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal entity_ptr_block_array_index
AtElements( entity_ptr_block_array *Arr )
{
  entity_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( entity_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal entity_ptr_block_array_index
LastIndex( entity_ptr_block_array *Arr )
{
  entity_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( entity_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal entity_ptr_block *
GetBlock( entity_ptr_block_array *Arr, entity_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  entity_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal entity_ptr 
GetPtr( entity_ptr_block_array *Arr, entity_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  entity_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  entity_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}


link_internal entity_ptr 
GetPtr( entity_ptr_block_array *Arr, umm Index )
{
  entity_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal entity_ptr 
TryGetPtr( entity_ptr_block_array *Arr, entity_ptr_block_array_index Index)
{
  entity_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal entity_ptr 
TryGetPtr( entity_ptr_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, entity_ptr_block_array_index{Index});
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

