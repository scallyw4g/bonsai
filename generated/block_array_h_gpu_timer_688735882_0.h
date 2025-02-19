// src/engine/render/gpu_timer.h:9:0





struct gpu_timer_block
{
  u32 Index;
  u32 At;
  gpu_timer *Elements;
  gpu_timer_block *Next;
};

struct gpu_timer_block_array_index
{
  gpu_timer_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct gpu_timer_block_array
{
  gpu_timer_block *First;
  gpu_timer_block *Current;
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
  if (I0.Block)
  {
    if (I0.ElementIndex == 128-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = I0.Block->Next;
    }
    else
    {
      I0.ElementIndex++;
    }
  }
  else
  {
    I0.ElementIndex++;
  }
  return I0;
}

link_internal b32
operator<( gpu_timer_block_array_index I0, gpu_timer_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( gpu_timer_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*128);
  return Result;
}

link_internal gpu_timer_block_array_index
ZerothIndex( gpu_timer_block_array *Arr)
{
  gpu_timer_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( gpu_timer_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 128) + Arr->Current->At;
  }
  return Result;
}

link_internal gpu_timer_block_array_index
LastIndex( gpu_timer_block_array *Arr)
{
  gpu_timer_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
    Assert(Result.ElementIndex);
    Result.ElementIndex--;
  }
  return Result;
}

link_internal gpu_timer_block_array_index
AtElements( gpu_timer_block_array *Arr)
{
  gpu_timer_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( gpu_timer_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal gpu_timer *
GetPtr(gpu_timer_block_array *Arr, gpu_timer_block_array_index Index)
{
  gpu_timer *Result = {};
  if (Index.Block) { Result = (Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal gpu_timer *
GetPtr(gpu_timer_block *Block, umm Index)
{
  gpu_timer *Result = {};
  if (Index < Block->At) { Result = (Block->Elements + Index); }
  return Result;
}

link_internal gpu_timer *
GetPtr(gpu_timer_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 128;
  umm ElementIndex = Index % 128;

  umm AtBlock = 0;
  gpu_timer_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  gpu_timer *Result = (Block->Elements+ElementIndex);
  return Result;
}

link_internal gpu_timer *
TryGetPtr(gpu_timer_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 128;
  umm ElementIndex = Index % 128;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  gpu_timer *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(gpu_timer_block *Block)
{
  return Block->At;
}

