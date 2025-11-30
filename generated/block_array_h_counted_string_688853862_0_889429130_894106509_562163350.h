// callsite
// external/bonsai_stdlib/src/counted_string.h:1161:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct counted_string_block
{
  /* u32 Index; */
  umm At;
  counted_string Elements[32];
};


struct counted_string_block_array_index
{
  umm Index; 
};

struct counted_string_block_array
poof(
  @collection
  
  
)
{
  counted_string_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal counted_string_block_array
CountedStringBlockArray(memory_arena *Memory)
{
  counted_string_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(counted_string_block_array_index *Thing1, counted_string_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( counted_string_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(counted_string_block_array_index Thing1, counted_string_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( counted_string_block_array_index ) );

  return Result;
}


typedef counted_string_block_array counted_string_paged_list;

link_internal counted_string_block_array_index
operator++( counted_string_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( counted_string_block_array_index I0, counted_string_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( counted_string_block_array_index I0, counted_string_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( counted_string_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal counted_string_block_array_index
ZerothIndex( counted_string_block_array *Arr )
{
  return {};
}

link_internal counted_string_block_array_index
Capacity( counted_string_block_array *Arr )
{
  counted_string_block_array_index Result = {Arr->BlockCount * 32};
  return Result;
}

link_internal counted_string_block_array_index
AtElements( counted_string_block_array *Arr )
{
  counted_string_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( counted_string_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal counted_string_block_array_index
LastIndex( counted_string_block_array *Arr )
{
  counted_string_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( counted_string_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal counted_string_block *
GetBlock( counted_string_block_array *Arr, counted_string_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 32;
  Assert(BlockIndex < Arr->BlockCount);
  counted_string_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal counted_string *
GetPtr( counted_string_block_array *Arr, counted_string_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  counted_string_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 32;
  counted_string *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal counted_string *
GetPtr( counted_string_block_array *Arr, umm Index )
{
  counted_string_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal counted_string *
TryGetPtr( counted_string_block_array *Arr, counted_string_block_array_index Index)
{
  counted_string * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal counted_string *
TryGetPtr( counted_string_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, counted_string_block_array_index{Index});
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

