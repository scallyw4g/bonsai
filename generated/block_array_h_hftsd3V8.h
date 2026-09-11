// callsite
// src/engine/editor.h:2182:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2310:0




struct undo_record_block
{
  /* u32 Index; */
  umm At;
  undo_record Elements[64];
};


struct undo_record_block_array_index
{
  umm Index; 
};

struct undo_record_block_array
poof(
  @collection
  
  
)
{
  undo_record_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal undo_record_block_array
UndoRecordBlockArray(memory_arena *Memory)
{
  undo_record_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(undo_record_block_array_index *Thing1, undo_record_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( undo_record_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(undo_record_block_array_index Thing1, undo_record_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( undo_record_block_array_index ) );

  return Result;
}


typedef undo_record_block_array undo_record_paged_list;

link_internal undo_record_block_array_index
operator++( undo_record_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( undo_record_block_array_index I0, undo_record_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( undo_record_block_array_index I0, undo_record_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( undo_record_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal undo_record_block_array_index
ZerothIndex( undo_record_block_array *Arr )
{
  return {};
}

link_internal undo_record_block_array_index
Capacity( undo_record_block_array *Arr )
{
  undo_record_block_array_index Result = {Arr->BlockCount * 64};
  return Result;
}

link_internal undo_record_block_array_index
AtElements( undo_record_block_array *Arr )
{
  undo_record_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( undo_record_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal undo_record_block_array_index
LastIndex( undo_record_block_array *Arr )
{
  undo_record_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( undo_record_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal undo_record_block *
GetBlock( undo_record_block_array *Arr, undo_record_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 64;
  Assert(BlockIndex < Arr->BlockCount);
  undo_record_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal undo_record *
GetPtr( undo_record_block_array *Arr, undo_record_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  undo_record_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 64;
  undo_record *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal undo_record *
GetPtr( undo_record_block_array *Arr, umm Index )
{
  undo_record_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal undo_record *
TryGetPtr( undo_record_block_array *Arr, undo_record_block_array_index Index)
{
  undo_record * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal undo_record *
TryGetPtr( undo_record_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, undo_record_block_array_index{Index});
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

