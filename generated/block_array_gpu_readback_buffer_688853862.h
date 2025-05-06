// src/engine/graphics.h:119:0





struct gpu_readback_buffer_block
{
  u32 Index;
  u32 At;
  gpu_readback_buffer *Elements;
  gpu_readback_buffer_block *Next;
};

struct gpu_readback_buffer_block_array_index
{
  gpu_readback_buffer_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct gpu_readback_buffer_block_array
{
  gpu_readback_buffer_block *First;
  gpu_readback_buffer_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal gpu_readback_buffer_block_array
GpuReadbackBufferBlockArray(memory_arena *Memory)
{
  gpu_readback_buffer_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(gpu_readback_buffer_block_array_index *Thing1, gpu_readback_buffer_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( gpu_readback_buffer_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(gpu_readback_buffer_block_array_index Thing1, gpu_readback_buffer_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( gpu_readback_buffer_block_array_index ) );

  return Result;
}


typedef gpu_readback_buffer_block_array gpu_readback_buffer_paged_list;

link_internal gpu_readback_buffer_block_array_index
operator++( gpu_readback_buffer_block_array_index &I0 )
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
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
operator<( gpu_readback_buffer_block_array_index I0, gpu_readback_buffer_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( gpu_readback_buffer_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal gpu_readback_buffer_block_array_index
ZerothIndex( gpu_readback_buffer_block_array *Arr)
{
  gpu_readback_buffer_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( gpu_readback_buffer_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal gpu_readback_buffer_block_array_index
LastIndex( gpu_readback_buffer_block_array *Arr)
{
  gpu_readback_buffer_block_array_index Result = {};
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

link_internal gpu_readback_buffer_block_array_index
AtElements( gpu_readback_buffer_block_array *Arr)
{
  gpu_readback_buffer_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( gpu_readback_buffer_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal gpu_readback_buffer *
GetPtr(gpu_readback_buffer_block_array *Arr, gpu_readback_buffer_block_array_index Index)
{
  gpu_readback_buffer *Result = {};
  if (Index.Block) { Result = (Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal gpu_readback_buffer *
GetPtr(gpu_readback_buffer_block *Block, umm Index)
{
  gpu_readback_buffer *Result = {};
  if (Index < Block->At) { Result = (Block->Elements + Index); }
  return Result;
}

link_internal gpu_readback_buffer *
GetPtr(gpu_readback_buffer_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  gpu_readback_buffer_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  gpu_readback_buffer *Result = (Block->Elements+ElementIndex);
  return Result;
}

link_internal gpu_readback_buffer *
TryGetPtr(gpu_readback_buffer_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  gpu_readback_buffer *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(gpu_readback_buffer_block *Block)
{
  return Block->At;
}





link_internal gpu_readback_buffer_block *
Allocate_gpu_readback_buffer_block(memory_arena *Memory)
{
  gpu_readback_buffer_block *Result = Allocate( gpu_readback_buffer_block, Memory, 1);
  Result->Elements = Allocate( gpu_readback_buffer, Memory, 8);
  return Result;
}

link_internal cs
CS( gpu_readback_buffer_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal gpu_readback_buffer *
Set( gpu_readback_buffer_block_array *Arr,
  gpu_readback_buffer *Element,
  gpu_readback_buffer_block_array_index Index )
{
  gpu_readback_buffer *Result = {};
  if (Index.Block)
  {
    gpu_readback_buffer *Slot = &Index.Block->Elements[Index.ElementIndex];
    *Slot = *Element;

    Result = Slot;
  }

  return Result;
}

link_internal void
RemoveUnordered( gpu_readback_buffer_block_array *Array, gpu_readback_buffer_block_array_index Index)
{
  gpu_readback_buffer_block_array_index LastI = LastIndex(Array);

  gpu_readback_buffer *Element = GetPtr(Array, Index);
  gpu_readback_buffer *LastElement = GetPtr(Array, LastI);

  Set(Array, LastElement, Index);

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // TODO(Jesse): There's obviously a way better way to do this ..
    auto AtE = AtElements(Array);
    s32 Count = s32(GetIndex(&AtE));

    if (Count == 0)
    {
      // Nothing to be done, we've popping the last thing off the array
      Assert(Index.Block == Array->First);
      Assert(Index.Block == Array->Current);
      Assert(Index.BlockIndex == 0);
      Assert(Index.ElementIndex == 0);
    }
    else
    {
      // Walk the chain till we get to the second-last one
      gpu_readback_buffer_block *Current = Array->First;
      gpu_readback_buffer_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal gpu_readback_buffer_block_array_index
Find( gpu_readback_buffer_block_array *Array, gpu_readback_buffer *Query)
{
  gpu_readback_buffer_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
  IterateOver(Array, E, Index)
  {
    if ( E == Query)
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

link_internal b32
IsValid(gpu_readback_buffer_block_array_index *Index)
{
  gpu_readback_buffer_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal gpu_readback_buffer *
Push( gpu_readback_buffer_block_array *Array, gpu_readback_buffer *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_gpu_readback_buffer_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      gpu_readback_buffer_block *Next = Allocate_gpu_readback_buffer_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  gpu_readback_buffer *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

link_internal gpu_readback_buffer *
Push( gpu_readback_buffer_block_array *Array )
{
  gpu_readback_buffer Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}


