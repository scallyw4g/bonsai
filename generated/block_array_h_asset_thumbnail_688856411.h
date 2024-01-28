struct asset_thumbnail_block
{
  u32 Index;
  u32 At;
  asset_thumbnail *Elements;
  asset_thumbnail_block *Next;
};

struct asset_thumbnail_block_array_index
{
  asset_thumbnail_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct asset_thumbnail_block_array
{
  asset_thumbnail_block *First;
  asset_thumbnail_block *Current;
  memory_arena *Memory; poof(@no_serialize)
};

link_internal asset_thumbnail_block_array_index
operator++(asset_thumbnail_block_array_index &I0)
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
operator<(asset_thumbnail_block_array_index I0, asset_thumbnail_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(asset_thumbnail_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal asset_thumbnail_block_array_index
ZerothIndex(asset_thumbnail_block_array *Arr)
{
  asset_thumbnail_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
  return Result;
}

link_internal umm
TotalElements(asset_thumbnail_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal asset_thumbnail_block_array_index
LastIndex(asset_thumbnail_block_array *Arr)
{
  asset_thumbnail_block_array_index Result = {};
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

link_internal asset_thumbnail_block_array_index
AtElements(asset_thumbnail_block_array *Arr)
{
  asset_thumbnail_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal asset_thumbnail *
GetPtr(asset_thumbnail_block_array *Arr, asset_thumbnail_block_array_index Index)
{
  asset_thumbnail *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal asset_thumbnail *
GetPtr(asset_thumbnail_block *Block, umm Index)
{
  asset_thumbnail *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal asset_thumbnail *
GetPtr(asset_thumbnail_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  asset_thumbnail_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  asset_thumbnail *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal u32
AtElements(asset_thumbnail_block *Block)
{
  return Block->At;
}

