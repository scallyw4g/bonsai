// callsite
// src/engine/render/gpu_timer.h:9:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct gpu_timer_block
{
  /* u32 Index; */
  umm At;
  gpu_timer Elements[128];
};


struct gpu_timer_block_array_index
{
  umm Index; 
};

struct gpu_timer_block_array
poof(
  @collection
  
  
)
{
  gpu_timer_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal gpu_timer_block_array
GpuTimerBlockArray(memory_arena *Memory)
{
  gpu_timer_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(gpu_timer_block_array_index *Thing1, gpu_timer_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( gpu_timer_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(gpu_timer_block_array_index Thing1, gpu_timer_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( gpu_timer_block_array_index ) );

  return Result;
}


typedef gpu_timer_block_array gpu_timer_paged_list;

link_internal gpu_timer_block_array_index
operator++( gpu_timer_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( gpu_timer_block_array_index I0, gpu_timer_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( gpu_timer_block_array_index I0, gpu_timer_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( gpu_timer_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal gpu_timer_block_array_index
ZerothIndex( gpu_timer_block_array *Arr )
{
  return {};
}

link_internal gpu_timer_block_array_index
Capacity( gpu_timer_block_array *Arr )
{
  gpu_timer_block_array_index Result = {Arr->BlockCount * 128};
  return Result;
}

link_internal gpu_timer_block_array_index
AtElements( gpu_timer_block_array *Arr )
{
  gpu_timer_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( gpu_timer_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal gpu_timer_block_array_index
LastIndex( gpu_timer_block_array *Arr )
{
  gpu_timer_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( gpu_timer_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal gpu_timer_block *
GetBlock( gpu_timer_block_array *Arr, gpu_timer_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 128;
  Assert(BlockIndex < Arr->BlockCount);
  gpu_timer_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal gpu_timer *
GetPtr( gpu_timer_block_array *Arr, gpu_timer_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  gpu_timer_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 128;
  gpu_timer *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal gpu_timer *
GetPtr( gpu_timer_block_array *Arr, umm Index )
{
  gpu_timer_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal gpu_timer *
TryGetPtr( gpu_timer_block_array *Arr, gpu_timer_block_array_index Index)
{
  gpu_timer * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal gpu_timer *
TryGetPtr( gpu_timer_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, gpu_timer_block_array_index{Index});
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

