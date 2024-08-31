// src/engine/model.h:91:0

struct model_block
{
  u32 Index;
  u32 At;
  model *Elements;
  model_block *Next;
};

struct model_block_array_index
{
  model_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct model_block_array
{
  model_block *First;
  model_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

typedef model_block_array model_paged_list;

link_internal model_block_array_index
operator++(model_block_array_index &I0)
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
operator<(model_block_array_index I0, model_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(model_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal model_block_array_index
ZerothIndex(model_block_array *Arr)
{
  model_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
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
LastIndex(model_block_array *Arr)
{
  model_block_array_index Result = {};
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

link_internal model_block_array_index
AtElements(model_block_array *Arr)
{
  model_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal model *
GetPtr(model_block_array *Arr, model_block_array_index Index)
{
  model *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal model *
GetPtr(model_block *Block, umm Index)
{
  model *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal model *
GetPtr(model_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  model_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  model *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal model *
TryGetPtr(model_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  model *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(model_block *Block)
{
  return Block->At;
}


link_internal model_block*
Allocate_model_block(memory_arena *Memory)
{
  model_block *Result = Allocate(model_block, Memory, 1);
  Result->Elements = Allocate(model, Memory, 8);
  return Result;
}

link_internal cs
CS(model_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(model_block_array *Array, model_block_array_index Index)
{
  model_block_array_index LastI = LastIndex(Array);

  model *Element = GetPtr(Array, Index);
  model *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    model_block *Current = Array->First;
    model_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal model *
Push(model_block_array *Array, model *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_model_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      model_block *Next = Allocate_model_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  model *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


