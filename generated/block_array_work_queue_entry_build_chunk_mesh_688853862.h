// src/engine/work_queue.h:73:0

struct work_queue_entry_build_chunk_mesh_block
{
  u32 Index;
  u32 At;
  work_queue_entry_build_chunk_mesh *Elements;
  work_queue_entry_build_chunk_mesh_block *Next;
};

struct work_queue_entry_build_chunk_mesh_block_array_index
{
  work_queue_entry_build_chunk_mesh_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct work_queue_entry_build_chunk_mesh_block_array
{
  work_queue_entry_build_chunk_mesh_block *First;
  work_queue_entry_build_chunk_mesh_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

typedef work_queue_entry_build_chunk_mesh_block_array work_queue_entry_build_chunk_mesh_paged_list;

link_internal work_queue_entry_build_chunk_mesh_block_array_index
operator++(work_queue_entry_build_chunk_mesh_block_array_index &I0)
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
operator<(work_queue_entry_build_chunk_mesh_block_array_index I0, work_queue_entry_build_chunk_mesh_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(work_queue_entry_build_chunk_mesh_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh_block_array_index
ZerothIndex(work_queue_entry_build_chunk_mesh_block_array *Arr)
{
  work_queue_entry_build_chunk_mesh_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
  return Result;
}

link_internal umm
TotalElements(work_queue_entry_build_chunk_mesh_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh_block_array_index
LastIndex(work_queue_entry_build_chunk_mesh_block_array *Arr)
{
  work_queue_entry_build_chunk_mesh_block_array_index Result = {};
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

link_internal work_queue_entry_build_chunk_mesh_block_array_index
AtElements(work_queue_entry_build_chunk_mesh_block_array *Arr)
{
  work_queue_entry_build_chunk_mesh_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh *
GetPtr(work_queue_entry_build_chunk_mesh_block_array *Arr, work_queue_entry_build_chunk_mesh_block_array_index Index)
{
  work_queue_entry_build_chunk_mesh *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh *
GetPtr(work_queue_entry_build_chunk_mesh_block *Block, umm Index)
{
  work_queue_entry_build_chunk_mesh *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh *
GetPtr(work_queue_entry_build_chunk_mesh_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  work_queue_entry_build_chunk_mesh_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  work_queue_entry_build_chunk_mesh *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal work_queue_entry_build_chunk_mesh *
TryGetPtr(work_queue_entry_build_chunk_mesh_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  work_queue_entry_build_chunk_mesh *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(work_queue_entry_build_chunk_mesh_block *Block)
{
  return Block->At;
}


link_internal work_queue_entry_build_chunk_mesh_block*
Allocate_work_queue_entry_build_chunk_mesh_block(memory_arena *Memory)
{
  work_queue_entry_build_chunk_mesh_block *Result = Allocate(work_queue_entry_build_chunk_mesh_block, Memory, 1);
  Result->Elements = Allocate(work_queue_entry_build_chunk_mesh, Memory, 8);
  return Result;
}

link_internal cs
CS(work_queue_entry_build_chunk_mesh_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(work_queue_entry_build_chunk_mesh_block_array *Array, work_queue_entry_build_chunk_mesh_block_array_index Index)
{
  work_queue_entry_build_chunk_mesh_block_array_index LastI = LastIndex(Array);

  work_queue_entry_build_chunk_mesh *Element = GetPtr(Array, Index);
  work_queue_entry_build_chunk_mesh *LastElement = GetPtr(Array, LastI);

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
      work_queue_entry_build_chunk_mesh_block *Current = Array->First;
      work_queue_entry_build_chunk_mesh_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal work_queue_entry_build_chunk_mesh *
Push(work_queue_entry_build_chunk_mesh_block_array *Array, work_queue_entry_build_chunk_mesh *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_work_queue_entry_build_chunk_mesh_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      work_queue_entry_build_chunk_mesh_block *Next = Allocate_work_queue_entry_build_chunk_mesh_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  work_queue_entry_build_chunk_mesh *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


