// callsite
// src/engine/editor.h:1023:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct asset_thumbnail_block
{
  /* u32 Index; */
  umm At;
  asset_thumbnail Elements[8];
};


struct asset_thumbnail_block_array_index
{
  umm Index; 
};

struct asset_thumbnail_block_array
poof(
  @collection
  
   @do_editor_ui 
)
{
  asset_thumbnail_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal asset_thumbnail_block_array
AssetThumbnailBlockArray(memory_arena *Memory)
{
  asset_thumbnail_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(asset_thumbnail_block_array_index *Thing1, asset_thumbnail_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( asset_thumbnail_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(asset_thumbnail_block_array_index Thing1, asset_thumbnail_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( asset_thumbnail_block_array_index ) );

  return Result;
}


typedef asset_thumbnail_block_array asset_thumbnail_paged_list;

link_internal asset_thumbnail_block_array_index
operator++( asset_thumbnail_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( asset_thumbnail_block_array_index I0, asset_thumbnail_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( asset_thumbnail_block_array_index I0, asset_thumbnail_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( asset_thumbnail_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal asset_thumbnail_block_array_index
ZerothIndex( asset_thumbnail_block_array *Arr )
{
  return {};
}

link_internal asset_thumbnail_block_array_index
Capacity( asset_thumbnail_block_array *Arr )
{
  asset_thumbnail_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal asset_thumbnail_block_array_index
AtElements( asset_thumbnail_block_array *Arr )
{
  asset_thumbnail_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( asset_thumbnail_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal asset_thumbnail_block_array_index
LastIndex( asset_thumbnail_block_array *Arr )
{
  asset_thumbnail_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( asset_thumbnail_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal asset_thumbnail_block *
GetBlock( asset_thumbnail_block_array *Arr, asset_thumbnail_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  asset_thumbnail_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal asset_thumbnail *
GetPtr( asset_thumbnail_block_array *Arr, asset_thumbnail_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  asset_thumbnail_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  asset_thumbnail *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal asset_thumbnail *
GetPtr( asset_thumbnail_block_array *Arr, umm Index )
{
  asset_thumbnail_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal asset_thumbnail *
TryGetPtr( asset_thumbnail_block_array *Arr, asset_thumbnail_block_array_index Index)
{
  asset_thumbnail * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal asset_thumbnail *
TryGetPtr( asset_thumbnail_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, asset_thumbnail_block_array_index{Index});
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

