struct texture_ptr_block
{
  u32 Index;
  u32 At;
  texture_ptr *Elements;
  texture_ptr_block *Next;
};

struct texture_ptr_block_array_index
{
  texture_ptr_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct texture_ptr_block_array
{
  texture_ptr_block *First;
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
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
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
LastIndex(texture_ptr_block_array *Arr)
{
  texture_ptr_block_array_index Result = {};
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

link_internal texture_ptr_block_array_index
AtElements(texture_ptr_block_array *Arr)
{
  texture_ptr_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal texture_ptr *
GetPtr(texture_ptr_block_array *Arr, texture_ptr_block_array_index Index)
{
  texture_ptr *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
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
  texture_ptr_block *Block = Arr->First;
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

link_internal cs
CS(texture_ptr_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(texture_ptr_block_array *Array, texture_ptr_block_array_index Index)
{
  texture_ptr_block_array_index LastI = LastIndex(Array);

  texture_ptr *Element = GetPtr(Array, Index);
  texture_ptr *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    texture_ptr_block *Current = Array->First;
    texture_ptr_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal texture_ptr *
Push(texture_ptr_block_array *Array, texture_ptr *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_texture_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      texture_ptr_block *Next = Allocate_texture_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  texture_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


