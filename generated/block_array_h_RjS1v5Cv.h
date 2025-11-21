// callsite
// external/bonsai_stdlib/src/framebuffer.h:23:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2278:0




struct rtt_framebuffer_block
{
  /* u32 Index; */
  umm At;
  rtt_framebuffer Elements[8];
};


struct rtt_framebuffer_block_array_index
{
  umm Index; 
};

struct rtt_framebuffer_block_array
{
  rtt_framebuffer_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal rtt_framebuffer_block_array
RttFramebufferBlockArray(memory_arena *Memory)
{
  rtt_framebuffer_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}



  typedef rtt_framebuffer_block_array rtt_framebuffer_paged_list;

  link_internal rtt_framebuffer_block_array_index
  operator++( rtt_framebuffer_block_array_index &I0 )
  {
    I0.Index++;
    return I0;
  }

  link_internal b32
  operator<( rtt_framebuffer_block_array_index I0, rtt_framebuffer_block_array_index I1 )
  {
    b32 Result = I0.Index < I1.Index;
    return Result;
  }

  link_internal b32
  operator==( rtt_framebuffer_block_array_index I0, rtt_framebuffer_block_array_index I1 )
  {
    b32 Result = I0.Index == I1.Index;
    return Result;
  }

  link_inline umm
  GetIndex( rtt_framebuffer_block_array_index *Index)
  {
    umm Result = Index->Index;
    return Result;
  }

  
  link_internal rtt_framebuffer_block_array_index
  ZerothIndex( rtt_framebuffer_block_array *Arr )
  {
    return {};
  }

  link_internal rtt_framebuffer_block_array_index
  Capacity( rtt_framebuffer_block_array *Arr )
  {
    rtt_framebuffer_block_array_index Result = {Arr->BlockCount * 8};
    return Result;
  }

  link_internal rtt_framebuffer_block_array_index
  AtElements( rtt_framebuffer_block_array *Arr )
  {
    rtt_framebuffer_block_array_index Result = {Arr->ElementCount};
    return Result;
  }

  
  link_internal umm
  TotalElements( rtt_framebuffer_block_array *Arr )
  {
    umm Result = AtElements(Arr).Index;
    return Result;
  }

  
  link_internal rtt_framebuffer_block_array_index
  LastIndex( rtt_framebuffer_block_array *Arr )
  {
    rtt_framebuffer_block_array_index Result = {};
    umm Count = AtElements(Arr).Index;
    if (Count) Result.Index = Count-1;
    return Result;
  }

  link_internal umm
  Count( rtt_framebuffer_block_array *Arr )
  {
    auto Result = AtElements(Arr).Index;
    return Result;
  }

  link_internal rtt_framebuffer_block *
  GetBlock( rtt_framebuffer_block_array *Arr, rtt_framebuffer_block_array_index Index )
  {
    umm BlockIndex   = Index.Index / 8;
    Assert(BlockIndex < Arr->BlockCount);
    rtt_framebuffer_block *Block = Arr->BlockPtrs[BlockIndex];
    return Block;
  }

  link_internal rtt_framebuffer *
  GetPtr( rtt_framebuffer_block_array *Arr, rtt_framebuffer_block_array_index Index )
  {
    Assert(Arr->BlockPtrs);
    Assert(Index.Index < Capacity(Arr).Index);

    rtt_framebuffer_block *Block = GetBlock(Arr, Index);

    umm ElementIndex = Index.Index % 8;
    rtt_framebuffer *Result = (Block->Elements + ElementIndex);
    return Result;
  }


  link_internal rtt_framebuffer *
  GetPtr( rtt_framebuffer_block_array *Arr, umm Index )
  {
    rtt_framebuffer_block_array_index I = {Index};
    return GetPtr(Arr, I);
  }


  link_internal rtt_framebuffer *
  TryGetPtr( rtt_framebuffer_block_array *Arr, rtt_framebuffer_block_array_index Index)
  {
    rtt_framebuffer * Result = {};
    if (Arr->BlockPtrs && Index < AtElements(Arr))
    {
      Result = GetPtr(Arr, Index);
    }
    return Result;
  }

  link_internal rtt_framebuffer *
  TryGetPtr( rtt_framebuffer_block_array *Arr, umm Index)
  {
    auto Result = TryGetPtr(Arr, rtt_framebuffer_block_array_index{Index});
    return Result;
  }

