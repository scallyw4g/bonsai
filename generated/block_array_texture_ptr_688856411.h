struct texture_ptr_block
{
  u32 Index;
  u32 At;
  texture_ptr *Elements;
  texture_ptr_block *Next;
};

struct texture_ptr_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct texture_ptr_block_array
{
  texture_ptr_block First;
  texture_ptr_block *Current;
  memory_arena *Memory;
};

link_internal texture_ptr_block_array_index
operator++(texture_ptr_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(texture_ptr_block*, I0.Block)->Next;
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
operator<(texture_ptr_block_array_index I0, texture_ptr_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(texture_ptr_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal texture_ptr_block_array_index
ZerothIndex(texture_ptr_block_array *Arr)
{
  texture_ptr_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(Cast(texture_ptr_block*, Result.Block)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(texture_ptr_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal texture_ptr_block_array_index
AtElements(texture_ptr_block_array *Arr)
{
  texture_ptr_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Cast(texture_ptr_block*, Arr->Current)->Index;
    Result.ElementIndex = Cast(texture_ptr_block*, Arr->Current)->At;
  }
  return Result;
}

link_internal texture_ptr *
GetPtr(texture_ptr_block_array *Arr, texture_ptr_block_array_index Index)
{
  texture_ptr *Result = {};
  if (Index.Block) { Result = Cast(texture_ptr_block *, Index.Block)->Elements + Index.ElementIndex; }
  return Result;
}

link_internal texture_ptr *
GetPtr(texture_ptr_block *Block, umm Index)
{
  texture_ptr *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal texture_ptr *
GetPtr(texture_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  texture_ptr_block *Block = &Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  texture_ptr *Result = Block->Elements+ElementIndex;
  return Result;
}


link_internal u32
AtElements(texture_ptr_block *Block)
{
  return Block->At;
}


link_internal texture_ptr_block*
Allocate_texture_ptr_block(memory_arena *Memory)
{
  texture_ptr_block *Result = Allocate(texture_ptr_block, Memory, 1);
  Result->Elements = Allocate(texture_ptr, Memory, 8);
  return Result;
}

link_internal texture_ptr *
Push(texture_ptr_block_array *Array, texture_ptr *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_texture_ptr_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    texture_ptr_block *Next = Allocate_texture_ptr_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  texture_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


