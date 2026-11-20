// callsite
// external/bonsai_stdlib/src/ui/ui.h:427:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2395:0




struct ui_render_op_block
{
  ui_render_op Elements[1024];
};


struct ui_render_op_block_array_index
{
  umm Index; 
};

struct ui_render_op_block_array
poof(
  @collection
  
  
)
{
  ui_render_op_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal ui_render_op_block_array
UiRenderOpBlockArray(memory_arena *Memory)
{
  ui_render_op_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(ui_render_op_block_array_index *Thing1, ui_render_op_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( ui_render_op_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(ui_render_op_block_array_index Thing1, ui_render_op_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( ui_render_op_block_array_index ) );

  return Result;
}


typedef ui_render_op_block_array ui_render_op_paged_list;

link_internal ui_render_op_block_array_index
operator++( ui_render_op_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( ui_render_op_block_array_index I0, ui_render_op_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( ui_render_op_block_array_index I0, ui_render_op_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( ui_render_op_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal ui_render_op_block_array_index
ZerothIndex( ui_render_op_block_array *Arr )
{
  return {};
}

link_internal ui_render_op_block_array_index
Capacity( ui_render_op_block_array *Arr )
{
  ui_render_op_block_array_index Result = {Arr->BlockCount * 1024};
  return Result;
}

link_internal ui_render_op_block_array_index
AtElements( ui_render_op_block_array *Arr )
{
  ui_render_op_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( ui_render_op_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal ui_render_op_block_array_index
LastIndex( ui_render_op_block_array *Arr )
{
  ui_render_op_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( ui_render_op_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal ui_render_op_block *
GetBlock( ui_render_op_block_array *Arr, ui_render_op_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 1024;
  Assert(BlockIndex < Arr->BlockCount);
  ui_render_op_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal ui_render_op *
GetPtr( ui_render_op_block_array *Arr, ui_render_op_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < AtElements(Arr).Index);
  Assert(Index.Index < Capacity(Arr).Index);

  ui_render_op_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 1024;
  ui_render_op *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal ui_render_op *
GetPtr( ui_render_op_block_array *Arr, umm Index )
{
  ui_render_op_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal ui_render_op *
TryGetPtr( ui_render_op_block_array *Arr, ui_render_op_block_array_index Index)
{
  ui_render_op * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal ui_render_op *
TryGetPtr( ui_render_op_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, ui_render_op_block_array_index{Index});
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

