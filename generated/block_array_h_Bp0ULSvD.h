// callsite
// external/bonsai_stdlib/src/primitive_containers.h:41:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2315:0




struct base_ptr_relative_edit_block
{
  /* u32 Index; */
  umm At;
  base_ptr_relative_edit Elements[8];
};


struct base_ptr_relative_edit_block_array_index
{
  umm Index; 
};

struct base_ptr_relative_edit_block_array
poof(
  @collection
   @serdes 
   @do_editor_ui 
)
{
  base_ptr_relative_edit_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  u64 BasePtr;
};

link_internal base_ptr_relative_edit_block_array
BasePtrRelativeEditBlockArray(memory_arena *Memory)
{
  base_ptr_relative_edit_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(base_ptr_relative_edit_block_array_index *Thing1, base_ptr_relative_edit_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( base_ptr_relative_edit_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(base_ptr_relative_edit_block_array_index Thing1, base_ptr_relative_edit_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( base_ptr_relative_edit_block_array_index ) );

  return Result;
}


typedef base_ptr_relative_edit_block_array base_ptr_relative_edit_paged_list;

link_internal base_ptr_relative_edit_block_array_index
operator++( base_ptr_relative_edit_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( base_ptr_relative_edit_block_array_index I0, base_ptr_relative_edit_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( base_ptr_relative_edit_block_array_index I0, base_ptr_relative_edit_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( base_ptr_relative_edit_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal base_ptr_relative_edit_block_array_index
ZerothIndex( base_ptr_relative_edit_block_array *Arr )
{
  return {};
}

link_internal base_ptr_relative_edit_block_array_index
Capacity( base_ptr_relative_edit_block_array *Arr )
{
  base_ptr_relative_edit_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal base_ptr_relative_edit_block_array_index
AtElements( base_ptr_relative_edit_block_array *Arr )
{
  base_ptr_relative_edit_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( base_ptr_relative_edit_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal base_ptr_relative_edit_block_array_index
LastIndex( base_ptr_relative_edit_block_array *Arr )
{
  base_ptr_relative_edit_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( base_ptr_relative_edit_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal base_ptr_relative_edit_block *
GetBlock( base_ptr_relative_edit_block_array *Arr, base_ptr_relative_edit_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  base_ptr_relative_edit_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal base_ptr_relative_edit *
GetPtr( base_ptr_relative_edit_block_array *Arr, base_ptr_relative_edit_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < AtElements(Arr).Index);
  Assert(Index.Index < Capacity(Arr).Index);

  base_ptr_relative_edit_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  base_ptr_relative_edit *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal base_ptr_relative_edit *
GetPtr( base_ptr_relative_edit_block_array *Arr, umm Index )
{
  base_ptr_relative_edit_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal base_ptr_relative_edit *
TryGetPtr( base_ptr_relative_edit_block_array *Arr, base_ptr_relative_edit_block_array_index Index)
{
  base_ptr_relative_edit * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal base_ptr_relative_edit *
TryGetPtr( base_ptr_relative_edit_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, base_ptr_relative_edit_block_array_index{Index});
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

