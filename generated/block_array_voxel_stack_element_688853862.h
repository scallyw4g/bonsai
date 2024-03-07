// src/engine/world_chunk.cpp:4000:0

struct voxel_stack_element_block
{
  u32 Index;
  u32 At;
  voxel_stack_element *Elements;
  voxel_stack_element_block *Next;
};

struct voxel_stack_element_block_array_index
{
  voxel_stack_element_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct voxel_stack_element_block_array
{
  voxel_stack_element_block *First;
  voxel_stack_element_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal voxel_stack_element_block_array_index
operator++(voxel_stack_element_block_array_index &I0)
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
operator<(voxel_stack_element_block_array_index I0, voxel_stack_element_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(voxel_stack_element_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal voxel_stack_element_block_array_index
ZerothIndex(voxel_stack_element_block_array *Arr)
{
  voxel_stack_element_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
  return Result;
}

link_internal umm
TotalElements(voxel_stack_element_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal voxel_stack_element_block_array_index
LastIndex(voxel_stack_element_block_array *Arr)
{
  voxel_stack_element_block_array_index Result = {};
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

link_internal voxel_stack_element_block_array_index
AtElements(voxel_stack_element_block_array *Arr)
{
  voxel_stack_element_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal voxel_stack_element *
GetPtr(voxel_stack_element_block_array *Arr, voxel_stack_element_block_array_index Index)
{
  voxel_stack_element *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal voxel_stack_element *
GetPtr(voxel_stack_element_block *Block, umm Index)
{
  voxel_stack_element *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal voxel_stack_element *
GetPtr(voxel_stack_element_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  voxel_stack_element_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  voxel_stack_element *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal u32
AtElements(voxel_stack_element_block *Block)
{
  return Block->At;
}


link_internal voxel_stack_element_block*
Allocate_voxel_stack_element_block(memory_arena *Memory)
{
  voxel_stack_element_block *Result = Allocate(voxel_stack_element_block, Memory, 1);
  Result->Elements = Allocate(voxel_stack_element, Memory, 8);
  return Result;
}

link_internal cs
CS(voxel_stack_element_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(voxel_stack_element_block_array *Array, voxel_stack_element_block_array_index Index)
{
  voxel_stack_element_block_array_index LastI = LastIndex(Array);

  voxel_stack_element *Element = GetPtr(Array, Index);
  voxel_stack_element *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    voxel_stack_element_block *Current = Array->First;
    voxel_stack_element_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal voxel_stack_element *
Push(voxel_stack_element_block_array *Array, voxel_stack_element *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_voxel_stack_element_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      voxel_stack_element_block *Next = Allocate_voxel_stack_element_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  voxel_stack_element *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


