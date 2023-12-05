struct asset_file_block
{
  u32 Index;
  u32 At;
  asset_file *Elements;
  asset_file_block *Next;
};

struct asset_file_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct asset_file_block_array
{
  asset_file_block First;
  asset_file_block *Current;
  memory_arena *Memory;
};

link_internal asset_file_block_array_index
operator++(asset_file_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(asset_file_block*, I0.Block)->Next;
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
operator<(asset_file_block_array_index I0, asset_file_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(asset_file_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal asset_file_block_array_index
ZerothIndex(asset_file_block_array *Arr)
{
  asset_file_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(Cast(asset_file_block*, Result.Block)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(asset_file_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal asset_file_block_array_index
AtElements(asset_file_block_array *Arr)
{
  asset_file_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Cast(asset_file_block*, Arr->Current)->Index;
    Result.ElementIndex = Cast(asset_file_block*, Arr->Current)->At;
  }
  return Result;
}

link_internal asset_file *
GetPtr(asset_file_block_array *Arr, asset_file_block_array_index Index)
{
  asset_file *Result = {};
  if (Index.Block) { Result = Cast(asset_file_block *, Index.Block)->Elements + Index.ElementIndex; }
  return Result;
}

link_internal asset_file *
GetPtr(asset_file_block *Block, umm Index)
{
  asset_file *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal asset_file *
GetPtr(asset_file_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  asset_file_block *Block = &Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  asset_file *Result = Block->Elements+ElementIndex;
  return Result;
}


link_internal u32
AtElements(asset_file_block *Block)
{
  return Block->At;
}


link_internal asset_file_block*
Allocate_asset_file_block(memory_arena *Memory)
{
  asset_file_block *Result = Allocate(asset_file_block, Memory, 1);
  Result->Elements = Allocate(asset_file, Memory, 8);
  return Result;
}

link_internal asset_file *
Push(asset_file_block_array *Array, asset_file *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_asset_file_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    asset_file_block *Next = Allocate_asset_file_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  asset_file *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


