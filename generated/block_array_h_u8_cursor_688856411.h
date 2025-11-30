// callsite
// external/bonsai_stdlib/src/ansi_stream.h:6:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct u8_cursor_block
{
  /* u32 Index; */
  umm At;
  u8_cursor Elements[8];
};


struct u8_cursor_block_array_index
{
  umm Index; 
};

struct u8_cursor_block_array
poof(
  @collection
  
  
)
{
  u8_cursor_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  u64 BlockSize;
};

link_internal u8_cursor_block_array
U8CursorBlockArray(memory_arena *Memory)
{
  u8_cursor_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(u8_cursor_block_array_index *Thing1, u8_cursor_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( u8_cursor_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(u8_cursor_block_array_index Thing1, u8_cursor_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( u8_cursor_block_array_index ) );

  return Result;
}


typedef u8_cursor_block_array u8_cursor_paged_list;

link_internal u8_cursor_block_array_index
operator++( u8_cursor_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( u8_cursor_block_array_index I0, u8_cursor_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( u8_cursor_block_array_index I0, u8_cursor_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( u8_cursor_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal u8_cursor_block_array_index
ZerothIndex( u8_cursor_block_array *Arr )
{
  return {};
}

link_internal u8_cursor_block_array_index
Capacity( u8_cursor_block_array *Arr )
{
  u8_cursor_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal u8_cursor_block_array_index
AtElements( u8_cursor_block_array *Arr )
{
  u8_cursor_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( u8_cursor_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal u8_cursor_block_array_index
LastIndex( u8_cursor_block_array *Arr )
{
  u8_cursor_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( u8_cursor_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal u8_cursor_block *
GetBlock( u8_cursor_block_array *Arr, u8_cursor_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  u8_cursor_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal u8_cursor *
GetPtr( u8_cursor_block_array *Arr, u8_cursor_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  u8_cursor_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  u8_cursor *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal u8_cursor *
GetPtr( u8_cursor_block_array *Arr, umm Index )
{
  u8_cursor_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal u8_cursor *
TryGetPtr( u8_cursor_block_array *Arr, u8_cursor_block_array_index Index)
{
  u8_cursor * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal u8_cursor *
TryGetPtr( u8_cursor_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, u8_cursor_block_array_index{Index});
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

