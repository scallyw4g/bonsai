// callsite
// external/bonsai_stdlib/src/work_queue.h:38:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2364:0




struct global_job_index_block
{
  global_job_index Elements[64];
};


struct global_job_index_block_array_index
{
  umm Index; 
};

struct global_job_index_block_array
poof(
  @collection
  
  
)
{
  global_job_index_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal global_job_index_block_array
GlobalJobIndexBlockArray(memory_arena *Memory)
{
  global_job_index_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(global_job_index_block_array_index *Thing1, global_job_index_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( global_job_index_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(global_job_index_block_array_index Thing1, global_job_index_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( global_job_index_block_array_index ) );

  return Result;
}


typedef global_job_index_block_array global_job_index_paged_list;

link_internal global_job_index_block_array_index
operator++( global_job_index_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( global_job_index_block_array_index I0, global_job_index_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( global_job_index_block_array_index I0, global_job_index_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( global_job_index_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal global_job_index_block_array_index
ZerothIndex( global_job_index_block_array *Arr )
{
  return {};
}

link_internal global_job_index_block_array_index
Capacity( global_job_index_block_array *Arr )
{
  global_job_index_block_array_index Result = {Arr->BlockCount * 64};
  return Result;
}

link_internal global_job_index_block_array_index
AtElements( global_job_index_block_array *Arr )
{
  global_job_index_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( global_job_index_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal global_job_index_block_array_index
LastIndex( global_job_index_block_array *Arr )
{
  global_job_index_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( global_job_index_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal global_job_index_block *
GetBlock( global_job_index_block_array *Arr, global_job_index_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 64;
  Assert(BlockIndex < Arr->BlockCount);
  global_job_index_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal global_job_index *
GetPtr( global_job_index_block_array *Arr, global_job_index_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < AtElements(Arr).Index);
  Assert(Index.Index < Capacity(Arr).Index);

  global_job_index_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 64;
  global_job_index *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal global_job_index *
GetPtr( global_job_index_block_array *Arr, umm Index )
{
  global_job_index_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal global_job_index *
TryGetPtr( global_job_index_block_array *Arr, global_job_index_block_array_index Index)
{
  global_job_index * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal global_job_index *
TryGetPtr( global_job_index_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, global_job_index_block_array_index{Index});
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

