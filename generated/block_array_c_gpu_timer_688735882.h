// src/engine/render/gpu_timer.cpp:1:0


link_internal gpu_timer_block*
Allocate_gpu_timer_block(memory_arena *Memory)
{
  gpu_timer_block *Result = Allocate(gpu_timer_block, Memory, 1);
  Result->Elements = Allocate(gpu_timer, Memory, 128);
  return Result;
}

link_internal cs
CS(gpu_timer_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(gpu_timer_block_array *Array, gpu_timer_block_array_index Index)
{
  gpu_timer_block_array_index LastI = LastIndex(Array);

  gpu_timer *Element = GetPtr(Array, Index);
  gpu_timer *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

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
      gpu_timer_block *Current = Array->First;
      gpu_timer_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal gpu_timer *
Push(gpu_timer_block_array *Array, gpu_timer *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_gpu_timer_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 128)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      gpu_timer_block *Next = Allocate_gpu_timer_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  gpu_timer *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}
