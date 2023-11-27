struct texture_block
{
  u32 Index;
  u32 At;
  texture *Elements;
  texture_block *Next;
};

struct texture_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct texture_block_array
{
  texture_block First;
  texture_block *Current;
  memory_arena *Memory;
};

link_internal texture_block_array_index
operator++(texture_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(texture_block*, I0.Block)->Next;
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
operator<(texture_block_array_index I0, texture_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(texture_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal texture_block_array_index
ZerothIndex(texture_block_array *Arr)
{
  texture_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(Cast(texture_block*, Result.Block)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(texture_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal texture_block_array_index
AtElements(texture_block_array *Arr)
{
  texture_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Cast(texture_block*, Arr->Current)->Index;
    Result.ElementIndex = Cast(texture_block*, Arr->Current)->At;
  }
  return Result;
}

link_internal texture *
GetPtr(texture_block_array *Arr, texture_block_array_index Index)
{
  texture *Result = {};
  if (Index.Block) { Result = Cast(texture_block *, Index.Block)->Elements + Index.ElementIndex; }
  return Result;
}

link_internal texture *
GetPtr(texture_block *Block, umm Index)
{
  texture *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal texture *
GetPtr(texture_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  texture_block *Block = &Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  texture *Result = Block->Elements+ElementIndex;
  return Result;
}


link_internal u32
AtElements(texture_block *Block)
{
  return Block->At;
}

