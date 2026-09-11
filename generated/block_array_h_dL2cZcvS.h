// callsite
// src/engine/editor.h:1932:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2310:0




struct edit_record_block
{
  /* u32 Index; */
  umm At;
  edit_record Elements[32];
};


struct edit_record_block_array_index
{
  umm Index; 
};

struct edit_record_block_array
poof(
  @collection
   @serdes 
   @do_editor_ui 
)
{
  edit_record_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal edit_record_block_array
EditRecordBlockArray(memory_arena *Memory)
{
  edit_record_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(edit_record_block_array_index *Thing1, edit_record_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( edit_record_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(edit_record_block_array_index Thing1, edit_record_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( edit_record_block_array_index ) );

  return Result;
}


typedef edit_record_block_array edit_record_paged_list;

link_internal edit_record_block_array_index
operator++( edit_record_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( edit_record_block_array_index I0, edit_record_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( edit_record_block_array_index I0, edit_record_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( edit_record_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal edit_record_block_array_index
ZerothIndex( edit_record_block_array *Arr )
{
  return {};
}

link_internal edit_record_block_array_index
Capacity( edit_record_block_array *Arr )
{
  edit_record_block_array_index Result = {Arr->BlockCount * 32};
  return Result;
}

link_internal edit_record_block_array_index
AtElements( edit_record_block_array *Arr )
{
  edit_record_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( edit_record_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal edit_record_block_array_index
LastIndex( edit_record_block_array *Arr )
{
  edit_record_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( edit_record_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal edit_record_block *
GetBlock( edit_record_block_array *Arr, edit_record_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 32;
  Assert(BlockIndex < Arr->BlockCount);
  edit_record_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal edit_record *
GetPtr( edit_record_block_array *Arr, edit_record_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  edit_record_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 32;
  edit_record *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal edit_record *
GetPtr( edit_record_block_array *Arr, umm Index )
{
  edit_record_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal edit_record *
TryGetPtr( edit_record_block_array *Arr, edit_record_block_array_index Index)
{
  edit_record * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal edit_record *
TryGetPtr( edit_record_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, edit_record_block_array_index{Index});
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

