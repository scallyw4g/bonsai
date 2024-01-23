struct bitmap_block
{
  u32 Index;
  u32 At;
  bitmap *Elements;
  bitmap_block *Next;
};

struct bitmap_block_array_index
{
  bitmap_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct bitmap_block_array
{
  bitmap_block *First;
  bitmap_block *Current;
  memory_arena *Memory;
};

link_internal bitmap_block_array_index
operator++(bitmap_block_array_index &I0)
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
operator<(bitmap_block_array_index I0, bitmap_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(bitmap_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal bitmap_block_array_index
ZerothIndex(bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
  return Result;
}

link_internal umm
TotalElements(bitmap_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal bitmap_block_array_index
LastIndex(bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
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

link_internal bitmap_block_array_index
AtElements(bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block_array *Arr, bitmap_block_array_index Index)
{
  bitmap *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block *Block, umm Index)
{
  bitmap *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  bitmap_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  bitmap *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal u32
AtElements(bitmap_block *Block)
{
  return Block->At;
}


link_internal bitmap_block*
Allocate_bitmap_block(memory_arena *Memory)
{
  bitmap_block *Result = Allocate(bitmap_block, Memory, 1);
  Result->Elements = Allocate(bitmap, Memory, 8);
  return Result;
}

link_internal cs
CS(bitmap_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(bitmap_block_array *Array, bitmap_block_array_index Index)
{
  bitmap_block_array_index LastI = LastIndex(Array);

  bitmap *Element = GetPtr(Array, Index);
  bitmap *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    bitmap_block *Current = Array->First;
    bitmap_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal bitmap *
Push(bitmap_block_array *Array, bitmap *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_bitmap_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      bitmap_block *Next = Allocate_bitmap_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  bitmap *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


