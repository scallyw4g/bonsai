struct model_block
{
  u32 Index;
  u32 At;
  model *Elements;
  model_block *Next;
};

struct model_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

link_inline umm
GetIndex(model_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}


link_internal model_block*
Allocate_model_block(memory_arena *Memory)
{
  model_block *Result = Allocate(model_block, Memory, 1);
  Result->Elements = Allocate(model, Memory, 8);
  return Result;
}

struct model_block_array
{
  model_block First;
  model_block *Current;

  memory_arena *Memory;
};

link_internal void
Push(model_block_array *Array, model *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_model_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    model_block *Next = Allocate_model_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  Array->Current->Elements[Array->Current->At++] = *Element;
}

link_internal model_block_array_index
operator++(model_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(model_block*, I0.Block)->Next;
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
operator<(model_block_array_index I0, model_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}


link_internal model_block_array_index
ZerothIndex(model_block_array *Arr)
{
  model_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(Cast(model_block*, Result.Block)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(model_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal model_block_array_index
AtElements(model_block_array *Arr)
{
  model_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Cast(model_block*, Arr->Current)->Index;
    Result.ElementIndex = Cast(model_block*, Arr->Current)->At;
  }
  return Result;
}

link_internal model *
GetPtr(model_block_array *Arr, model_block_array_index Index)
{
  model *Result = {};
  if (Index.Block) { Result = Cast(model_block *, Index.Block)->Elements + Index.ElementIndex; }
  return Result;
}




link_internal model *
GetPtr(model_block *Block, umm Index)
{
  model *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal u32
AtElements(model_block *Block)
{
  return Block->At;
}


