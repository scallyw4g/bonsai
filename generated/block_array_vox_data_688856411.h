struct vox_data_block
{
  u32 Index;
  u32 At;
  vox_data *Elements;
  vox_data_block *Next;
};

struct vox_data_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

link_inline umm
GetIndex(vox_data_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}


link_internal vox_data_block*
Allocate_vox_data_block(memory_arena *Memory)
{
  vox_data_block *Result = Allocate(vox_data_block, Memory, 1);
  Result->Elements = Allocate(vox_data, Memory, 8);
  return Result;
}

struct vox_data_block_array
{
  vox_data_block First;
  vox_data_block *Current;

  memory_arena *Memory;
};

link_internal void
Push(vox_data_block_array *Array, vox_data *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_vox_data_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    vox_data_block *Next = Allocate_vox_data_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  Array->Current->Elements[Array->Current->At++] = *Element;
}

link_internal vox_data_block_array_index
operator++(vox_data_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(vox_data_block*, I0.Block)->Next;
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
operator<(vox_data_block_array_index I0, vox_data_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}


link_internal vox_data_block_array_index
ZerothIndex(vox_data_block_array *Arr)
{
  vox_data_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(Cast(vox_data_block*, Result.Block)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(vox_data_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal vox_data_block_array_index
AtElements(vox_data_block_array *Arr)
{
  vox_data_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Cast(vox_data_block*, Arr->Current)->Index;
    Result.ElementIndex = Cast(vox_data_block*, Arr->Current)->At;
  }
  return Result;
}

link_internal vox_data *
GetPtr(vox_data_block_array *Arr, vox_data_block_array_index Index)
{
  vox_data *Result = {};
  if (Index.Block) { Result = Cast(vox_data_block *, Index.Block)->Elements + Index.ElementIndex; }
  return Result;
}




link_internal vox_data *
GetPtr(vox_data_block *Block, umm Index)
{
  vox_data *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal u32
AtElements(vox_data_block *Block)
{
  return Block->At;
}


